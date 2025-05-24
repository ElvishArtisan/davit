// createdb.cpp
//
// Create, Initialize and/or Update a Davit Database
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

#include <stdlib.h>
#ifndef WIN32
#include <netdb.h>
#include <sys/socket.h>
#endif  // WIN32

#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <dvt.h>
#include <dvtconf.h>

#include "createdb.h"

bool RunQuery(QString sql)
{
  QSqlQuery *q=new QSqlQuery(sql);
  if(!q->isActive()) {
    printf("SQL: %s\n",sql.toUtf8().constData());
    printf("SQL Error: %s\n",q->lastError().databaseText().toUtf8().constData());
    delete q;
    return false;
  }
  delete q;
  return true;
}


bool CreateDb(QString name,QString pwd)
{
  QString sql;

  //
  // Create VERSION Table
  //
  sql=QString("create table if not exists VERSION (\
               DB int not null primary key)");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create AFFILIATES Table
  //
  sql=QString("create table if not exists AFFILIATES (\
               ID int auto_increment not null primary key,\
               STATION_CALL char(10) not null,\
               STATION_TYPE char(1),\
               STATION_POWER int,\
               LICENSE_CITY char(64),\
               LICENSE_STATE char(2),\
               LICENSE_COUNTRY char(2),\
               BUSINESS_NAME char(64),\
               ADDRESS1 char(64),\
               ADDRESS2 char(64),\
               CITY char(64),\
               STATE char(2),\
               ZIPCODE char(10),\
               USER_PASSWORD char(41),\
               COUNTRY char(2),\
               CONTACT_NAME char(64),\
               CONTACT_PHONE char(20),\
               CONTACT_FAX char(20),\
               CONTACT_EMAIL char(64))");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create PROVIDERS Table
  //
  sql=QString("create table if not exists PROVIDERS (\
               ID int auto_increment not null primary key,\
               BUSINESS_NAME char(64) unique,\
               ADDRESS1 char(64),\
               ADDRESS2 char(64),\
               CITY char(64),\
               STATE char(2),\
               ZIPCODE char(10),\
               CONTACT_NAME char(64),\
               CONTACT_PHONE char(20),\
               CONTACT_FAX char(20),\
               CONTACT_EMAIL char(64),\
               index BUSINESS_NAME_IDX (BUSINESS_NAME))");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create PROGRAMS Table
  //
  sql=QString("create table if not exists PROGRAMS (\
               ID int auto_increment not null primary key,\
               PROVIDER_ID int not null,\
               PROGRAM_NAME char(64) unique,\
               AIR_LENGTH int,\
               CONTACT_NAME char(64),\
               CONTACT_PHONE char(20),\
               CONTACT_FAX char(20),\
               CONTACT_EMAIL char(64),\
               PRIMARY_ISDN_NUMBER char(20),\
               PRIMARY_ISDN_TX_ALGO char(1) default \"G\",\
               PRIMARY_ISDN_RX_ALGO char(1) default \"G\",\
               PRIMARY_ISDN_BITRATE int default 64000,\
               PRIMARY_ISDN_SAMPLERATE int default 32000,\
               PRIMARY_ISDN_CHANNELS int default 1,\
               SECONDARY_ISDN_NUMBER char(20),\
               SECONDARY_ISDN_TX_ALGO char(1) default \"G\",\
               SECONDARY_ISDN_RX_ALGO char(1) default \"G\",\
               SECONDARY_ISDN_BITRATE int default 64000,\
               SECONDARY_ISDN_SAMPLERATE int default 32000,\
               SECONDARY_ISDN_CHANNELS int default 1,\
               REMARKS char(255),\
               index PROGRAM_NAME_IDX (PROGRAM_NAME))");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create FEEDS Table
  //
  sql=QString("create table if not exists FEEDS (\
               ID int auto_increment not null primary key,\
               PROGRAM_ID int not null,\
               AIR_SUN enum('N','Y') default 'N',\
               AIR_MON enum('N','Y') default 'N',\
               AIR_TUE enum('N','Y') default 'N',\
               AIR_WED enum('N','Y') default 'N',\
               AIR_THU enum('N','Y') default 'N',\
               AIR_FRI enum('N','Y') default 'N',\
               AIR_SAT enum('N','Y') default 'N',\
               AIR_TIME time,\
               index PROGRAM_ID_IDX (PROGRAM_ID))");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create AIRINGS Table
  //
  sql=QString("create table if not exists AIRINGS (\
               ID int auto_increment not null primary key,\
               PROGRAM_ID int not null,\
               AFFILIATE_ID int not null,\
               AIR_SUN enum('N','Y') default 'N',\
               AIR_MON enum('N','Y') default 'N',\
               AIR_TUE enum('N','Y') default 'N',\
               AIR_WED enum('N','Y') default 'N',\
               AIR_THU enum('N','Y') default 'N',\
               AIR_FRI enum('N','Y') default 'N',\
               AIR_SAT enum('N','Y') default 'N',\
               AIR_TIME time,\
               index PROGRAM_ID_IDX (PROGRAM_ID),\
               index AFFILIATE_ID_IDX (AFFILIATE_ID))");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create AIRED Table
  //
  sql=QString("create table if not exists AIRED (\
               ID int auto_increment not null primary key,\
               PROGRAM_ID int not null,\
               AFFILIATE_ID int not null,\
               AIR_DATETIME datetime,\
               CONTACT_NAME char(64),\
               CONTACT_PHONE char(20),\
               CONTACT_FAX char(20),\
               CONTACT_EMAIL char(64),\
               index PROGRAM_ID_IDX (PROGRAM_ID),\
               index AFFILIATE_ID_IDX (AFFILIATE_ID))");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create USERS Table
  //
  sql=QString("create table if not exists USERS (\
               USER_NAME char(40) not null primary key,\
               USER_PASSWORD char(41),\
               FULL_NAME char(64),\
               DESCRIPTION char(64),\
               PHONE_NUMBER char(20),\
               ADMIN_PRIV enum('N','Y') default 'N',\
               AFFILIATE_VIEW_PRIV enum('N','Y') default 'N',\
               AFFILIATE_EDIT_PRIV enum('N','Y') default 'N',\
               AFFILIATE_SCHEDULE_PRIV enum('N','Y') default 'N',\
               AFFILIATE_REMARK_PRIV enum('N','Y') default 'N',\
               PROVIDER_VIEW_PRIV enum('N','Y') default 'N',\
               PROVIDER_EDIT_PRIV enum('N','Y') default 'N',\
               PROGRAM_VIEW_PRIV enum('N','Y') default 'N',\
               PROGRAM_EDIT_PRIV enum('N','Y') default 'N')");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create AFFILIATE_REMARKS Table
  //
  sql=QString("create table if not exists AFFILIATE_REMARKS (\
               ID int not null primary key auto_increment,\
               EVENT_TYPE int not null default 0,\
               AFFILIATE_ID int not null,\
               PROGRAM_ID int,\
               REMARK_DATETIME datetime not null,\
               USER_NAME char(40) not null,\
               REMARK text,\
               index AFFILIATE_IDX(AFFILIATE_ID,REMARK_DATETIME))");
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create MSA_MARKETS Table
  //
  sql="create table if not exists MSA_MARKETS (\
       ID int not null primary key auto_increment,\
       NAME char(64) unique not null,\
       index NAME_IDX(name))";
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create DMA_MARKETS Table
  //
  sql="create table if not exists DMA_MARKETS (\
       ID int not null primary key auto_increment,\
       NAME char(64) unique not null,\
       index NAME_IDX(name))";
  if(!RunQuery(sql)) {
    return false;
  }

  UpdateDb(1);

  return true;
}


