// dvtsystem.cpp
//
// Abstract system-wide Davit settings.
//
// (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: dvtsystem.cpp,v 1.1 2010/10/22 20:55:01 pcvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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
//

#include <qsqldatabase.h>

#include <dvtconf.h>
#include <dvtsystem.h>

DvtSystem::DvtSystem()
{
}


QString DvtSystem::originEmail() const
{
  return sys_origin_email;
}


void DvtSystem::setOriginEmail(const QString &str)
{
  sys_origin_email=str;
}


QString DvtSystem::affidavitEmailSubject() const
{
  return sys_affidavit_email_subject;
}


void DvtSystem::setAffidavitEmailSubject(const QString &str)
{
  sys_affidavit_email_subject=str;
}


QString DvtSystem::affidavitEmailTemplate() const
{
  return sys_affidavit_email_template;
}


void DvtSystem::setAffidavitEmailTemplate(const QString &str)
{
  sys_affidavit_email_template=str;
}


void DvtSystem::load()
{
  QString sql;
  QSqlQuery *q;

  clear();
  sql="select ORIGIN_EMAIL,AFFIDAVIT_EMAIL_SUBJECT,AFFIDAVIT_EMAIL_TEMPLATE \
       from SYSTEM";
  q=new QSqlQuery(sql);
  if(q->first()) {
    sys_origin_email=q->value(0).toString();
    sys_affidavit_email_subject=q->value(1).toString();
    sys_affidavit_email_template=q->value(2).toString();
  }
  delete q;
}


void DvtSystem::save()
{
  QString sql;
  QSqlQuery *q;

  sql=QString().
    sprintf("update SYSTEM set \
             ORIGIN_EMAIL=\"%s\",\
             AFFIDAVIT_EMAIL_SUBJECT=\"%s\",\
             AFFIDAVIT_EMAIL_TEMPLATE=\"%s\"",
	    (const char *)DvtEscapeString(sys_origin_email),
	    (const char *)DvtEscapeString(sys_affidavit_email_subject),
	    (const char *)DvtEscapeString(sys_affidavit_email_template));
  q=new QSqlQuery(sql);
  delete q;
}


void DvtSystem::clear()
{
  sys_origin_email="";
  sys_affidavit_email_subject="";
  sys_affidavit_email_template="";
}
