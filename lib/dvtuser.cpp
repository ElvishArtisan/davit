// dvtuser.cpp
//
// Abstract a Davit user.
//
// (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: dvtuser.cpp,v 1.2 2010/10/21 21:07:54 pcvs Exp $
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

#include <dvtuser.h>
#include <dvtconf.h>

DvtUser::DvtUser(const QString &username)
{
  user_name=username;
  load();
}


QString DvtUser::name() const
{
  return user_name;
}


QString DvtUser::fullName() const
{
  return user_full_name;
}


void DvtUser::setFullName(const QString &str)
{
  user_full_name=str;
}


QString DvtUser::description() const
{
  return user_description;
}


void DvtUser::setDescription(const QString &str)
{
  user_description=str;
}


QString DvtUser::phoneNumber() const
{
  return user_phone_number;
}


void DvtUser::setPhoneNumber(const QString &str)
{
  user_phone_number=str;
}


QString DvtUser::email() const
{
  return user_email;
}


void DvtUser::setEmail(const QString &str)
{
  user_email=str;
}


bool DvtUser::privilege(DvtUser::Privilege priv) const
{
  return user_privilege[priv];
}


void DvtUser::setPrivilege(DvtUser::Privilege priv,bool state)
{
  user_privilege[priv]=state;
}


void DvtUser::load()
{
  QString sql;
  QSqlQuery *q;

  sql=QString().sprintf("select FULL_NAME,DESCRIPTION,PHONE_NUMBER,EMAIL,\
                         ADMIN_PRIV,\
                         AFFILIATE_VIEW_PRIV,AFFILIATE_EDIT_PRIV,\
                         AFFILIATE_SCHEDULE_PRIV,AFFILIATE_REMARK_PRIV,\
                         PROVIDER_VIEW_PRIV,PROVIDER_EDIT_PRIV,\
                         PROGRAM_VIEW_PRIV,PROGRAM_EDIT_PRIV,\
                         REPORT_VIEW_PRIV from USERS where USER_NAME=\"%s\"",
			(const char *)DvtEscapeString(user_name));
  q=new QSqlQuery(sql);
  if(q->first()) {
    user_full_name=q->value(0).toString();
    user_description=q->value(1).toString();
    user_phone_number=q->value(2).toString();
    user_email=q->value(3).toString();
    for(unsigned i=0;i<DvtUser::PrivLastItem;i++) {
      user_privilege[i]=DvtBool(q->value(4+i).toString());
    }
  }
  delete q;
}


void DvtUser::save()
{
  QString sql;
  QSqlQuery *q;

  if(!user_privilege[DvtUser::PrivAffiliateView]) {
    user_privilege[DvtUser::PrivAffiliateEdit]=false;
    user_privilege[DvtUser::PrivAffiliateSchedule]=false;
    user_privilege[DvtUser::PrivAffiliateRemark]=false;
  }
  if(!user_privilege[DvtUser::PrivProviderView]) {
    user_privilege[DvtUser::PrivProviderEdit]=false;
  }
  if(!user_privilege[DvtUser::PrivProgramView]) {
    user_privilege[DvtUser::PrivProgramEdit]=false;
  }
  sql=QString().sprintf("update USERS set \
                         FULL_NAME=\"%s\",\
                         DESCRIPTION=\"%s\",\
                         PHONE_NUMBER=\"%s\",\
                         EMAIL=\"%s\",\
                         ADMIN_PRIV=\"%s\",\
                         AFFILIATE_VIEW_PRIV=\"%s\",\
                         AFFILIATE_EDIT_PRIV=\"%s\",\
                         AFFILIATE_SCHEDULE_PRIV=\"%s\",\
                         AFFILIATE_REMARK_PRIV=\"%s\",\
                         PROVIDER_VIEW_PRIV=\"%s\",\
                         PROVIDER_EDIT_PRIV=\"%s\",\
                         PROGRAM_VIEW_PRIV=\"%s\",\
                         PROGRAM_EDIT_PRIV=\"%s\",\
                         REPORT_VIEW_PRIV=\"%s\" \
                         where USER_NAME=\"%s\"",
			(const char *)DvtEscapeString(user_full_name),
			(const char *)DvtEscapeString(user_description),
			(const char *)DvtEscapeString(user_phone_number),
			(const char *)DvtEscapeString(user_email),
			(const char *)DvtYesNo(user_privilege[0]),
			(const char *)DvtYesNo(user_privilege[1]),
			(const char *)DvtYesNo(user_privilege[2]),
			(const char *)DvtYesNo(user_privilege[3]),
			(const char *)DvtYesNo(user_privilege[4]),
			(const char *)DvtYesNo(user_privilege[5]),
			(const char *)DvtYesNo(user_privilege[6]),
			(const char *)DvtYesNo(user_privilege[7]),
			(const char *)DvtYesNo(user_privilege[8]),
			(const char *)DvtYesNo(user_privilege[9]),
			(const char *)DvtEscapeString(user_name));
  q=new QSqlQuery(sql);
  delete q;
}


void DvtUser::clear()
{
  user_name="";
  user_full_name="";
  user_description="";
  user_phone_number="";
  user_email="";
  for(unsigned i=0;i<DvtUser::PrivLastItem;i++) {
    user_privilege[i]=false;
  }
}
