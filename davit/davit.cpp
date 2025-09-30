// davit.cpp
//
// The Davit Afilliat Manager Interface
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <unistd.h>
#include <sys/wait.h>

#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/types.h>

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>

#include <dvt.h>
#include <dvtcmdswitch.h>
#include <dvtconfig.h>
#include <dvtdb.h>
#include <dvtimport.h>

#include "davit.h"
#include "globals.h"
#include "list_providers.h"
#include "list_reports.h"

//
// Global Classes
//
DvtUser *global_dvtuser;
DvtSystem *global_dvtsystem;
QString openoffice_path;
QStringList temp_files;
Geometry *global_geometry=NULL;
MailDialog *mail_dialog=NULL;
ViewerProcessList *global_viewer_list=NULL;
bool global_email_dry_run=false;

//
// Icons
//
#include "../icons/davit-22x22.xpm"

MainWidget::MainWidget(QWidget *parent)
  :QWidget(parent)
{
  QString sql;
  DvtSqlQuery *q;
  QString loginname;
  QString err_msg;

  d_wireguard_process=NULL;

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont default_font("Helvetica",12,QFont::Normal);
  default_font.setPixelSize(12);
  QFont title_font=QFont("Helvetica",16,QFont::Bold);
  title_font.setPixelSize(16);
  qApp->setFont(default_font);

  //
  // Create And Set Icon
  //
  QPixmap map(davit_22x22_xpm);
  setWindowIcon(map);

  //
  // Viewer Instance Manager
  //
  global_viewer_list=new ViewerProcessList(this);
  connect(global_viewer_list,SIGNAL(cleanupComplete()),
	  this,SLOT(quitMainWidget()));

  //
  // Load Geometry
  //
  global_geometry=new Geometry();
  global_geometry->load();

  //
  // Load Configs
  //
  DvtCmdSwitch *cmd=new DvtCmdSwitch("davit",DAVIT_USAGE);
  d_config=new DvtConfig();
  d_config->load(cmd);

  //
  // Command-Line Switches
  //
  for(int i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--email-dry-run") {
      global_email_dry_run=true;
      cmd->setProcessed(i,true);
    }
    if(!cmd->processed(i)) {
      fprintf(stderr,"davit: unrecognized option \"%s\"\n",
	      cmd->key(i).toUtf8().constData());
      CleanExit(1);
    }
  }

  //
  // Configure Signals
  //
  d_signal_monitor=new SignalMonitor(this);
  connect(d_signal_monitor,SIGNAL(receivedSignal(int)),
	  this,SLOT(receivedSignalData(int)));
  d_signal_monitor->addSignal(SIGINT);
  d_signal_monitor->addSignal(SIGTERM);

  //
  // Dialogs
  //
  d_login_dialog=new Login(d_config,this);

  //
  // Davit Process Monitor
  //
  d_instance_monitor=new DvtInstanceMonitor();
  if((!d_config->wireguardConfiguration().isEmpty())&&
     (d_instance_monitor->processIds().size()==0)) {
    d_wireguard_process=new QProcess(this);
    sleep(1);
  }

  //
  // Start VPN Tunnel
  //
  if(!d_config->wireguardConfiguration().isEmpty()) {
    if(d_config->wireguardConnectionPriority()==DvtConfig::VpnFirst) {
      WireguardTunnel(true);
    }
  }

  //
  // Open Database
  //
  if(!DvtOpenDb(&err_msg,d_config)) {
    if((d_config->wireguardConnectionPriority()==DvtConfig::LocalOnly)||
       d_config->wireguardConfiguration().isEmpty()) {
      QMessageBox::critical(this,"Davit - "+tr("Database Error"),err_msg);
      CleanExit(1);
    }
    //
    // Flip the tunnel state
    //
    WireguardTunnel(d_config->wireguardConnectionPriority()==
		    DvtConfig::LocalFirst);

    //
    // Try Again
    //
    if(!DvtOpenDb(&err_msg,d_config)) {
      QMessageBox::critical(this,"Davit - "+tr("Database Error"),err_msg);
      CleanExit(1);
    }
  }
  
  //
  // Log In
  //
  QString password;
  if(!d_login_dialog->exec(&loginname,&password)) {
    CleanExit(0);
  }
  sql=QString("select ")+
    "`USER_NAME` "+  // 00
    "from `USERS` where "+
    "`USER_NAME`="+DvtSqlQuery::escape(loginname)+" && "+
    "`USER_PASSWORD`=password("+DvtSqlQuery::escape(password)+")";
  q=new DvtSqlQuery(sql);
  if(q->size()<=0) {
    QMessageBox::information(d_login_dialog,"Login Failed","Invalid Login!");
    CleanExit(0);
    delete q;
  }
  global_dvtuser=new DvtUser(loginname);
  global_dvtsystem=new DvtSystem();
  global_dvtsystem->load();
  setWindowTitle(QString::asprintf("Davit - User: %s",
			       loginname.toUtf8().constData()));

  openoffice_path=global_geometry->openofficePath();

  //
  // Create Dialogs
  //
  mail_dialog=new MailDialog(d_config,this);
  d_reports_dialog=new ListReports(d_config,this);
  connect(global_viewer_list,SIGNAL(reportStartupComplete()),
	  d_reports_dialog,SLOT(clearBusyCursor()));
  d_users_dialog=new ListUsers(d_config,this);
  d_affiliates_dialog=new ListAffiliates(d_config,this);
  d_networks_dialog=new ListNetworks(d_config,this);
  d_providers_dialog=new ListProviders(d_config,this);
  d_system_dialog=new EditSystem(d_config,this);

  //
  // Title
  //
  QLabel *label=new QLabel(tr("Davit Affiliate Manager"),this);
  label->setGeometry(10,5,sizeHint().width()-20,20);
  label->setFont(title_font);
  label->setAlignment(Qt::AlignCenter);

  label=new QLabel(tr("Database Administrator"),this);
  label->setGeometry(10,25,sizeHint().width()-20,20);
  label->setFont(default_font);
  label->setAlignment(Qt::AlignCenter);

  //
  // Manage Users Button
  //
  QPushButton *button=new QPushButton(this);
  button->setGeometry(10,50,120,60);
  button->setFont(font);
  button->setText(tr("Manage")+"\n"+tr("Users"));
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivAdmin));
  connect(button,SIGNAL(clicked()),this,SLOT(manageUsersData()));

  //
  // Generate Reports Button
  //
  button=new QPushButton(this);
  button->setGeometry(150,50,120,60);
  button->setFont(font);
  button->setText(tr("Generate")+"\n"+tr("Reports"));
  connect(button,SIGNAL(clicked()),this,SLOT(generateReportsData()));
