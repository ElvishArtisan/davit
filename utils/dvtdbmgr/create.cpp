// create.cpp
//
// Methods for creating databases
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
#include <QSqlError>

#include <dvtconf.h>
#include <dvtdb.h>

#include "dvtdbmgr.h"

void MainObject::Create() const
{
  if(!DbIsEmpty()) {
    fprintf(stderr,"dvtdbmgr: database is not empty, initialization aborted\n");
    exit(1);
  }
  CreateDb();
  InitDb();
}


bool MainObject::CreateDb() const
{
  //
  // Create an "empty" Version 22 Database
  //
  QString sql;

  //
  // Create AFFILIATES Table
  //
  sql=QString("create table if not exists `AFFILIATES` (")+
    "`ID` int auto_increment not null primary key,"+
    "`BIA_CODE` char(10),"+
    "`STATION_CALL` char(10) not null,"+
    "`STATION_PREV_CALL` char(10),"+
    "`STATION_TYPE` char(1),"+
    "`STATION_POWER` int,"+
    "`STATION_NIGHT_POWER` int,"+
    "`STATION_FREQUENCY` double,"+
    "`STATION_FORMAT` char(64),"+
    "`LICENSE_CITY` char(64),"+
    "`LICENSE_STATE` char(2),"+
    "`LICENSE_COUNTRY` char(2),"+
    "`BUSINESS_NAME` char(64),"+
    "`ADDRESS1` char(64),"+
    "`ADDRESS2` char(64),"+
    "`CITY` char(64),"+
    "`STATE` char(2),"+
    "`ZIPCODE` char(10),"+
    "`USER_PASSWORD` char(41),"+
    "`COUNTRY` char(2),"+
    "`PHONE` char(20),"+
    "`FAX` char(20),"+
    "`EMAIL_ADDR` char(64),"+
    "`WEB_URL` char(64),"+
    "`MARKET_NAME` char(64),"+
    "`MARKET_RANK` int default 0,"+
    "`DMA_NAME` char(64),"+
    "`DMA_RANK` int default 0,"+
    "`TIME_ZONE` char(8) default 'EDT',"+
    "`IS_AFFILIATE` enum('N','Y') default 'N',"+
    "`AFFIDAVIT_ACTIVE` enum('N','Y') default 'N',"+
    "`SECOND_NETWORK_ID` int,"+
    "`AFFADAVIT_CONTACT` int default 2,"+
    "index `IMPORT_IDX` (`STATION_FREQUENCY`,`LICENSE_CITY`,`LICENSE_STATE`),"+
    "index `IS_AFFILIATE_IDX` (`IS_AFFILIATE`),"+
    "index `AFFIDAVIT_ACTIVE_IDX` (`AFFIDAVIT_ACTIVE`))";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create AFFILIATE_REMARKS Table
  //
  sql=QString("create table if not exists `AFFILIATE_REMARKS` (")+
    "`ID` int not null primary key auto_increment,"+
    "`EVENT_TYPE` int not null default 0,"+
    "`AFFILIATE_ID` int not null,"+
    "`PROGRAM_ID` int,"+
    "`REMARK_DATETIME` datetime not null,"+
    "`USER_NAME` char(40) not null,"+
    "`REMARK` text,"+
    "index `AFFILIATE_IDX`(`AFFILIATE_ID`,`REMARK_DATETIME`))";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create AIRED Table
  //
  sql=QString("create table if not exists `AIRED` (")+
    "`ID` int auto_increment not null primary key,"+
    "`PROGRAM_ID` int not null,"+
    "`AFFILIATE_ID` int not null,"+
    "`STATE` int not null default 2,"+
    "`AIR_DATETIME` datetime,"+
    "`AIR_LENGTH` int,"+
    "`ORIGIN_DATETIME` datetime,"+
    "`CONTACT_NAME` char(64),"+
    "`CONTACT_PHONE` char(20),"+
    "`CONTACT_FAX` char(20),"+
    "`CONTACT_EMAIL` char(64),"+
    "`STAMP_DATETIME` datetime,"+
    "index `PROGRAM_ID_IDX` (`PROGRAM_ID`),"+
    "index `AFFILIATE_ID_IDX` (`AFFILIATE_ID`),"+
    "index `AFFIDAVIT_IDX` (`AFFILIATE_ID`,`PROGRAM_ID`,`AIR_DATETIME`),"+
    "index `STATE_IDX` (`STATE`,`AIR_DATETIME`))";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create AIRINGS Table
  //
  sql=QString("create table if not exists `AIRINGS` (")+
    "`ID` int auto_increment not null primary key,"+
    "`PROGRAM_ID` int not null,"+
    "`AFFILIATE_ID` int not null,"+
    "`AIR_SUN` enum('N','Y') default 'N',"+
    "`AIR_MON` enum('N','Y') default 'N',"+
    "`AIR_TUE` enum('N','Y') default 'N',"+
    "`AIR_WED` enum('N','Y') default 'N',"+
    "`AIR_THU` enum('N','Y') default 'N',"+
    "`AIR_FRI` enum('N','Y') default 'N',"+
    "`AIR_SAT` enum('N','Y') default 'N',"+
    "`AIR_TIME` time,"+
    "`AIR_LENGTH` int,"+
    "index `PROGRAM_ID_IDX` (`PROGRAM_ID`),"+
    "index `AFFILIATE_ID_IDX` (`AFFILIATE_ID`))";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create CONTACTS Table
  //
  sql=QString("create table if not exists `CONTACTS` (")+
    "`ID` int not null primary key auto_increment,"+
    "`AFFILIATE_ID` int,"+
    "`NAME` char(64),"+
    "`TITLE` char(64),"+
    "`PHONE` char(64),"+
    "`FAX` char(64),"+
    "`EMAIL` char(64),"+
    "`AFFIDAVIT` enum('N','Y') default 'N',"+
    "`PROGRAM_DIRECTOR` enum('N','Y') default 'N',"+
    "`GENERAL_MANAGER` enum('N','Y') default 'N',"+
    "`LOCKED` enum('N','Y') default 'N',"+
    "index `AFFILIATE_ID_IDX` (`AFFILIATE_ID`))";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create DMA_MARKETS Table
  //
  sql=QString("create table if not exists `DMA_MARKETS` (")+
    "`ID` int not null primary key auto_increment,"+
    "`NAME` char(64) unique not null,"+
    "`RANK` int,"+
    "index `NAME_IDX`(`NAME`))";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create FEEDS Table
  //
  sql=QString("create table if not exists `FEEDS` (")+
    "`ID` int auto_increment not null primary key,"+
    "`PROGRAM_ID` int not null,"+
    "`AIR_SUN` enum('N','Y') default 'N',"+
    "`AIR_MON` enum('N','Y') default 'N',"+
    "`AIR_TUE` enum('N','Y') default 'N',"+
    "`AIR_WED` enum('N','Y') default 'N',"+
    "`AIR_THU` enum('N','Y') default 'N',"+
    "`AIR_FRI` enum('N','Y') default 'N',"+
    "`AIR_SAT` enum('N','Y') default 'N',"+
    "`AIR_TIME` time,"+
    "index `PROGRAM_ID_IDX` (`PROGRAM_ID`))";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create MSA_MARKETS Table
  //
  sql=QString("create table if not exists `MSA_MARKETS` (")+
    "`ID` int not null primary key auto_increment,"+
    "`NAME` char(64) unique not null,"+
    "`RANK` int,"+
    "index `NAME_IDX`(`NAME`))";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create NETWORKS Table
  //
  sql=QString("create table if not exists `NETWORKS` (")+
    "`ID` int not null primary key auto_increment,"+
    "`NAME` char(64) unique not null,"+
    "index `NAME_IDX` (`NAME`))";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create PROGRAMS Table
  //
  sql=QString("create table if not exists `PROGRAMS` (")+
    "`ID` int auto_increment not null primary key,"+
    "`PROVIDER_ID` int not null,"+
    "`PROGRAM_NAME` char(64) unique,"+
    "`AIR_LENGTH` int,"+
    "`CONTACT_NAME` char(64),"+
    "`CONTACT_PHONE` char(20),"+
    "`CONTACT_FAX` char(20),"+
    "`CONTACT_EMAIL` char(64),"+
    "`PRIMARY_ISDN_NUMBER` char(20),"+
    "`PRIMARY_ISDN_TX_ALGO` char(1) default 'G',"+
    "`PRIMARY_ISDN_RX_ALGO` char(1) default 'G',"+
    "`PRIMARY_ISDN_BITRATE` int default 64000,"+
    "`PRIMARY_ISDN_SAMPLERATE` int default 32000,"+
    "`PRIMARY_ISDN_CHANNELS` int default 1,"+
    "`SECONDARY_ISDN_NUMBER` char(20),"+
    "`SECONDARY_ISDN_TX_ALGO` char(1) default 'G',"+
    "`SECONDARY_ISDN_RX_ALGO` char(1) default 'G',"+
    "`SECONDARY_ISDN_BITRATE` int default 64000,"+
    "`SECONDARY_ISDN_SAMPLERATE` int default 32000,"+
    "`SECONDARY_ISDN_CHANNELS` int default 1,"+
    "`REMARKS` char(255),"+
    "index `PROGRAM_NAME_IDX` (`PROGRAM_NAME`))";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create PROVIDERS Table
  //
  sql=QString("create table if not exists `PROVIDERS` (")+
    "`ID` int auto_increment not null primary key,"+
    "`BUSINESS_NAME` char(64) unique,"+
    "`ADDRESS1` char(64),"+
    "`ADDRESS2` char(64),"+
    "`CITY` char(64),"+
    "`STATE` char(2),"+
    "`ZIPCODE` char(10),"+
    "`CONTACT_NAME` char(64),"+
    "`CONTACT_PHONE` char(20),"+
    "`CONTACT_FAX` char(20),"+
    "`CONTACT_EMAIL` char(64),"+
    "index `BUSINESS_NAME_IDX` (`BUSINESS_NAME`))";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create SYSTEM Table
  //
  sql=QString("create table if not exists `SYSTEM` (")+
    "`ID` int not null primary key auto_increment,"+
    "`SMTP_HOSTNAME` char(255),"+
    "`SMTP_PORT` int default 25,"+
    "`ORIGIN_EMAIL` char(64),"+
    "`ALERT_EMAIL` char(64),"+
    "`AFFIDAVIT_EMAIL_SUBJECT` char(64),"+
    "`AFFIDAVIT_EMAIL_TEMPLATE` text)";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create USERS Table
  //
  sql=QString("create table if not exists `USERS` (")+
    "`USER_NAME` char(40) not null primary key,"+
    "`USER_PASSWORD` char(41),"+
    "`FULL_NAME` char(64),"+
    "`DESCRIPTION` char(64),"+
    "`PHONE_NUMBER` char(20),"+
    "`EMAIL` char(64),"+
    "`ADMIN_PRIV` enum('N','Y') default 'N',"+
    "`AFFILIATE_VIEW_PRIV` enum('N','Y') default 'N',"+
    "`AFFILIATE_EDIT_PRIV` enum('N','Y') default 'N',"+
    "`AFFILIATE_SCHEDULE_PRIV` enum('N','Y') default 'N',"+
    "`AFFILIATE_REMARK_PRIV` enum('N','Y') default 'N',"+
    "`PROVIDER_VIEW_PRIV` enum('N','Y') default 'N',"+
    "`PROVIDER_EDIT_PRIV` enum('N','Y') default 'N',"+
    "`PROGRAM_VIEW_PRIV` enum('N','Y') default 'N',"+
    "`PROGRAM_EDIT_PRIV` enum('N','Y') default 'N',"+
    "`REPORT_VIEW_PRIV` enum('N','Y') default 'N')";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create VERSION Table
  //
  sql=QString("create table if not exists `VERSION` (")+
    "`DB` int not null primary key)";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create WEB_CONNECTIONS Table
  //
  sql=QString("create table if not exists `WEB_CONNECTIONS` (")+
    "`SESSION_ID` int unsigned not null primary key,"+
    "`AFFILIATE_ID` int,"+
    "`IP_ADDRESS` char(16),"+
    "`TIME_STAMP` datetime)";
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  return true;
}


bool MainObject::InitDb() const
{
  //
  // Initialize an "empty" Version 22 Database
  //
  // Specifically, this will create a default user with administrative
  // permissions.
  //
  QString sql;

  //
  // Generate Version Number
  //
  sql=QString("insert into `VERSION` ")+
    QString::asprintf("(`DB`) values (%d)",DVT_VERSION_DATABASE_BASE);
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  //
  // Create Default User
  //
  sql=QString("insert into `USERS` set ")+
    "`USER_NAME`="+DvtSqlQuery::escape(DVT_DEFAULT_LOGIN_NAME)+","+
    "`FULL_NAME`="+DvtSqlQuery::escape(DVT_DEFAULT_FULL_NAME)+","+
    "`DESCRIPTION`="+DvtSqlQuery::escape(DVT_DEFAULT_DESCRIPTION_NAME)+","+
    "`USER_PASSWORD`=password(''),"+
    "`ADMIN_PRIV`='Y'";
  printf("SQL: %s\n",sql.toUtf8().constData());
  if(!DvtSqlQuery::apply(sql)) {
    return false;
  }

  return true;
}
