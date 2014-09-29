// opendb.cpp
//
// Open a Davit Database
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
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

#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#endif  // WIN32
#include <qsqldatabase.h>
#include <qsqldriver.h>
#include <qmessagebox.h>

#include <globals.h>
#include <opendb.h>
#include <createdb.h>
#include <dvt.h>
#include <mysql_login.h>


bool OpenDb(QString dbname,QString login,QString pwd,QString host)
{
  QString admin_name;
  QString admin_pwd;
  QString sql;
  QSqlQuery *q;
  QString msg;
  MySqlLogin *mysql_login;

  //
  // Open Database
  //
  QSqlDatabase *db=QSqlDatabase::addDatabase(config->mysqlDbtype());
  if(!db) {
    return false;
  }
  db->setDatabaseName(dbname);
  db->setUserName(login);
  db->setPassword(pwd);
  db->setHostName(host);
  if(!db->open()) {
    msg="Unable to access the Davit Database!\n\
Please enter a login for an account with\n\
administrative rights on the mySQL server,\n\
and we will try to get this straightened out.";
    mysql_login=new MySqlLogin(msg,&admin_name,&admin_pwd);
    if(mysql_login->exec()!=0) {
      delete mysql_login;
      db->removeDatabase(dbname);
      return false;
    }
    delete mysql_login;
    db->setUserName(admin_name);
    db->setPassword(admin_pwd);
    if(db->open()) {      // Fixup DB Access Permsissions
	QMessageBox::warning(NULL,"Can't Connect",
			     "Wrong access permissions for accessing mySQL!");
	db->removeDatabase("mysql");
	return false;
    }
    else {
      db->setDatabaseName("mysql");
      if(!db->open()) {   // mySQL is hosed -- scream and die.
	QMessageBox::warning(NULL,"Can't Connect",
			     "Unable to connect to mySQL!");
	db->removeDatabase("mysql");
	return false;
      }
      else {              // Create a new Davit Database
	q=new QSqlQuery(QString().sprintf("create database %s",
					  (const char *)dbname));
	if(!q->isActive()) {   // Can't create DB.
	  delete q;
	  QMessageBox::warning(NULL,"Can't Create DB",
			       "Unable to create a Davit Database!");
	  db->removeDatabase("mysql");
	  return false;
	}
	delete q;
	sql=QString().sprintf("insert into user set Host=\"localhost\",\
            User=\"%s\",Password=PASSWORD(\"%s\")",
			      (const char *)login,(const char *)pwd);
	q=new QSqlQuery(sql);
	delete q;
	sql=QString().sprintf("insert into user set Host=\"\",\
            User=\"%s\",Password=PASSWORD(\"%s\")",
			      (const char *)login,(const char *)pwd);
	q=new QSqlQuery(sql);
	delete q;
	sql=QString().
	  sprintf("insert into db set Host=\"%%\",Db=\"%s\",\
            User=\"%s\",Select_priv=\"Y\",Insert_priv=\"Y\",Update_priv=\"Y\",\
            Delete_priv=\"Y\",Create_priv=\"Y\",Drop_priv=\"Y\",\
            Index_priv=\"Y\",Alter_priv=\"Y\"",
		  (const char *)dbname,(const char *)login);
	q=new QSqlQuery(sql);
	delete q;
	q=new QSqlQuery("flush privileges");
	delete q;
	db->close();   // Relinquish admin perms
	db->setDatabaseName(dbname);
	db->setUserName(login);
	db->setPassword(pwd);
	if(!db->open()) {   // Can't open new database
	  QMessageBox::warning(NULL,"Can't Connect",
			   "Unable to connect to new Davit Database!");
	  db->removeDatabase(dbname);
	  return false;
	}
	if(!CreateDb(login,pwd)) {   // Can't create tables.
	  QMessageBox::warning(NULL,"Can't Create",
			       "Unable to create Davit Database!");
	  db->removeDatabase(dbname);
	  return false;
	}
	db->close();
	db->setDatabaseName(dbname);
	db->setUserName(login);
	db->setPassword(pwd);
	if(!db->open()) {
	  QMessageBox::warning(NULL,"Can't Connect",
			       "Unable to connect to Davit Database!");
	  db->removeDatabase(dbname);
	  return false;
	}	  
	if(!InitDb(login,pwd)) {  // Can't initialize tables.
	  QMessageBox::warning(NULL,"Can't Initialize",
			       "Unable to initialize Davit Database!");
	  db->removeDatabase(dbname);
	  return false;
	}
	UpdateDb(DVT_VERSION_DATABASE);
	QMessageBox::information(NULL,"Created Database",
			     "New Davit Database Created!");
	return true;
      }
    }
    return false;
  }
  q=new QSqlQuery("select DB from VERSION");
  if(!q->first()) {
    QMessageBox::warning(NULL,"Can't Access",
			 "Unable to access Davit Database!");
    db->removeDatabase(dbname);
    return false;
  }
  int ver=q->value(0).toInt();
  delete q;
  if(ver<DVT_VERSION_DATABASE) {
    if(QMessageBox::warning(NULL,"Update Needed",
	       "The Davit Database needs to be updated.\nContinue?",
	       QMessageBox::Yes,QMessageBox::No)==QMessageBox::No) {
      db->removeDatabase(dbname);
      return false;
    }      
    if(!UpdateDb(ver)) {
      QMessageBox::warning(NULL,"Can't Update",
			   "Unable to update Davit Database!");
      db->removeDatabase(dbname);
      return false;
    }
    msg=QString().
      sprintf("The Davit Database has\nbeen updated to Version %d",
	      DVT_VERSION_DATABASE);
    QMessageBox::information(NULL,"Database Updated",msg);
  }
  return true;
}
