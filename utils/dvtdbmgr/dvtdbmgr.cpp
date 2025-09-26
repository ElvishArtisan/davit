// dvtdbmgr.cpp
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

#include "dvtdbmgr.h"

MainObject::MainObject()
  : QObject()
{
  QString err_msg;

  d_command=MainObject::NoCommand;
  d_yes=false;
  d_no=false;

  //
  // Read Configuration
  //
  DvtCmdSwitch *cmd=
    new DvtCmdSwitch("dvtdbmgr",DVTDBMGR_USAGE);
  dvtconfig=new DvtConfig();
  dvtconfig->load(cmd);

  //
  // Read Command Options
  //
  for(int i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--check") {
      if(d_command!=MainObject::NoCommand) {
	fprintf(stderr,"dvtdbmgr: only one command may be specified (--check, --create or --modify\n");
	exit(1);
      }
      d_command=MainObject::CheckCommand;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--create") {
      if(d_command!=MainObject::NoCommand) {
	fprintf(stderr,"dvtdbmgr: only one command may be specified (--check, --create or --modify\n");
	exit(1);
      }
      d_command=MainObject::CreateCommand;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--modify") {
      if(d_command!=MainObject::NoCommand) {
	fprintf(stderr,"dvtdbmgr: only one command may be specified (--check, --create or --modify\n");
	exit(1);
      }
      d_command=MainObject::ModifyCommand;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--yes") {
      d_yes=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--no") {
      d_no=true;
      cmd->setProcessed(i,true);
    }
    if(!cmd->processed(i)) {
      fprintf(stderr,"dvtdbmgr: unrecognized option \"%s\"\n",
	      cmd->key(i).toUtf8().constData());
      exit(1);
    }
  }
  if(d_yes&&d_no) {
    fprintf(stderr,"dvtdbmgr: '--yes' and '--no' are mutually exclusive\n");
    exit(256);
  }

  //
  // Check for Root Perms
  //
  if(geteuid()!=0) {
    fprintf(stderr,"dvtdbmgr: must be user \"root\"\n");
    exit(256);
  }

  //
  // Open Database
  //
  if(dvtconfig->mysqlParamsOveridden()) {
    printf("Overriding configured database parameters!\n");
    printf("  ServerType = %s\n",dvtconfig->mysqlDbname().toUtf8().constData());
    printf("  Hostname = %s\n",dvtconfig->mysqlHostname().toUtf8().constData());
    printf("  Loginname = %s\n",
	   dvtconfig->mysqlUsername().toUtf8().constData());
    printf("  Password = %s\n",dvtconfig->mysqlPassword().toUtf8().constData());
    if(dvtconfig->mysqlConnectionTimeout()<0) {
      printf("  ConnectionTimeout = [default]\n");
    }
    else {
      printf("  ConnectionTimeout = %d\n",dvtconfig->mysqlConnectionTimeout());
    }
  }
  if(!DvtOpenDb(&err_msg,dvtconfig)) {
    fprintf(stderr,"dvtdbmgr: unable to connect to database [%s]\n",
	    err_msg.toUtf8().constData());
    exit(256);
  }

  //
  // Dispatch Command
  //
  switch(d_command) {
  case MainObject::NoCommand:
    PrintStatus();
    break;

  case MainObject::ModifyCommand:
    Modify();
    break;

  case MainObject::CreateCommand:
    Create();
    break;

  case MainObject::CheckCommand:
    Check();
    break;
  }

  exit(0);
}


int MainObject::GetCurrentSchema() const
{
  int ret=0;

  QString sql=QString("select `DB` from `VERSION`");
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  if(q->first()) {
    ret=q->value(0).toInt();
  }
  delete q;

  return ret;
}


void MainObject::PrintStatus() const
{
  QString sql;
  DvtSqlQuery *q;
  bool found_version=false;
  bool found_affiliates=false;
  
  if(DbIsEmpty()) {
    printf("empty database\n");
    return;
  }
  sql=QString("show tables");
  q=new DvtSqlQuery(sql,false);
  while(q->next()) {
    if(q->value(0).toString()=="VERSION") {
      found_version=true;
    }
    if(q->value(0).toString()=="AFFILIATES") {
      found_affiliates=true;
    }
  }
  delete q;

  if((!found_version)||(!found_affiliates)) {
    printf("not a Davit database!\n");
    return;
  }
  sql=QString("select ")+
    "`DB` "+  // 00
    "from `VERSION`";
  q=new DvtSqlQuery(sql);
  if(q->first()) {
    printf("Davit database, schema %d\n",q->value(0).toInt());
    exit(0);
  }
  fprintf(stderr,"dvtdbmgr: failed to read schema version\n");
  exit(1);
}


bool MainObject::DbIsEmpty() const
{
  bool ret;
  
  QString sql=QString("show tables");
  DvtSqlQuery *q=new DvtSqlQuery(sql,false);
  ret=q->size()==0;
  delete q;

  return ret;
}


bool MainObject::UserResponse() const
{
  char c=0;

  if(d_yes) {
    printf("y\n");
    return true;
  }
  if(d_no) {
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
