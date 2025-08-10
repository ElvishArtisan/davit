// dvtstamp.cpp
//
// Utility for instantiating affiliate schedule data for Davit
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

#include <limits.h>
#include <glob.h>
#include <signal.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#include <QCoreApplication>
#include <QDir>
#include <QSqlQuery>
#include <QStringList>

#include <dvt.h>
#include <dvtconf.h>
#include <dvtdb.h>
#include <dvtimport.h>

#include "dvtstamp.h"

MainObject::MainObject(QObject *parent)
  :QObject(parent)
{
  d_command=MainObject::ListCommand;
  d_dry_run=false;
  d_filter_date_offset=0;
  d_filter_for_date=QDate::currentDate();
  d_filter_whole_month=false;
  d_use_numeric_ids=false;
  QStringList fields;
  bool ok=false;

  //
  // Read Command Options
  //
  import_cmd=new DvtCmdSwitch("dvtstamp",DVTSTAMP_USAGE);
  for(int i=0;i<import_cmd->keys();i++) {
    for(int j=0;j<MainObject::LastCommand;j++) {
      MainObject::Command command=(MainObject::Command)j;
      if(import_cmd->key(i)=="--"+MainObject::commandText(command)) {
	d_command=command;
	import_cmd->setProcessed(i,true);
	break;
      }
    }
    if(import_cmd->key(i)=="--date-offset") {
      d_filter_date_offset=import_cmd->value(i).toInt(&ok);
      if(!ok) {
	fprintf(stderr,"dvtstamp: invalid --date-stamp value\n");
	exit(256);
      }
      import_cmd->setProcessed(i,true);
    }
    if(import_cmd->key(i)=="--dry-run") {
      d_dry_run=true;
      import_cmd->setProcessed(i,true);
    }
    if(import_cmd->key(i)=="--for-date") {
      fields=import_cmd->value(i).split("-");
      if(fields.size()==3) {
	d_filter_for_date=QDate(fields[0].toInt(),fields[1].toInt(),fields[2].toInt());
      }
      if(fields.size()==2) {
	d_filter_for_date=QDate(fields[0].toInt(),fields[1].toInt(),1);
	d_filter_whole_month=true;
      }
      if(!d_filter_for_date.isValid()) {
	fprintf(stderr,"dvtstamp: invalid --for-date value\n");
	exit(256);
      }
      import_cmd->setProcessed(i,true);
    }
    if(import_cmd->key(i)=="--for-affiliate-id") {
      d_affiliate_ids.push_back(import_cmd->value(i).toUInt(&ok));
      if(!ok) {
	fprintf(stderr,"dvtstamp: invalid --for-affiliate-id value\n");
	exit(256);
      }
      import_cmd->setProcessed(i,true);
    }
    if(import_cmd->key(i)=="--use-numeric-ids") {
      d_use_numeric_ids=true;
      import_cmd->setProcessed(i,true);
    }
      
    if(!import_cmd->processed(i)) {
      fprintf(stderr,"dvtstamp: unknown option %s\n",
	      import_cmd->key(i).toUtf8().constData());
      exit(256);
    }
  }
  if(d_filter_whole_month&&(d_filter_date_offset!=0)) {
    fprintf(stderr,
	    "dvtstamp: --date-offset and --for-date are mutually exclusive\n");
    exit(256);
  }

  //
  // Check For Root Permissions
  //
  if((d_command!=MainObject::ListCommand)&&(getuid()!=0)) {
    fprintf(stderr,"dvtstamp: only root can do that\n");
    exit(1);
  }
  
  //
  // Read Configuration
  //
  import_config=new DvtConfig();
  import_config->load();

  //
  // Open Database
  //
  QSqlDatabase db=QSqlDatabase::addDatabase(import_config->mysqlServertype());
  db.setDatabaseName(import_config->mysqlDbname());
  db.setUserName(import_config->mysqlUsername());
  db.setPassword(import_config->mysqlPassword());
  db.setHostName(import_config->mysqlHostname());
  if(!db.open()) {
    fprintf(stderr,"dvtstamp: unable to connect to database\n");
    db.removeDatabase(import_config->mysqlDbname());
    exit(256);
  }

  //
  // Dispatch
  //
  switch(d_command) {
  case MainObject::ClearCommand:
    Clear();
    break;

  case MainObject::CreateCommand:
    Create();
    break;

  case MainObject::DeleteCommand:
    Delete();
    break;

  case MainObject::ListCommand:
    List();
    break;

  case MainObject::LastCommand:
    break;
  }
  
  //
  // Clean Up and Exit
  //
  delete import_cmd;

  exit(0);
}


