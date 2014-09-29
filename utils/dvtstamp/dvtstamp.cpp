// dvtstamp.cpp
//
// Utility for instantiating affiliate schedule data for Davit
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: dvtstamp.cpp,v 1.4 2013/02/26 19:06:17 pcvs Exp $
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

#include <limits.h>
#include <glob.h>
#include <signal.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#include <qapplication.h>
#include <qdir.h>
#include <qstringlist.h>

#include <dvt.h>
#include <dvtconf.h>
#include <dvtimport.h>

#include <dvtstamp.h>


MainObject::MainObject(QObject *parent,const char *name)
  :QObject(parent,name)
{
  QString sql;
  QSqlQuery *q;
  int date_offset=0;
  QDate for_date=QDate::currentDate();
  bool whole_month=false;
  int affiliate_id=-1;
  QStringList fields;
  bool ok=false;

  //
  // Read Command Options
  //
  import_cmd=
    new DvtCmdSwitch(qApp->argc(),qApp->argv(),"dvtstamp",DVTSTAMP_USAGE);
  for(unsigned i=0;i<import_cmd->keys();i++) {
    if(import_cmd->key(i)=="--date-offset") {
      date_offset=import_cmd->value(i).toInt(&ok);
      if(!ok) {
	fprintf(stderr,"dvtstamp: invalid --date-stamp value\n");
	exit(256);
      }
      import_cmd->setProcessed(i,true);
    }
    if(import_cmd->key(i)=="--for-date") {
      fields=fields.split("-",import_cmd->value(i));
      if(fields.size()==3) {
	for_date=QDate(fields[0].toInt(),fields[1].toInt(),fields[2].toInt());
      }
      if(fields.size()==2) {
	for_date=QDate(fields[0].toInt(),fields[1].toInt(),1);
	whole_month=true;
      }
      if(!for_date.isValid()) {
	fprintf(stderr,"dvtstamp: invalid --for-date value\n");
	exit(256);
      }
      import_cmd->setProcessed(i,true);
    }
    if(import_cmd->key(i)=="--for-affiliate-id") {
      affiliate_id=import_cmd->value(i).toInt(&ok);
      if((!ok)||(affiliate_id<0)) {
	fprintf(stderr,"dvtstamp: invalid --for-affiliate-id value\n");
	exit(256);
      }
      import_cmd->setProcessed(i,true);
    }
    if(!import_cmd->processed(i)) {
      fprintf(stderr,"dvtstamp: unknown option %s\n",
	      (const char *)import_cmd->key(i));
      exit(256);
    }
  }
  if(whole_month&&(date_offset!=0)) {
    fprintf(stderr,
	    "dvtstamp: --date-offset cannot be used for entire months\n");
    exit(256);
  }

  //
  // Read Configuration
  //
  import_config=new DvtConfig();
  import_config->load();

  //
  // Open Database
  //
  QSqlDatabase *db=QSqlDatabase::addDatabase(import_config->mysqlDbtype());
  if(!db) {
    fprintf(stderr,"rdimport: unable to initialize connection to database\n");
    delete import_cmd;
    exit(256);
  }
  db->setDatabaseName(import_config->mysqlDbname());
  db->setUserName(import_config->mysqlUsername());
  db->setPassword(import_config->mysqlPassword());
  db->setHostName(import_config->mysqlHostname());
  if(!db->open()) {
    fprintf(stderr,"dvtstamp: unable to connect to database\n");
    db->removeDatabase(import_config->mysqlDbname());
    exit(256);
  }

  //
  // Generate Date Structures
  //
  if(!whole_month) {
    for_date=for_date.addDays(date_offset);
  }

  //
  // Generate Schedules
  //
  if(whole_month) {
    QDate date;
    for(int i=0;i<for_date.daysInMonth();i++) {
      date=QDate(for_date.year(),for_date.month(),i+1);
      if(affiliate_id<0) {
	sql="select ID from AFFILIATES where AFFIDAVIT_ACTIVE=\"Y\"";
	q=new QSqlQuery(sql);
	while(q->next()) {
	  GenerateSchedule(q->value(0).toInt(),date);
	}
	delete q;
      }
      else {
	GenerateSchedule(affiliate_id,date);
      }
    }
  }
  else {
    if(affiliate_id<0) {
      sql="select ID from AFFILIATES where AFFIDAVIT_ACTIVE=\"Y\"";
      q=new QSqlQuery(sql);
      while(q->next()) {
	GenerateSchedule(q->value(0).toInt(),for_date);
      }
      delete q;
    }
    else {
      GenerateSchedule(affiliate_id,for_date);
    }
  }

  //
  // Clean Up and Exit
  //
  delete import_cmd;

  exit(0);
}


void MainObject::GenerateSchedule(int affiliate_id,const QDate &date)
{
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
  QString dow_field="AIR_"+date.toString("ddd").upper();

  sql=QString().sprintf("select PROGRAM_ID,AIR_TIME,AIR_LENGTH from AIRINGS \
                         where (AFFILIATE_ID=%d)&&(%s=\"Y\")",
			affiliate_id,
			(const char *)dow_field);
  q=new QSqlQuery(sql);
  while(q->next()) {
    sql=QString().sprintf("insert into AIRED set \
                           AFFILIATE_ID=%d,\
                           PROGRAM_ID=%d,\
                           STATE=%d,\
                           AIR_DATETIME=\"%s %s\",\
                           AIR_LENGTH=%d,\
                           STAMP_DATETIME=now()",
			  affiliate_id,
			  q->value(0).toInt(),
			  Dvt::AiredStateScheduled,
			  (const char *)date.toString("yyyy-MM-dd"),
			  (const char *)q->value(1).toTime().
			  toString("hh:mm:ss"),
			  q->value(2).toInt());
    q1=new QSqlQuery(sql);
    delete q1;
  }
  delete q;
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv,false);
  new MainObject(NULL,"main");
  return a.exec();
}
