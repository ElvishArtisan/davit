// dvtdbcheck.cpp
//
// A Database Check/Repair Tool for Davit
//
//   (C) Copyright 2011 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: dvtdbcheck.cpp,v 1.1 2011/04/29 21:04:59 pcvs Exp $
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

#include <qapplication.h>
#include <qdir.h>
#include <qfileinfo.h>

#include <dvtconf.h>
#include <dvtdbcheck.h>

MainObject::MainObject(QObject *parent,const char *name)
  :QObject(parent,name)
{
  check_yes=false;
  check_no=false;

  //
  // Read Command Options
  //
  DvtCmdSwitch *cmd=
    new DvtCmdSwitch(qApp->argc(),qApp->argv(),"dvtdbcheck",DVTDBCHECK_USAGE);
  for(unsigned i=0;i<cmd->keys();i++) {
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
  //
  // Open Database
  //
  QSqlDatabase *db=QSqlDatabase::addDatabase(dvtconfig->mysqlDbtype());
  if(!db) {
    fprintf(stderr,"rdimport: unable to initialize connection to database\n");
    exit(256);
  }
  db->setDatabaseName(dvtconfig->mysqlDbname());
  db->setUserName(dvtconfig->mysqlUsername());
  db->setPassword(dvtconfig->mysqlPassword());
  db->setHostName(dvtconfig->mysqlHostname());
  if(!db->open()) {
    fprintf(stderr,"dvtstamp: unable to connect to database\n");
    db->removeDatabase(dvtconfig->mysqlDbname());
    exit(256);
  }

  CheckDuplicateAffiliates();

  exit(0);
}


void MainObject::CheckDuplicateAffiliates()
{
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
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

  sql="select ID,STATION_CALL,STATION_TYPE,STATION_FREQUENCY,\
       LICENSE_CITY,LICENSE_STATE from AFFILIATES \
       order by STATION_CALL,STATION_TYPE";
  q=new QSqlQuery(sql);
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
	sql=QString().sprintf("select ID from CONTACTS \
                               where (AFFILIATE_ID=%d)&&(AFFIDAVIT=\"Y\")",
			      last_id);
	q1=new QSqlQuery(sql);
	last_contacts=q1->size();
	delete q1;
	sql=QString().sprintf("select ID from CONTACTS \
                               where (AFFILIATE_ID=%d)&&(AFFIDAVIT=\"Y\")",
			      q->value(0).toInt());
	q1=new QSqlQuery(sql);
	curr_contacts=q1->size();
	delete q1;


	sql=QString().sprintf("select ID from AIRED where AFFILIATE_ID=%d",
			      last_id);
	q1=new QSqlQuery(sql);
	last_aired=q1->size();
	delete q1;
	sql=QString().sprintf("select ID from AIRED where AFFILIATE_ID=%d",
			      q->value(0).toInt());
	q1=new QSqlQuery(sql);
	curr_aired=q1->size();
	delete q1;


	sql=QString().sprintf("select ID from AFFILIATE_REMARKS where AFFILIATE_ID=%d",
			      last_id);
	q1=new QSqlQuery(sql);
	last_remarks=q1->size();
	delete q1;
	sql=QString().sprintf("select ID from AFFILIATE_REMARKS where AFFILIATE_ID=%d",
			      q->value(0).toInt());
	q1=new QSqlQuery(sql);
	curr_remarks=q1->size();
	delete q1;
	printf("\n");
	printf("1: %2d %3d %4d: %s %6.1lf  %s, %s\n",
	       curr_contacts,curr_remarks,curr_aired,
	       (const char *)DvtStationCallString(q->value(1).toString(),
						  q->value(2).toString()),
	       q->value(3).toDouble(),
	       (const char *)q->value(4).toString(),
	       (const char *)q->value(5).toString());

	printf("2: %2d %3d %4d: %s %6.1lf  %s, %s\n",
	       last_contacts,last_remarks,last_aired,
	       (const char *)DvtStationCallString(last_call,last_type),
	       last_frequency,
	       (const char *)last_city,
	       (const char *)last_state);
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
  QSqlQuery *q;
  QSqlQuery *q1;
  QSqlQuery *q2;

  //
  // AIRED Data
  //
  sql=QString().sprintf("select ID,STATE,AIR_DATETIME,AIR_LENGTH,PROGRAM_ID \
                         from AIRED where AFFILIATE_ID=%d",src_id);
  q=new QSqlQuery(sql);
  while(q->next()) {
    sql=QString().sprintf("select ID from AIRED where \
                           (AFFILIATE_ID=%d)&&(PROGRAM_ID=%d)&&\
                           (AIR_DATETIME=\"%s\")&&(AIR_LENGTH=%d)",
			  dest_id,q->value(4).toInt(),
			  (const char *)q->value(2).toDateTime().
			  toString("yyyy-MM-dd hh:mm:ss"),
			  q->value(3).toInt());
    q1=new QSqlQuery(sql);
    if(q1->first()) {
      if((q->value(1).toInt()==Dvt::AiredStateScheduled)||
	 (q->value(1).toInt()==Dvt::AiredStateUnknown)) {
	sql=QString().sprintf("delete from AIRED where ID=%d",
			      q->value(0).toInt());
	q2=new QSqlQuery(sql);
	delete q2;
      }
      else {
	sql=QString().sprintf("delete from AIRED where ID=%d",
			      q1->value(0).toInt());
	q2=new QSqlQuery(sql);
	delete q2;
	sql=QString().sprintf("update AIRED set AFFILIATE_ID=%d where ID=%d",
			      dest_id,q->value(0).toInt());
	q2=new QSqlQuery(sql);
	delete q2;
      }
    }
    else {
      sql=QString().sprintf("update AIRED set AFFILIATE_ID=%d where ID=%d",
			    dest_id,q->value(0).toInt());
      q2=new QSqlQuery(sql);
      delete q2;
    }
    delete q1;
  }
  delete q;

  //
  // AIRINGS Data
  //
  sql=QString().sprintf("select ID,PROGRAM_ID,AIR_TIME \
                         from AIRINGS where AFFILIATE_ID=%d",
			src_id);
  q=new QSqlQuery(sql);
  while(q->next()) {
    sql=QString().sprintf("select ID from AIRINGS \
                           where (AFFILIATE_ID=%d)&&(PROGRAM_ID=%d)&&\
                           (AIR_TIME=\"%s\")",
			  dest_id,q->value(1).toInt(),
			  (const char *)q->value(2).toTime().
			  toString("hh:mm:ss"));
    q1=new QSqlQuery(sql);
    if(q1->first()) {
      sql=QString().sprintf("delete from AIRINGS where ID=%d",
			    q->value(0).toInt());
      q2=new QSqlQuery(sql);
      delete q2;
    }
    else {
      sql=QString().sprintf("update AIRINGS set AFFILIATE_ID=%d \
                             where ID=%d",
			    dest_id,q->value(0).toInt());
      q2=new QSqlQuery(sql);
      delete q2;
    }
    delete q1;
  }
  delete q;

  //
  // AFFILIATE_REMARKS Data
  //
  sql=QString().sprintf("update AFFILIATE_REMARKS set AFFILIATE_ID=%d \
                         where AFFILIATE_ID=%d",
			dest_id,src_id);
  q=new QSqlQuery(sql);
  delete q;

  //
  // CONTACT Data
  //
  sql=QString().sprintf("update CONTACTS set AFFILIATE_ID=%d \
                         where (AFFILIATE_ID=%d)&&(LOCKED=\"Y\")",
			dest_id,src_id);
  q=new QSqlQuery(sql);
  delete q;

  sql=QString().sprintf("delete from CONTACTS where AFFILIATE_ID=%d",src_id);
  q=new QSqlQuery(sql);
  delete q;

  //
  // AFFILIATES Data
  //
  sql=QString().sprintf("select AFFIDAVIT_ACTIVE from AFFILIATES where ID=%d",
			src_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    delete q;
    sql=QString().sprintf("update AFFILIATES set AFFIDAVIT_ACTIVE=\"Y\" \
                           where ID=%d",dest_id);
    q=new QSqlQuery(sql);
  }
  delete q;
  sql=QString().sprintf("delete from AFFILIATES where ID=%d",src_id);
  q=new QSqlQuery(sql);
  delete q;
}


bool MainObject::UserResponse()
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
    scanf("%c",&c);
    if((c=='y')||(c=='Y')) {
      scanf("%c",&c);
      return true;
    }
    if(c=='\n') {
      return false;
    }
  }
  scanf("%c",&c);
  return false;
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv,false);
  new MainObject(NULL,"main");
  return a.exec();
}
