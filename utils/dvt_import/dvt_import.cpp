// dvt_import.cpp
//
// A Batch Importer for Davit
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

#include <dvt.h>
#include <dvtconf.h>
#include <dvtimport.h>

#include "dvt_import.h"


MainObject::MainObject(QObject *parent)
  :QObject(parent)
{
  MainObject::TableType table=MainObject::NoTable;
  QString filename;

  //
  // Read Command Options
  //
  import_cmd=new DvtCmdSwitch("dvt_import",DVT_IMPORT_USAGE);
  if(import_cmd->keys()<1) {
    fprintf(stderr,"\n");
    fprintf(stderr,"%s",DVT_IMPORT_USAGE);
    fprintf(stderr,"\n");
    delete import_cmd;
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
  /*
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
    fprintf(stderr,"dvt_import: unable to connect to database\n");
    db->removeDatabase(import_config->mysqlDbname());
    exit(256);
  }
  */
  //
  // Get Arguments
  //
  for(int i=0;i<import_cmd->keys();i++) {
    if(import_cmd->key(i)=="--station-table") {
      table=MainObject::StationsTable;
      filename=import_cmd->value(i);
    }
    if(import_cmd->key(i)=="--program-table") {
      table=MainObject::ProgramsTable;
      filename=import_cmd->value(i);
    }
  }
  switch(table) {
    case MainObject::StationsTable:
      ImportStationsTable(filename);
      break;

    case MainObject::ProgramsTable:
      ImportProgramsTable(filename);
      break;

    default:
      fprintf(stderr,"dvt_import: no table specified\n");
      exit(256);
  }


  //
  // Clean Up and Exit
  //
  delete import_cmd;

  exit(0);
}


void MainObject::ImportStationsTable(const QString &filename)
{
  QString sql;
  QString sql1;
  QSqlQuery *q;
  char buf[1024];
  QStringList values;
  QString type;
  QString biacode;
  int second_network_id=0;
  int new_network_id=0;
  double freq;
  bool ok=false;
  int line=1;

  //
  //Calculate next free network id
  //
  sql="select ID from NETWORKS order by ID desc";
  q=new QSqlQuery(sql);
  if(q->first()) {
    new_network_id=q->value(0).toInt()+1;
  }
  delete q;

  //
  // Open source file
  //
  if(filename.isNull()) {
    fprintf(stderr,"dvt_import: invalid filename\n");
    exit(256);
  }
  FILE *f=fopen(filename.toUtf8(),"r");
  if(f==NULL) {
    perror("dvt_import");
    exit(256);
  }

  //
  // Process Lines
  //
  while(fgets(buf,1024,f)!=NULL) {
    second_network_id=0;
    values=DvtGetCommaList(buf,true);
    type=values[3];
    biacode=values[1];
    freq=values[5].toDouble();
    fflush(stdout);
    sql=QString::asprintf("select ID from AFFILIATES where ID=%d",
			  values[0].toInt(&ok));
    if(ok) {
      //
      // Process Second Network Entry
      //
      if(!values[52].isEmpty()) {
	sql1=QString::asprintf("select ID from NETWORKS where NAME=\"%s\"",
			       values[52].toUtf8().constData());
	q=new QSqlQuery(sql1);
	if(q->first()) {
	  second_network_id=q->value(0).toInt();
	}
	else {
	  second_network_id=new_network_id;
	  sql1=QString::asprintf("insert into NETWORKS set ID=%d,NAME=\"%s\"",
				 new_network_id,
				 values[52].toUtf8().constData());
	  delete q;
	  q=new QSqlQuery(sql1);
	  new_network_id++;
	}
	delete q;
      }

      //
      // Import Affiliate Record
      //
      q=new QSqlQuery(sql);
      if(q->first()) {
	sql=QString::asprintf("update AFFILIATES set\
                               STATION_CALL=\"%s\",\
                               STATION_TYPE=\"%s\",\
                               STATION_FREQUENCY=%6.1lf,\
                               STATION_POWER=%6.0lf,\
                               STATION_NIGHT_POWER=%6.0lf,\
                               STATION_FORMAT=\"%s\",\
                               LICENSE_CITY=\"%s\",\
                               LICENSE_STATE=\"%s\",\
                               BUSINESS_NAME=\"%s\",\
                               ADDRESS1=\"%s\",\
                               CITY=\"%s\",\
                               STATE=\"%s\",\
                               ZIPCODE=\"%s\",\
                               PHONE=\"%s\",\
                               FAX=\"%s\",\
                               EMAIL_ADDR=\"%s\",\
                               WEB_URL=\"%s\",\
                               MARKET_NAME=\"%s\",\
                               MARKET_RANK=%4.0lf,\
                               DMA_RANK=%4.0lf,\
                               CONTACT_LAST_NAME=\"%s\",\
                               CONTACT_TITLE=\"%s\",\
                               CONTACT_PHONE=\"%s\",\
                               CONTACT_EMAIL=\"%s\",\
                               CONTACT1_LAST_NAME=\"%s\",\
                               CONTACT1_TITLE=\"Program Director\",\
                               CONTACT1_PHONE=\"%s\",\
                               CONTACT1_EMAIL=\%s\",\
                               CONTACT2_LAST_NAME=\"%s\",\
                               CONTACT2_TITLE=\"%s\",\
                               CONTACT2_PHONE=\"%s\",\
                               CONTACT2_EMAIL=\%s\",\
                               BIA_CODE=\"%s\",\
                               TIME_ZONE=\"%s\",\
                               SECOND_NETWORK_ID=%d \
                               where ID=%d",
			      DvtEscapeString(values[2]).toUtf8().constData(),
			      (const char *)DvtEscapeString(type).toUtf8().constData(),
			      freq,
			      values[19].toDouble(),
			      values[20].toDouble(),
			      DvtEscapeString(values[4]).toUtf8().constData(),
			      DvtEscapeString(values[28]).toUtf8().constData(),
			      DvtEscapeString(values[29]).toUtf8().constData(),
			      DvtEscapeString(values[22]).toUtf8().constData(),
			      DvtEscapeString(values[6]).toUtf8().constData(),
			      DvtEscapeString(values[7]).toUtf8().constData(),
			      DvtEscapeString(values[8]).toUtf8().constData(),
			      DvtEscapeString(values[9]).toUtf8().constData(),
			      DvtEscapeString(values[14]).toUtf8().constData(),
			      DvtEscapeString(values[15]).toUtf8().constData(),
			      DvtEscapeString(values[23]).toUtf8().constData(),
			      DvtEscapeString(values[24]).toUtf8().constData(),
			      DvtEscapeString(values[26]).toUtf8().constData(),
			      values[27].toDouble(),
			      values[21].toDouble(),
			      DvtEscapeString(values[16]).toUtf8().constData(),
			      DvtEscapeString(values[17]).toUtf8().constData(),
			      DvtEscapeString(values[36]).toUtf8().constData(),
			      DvtEscapeString(values[37]).toUtf8().constData(),
			      DvtEscapeString(values[18]).toUtf8().constData(),
			      DvtEscapeString(values[38]).toUtf8().constData(),
			      DvtEscapeString(values[39]).toUtf8().constData(),
			      DvtEscapeString(values[40]).toUtf8().constData(),
			      DvtEscapeString(values[41]).toUtf8().constData(),
			      DvtEscapeString(values[42]).toUtf8().constData(),
			      DvtEscapeString(values[43]).toUtf8().constData(),
			      DvtEscapeString(biacode).toUtf8().constData(),
                              DvtEscapeString(values[48]).toUtf8().constData(),
			      second_network_id,
			      values[0].toInt());
      }
      else {
	sql=QString::asprintf("insert into AFFILIATES set\
                               ID=%d,\
                               STATION_CALL=\"%s\",\
                               STATION_TYPE=\"%s\",\
                               USER_PASSWORD=password(\"%s-%s\"),\
                               STATION_FREQUENCY=%6.1lf,\
                               STATION_POWER=%6.0lf,\
                               STATION_NIGHT_POWER=%6.0lf,\
                               STATION_FORMAT=\"%s\",\
                               LICENSE_CITY=\"%s\",\
                               LICENSE_STATE=\"%s\",\
                               BUSINESS_NAME=\"%s\",\
                               ADDRESS1=\"%s\",\
                               CITY=\"%s\",\
                               STATE=\"%s\",\
                               ZIPCODE=\"%s\",\
                               PHONE=\"%s\",\
                               FAX=\"%s\",\
                               EMAIL_ADDR=\"%s\",\
                               WEB_URL=\"%s\",\
                               MARKET_NAME=\"%s\",\
                               MARKET_RANK=%4.0lf,\
                               DMA_RANK=%4.0lf,\
                               CONTACT_LAST_NAME=\"%s\",\
                               CONTACT_TITLE=\"%s\",\
                               CONTACT_PHONE=\"%s\",\
                               CONTACT_EMAIL=\"%s\",\
                               CONTACT1_LAST_NAME=\"%s\",\
                               CONTACT1_TITLE=\"Program Director\",\
                               CONTACT1_PHONE=\"%s\",\
                               CONTACT1_EMAIL=\"%s\",\
                               CONTACT2_LAST_NAME=\"%s\",\
                               CONTACT2_TITLE=\"%s\",\
                               CONTACT2_PHONE=\"%s\",\
                               CONTACT2_EMAIL=\"%s\",\
                               BIA_CODE=\"%s\",\
                               TIME_ZONE=\"%s\",\
                               SECOND_NETWORK_ID=%d",
			      values[0].toInt(),
			      DvtEscapeString(values[2]).toUtf8().constData(),
			      DvtEscapeString(type).toUtf8().constData(),
			      DvtEscapeString(values[2].toLower()).
			      toUtf8().constData(),
			      DvtEscapeString(type.toLower()).
			      toUtf8().constData(),
			      freq,
			      values[19].toDouble(),
			      values[20].toDouble(),
			      DvtEscapeString(values[4]).toUtf8().constData(),
			      DvtEscapeString(values[28]).toUtf8().constData(),
			      DvtEscapeString(values[29]).toUtf8().constData(),
			      DvtEscapeString(values[22]).toUtf8().constData(),
			      DvtEscapeString(values[6]).toUtf8().constData(),
			      DvtEscapeString(values[7]).toUtf8().constData(),
			      DvtEscapeString(values[8]).toUtf8().constData(),
			      DvtEscapeString(values[9]).toUtf8().constData(),
			      DvtEscapeString(values[14]).toUtf8().constData(),
			      DvtEscapeString(values[15]).toUtf8().constData(),
			      DvtEscapeString(values[23]).toUtf8().constData(),
			      DvtEscapeString(values[24]).toUtf8().constData(),
			      DvtEscapeString(values[26]).toUtf8().constData(),
			      values[27].toDouble(),
			      values[21].toDouble(),
			      DvtEscapeString(values[16]).toUtf8().constData(),
			      DvtEscapeString(values[17]).toUtf8().constData(),
			      DvtEscapeString(values[36]).toUtf8().constData(),
			      DvtEscapeString(values[37]).toUtf8().constData(),
			      DvtEscapeString(values[18]).toUtf8().constData(),
			      DvtEscapeString(values[38]).toUtf8().constData(),
			      DvtEscapeString(values[39]).toUtf8().constData(),
			      DvtEscapeString(values[40]).toUtf8().constData(),
			      DvtEscapeString(values[41]).toUtf8().constData(),
			      DvtEscapeString(values[42]).toUtf8().constData(),
			      DvtEscapeString(values[43]).toUtf8().constData(),
			      DvtEscapeString(biacode).toUtf8().constData(),
			      DvtEscapeString(values[48]).toUtf8().constData(),
			      second_network_id);
      }
      delete q;
      q=new QSqlQuery(sql);
      if(!q->isActive()) {
	fprintf(stderr,"*************************************************\n");
	fprintf(stderr,"Insert failed for %s-%s\n\n",
		values[2].toUtf8().constData(),type.toUtf8().constData());
	fprintf(stderr,"  Line %d: %s",line,buf);
	fprintf(stderr,"\n");
	fprintf(stderr,"  Rejected SQL: %s\n\n",sql.toUtf8().constData());
	fprintf(stderr,"*************************************************\n");
      }
      delete q;
    }
    else {
      fprintf(stderr,"%s",buf);
      fprintf(stderr,"\n");
      fprintf(stderr,"*************************************************\n");
      fprintf(stderr,"Rejected import (invalid key value)\n");
      fprintf(stderr,"  Line %d: %s",line,buf);
      fprintf(stderr,"*************************************************\n");
    }
    line++;
  }
}


void MainObject::ImportProgramsTable(const QString &filename)
{
  QString sql;
  QSqlQuery *q;
  char buf[1024];
  QStringList values;
  int program_id=0;
  bool dow[7];
  QTime start_time;
  int length;

  if(filename.isNull()) {
    fprintf(stderr,"dvt_import: invalid filename\n");
    exit(256);
  }
  FILE *f=fopen(filename.toUtf8(),"r");
  if(f==NULL) {
    perror("dvt_import");
    exit(256);
  }
  //
  // Build PROGRAMS Table
  //
  while(fgets(buf,1024,f)!=NULL) {
    values=DvtGetCommaList(buf,true);
    sql=QString::asprintf("select ID from PROGRAMS where PROGRAM_NAME=\"%s\"",
			  DvtEscapeString(values[2]).toUtf8().constData());
    q=new QSqlQuery(sql);
    if(q->first()) {
      program_id=q->value(0).toInt();
    }
    else {
      sql=QString::asprintf("insert into PROGRAMS set PROGRAM_NAME=\"%s\",\
                             PROVIDER_ID=1",
			    DvtEscapeString(values[2]).toUtf8().constData());
      delete q;
      q=new QSqlQuery(sql);
      delete q;
      sql=QString::asprintf("select ID from PROGRAMS where \
                             PROGRAM_NAME=\"%s\"",
			    DvtEscapeString(values[2]).toUtf8().constData());
      q=new QSqlQuery(sql);
      if(q->first()) {
	program_id=q->value(0).toInt();
      }
    }
    delete q;

    //
    // Build AIRINGS Entries
    //
    start_time=QTime();
    length=0;
    for(int i=0;i<7;i++) {
      if(values[3+i*4].toInt()!=0) {
	dow[i]=true;
	start_time=DvtSetTime(values[3+i*4+1]);
	length=start_time.secsTo(DvtSetTime(values[3+i*4+2]));
      }
      else {
	dow[i]=false;
      }
    }
    if(!start_time.isNull()) {
      sql=QString::asprintf("insert into AIRINGS set AFFILIATE_ID=%d,\
                             PROGRAM_ID=%d,AIR_TIME=\"%s\",AIR_LENGTH=%d,",
			    values[1].toInt(),program_id,
			    start_time.toString("hh:mm:ss").
			    toUtf8().constData(),
			    length);
      if(dow[0]) {
	sql+="AIR_MON=\"Y\",";
      }
      if(dow[1]) {
	sql+="AIR_TUE=\"Y\",";
      }
      if(dow[2]) {
	sql+="AIR_WED=\"Y\",";
      }
      if(dow[3]) {
	sql+="AIR_THU=\"Y\",";
      }
      if(dow[4]) {
	sql+="AIR_FRI=\"Y\",";
      }
      if(dow[5]) {
	sql+="AIR_SAT=\"Y\",";
      }
      if(dow[6]) {
	sql+="AIR_SUN=\"Y\",";
      }
      sql=sql.left(sql.length()-1);
      q=new QSqlQuery(sql);
      delete q;
    }
  }


  fclose(f);
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);
  new MainObject();
  return a.exec();
}
