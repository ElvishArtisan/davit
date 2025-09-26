// rddb.cpp
//
//   Database driver with automatic reconnect
//
//   (C) Copyright 2007 Dan Mills <dmills@exponent.myzen.co.uk>
//   (C) Copyright 2018-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <netdb.h>
#include <stdio.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <QObject>
#include <QString>
#include <QTranslator>
#include <QSqlError>
#include <QStringList>
#include <QVariant>

//#include "rdapplication.h"
#include "dvtdb.h"
//#include "rddbheartbeat.h"

DvtSqlQuery::DvtSqlQuery (const QString &query,bool reconnect):
  QSqlQuery(query)
{
  QSqlDatabase db;
  QString err;
  sql_columns=0;

  if (!isActive() && reconnect) {
    db = QSqlDatabase::database();

    if (db.open()) {
      clear();
      exec(query);
      err=QObject::tr("DB connection re-established");
    }
    else {
      err=QObject::tr("Could not re-establish DB connection")+
      +"["+db.lastError().text()+"]";
    }

    fprintf(stderr,"%s\n",err.toUtf8().constData());
    /*
    if(rda!=NULL) {
      rda->syslog(LOG_ERR,err.toUtf8().constData());
    }
    */
  }

  if(isActive()) {
    /*
    if((rda!=NULL)&&(rda->config()->logSqlQueries())) {
      rda->syslog(rda->config()->logSqlQueriesLevel(),
		  "SQL: %s",query.toUtf8().constData());
    }
    */
    //printf("QUERY: %s\n",(const char *)query.toUtf8());
    QStringList f0=query.split(" ");
    if(f0[0].toLower()=="select") {
      for(int i=1;i<f0.size();i++) {
	if(f0[i].toLower()=="from") {
	  QString fields;
	  for(int j=1;j<i;j++) {
	    fields+=f0[j];
	  }
	  QStringList f1=fields.split(",");
	  sql_columns=f1.size();
	  continue;
	}
      }
    }
  }
  else {
    err=QObject::tr("invalid SQL or failed DB connection")+
      +"["+lastError().text()+"]: "+query;

    fprintf(stderr,"%s\n",err.toUtf8().constData());
    syslog(LOG_ERR,"%s",err.toUtf8().constData());
  }
}


int DvtSqlQuery::columns() const
{
  return sql_columns;
}


QVariant DvtSqlQuery::value(int index) const
{
  QVariant ret=QSqlQuery::value(index);

  if(!ret.isValid()) {
    fprintf(stderr,"for query: %s\n\n",lastQuery().toUtf8().constData());
  }

  return ret;
}


QVariant DvtSqlQuery::run(const QString &sql,bool *ok)
{
  QVariant ret;

  DvtSqlQuery *q=new DvtSqlQuery(sql);
  if(ok!=NULL) {
    *ok=q->isActive();
  }
  ret=q->lastInsertId();
  delete q;

  return ret;
}


bool DvtSqlQuery::apply(const QString &sql,QString *err_msg)
{
  bool ret=false;

  DvtSqlQuery *q=new DvtSqlQuery(sql);
  ret=q->isActive();
  if((err_msg!=NULL)&&(!ret)) {
    *err_msg="sql error: "+q->lastError().text()+" query: "+sql;
  }
  delete q;

  return ret;
}


int DvtSqlQuery::rows(const QString &sql)
{
  int ret=0;

  DvtSqlQuery *q=new DvtSqlQuery(sql);
  ret=q->size();
  delete q;

  return ret;
}


QString DvtSqlQuery::escape(const QString &str)
{
  QString res;

  for(int i=0;i<str.length();i++) {
    bool modified=false;
    if(str.at(i)==QChar('"')) {
      res+=QString("\\\"");
      modified=true;
    }

    if(str.at(i)==QChar('`')) {
      res+=QString("\\`");
      modified=true;
    }

    if(str.at(i)==QChar('\'')) {
      res+=QString("\\\'");
      modified=true;
    }

    if(str.at(i)==QChar('\\')) {
      res+=QString("\\");
      res+=QString("\\");
      modified=true;
    }

    if(!modified) {
      res+=str.at(i);
    }
  }

  return "'"+res+"'";
}


QString DvtSqlQuery::escape(const char *str)
{
  return DvtSqlQuery::escape(QString::fromUtf8(str));
}


QString DvtSqlQuery::escape(bool state)
{
  if(state) {
    return QString("'Y'");
  }
  return QString("'N'");
}


bool DvtOpenDb(QString *err_msg,DvtConfig *c)
{
  QString msg;

  //
  // Open Database
  //
  QSqlDatabase db=QSqlDatabase::addDatabase(c->mysqlServertype());
  db.setDatabaseName(c->mysqlDbname());
  db.setUserName(c->mysqlUsername());
  db.setPassword(c->mysqlPassword());
  db.setHostName(c->mysqlHostname());
  if(!db.open()) {
    *err_msg=QObject::tr("Unable to access database")+
      " '"+c->mysqlDbname()+"'@'"+c->mysqlHostname()+"'.";
    return false;
  }

  //
  // Verify Schema Version
  //
  QString sql=QString("select ")+
    "`DB` "+  // 00
    "from `VERSION`";
  QSqlQuery *q=new DvtSqlQuery(sql);
  if(!q->first()) {
    *err_msg=QObject::tr("Database")+
      " '"+c->mysqlDbname()+"'@'"+c->mysqlHostname()+"' "+
      QObject::tr("does not appear to be a Davit database.");
    delete q;
    return false;
  }
  if(q->value(0).toInt()!=DVT_VERSION_DATABASE) {
    *err_msg=QObject::tr("Database")+
      " '"+c->mysqlDbname()+"'@'"+c->mysqlHostname()+"' "+
      QObject::tr("is at schema version")+
      QString::asprintf(" %d, ",q->value(0).toInt())+
      QObject::tr("expecting")+QString::asprintf(" %d.",DVT_VERSION_DATABASE);
    delete q;
    return false;
  }
  delete q;
  return true;
}