QString MainObject::commandText(Command cmd)
{
  QString ret="unknown";

  switch(cmd) {
  case MainObject::ClearCommand:
    ret="clear";
    break;

  case MainObject::CreateCommand:
    ret="create";
    break;

  case MainObject::DeleteCommand:
    ret="delete";
    break;

  case MainObject::ListCommand:
    ret="list";
    break;

  case MainObject::LastCommand:
    break;
  }

  return ret;
}


void MainObject::Clear() const
{
  QString sql=QString("update `AIRED` set ")+
    QString::asprintf("`STATE`=%d ",Dvt::AiredStateScheduled)+
    FilterSql(d_filter_for_date,d_filter_date_offset,d_filter_whole_month,
	      d_affiliate_ids);
  ApplySql(sql);
}


void MainObject::Create() const
{
  QString sql;
  DvtSqlQuery *q;

  if(d_filter_whole_month) {
    QDate date;
    for(int i=0;i<d_filter_for_date.daysInMonth();i++) {
      date=QDate(d_filter_for_date.year(),d_filter_for_date.month(),i+1);
      if(d_affiliate_ids.size()==0) {
	sql=QString("select ")+
	  "`ID` "+  // 00
	  "from `AFFILIATES` where "+
	  "`AFFIDAVIT_ACTIVE`='Y'";
	q=new DvtSqlQuery(sql);
	while(q->next()) {
	  CreateSchedule(q->value(0).toInt(),date);
	}
	delete q;
      }
      else {
	for(int j=0;j<d_affiliate_ids.size();j++) {
	  CreateSchedule(d_affiliate_ids.at(j),date);
	}
      }
    }
  }
  else {
    if(d_affiliate_ids.size()==0) {
      sql=QString("select ")+
	"`ID` "+  // 00
	"from `AFFILIATES` where "+
	"`AFFIDAVIT_ACTIVE`='Y'";
      q=new DvtSqlQuery(sql);
      while(q->next()) {
	CreateSchedule(q->value(0).toInt(),d_filter_for_date);
      }
      delete q;
    }
    else {
      for(int j=0;j<d_affiliate_ids.size();j++) {
	CreateSchedule(d_affiliate_ids.at(j),d_filter_for_date);
      }
    }
  }
}


void MainObject::Delete() const
{
  QString sql=QString("delete from `AIRED` ")+
    FilterSql(d_filter_for_date,d_filter_date_offset,d_filter_whole_month,
	      d_affiliate_ids);
  ApplySql(sql);
}


void MainObject::List() const
{
  int count=0;

  QString sql=QString("select ")+
    "`AIRED`.`ID`,"+                 // 00
    "`AFFILIATES`.`ID`,"+            // 01
    "`PROGRAMS`.`ID`,"+              // 02
    "`AIRED`.`AIR_DATETIME`,"+       // 03
    "`AIRED`.`ORIGIN_DATETIME`,"+    // 04
    "`AIRED`.`STAMP_DATETIME`,"+     // 05
    "`AFFILIATES`.`STATION_TYPE`,"+  // 06
    "`AFFILIATES`.`STATION_CALL`,"+  // 07
    "`AFFILIATES`.`STATION_TYPE`,"+  // 08
    "`PROGRAMS`.`PROGRAM_NAME`,"+    // 09
    "`AIRED`.`STATE` "+              // 10
    "from `AFFILIATES` right join `AIRED` "+
    "on `AFFILIATES`.`ID`=`AIRED`.`AFFILIATE_ID` "+
    "left join `PROGRAMS` "+
    "on `AIRED`.`PROGRAM_ID`=`PROGRAMS`.`ID` "+
    FilterSql(d_filter_for_date,d_filter_date_offset,
			      d_filter_whole_month,d_affiliate_ids)+
    "order by `AFFILIATES`.`ID`,`PROGRAMS`.`ID` ";
  //  printf("LIST SQL: %s\n",sql.toUtf8().constData());
  printf("--------------------------------------------------------------------------\n");
  printf("| AIR DATE-TIME     | AFFILIATE | PROGRAM                      | STATE   |\n");
  printf("--------------------|-----------|------------------------------|----------\n");
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  while(q->next()) {
    QString affiliate_name=QString::asprintf("%d",q->value(1).toInt());
    if(!d_use_numeric_ids) {
      if(q->value(8).toString()=="I") {
	affiliate_name=q->value(7).toString().toUpper()+"-"+"NET";
      }
      else {
	affiliate_name=
	  q->value(7).toString().toUpper()+"-"+q->value(8).toString()+"M";
      }
    }
    QString program_name=QString::asprintf("%d",q->value(2).toInt());
    if(!d_use_numeric_ids) {
      program_name=q->value(9).toString();
    }
    printf("|%s|%-11s|%-30s|%-9s|\n",
	   q->value(3).toDateTime().toString("yyyy-MM-dd hh:MM:ss").toUtf8().constData(),
	   affiliate_name.toUtf8().constData(),
	   program_name.toUtf8().constData(),
	   Dvt::airedStateText((Dvt::AiredState)q->value(10).toInt()).
	   toUtf8().constData());
    count++;
  }
  printf("--------------------------------------------------------------------------\n");
  delete q;
  printf("Printed %d records\n",count);
}


