// dvtdbcheck.cpp
//
// A Database Check/Repair Tool for Davit
//
//   (C) Copyright 2011-2025 Fred Gleason <fredg@paravelsystems.com>
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
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

#include <dvtconf.h>
#include <dvtdb.h>

#include "dvtdbcheck.h"

MainObject::MainObject()
  : QObject()
{
  check_yes=false;
  check_no=false;

  //
  // Read Command Options
  //
  DvtCmdSwitch *cmd=
    new DvtCmdSwitch("dvtdbcheck",DVTDBCHECK_USAGE);
  for(int i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--yes") {
      check_yes=true;
    }
    if(cmd->key(i)=="--no") {
      check_no=true;
    }
  }
  if(check_yes&&check_no) {
    fprintf(stderr,"dvtdbcheck: '--yes' and '--no' are mutually exclusive\n");
    exit(256);
  }

  //
  // Check for Root Perms
  //
  if(geteuid()!=0) {
    fprintf(stderr,"dvtdbcheck: must be user \"root\"\n");
    exit(256);
  }

  //
  // Read Configuration
  //
  dvtconfig=new DvtConfig();
  dvtconfig->load();

  //
  // Open Database
  //
  QSqlDatabase db=QSqlDatabase::addDatabase(dvtconfig->mysqlDbtype());
  db.setDatabaseName(dvtconfig->mysqlDbname());
  db.setUserName(dvtconfig->mysqlUsername());
  db.setPassword(dvtconfig->mysqlPassword());
  db.setHostName(dvtconfig->mysqlHostname());
  if(!db.open()) {
    fprintf(stderr,"dvtstamp: unable to connect to database\n");
    db.removeDatabase(dvtconfig->mysqlDbname());
    exit(256);
  }
  CheckDuplicateAffiliates();

  exit(0);
}


void MainObject::CheckDuplicateAffiliates()
{
  QString sql;
  DvtSqlQuery *q;
  DvtSqlQuery *q1;
  QString last_call;
  QString last_type;
  double last_frequency;
  QString last_city;
  QString last_state;
  int last_id;
  int curr_contacts;
  int last_contacts;
  int curr_aired;
  int last_aired;
  int curr_remarks;
  int last_remarks;
  int dups=0;

  sql=QString("select ")+
    "`ID`,"+
    "`STATION_CALL`,"+       // 00
    "`STATION_TYPE`,"+       // 01
    "`STATION_FREQUENCY`,"+  // 02
    "`LICENSE_CITY`,"+       // 03
    "`LICENSE_STATE` "+      // 04
    "from `AFFILIATES` "+
    "order by `STATION_CALL`,`STATION_TYPE` ";
  q=new DvtSqlQuery(sql);
  if(q->first()) {
    last_id=q->value(0).toInt();
    last_call=q->value(1).toString();
    last_type=q->value(2).toString();
    last_frequency=q->value(3).toDouble();
    last_city=q->value(4).toString();
    last_state=q->value(5).toString();
    while(q->next()) {
      if((q->value(1).toString().left(3)!="NEW")&&
	 (q->value(1).toString()==last_call)&&(q->value(2)==last_type)) {
	sql=QString("select ")+
	  "`ID` "+  // 00
	  "from `CONTACTS` where "+
	  QString::asprintf("(`AFFILIATE_ID`=%d)&&",last_id)+
	  "(`AFFIDAVIT`='Y')";
	q1=new DvtSqlQuery(sql);
	last_contacts=q1->size();
	delete q1;
	sql=QString("select ")+
	  "`ID` "+  // 00
	  "from `CONTACTS` where "+
	  QString::asprintf("(AFFILIATE_ID=%d)&&",q->value(0).toInt())+
	  "(`AFFIDAVIT`='Y')",
	q1=new DvtSqlQuery(sql);
	curr_contacts=q1->size();
	delete q1;

	sql=QString("select ")+
	  "`ID` "+  // 00
	  "from `AIRED` where "+
	  QString::asprintf("`AFFILIATE_ID`=%d",last_id);
	q1=new DvtSqlQuery(sql);
	last_aired=q1->size();
	delete q1;
	sql=QString("select ")+
	  "`ID` "+  // 00
	  "from `AIRED` where "+
	  QString::asprintf("`AFFILIATE_ID`=%d",q->value(0).toInt());
	q1=new DvtSqlQuery(sql);
	curr_aired=q1->size();
	delete q1;

	sql=QString("select ")+
	  "`ID` "+  // 00
	  "from `AFFILIATE_REMARKS` where "+
	  QString::asprintf("`AFFILIATE_ID`=%d",last_id);
	q1=new DvtSqlQuery(sql);
	last_remarks=q1->size();
	delete q1;
	sql=QString("select ")+
	  "`ID` "+  // 00
	  "from `AFFILIATE_REMARKS` where "+
	  QString::asprintf("`AFFILIATE_ID`=%d",q->value(0).toInt());
	q1=new DvtSqlQuery(sql);
	curr_remarks=q1->size();
	delete q1;
	printf("\n");
	printf("1: %2d %3d %4d: %s %6.1lf  %s, %s\n",
	       curr_contacts,curr_remarks,curr_aired,
	       DvtStationCallString(q->value(1).toString().toUtf8().constData(),
				    q->value(2).toString()).
	       toUtf8().constData(),
	       q->value(3).toDouble(),
	       q->value(4).toString().toUtf8().constData(),
	       q->value(5).toString().toUtf8().constData());

	printf("2: %2d %3d %4d: %s %6.1lf  %s, %s\n",
	       last_contacts,last_remarks,last_aired,
	       DvtStationCallString(last_call,last_type).toUtf8().constData(),
	       last_frequency,
	       last_city.toUtf8().constData(),
	       last_state.toUtf8().constData());
	printf("Found duplicate affiliate entry.  Merge? (y/N) ");
	fflush(NULL);
	if(UserResponse()) {
	  MergeAffiliates(q->value(0).toInt(),last_id);
  	}
	dups++;
      }
      last_id=q->value(0).toInt();
      last_call=q->value(1).toString();
      last_type=q->value(2).toString();      
      last_frequency=q->value(3).toDouble();
      last_city=q->value(4).toString();
      last_state=q->value(5).toString();
    }
  }
  delete q;
  DvtUpdateIsAffiliateField();
  printf("Found %d duplicate records\n",dups);
}


