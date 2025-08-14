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
DvtConfig *config;
DvtUser *global_dvtuser;
DvtSystem *global_dvtsystem;
QString openoffice_path;
bool exiting=false;
QStringList temp_files;
bool email_enabled=false;
Geometry *global_geometry=NULL;
MailDialog *mail_dialog=NULL;
ViewerProcessList *global_viewer_list=NULL;

//
// Icons
//
#include "../icons/davit-22x22.xpm"

#ifndef WIN32
void SigHandler(int signo)
{
  switch(signo) {
  case SIGCHLD:
    while(waitpid(-1,NULL,WNOHANG)>0);
    signal(SIGCHLD,SigHandler);
    break;
  }
}
#endif  // WIN32

MainWidget::MainWidget(QWidget *parent)
  :QWidget(parent)
{
  QString sql;
  DvtSqlQuery *q;
  QString loginname;
  QString err_msg;

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
  config=new DvtConfig();
  config->load();

  //
  // Dialogs
  //
  d_login_dialog=new Login(config,this);

  //
  // Open Database
  //
  if(!OpenDb(&err_msg,config->mysqlDbname(),config->mysqlUsername(),
	     config->mysqlPassword(),config->mysqlHostname(),
	     config->mysqlServertype())) {
    QMessageBox::critical(this,"Davit - "+tr("Database Error"),err_msg);
    exit(1);
  }

  //
  // Log In
  //
  QString password;
  if(!d_login_dialog->exec(&loginname,&password)) {
    exit(0);
  }
  sql=QString("select ")+
    "`USER_NAME` "+  // 00
    "from `USERS` where "+
    "`USER_NAME`="+DvtSqlQuery::escape(loginname)+" && "+
    "`USER_PASSWORD`=password("+DvtSqlQuery::escape(password)+")";
  q=new DvtSqlQuery(sql);
  if(q->size()<=0) {
    QMessageBox::information(this,"Login Failed","Invalid Login!");
    exiting=true;
    delete q;
  }
  global_dvtuser=new DvtUser(loginname);
  global_dvtsystem=new DvtSystem();
  global_dvtsystem->load();
  setWindowTitle(QString::asprintf("Davit - User: %s",
			       loginname.toUtf8().constData()));

  //
  // Check for E-Mail Capability
  //
  email_enabled=QFile::exists(DVT_SENDMAIL_PATH);
  if(!email_enabled) {
    QMessageBox::information(this,"Davit - E-Mail Status",
			     tr("The Sendmail utility is not installed on this system.\nE-Mail functionality will be disabled!"));
  }

  //
  // Find OpenOffice (Win32 only)
  //
  /*
#ifdef WIN32
  bool ok=false;
  QSettings s;
  s.insertSearchPath(QSettings::Windows,"/OpenOffice.org");
  QStringList keys=s.subkeyList("/OpenOffice.org");
  QString key;
  float value=0.0;
  for(unsigned i=0;i<keys.size();i++) {
    keys[i].toFloat(&ok);
    if(ok&&(keys[i].toFloat()>value)) {
      value=keys[i].toFloat();
      key=keys[i];
    }
  }
  if(key.isEmpty()) {
    QMessageBox::information(this,"Davit",
			 tr("OpenOffice.org Calc is not installed on this system.\nReports will not be available!"));
  }
  else {
    openoffice_path=s.readEntry("/OpenOffice.org/"+key+"/Path");
  }
  openoffice_path="C:/Program Files/OpenOffice 4/program/soffice";
#else
  openoffice_path="soffice";
#endif  // WIN32
  */
  openoffice_path=global_geometry->openofficePath();

  //
  // Create Dialogs
  //
  mail_dialog=new MailDialog(config,this);
  d_reports_dialog=new ListReports(config,this);
  connect(global_viewer_list,SIGNAL(reportStartupComplete()),
	  d_reports_dialog,SLOT(clearBusyCursor()));
  d_users_dialog=new ListUsers(config,this);
  d_affiliates_dialog=new ListAffiliates(config,this);
  d_networks_dialog=new ListNetworks(config,this);
  d_providers_dialog=new ListProviders(config,this);
  d_system_dialog=new EditSystem(config,this);

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
  button->setText(tr("Manage\n&Users"));
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivAdmin));
  connect(button,SIGNAL(clicked()),this,SLOT(manageUsersData()));

  //
  // Generate Reports Button
  //
  button=new QPushButton(this);
  button->setGeometry(150,50,120,60);
  button->setFont(font);
  button->setText(tr("Generate\n&Reports"));
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
  button->setText(tr("Manage\nProviders"));
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivProviderView));
  connect(button,SIGNAL(clicked()),this,SLOT(manageProvidersData()));

  //
  // Manage Affiliate Button
  //
  button=new QPushButton(this);
  button->setGeometry(10,120,120,60);
  button->setFont(font);
  button->setText(tr("Manage\nAffiliates"));
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateView));
  connect(button,SIGNAL(clicked()),this,SLOT(manageAffiliateData()));

  //
  // Manage Networks Button
  //
  button=new QPushButton(this);
  button->setGeometry(10,190,120,60);
  button->setFont(font);
  button->setText(tr("Manage\nNetworks"));
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateView));
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

  //
  // Configure Signals
  //
#ifndef WIN32
  signal(SIGCHLD,SigHandler);
#endif  // WIN32
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


void MainWidget::quitMainWidget()
{
  global_geometry->save();

  //
  // Delete Temporary Files
  //
  for(unsigned i=0;i<temp_files.size();i++) {
    QFile::remove(temp_files[i]);
  }
  delete global_viewer_list;

  exit(0);
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  global_viewer_list->cleanup();
}


bool MainWidget::OpenDb(QString *err_msg,
			const QString &dbname,const QString &login,
			const QString &pwd,const QString &host,
			const QString &srv_type) const
{
  QString msg;

  //
  // Open Database
  //
  QSqlDatabase db=QSqlDatabase::addDatabase(srv_type);
  db.setDatabaseName(dbname);
  db.setUserName(login);
  db.setPassword(pwd);
  db.setHostName(host);
  if(!db.open()) {
    *err_msg=QObject::tr("Unable to access database")+
      "'"+dbname+"'@'"+host+"'.";
    return false;
  }

  //
  // Verify Schema Version
  //
  QString sql=QString("select ")+
    "`DB` "+  // 00
    "from `VERSION`";
  QSqlQuery *q=new DvtSqlQuery(sql);
  if(!q->first()) {
    *err_msg=QObject::tr("Database")+
      " '"+dbname+"'@'"+host+"' "+
      QObject::tr("does not appear to be a Davit database.");
    delete q;
    return false;
  }
  if(q->value(0).toInt()!=DVT_VERSION_DATABASE) {
    *err_msg=QObject::tr("Database")+
      " '"+dbname+"'@'"+host+"' "+
      QObject::tr("is at schema version")+
      QString::asprintf(" %d, ",q->value(0).toInt())+
      tr("expecting")+QString::asprintf(" %d.",DVT_VERSION_DATABASE);
    delete q;
    return false;
  }
  delete q;
  return true;
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);
  
  MainWidget *w=new MainWidget(NULL);
  if(exiting) {
      exit(0);
  }
  w->setGeometry(w->geometry().x(),w->geometry().y(),
		 w->sizeHint().width(),w->sizeHint().height());
  w->show();
  return a.exec();
}