bool InitDb(QString name,QString pwd)
{
  QString sql;

  //
  // Generate Version Number
  //
  sql="insert into VERSION (DB) values (1)";
  if(!RunQuery(sql)) {
    return false;
  }

  //
  // Create Default User
  //
  sql=QString().sprintf("insert into USERS set\
                         USER_NAME=\"%s\",\
                         FULL_NAME=\"%s\",\
                         DESCRIPTION=\"%s\",\
                         USER_PASSWORD=password(\"\"),\
                         ADMIN_PRIV=\"Y\"",
			DVT_DEFAULT_LOGIN_NAME,
			DVT_DEFAULT_FULL_NAME,
			DVT_DEFAULT_DESCRIPTION_NAME);
  if(!RunQuery(sql)) {
    return false;
  }

  return true;
}


bool UpdateDb(int ver)
{
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;

  // **** Start of version updates ****

  if(ver<1) {
  }

  if(ver<2) {
    sql="alter table AFFILIATES add column STATION_PREV_CALL char(10)\
         after STATION_CALL";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column STATION_NIGHT_POWER int\
         after STATION_POWER";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column MARKET_NAME char(64)\
         after COUNTRY";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column MARKET_RANK int default 0\
         after MARKET_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column DMA_RANK int default 0\
         after MARKET_RANK";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column TIME_ZONE char(8) default \"EDT\"\
         after DMA_RANK";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column SECOND_NETWORK_ID int\
         after TIME_ZONE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT_TITLE char(64)\
         after CONTACT_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT1_NAME char(64)\
         after CONTACT_EMAIL";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT1_TITLE char(64)\
         after CONTACT1_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT1_PHONE char(64)\
         after CONTACT1_TITLE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT1_FAX char(64)\
         after CONTACT1_PHONE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT1_EMAIL char(64)\
         after CONTACT1_FAX";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT2_NAME char(64)\
         after CONTACT1_EMAIL";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT2_TITLE char(64)\
         after CONTACT2_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT2_PHONE char(64)\
         after CONTACT2_TITLE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT2_FAX char(64)\
         after CONTACT2_PHONE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT2_EMAIL char(64)\
         after CONTACT2_FAX";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT3_NAME char(64)\
         after CONTACT2_EMAIL";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT3_TITLE char(64)\
         after CONTACT3_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT3_PHONE char(64)\
         after CONTACT3_TITLE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT3_FAX char(64)\
         after CONTACT3_PHONE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT3_EMAIL char(64)\
         after CONTACT3_FAX";
    q=new QSqlQuery(sql);
    delete q;

    sql="create table if not exists NETWORKS ( \
         ID int not null primary key auto_increment,\
         NAME char(64) unique not null,\
         INDEX NAME_IDX(NAME))";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<3) {
    sql="alter table AFFILIATES add column BIA_CODE char(10) after ID";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES alter column STATION_TYPE char(2)"; 
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column STATION_FREQUENCY real \
         after STATION_NIGHT_POWER";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column STATION_FORMAT char(64) \
         after STATION_FREQUENCY";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column PHONE char(20) after COUNTRY";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column FAX char(20) after PHONE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column EMAIL_ADDR char(64) after FAX";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column WEB_URL char(64) after EMAIL_ADDR";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT_FIRST_NAME char(64) \
         after CONTACT_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT_LAST_NAME char(64) \
         after CONTACT_FIRST_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT1_FIRST_NAME char(64) \
         after CONTACT1_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT1_LAST_NAME char(64) \
         after CONTACT1_FIRST_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT2_FIRST_NAME char(64) \
         after CONTACT2_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT2_LAST_NAME char(64) \
         after CONTACT2_FIRST_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT3_FIRST_NAME char(64) \
         after CONTACT3_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column CONTACT3_LAST_NAME char(64) \
         after CONTACT3_FIRST_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT1_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT2_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT3_NAME";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<4) {
    sql=QString("create table if not exists WEB_CONNECTIONS (\
        SESSION_ID int unsigned not null primary key,\
        AFFILIATE_ID int,\
        IP_ADDRESS char(16),\
        TIME_STAMP datetime)");
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<5) {
    sql="alter table AIRINGS add column AIR_LENGTH int after AIR_TIME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AIRED add column AIR_LENGTH int after AIR_DATETIME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column AFFADAVIT_CONTACT int default 2 \
         after SECOND_NETWORK_ID";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<6) {
    sql="alter table AIRED add column ORIGIN_DATETIME datetime \
         after AIR_LENGTH";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<7) {
    sql=QString("create table if not exists AFFILIATE_REMARKS (\
                 ID int not null primary key auto_increment,\
                 AFFILIATE_ID int not null,\
                 REMARK_DATETIME datetime not null,\
                 USER_NAME char(40) not null,\
                 REMARK text,\
                 index AFFILIATE_IDX(AFFILIATE_ID,REMARK_DATETIME))");
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<8) {
    sql="alter table AFFILIATE_REMARKS add column EVENT_TYPE int not null \
         default 0 after ID";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATE_REMARKS add column PROGRAM_ID int \
         after AFFILIATE_ID";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<9) {
    sql="alter table USERS add column AFFILIATE_VIEW_PRIV enum('N','Y') \
               default 'N' after ADMIN_PRIV";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table USERS add column AFFILIATE_EDIT_PRIV enum('N','Y') \
               default 'N' after AFFILIATE_VIEW_PRIV";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table USERS add column AFFILIATE_SCHEDULE_PRIV enum('N','Y') \
               default 'N' after AFFILIATE_EDIT_PRIV";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table USERS add column AFFILIATE_REMARK_PRIV enum('N','Y') \
               default 'N' after AFFILIATE_SCHEDULE_PRIV";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table USERS add column PROVIDER_VIEW_PRIV enum('N','Y') \
               default 'N' after AFFILIATE_REMARK_PRIV";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table USERS add column PROVIDER_EDIT_PRIV enum('N','Y') \
               default 'N' after PROVIDER_VIEW_PRIV";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table USERS add column PROGRAM_VIEW_PRIV enum('N','Y') \
               default 'N' after PROVIDER_EDIT_PRIV";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table USERS add column PROGRAM_EDIT_PRIV enum('N','Y') \
               default 'N' after PROGRAM_VIEW_PRIV";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table USERS add column REPORT_VIEW_PRIV enum('N','Y') \
               default 'N' after PROGRAM_EDIT_PRIV";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<10) {
    sql="alter table AFFILIATES add column AFFIDAVIT_ACTIVE enum('N','Y') \
         default 'N' after TIME_ZONE";
    q=new QSqlQuery(sql);
    delete q;

    sql="select AIRINGS.ID from AIRINGS left join PROGRAMS \
         on AIRINGS.PROGRAM_ID=PROGRAMS.ID where PROGRAMS.ID is null";
    q=new QSqlQuery(sql);
    while(q->next()) {
      sql=QString().sprintf("delete from AIRINGS where ID=%u",
			    q->value(0).toUInt());
      q1=new QSqlQuery(sql);
      delete q1;
    }
    delete q;

    sql="select AFFILIATE_ID from AIRINGS";
    q=new QSqlQuery(sql);
    while(q->next()) {
      sql=QString().
	sprintf("update AFFILIATES set AFFIDAVIT_ACTIVE=\"Y\" where ID=%u",
		q->value(0).toUInt());
      q1=new QSqlQuery(sql);
      delete q1;
    }
    delete q;
  }

  if(ver<11) {
    sql="create table if not exists CONTACTS (\
         ID int auto_increment not null primary key,\
         AFFILIATE_ID int,\
         NAME char(64),\
         TITLE char(64),\
         PHONE char(64),\
         FAX char(64),\
         EMAIL char(64),\
         AFFIDAVIT enum('N','Y') default 'N',\
         index AFFILIATE_ID_IDX(AFFILIATE_ID))";
    q=new QSqlQuery(sql);
    delete q;
    sql="select ID,AFFADAVIT_CONTACT,CONTACT_LAST_NAME,\
         CONTACT_TITLE,CONTACT_PHONE,CONTACT_FAX,CONTACT_EMAIL,\
         CONTACT1_LAST_NAME,\
         CONTACT1_TITLE,CONTACT1_PHONE,CONTACT1_FAX,CONTACT1_EMAIL,\
         CONTACT2_LAST_NAME,\
         CONTACT2_TITLE,CONTACT2_PHONE,CONTACT2_FAX,CONTACT2_EMAIL,\
         CONTACT3_LAST_NAME,\
         CONTACT3_TITLE,CONTACT3_PHONE,CONTACT3_FAX,CONTACT3_EMAIL \
         from AFFILIATES";
    q=new QSqlQuery(sql);
    while(q->next()) {
      for(int i=0;i<4;i++) {
	if((!q->value(2+5*i).toString().isEmpty())||
	   (!q->value(3+5*i).toString().isEmpty())||
	   (!q->value(4+5*i).toString().isEmpty())||
	   (!q->value(5+5*i).toString().isEmpty())||
	   (!q->value(6+5*i).toString().isEmpty())) {
	  char affidavit='N';
	  if(q->value(1).toInt()==i) {
	    affidavit='Y';
	  }
	  sql=QString().sprintf("insert into CONTACTS set AFFILIATE_ID=%d,\
                                 NAME=\"%s\",\
                                 TITLE=\"%s\",\
                                 PHONE=\"%s\",\
                                 FAX=\"%s\",\
                                 EMAIL=\"%s\",\
                                 AFFIDAVIT=\"%c\"",
				q->value(0).toInt(),
				DvtEscapeString(q->value(2+5*i).
						toString()).toUtf8().constData(),
				DvtEscapeString(q->value(3+5*i).
						toString()).toUtf8().constData(),
				DvtEscapeString(q->value(4+5*i).
						toString()).toUtf8().constData(),
				DvtEscapeString(q->value(5+5*i).
						toString()).toUtf8().constData(),
				DvtEscapeString(q->value(6+5*i).
						toString()).toUtf8().constData(),
				affidavit);
	  q1=new QSqlQuery(sql);
	  delete q1;
	}
      }
    }
    delete q;
  }

  if(ver<12) {
    sql="alter table USERS add column EMAIL char(64) after PHONE_NUMBER";
    q=new QSqlQuery(sql);
    delete q;

    sql="create table if not exists SYSTEM (\
         ID int auto_increment not null primary key,\
         ORIGIN_EMAIL char(64),\
         AFFIDAVIT_EMAIL_SUBJECT char(64),\
         AFFIDAVIT_EMAIL_TEMPLATE text)";
    q=new QSqlQuery(sql);
    delete q;

    sql="insert into SYSTEM set ORIGIN_EMAIL=\"\",\
        AFFIDAVIT_EMAIL_TEMPLATE=\"\"";
    q=new QSqlQuery(sql);
    delete q;

    sql="select ID,STATION_CALL,STATION_TYPE from AFFILIATES";
    q=new QSqlQuery(sql);
    while(q->next()) {
      sql=QString().
	sprintf("update AFFILIATES set USER_PASSWORD=\"%s-%sm\" where ID=%d",
		DvtEscapeString(q->value(1).toString().toLower()).toUtf8().constData(),
		q->value(2).toString().toLower().toUtf8().constData(),
		q->value(0).toInt());
      q1=new QSqlQuery(sql);
      delete q1;
    }
    delete q;
  }

  if(ver<13) {
    sql="alter table CONTACTS add column PROGRAM_DIRECTOR enum('N','Y') \
         default 'N' after AFFIDAVIT";
    q=new QSqlQuery(sql);
    delete q;
    /*
    sql="update CONTACTS set PROGRAM_DIRECTOR=\"Y\" \
         where TITLE=\"Program Director\"";
    q=new QSqlQuery(sql);
    delete q;
    */
  }

  if(ver<14) {
    sql="alter table CONTACTS add column LOCKED enum('N','Y') default 'N' \
         after PROGRAM_DIRECTOR";
    q=new QSqlQuery(sql);
    delete q;

    sql="update CONTACTS set LOCKED=\"Y\" where AFFIDAVIT=\"Y\"";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<15) {
    sql="alter table AFFILIATES add index \
         IMPORT_IDX (STATION_FREQUENCY,LICENSE_CITY,LICENSE_STATE)";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<16) {
    sql="alter table SYSTEM add column SMTP_HOSTNAME char(255) \
         after ID";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table SYSTEM add column SMTP_PORT int default 25 \
         after SMTP_HOSTNAME";
    q=new QSqlQuery(sql);
    delete q;
  }
    
  if(ver<17) {
    sql="alter table AFFILIATES add column DMA_NAME char(64) \
         after MARKET_RANK";
    q=new QSqlQuery(sql);
    delete q;

    sql="create table if not exists MSA_MARKETS (\
         ID int not null primary key auto_increment,\
         NAME char(64) unique not null,\
         index NAME_IDX(name))";
    q=new QSqlQuery(sql);
    delete q;

    sql="create table if not exists DMA_MARKETS (\
         ID int not null primary key auto_increment,\
         NAME char(64) unique not null,\
         index NAME_IDX(name))";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<18) {
    sql="alter table AIRED add column STAMP_DATETIME datetime \
         after CONTACT_EMAIL";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add column IS_AFFILIATE enum('N','Y') \
         default 'N' after TIME_ZONE";
    q=new QSqlQuery(sql);
    delete q;

    sql="update AFFILIATES set AFFIDAVIT_ACTIVE=\"N\"";
    q=new QSqlQuery(sql);
    delete q;
    DvtUpdateIsAffiliateField();
  }

  if(ver<19) {
    sql=
      "alter table SYSTEM add column ALERT_EMAIL char(64) after ORIGIN_EMAIL";
    q=new QSqlQuery(sql);
    delete q;

    //
    // Populate AIR_LENGTH for existing records
    //
    sql="select ID,AIR_LENGTH from PROGRAMS";
    q=new QSqlQuery(sql);
    while(q->next()) {
      sql=QString().
	sprintf("update AIRED set AIR_LENGTH=%d where PROGRAM_ID=%d",
		q->value(1).toInt(),q->value(0).toInt());
      q1=new QSqlQuery(sql);
      delete q1;
    }
    delete q;
    sql="select AFFILIATE_ID,PROGRAM_ID,AIR_LENGTH from AIRINGS";
    q=new QSqlQuery(sql);
    while(q->next()) {
      sql=QString().
	sprintf("update AIRED set AIR_LENGTH=%d \
                 where (PROGRAM_ID=%d)&&(AFFILIATE_ID=%d)",
		q->value(2).toInt(),
		q->value(1).toInt(),
		q->value(0).toInt());
      q1=new QSqlQuery(sql);
      delete q1;
    }
    delete q;

    //
    // Implement new affidavit tracking schema
    //
    sql="alter table AIRED add column STATE int default 2 not null \
         after AFFILIATE_ID";
    q=new QSqlQuery(sql);
    delete q;

    //
    // Synthesize records for "old" airings
    //
    QDate today=QDate::currentDate();
    QDate first(today.year(),today.month(),1);
    sql="select ID from AFFILIATES where IS_AFFILIATE=\"Y\"";
    q=new QSqlQuery(sql);
    for(int i=2008;i<=today.year();i++) {
      for(int j=1;j<=12;j++) {
	QDate date(i,j,1);
	if(date<first) {
	  q->seek(-1);
	  while(q->next()) {
	    sql=QString().sprintf("select ID from AIRED where \
                               (AFFILIATE_ID=%d)&&(PROGRAM_ID=-1)&&	\
                               ((AIR_DATETIME>=\"%s 00:00:00\")&&\
                                (AIR_DATETIME<\"%s 00:00:00\"))",
				  q->value(0).toInt(),
				  date.toString("yyyy-MM-dd").toUtf8().constData(),
				  date.addMonths(1).
				  toString("yyyy-MM-dd").toUtf8().constData());
	    q1=new QSqlQuery(sql);
	    if(!q1->first()) {   // Synthesize "Old" affidavit data
	      QString cmd=QString().
		sprintf("dvtstamp --for-date=%s --for-affiliate-id=%d",
			date.toString("yyyy-MM").toUtf8().constData(),
			q->value(0).toInt());
	      system(cmd.toUtf8());
	    }
	    delete q1;
	  }
	}
      }
    }
    delete q;

    //
    // Optimize Indexes
    //
    sql="alter table AIRED add index \
         AFFIDAVIT_IDX(AFFILIATE_ID,PROGRAM_ID,AIR_DATETIME)";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AIRED add index STATE_IDX(STATE,AIR_DATETIME)";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES add index IS_AFFILIATE_IDX(IS_AFFILIATE)";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES \
         add index AFFIDAVIT_ACTIVE_IDX(AFFIDAVIT_ACTIVE)";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<20) {
    sql="alter table AFFILIATES drop column CONTACT_FIRST_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT_LAST_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT_TITLE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT_PHONE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT_FAX";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT_EMAIL";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT1_FIRST_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT1_LAST_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT1_TITLE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT1_PHONE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT1_FAX";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT1_EMAIL";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT2_FIRST_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT2_LAST_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT2_TITLE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT2_PHONE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT2_FAX";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT2_EMAIL";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT3_FIRST_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT3_LAST_NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT3_TITLE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT3_PHONE";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT3_FAX";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table AFFILIATES drop column CONTACT3_EMAIL";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<21) {
    sql="alter table MSA_MARKETS add column RANK int after NAME";
    q=new QSqlQuery(sql);
    delete q;

    sql="alter table DMA_MARKETS add column RANK int after NAME";
    q=new QSqlQuery(sql);
    delete q;
  }

  if(ver<22) {
    sql="alter table CONTACTS add column GENERAL_MANAGER \
         enum('N','Y') default 'N' after PROGRAM_DIRECTOR";
    q=new QSqlQuery(sql);
    delete q;

    sql="update CONTACTS set GENERAL_MANAGER=\"Y\" \
         where TITLE=\"General Manager\"";
    q=new QSqlQuery(sql);
    delete q;
  }

  // **** End of version updates ****
  
  //
  // Update Version Field
  //
  q=new QSqlQuery(QString().sprintf("update VERSION set DB=%d",
				    DVT_VERSION_DATABASE));
  delete q;
  return true;
}