void MainObject::MergeAffiliates(int dest_id,int src_id)
{
  QString sql;
  DvtSqlQuery *q;
  DvtSqlQuery *q1;

  //
  // AIRED Data
  //
  sql=QString("select ")+
    "`ID`,"+            // 00
    "`STATE`,"+         // 01
    "`AIR_DATETIME`,"+  // 02
    "`AIR_LENGTH`,"+    // 03
    "`PROGRAM_ID` "+    // 04
    "from `AIRED` where "+
    QString::asprintf("`AFFILIATE_ID`=%d",src_id);
  q=new DvtSqlQuery(sql);
  while(q->next()) {
    sql=QString("select ")+
      "`ID` "+  // 00
      "from `AIRED` where "+
      QString::asprintf("(`AFFILIATE_ID`=%d)&&",dest_id)+
      QString::asprintf("(`PROGRAM_ID`=%d)&&",q->value(4).toInt())+
      "(`AIR_DATETIME`="+
      DvtSqlQuery::escape(q->value(2).toDateTime().
			  toString("yyyy-MM-dd hh:mm:ss"))+")&&"+
      QString::asprintf("(`AIR_LENGTH`=%d)",q->value(3).toInt());
    q1=new DvtSqlQuery(sql);
    if(q1->first()) {
      if((q->value(1).toInt()==Dvt::AiredStateScheduled)||
	 (q->value(1).toInt()==Dvt::AiredStateUnknown)) {
	sql=QString::asprintf("delete from AIRED where ID=%d",
			      q->value(0).toInt());
	DvtSqlQuery::apply(sql);
      }
      else {
	sql=QString("delete from `AIRED` where ")+
	  QString::asprintf("`ID`=%d",q1->value(0).toInt());
	DvtSqlQuery::apply(sql);
	sql=QString("update `AIRED` set ")+
	  QString::asprintf("`AFFILIATE_ID`=%d ",dest_id)+
	  "where "+
	  QString::asprintf("`ID`=%d",q->value(0).toInt());
	DvtSqlQuery::apply(sql);
      }
    }
    else {
      sql=QString("update AIRED set ")+
	QString::asprintf("`AFFILIATE_ID`=%d ",dest_id)+
	"where "+
	QString::asprintf("`ID`=%d",q->value(0).toInt());
      DvtSqlQuery::apply(sql);
    }
    delete q1;
  }
  delete q;

  //
  // AIRINGS Data
  //
  sql=QString("select ")+
    "`ID`,"+          // 00
    "`PROGRAM_ID`,"+  // 01
    "`AIR_TIME` "+    // 02
    "from `AIRINGS` where "+
    QString::asprintf("`AFFILIATE_ID`=%d",src_id);
  q=new DvtSqlQuery(sql);
  while(q->next()) {
    sql=QString("select ")+
      "`ID` "+  // 00
      "from `AIRINGS` where "+
      QString::asprintf("(`AFFILIATE_ID`=%d)&&",dest_id)+
      QString::asprintf("(`PROGRAM_ID`=%d)&&",q->value(1).toInt())+
      "(`AIR_TIME`="+
      DvtSqlQuery::escape(q->value(2).toTime().toString("hh:mm:ss"))+")";
    q1=new DvtSqlQuery(sql);
    if(q1->first()) {
      sql=QString("delete from `AIRINGS` ")+
	"where "+
	QString::asprintf("`ID`=%d",q->value(0).toInt());
      DvtSqlQuery::apply(sql);
    }
    else {
      sql=QString("update `AIRINGS` set ")+
	QString::asprintf("`AFFILIATE_ID`=%d ",dest_id)+
	"where "+
	QString::asprintf("`ID`=%d",q->value(0).toInt());
      DvtSqlQuery::apply(sql);
    }
    delete q1;
  }
  delete q;

  //
  // AFFILIATE_REMARKS Data
  //
  sql=QString("update `AFFILIATE_REMARKS` set ")+
    QString::asprintf("`AFFILIATE_ID`=%d ",dest_id)+
    "where "+
    QString::asprintf("`AFFILIATE_ID`=%d",src_id);
  DvtSqlQuery::apply(sql);

  //
  // CONTACT Data
  //
  sql=QString("update `CONTACTS` set ")+
    QString::asprintf("`AFFILIATE_ID`=%d ",dest_id)+
    "where "+
    QString::asprintf("(`AFFILIATE_ID`=%d)&&",src_id)+
    "(`LOCKED`='Y')";
  DvtSqlQuery::apply(sql);

  sql=QString("delete from `CONTACTS` ")+
    "where "+
    QString::asprintf("`AFFILIATE_ID`=%d",src_id);
  DvtSqlQuery::apply(sql);

  //
  // AFFILIATES Data
  //
  sql=QString("select ")+
    "`AFFIDAVIT_ACTIVE` "+   // 00
    "from `AFFILIATES` where "+
    QString::asprintf("`ID`=%d",src_id);
  q=new DvtSqlQuery(sql);
  if(q->first()) {
    delete q;
    sql=QString("update `AFFILIATES` set ")+
      "`AFFIDAVIT_ACTIVE`='Y' "+
      "where "+
      QString::asprintf("`ID`=%d",dest_id);
    DvtSqlQuery::apply(sql);
  }
  delete q;
  sql=QString("delete from `AFFILIATES` ")+
    "where "+
    QString::asprintf("`ID`=%d",src_id);
  DvtSqlQuery::apply(sql);
}


bool MainObject::UserResponse() const
{
  char c=0;

  if(check_yes) {
    printf("y\n");
    return true;
  }
  if(check_no) {
    printf("n\n");
    return false;
  }
  while((c!='y')&&(c!='Y')&&(c!='n')&&(c!='N')) {
    DvtCheckReturnCode("UserResponse()",scanf("%c",&c),1);
    if((c=='y')||(c=='Y')) {
      DvtCheckReturnCode("UserResponse()",scanf("%c",&c),1);
      return true;
    }
    if(c=='\n') {
      return false;
    }
  }
  DvtCheckReturnCode("UserResponse()",scanf("%c",&c),1);
  return false;
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);
  new MainObject();
  return a.exec();
}