void MainObject::CreateSchedule(int affiliate_id,const QDate &date) const
{
  QString sql;
  DvtSqlQuery *q;
  QString dow_field="AIR_"+date.toString("ddd").toUpper();

  sql=QString("select ")+
    "`PROGRAM_ID`,"+  // 00
    "`AIR_TIME`,"+    // 01
    "`AIR_LENGTH` "+  // 02
    "from `AIRINGS` where "+
    QString::asprintf("(`AFFILIATE_ID`=%d)&&",affiliate_id)+
    "(`"+dow_field+"`='Y')";
  q=new DvtSqlQuery(sql);
  while(q->next()) {
    sql=QString("insert into `AIRED` set ")+
      QString::asprintf("`AFFILIATE_ID`=%d,",affiliate_id)+
      QString::asprintf("`PROGRAM_ID`=%d,",q->value(0).toInt())+
      QString::asprintf("`STATE`=%d,",Dvt::AiredStateScheduled)+
      "`AIR_DATETIME`="+
      DvtSqlQuery::escape(date.toString("yyyy-MM-dd")+" "+
			  q->value(1).toTime().toString("hh:mm:ss"))+","+
      QString::asprintf("`AIR_LENGTH`=%d,",q->value(2).toInt())+
      "`STAMP_DATETIME`=now()";
    ApplySql(sql);
  }
  delete q;
}


QString MainObject::FilterSql(const QDate &date,int offset,bool whole_month,
			      const QList<int> &affiliate_ids) const
{
  QString sql="where ";

  if(whole_month) {
    sql+="(`AIRED`.`AIR_DATETIME`>="+
      DvtSqlQuery::escape(date.addDays(offset).
			  toString("yyyy-MM")+"-01 00:00:00")+")&&"+
      "(`AIRED`.`AIR_DATETIME`<"+
      DvtSqlQuery::escape(date.addDays(offset).addMonths(1).
			  toString("yyyy-MM")+"-01 00:00:00")+")";
  }
  else {
    sql+="(`AIRED`.`AIR_DATETIME`>="+
      DvtSqlQuery::escape(date.addDays(offset).
			  toString("yyyy-MM-dd")+" 00:00:00")+")&&"+
      "(`AIRED`.`AIR_DATETIME`<"+
      DvtSqlQuery::escape(date.addDays(offset).addDays(1).
			  toString("yyyy-MM-dd")+" 00:00:00")+")";
  }
  if(affiliate_ids.size()>0) {
    for(int i=0;i<affiliate_ids.size();i++) {
      sql+=QString::asprintf("&&(`AIRED`.`AFFILIATE_ID`=%d)",
			     affiliate_ids.at(i));
    }
  }
  sql+=" ";

  return sql;
}


bool MainObject::ApplySql(const QString &sql) const
{
  if(d_dry_run) {
    printf("%s\n",sql.toUtf8().constData());
    return true;
  }
  return DvtSqlQuery::apply(sql);
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);
  new MainObject();
  return a.exec();
}
