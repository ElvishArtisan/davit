// davit.cpp
//
// The Davit Afilliat Manager Interface
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef WIN32
#include <vmime/vmime.hpp>
#include <vmime/platforms/posix/posixHandler.hpp>
#include <unistd.h>
#include <sys/wait.h>
#endif  // WIN32

#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/types.h>
#include <qapplication.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qsqldatabase.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qfiledialog.h>
#include <qsettings.h>
#include <qfile.h>
#include <qstringlist.h>
#include <qpixmap.h>

#include <dvt.h>
#include <dvtconfig.h>
#include <dvtimport.h>
#include <login.h>

#include <opendb.h>
#include <createdb.h>
#include <davit.h>
#include <globals.h>
#include <list_users.h>
#include <list_affiliates.h>
#include <list_providers.h>
#include <list_programs.h>
#include <list_networks.h>
#include <list_reports.h>
#include <edit_system.h>

//
// Global Classes
//
DvtConfig *config;
DvtUser *global_dvtuser;
DvtSystem *global_dvtsystem;
QString openoffice_path;
bool exiting=false;
std::vector<QString> temp_files;
bool email_enabled=false;
Geometry *global_geometry=NULL;
MailDialog *mail_dialog=NULL;

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

MainWidget::MainWidget(QWidget *parent,const char *name)
  :QWidget(parent,name)
{
  QString sql;
  QSqlQuery *q;
  QString loginname;

  //
  // Initialize VMime
  //
#ifndef WIN32
  vmime::platform::setHandler<vmime::platforms::posix::posixHandler>();
#endif  // WIN32

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

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
  QPixmap *map=new QPixmap(callcommander_xpm);
  setIcon(*map);

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
  // Open Database
  //
  if(!OpenDb(config->mysqlDbname(),config->mysqlUsername(),
	     config->mysqlPassword(),config->mysqlHostname())) {
    exit(1);
  }

  //
  // Log In
  //
  QString password;
  Login *login=new Login(&loginname,&password,this,"login");
  if(login->exec()!=0) {
    exit(0);
  }
  sql=QString().sprintf("select USER_NAME from USERS where\
                         USER_NAME=\"%s\" && USER_PASSWORD=password(\"%s\")",
			(const char *)loginname,
			(const char *)password);
  q=new QSqlQuery(sql);
  if(q->size()<=0) {
    QMessageBox::information(this,"Login Failed","Invalid Login!");
    exiting=true;
    delete q;
  }
  global_dvtuser=new DvtUser(loginname);
  global_dvtsystem=new DvtSystem();
  global_dvtsystem->load();
  setCaption(QString().sprintf("Davit - User: %s",
			       (const char *)loginname));

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
  mail_dialog=new MailDialog(this);

  //
  // Title
  //
  QLabel *label=new QLabel("Davit Affiliate Manager",this,"main_title_label");
  label->setGeometry(10,5,sizeHint().width()-20,20);
  label->setFont(title_font);
  label->setAlignment(AlignCenter);

  label=new QLabel("Database Administrator",this,"sub_title_label");
  label->setGeometry(10,25,sizeHint().width()-20,20);
  label->setFont(default_font);
  label->setAlignment(AlignCenter);

  //
  // Manage Users Button
  //
  QPushButton *button=new QPushButton(this,"users_button");
  button->setGeometry(10,50,120,60);
  button->setFont(font);
  button->setText("Manage\n&Users");
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivAdmin));
  connect(button,SIGNAL(clicked()),this,SLOT(manageUsersData()));

  //
  // Generate Reports Button
  //
  button=new QPushButton(this,"reports_button");
  button->setGeometry(150,50,120,60);
  button->setFont(font);
  button->setText("Generate\n&Reports");
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
  button=new QPushButton(this,"providers_button");
  button->setGeometry(150,120,120,60);
  button->setFont(font);
  button->setText("Manage\n&Providers");
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivProviderView));
  connect(button,SIGNAL(clicked()),this,SLOT(manageProvidersData()));

  //
  // Manage Affiliate Button
  //
  button=new QPushButton(this,"affiliate_button");
  button->setGeometry(10,120,120,60);
  button->setFont(font);
  button->setText("Manage\n&Affiliates");
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateView));
  connect(button,SIGNAL(clicked()),this,SLOT(manageAffiliateData()));

  //
  // Manage Programs Button
  //
  button=new QPushButton(this,"programs_button");
  button->setGeometry(150,190,120,60);
  button->setFont(font);
  button->setText("Manage\nP&rograms");
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivProgramView));
  connect(button,SIGNAL(clicked()),this,SLOT(manageProgramsData()));

  //
  // Manage Networks Button
  //
  button=new QPushButton(this,"networks_button");
  button->setGeometry(10,190,120,60);
  button->setFont(font);
  button->setText("Manage\n&Networks");
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateView));
  connect(button,SIGNAL(clicked()),this,SLOT(manageNetworksData()));

  //
  // Manage System Settings Button
  //
  button=new QPushButton(this,"system_button");
  button->setGeometry(10,260,120,60);
  button->setFont(font);
  button->setText("Manage System\nSettings");
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivAdmin));
  connect(button,SIGNAL(clicked()),this,SLOT(manageSystemData()));

  //
  // Import External Data Button
  //
  button=new QPushButton(this,"bia_button");
  button->setGeometry(150,260,120,60);
  button->setFont(font);
  button->setText("&Import\nExternal Data");
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivAdmin));
  connect(button,SIGNAL(clicked()),this,SLOT(importExternalData()));

  //
  // Quit Button
  //
  button=new QPushButton(this,"quit_button");
  button->setGeometry(10,sizeHint().height()-70,sizeHint().width()-20,60);
  button->setFont(font);
  button->setText("&Quit");
  connect(button,SIGNAL(clicked()),this,SLOT(quitMainWidget()));

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
  return QSize(280,410);
}


QSizePolicy MainWidget::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void MainWidget::manageUsersData()
{
  ListUsers *list=new ListUsers(this,"list");
  list->exec();
  global_dvtuser->load();
  delete list;
}


void MainWidget::manageSystemData()
{
  EditSystem *list=new EditSystem(this);
  if(list->exec()==0) {
    global_dvtsystem->load();
  }
  delete list;
}


void MainWidget::manageProvidersData()
{
  ListProviders *list=new ListProviders(this,"list");
  list->exec();
  delete list;
}


void MainWidget::manageAffiliateData()
{
  ListAffiliates *list=new ListAffiliates(this,"list");
  list->exec();
  delete list;
}


void MainWidget::manageProgramsData()
{
  ListPrograms *list=new ListPrograms(-1,this,"list");
  list->exec();
  delete list;
}


void MainWidget::manageNetworksData()
{
  ListNetworks *list=new ListNetworks(this,"list");
  list->exec();
  delete list;
}


void MainWidget::generateReportsData()
{
  ListReports *d=new ListReports(this);
  d->exec();
  delete d;
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

  exit(0);
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  quitMainWidget();
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);
  
  MainWidget *w=new MainWidget(NULL,"main");
  if(exiting) {
      exit(0);
  }
  a.setMainWidget(w);
  w->setGeometry(w->geometry().x(),w->geometry().y(),
		 w->sizeHint().width(),w->sizeHint().height());
  w->show();
  return a.exec();
}