#ifdef WIN32
  button->setEnabled((!openoffice_path.isEmpty())&&
		     global_dvtuser->privilege(DvtUser::PrivReportView));
#else
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivReportView));
#endif  // WIN32

  //
  // Manage Program Providers Button
  //
  button=new QPushButton(this);
  button->setGeometry(150,120,120,60);
  button->setFont(font);
  button->setText(tr("Manage")+"\n"+tr("Providers"));
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivProviderView));
  connect(button,SIGNAL(clicked()),this,SLOT(manageProvidersData()));

  //
  // Manage Affiliate Button
  //
  button=new QPushButton(this);
  button->setGeometry(10,120,120,60);
  button->setFont(font);
  button->setText(tr("Manage")+"\n"+tr("Affiliates"));
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateView));
  connect(button,SIGNAL(clicked()),this,SLOT(manageAffiliateData()));

  //
  // Manage Networks Button
  //
  button=new QPushButton(this);
  button->setGeometry(10,190,120,60);
  button->setFont(font);
  button->setText(tr("Manage")+"\n"+tr("Networks"));
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivProviderView));
  connect(button,SIGNAL(clicked()),this,SLOT(manageNetworksData()));

  //
  // Manage System Settings Button
  //
  button=new QPushButton(this);
  button->setGeometry(150,190,120,60);
  button->setFont(font);
  button->setText(tr("Manage System")+"\n"+tr("E-Mail Settings"));
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivAdmin));
  connect(button,SIGNAL(clicked()),this,SLOT(manageSystemData()));

  //
  // Quit Button
  //
  button=new QPushButton(this);
  button->setGeometry(10,sizeHint().height()-70,sizeHint().width()-20,60);
  button->setFont(font);
  button->setText(tr("Quit"));
  connect(button,SIGNAL(clicked()),global_viewer_list,SLOT(cleanup()));
}


MainWidget::~MainWidget()
{
}


QSize MainWidget::sizeHint() const
{
  return QSize(280,340);
}


QSizePolicy MainWidget::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void MainWidget::manageUsersData()
{
  d_users_dialog->exec();
  global_dvtuser->load();
}


void MainWidget::manageSystemData()
{
  if(d_system_dialog->exec()) {
    global_dvtsystem->load();
  }
}


void MainWidget::manageProvidersData()
{
  d_providers_dialog->exec();
}


void MainWidget::manageAffiliateData()
{
  d_affiliates_dialog->exec();
}


void MainWidget::manageNetworksData()
{
  d_networks_dialog->exec();
}


void MainWidget::generateReportsData()
{
  d_reports_dialog->exec();
}


void MainWidget::receivedSignalData(int signum)
{
  switch(signum) {
  case SIGINT:
  case SIGTERM:
    CleanExit(0);
    break;
  }
}


void MainWidget::quitMainWidget()
{
  CleanExit(0);
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  global_viewer_list->cleanup();
}


void MainWidget::CleanExit(int exit_code)
{
  global_geometry->save();

  //
  // Delete Temporary Files
  //
  for(unsigned i=0;i<temp_files.size();i++) {
    QFile::remove(temp_files[i]);
  }
  delete global_viewer_list;

  //
  // Stop VPN Tunnels
  //
  WireguardTunnel(false);
  
  exit(exit_code);
}


bool MainWidget::WireguardTunnel(bool start_up)
{
  QStringList args;

  if(d_config->wireguardConfiguration().isEmpty()) {
    return false;
  }

  if(start_up) {
    args.push_back("--up");
  }
  else {
    args.push_back("--down");
  }
  QProcess *proc=new QProcess();
  proc->start("/usr/lib/davit/dvtwgmgr",args);
  proc->waitForFinished();
  if(start_up) {  // FIXME: QProcess falsely returns 'crashed' when stopping
    if(proc->exitStatus()!=QProcess::NormalExit) {
      QMessageBox::critical(this,"Davit - "+tr("Helper Error"),
			    tr("The wireguard helper application crashed!"));
      exit(1);
    }
    if(proc->exitCode()!=0) {
      QMessageBox::critical(this,"Davit - "+tr("Helper Error"),
			    tr("The wireguard helper application returned an error.")+
			    "\n\n"+QString::fromUtf8(proc->readAllStandardError()));
      exit(1);
    }
  }
  return true;
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);
  
  MainWidget *w=new MainWidget(NULL);
  w->setGeometry(w->geometry().x(),w->geometry().y(),
		 w->sizeHint().width(),w->sizeHint().height());
  w->show();
  return a.exec();
}
