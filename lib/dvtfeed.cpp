// dvtfeed.cpp
//
// A Container Class for Davit Feeds.
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
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

#include <QSqlQuery>
#include <QSqlDatabase>

#include "dvtconf.h"
#include "dvtfeed.h"

DvtFeed::DvtFeed()
{
  clear();
}


QString DvtFeed::name() const
{
  return slot_name;
}


void DvtFeed::setName(const QString &name)
{
  slot_name=name;
}


QTime DvtFeed::startTime() const
{
  return slot_start_time;
}


void DvtFeed::setStartTime(const QTime &time)
{
  slot_start_time=time;
}


void DvtFeed::setStartTime(const QString &time)
{
  slot_start_time=
    QTime(time.left(2).toInt(),time.mid(3,2).toInt(),time.right(2).toInt());
}


int DvtFeed::length() const
{
  return slot_length;
}


QTime DvtFeed::endTime() const
{
  return slot_start_time.addSecs(slot_length);
}


void DvtFeed::setLength(int len)
{
  slot_length=len;
}


void DvtFeed::setLength(const QString &time)
{
  slot_length=slot_start_time.
    secsTo(QTime(time.left(2).toInt(),time.mid(3,2).toInt(),
		 time.right(2).toInt()));
}


bool DvtFeed::dowActive(int dow) const
{
  return slot_dow_active[dow-1];
}


void DvtFeed::setDowActive(int dow,bool state)
{
  slot_dow_active[dow-1]=state;
}


int DvtFeed::addToAffiliate(int affiliate_id)
{
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
  int program_id=-1;

  sql=QString::asprintf("select ID from PROGRAMS where PROGRAM_NAME=\"%s\"",
			slot_name.toUtf8().constData());
  q=new QSqlQuery(sql);
  if(q->first()) {
    program_id=q->value(0).toInt();
    sql=QString::asprintf("insert into AIRINGS set \
                           AFFILIATE_ID=%d,\
                           PROGRAM_ID=%d",
			  affiliate_id,
			  program_id);
    q1=new QSqlQuery(sql);
    delete q1;
    sql="select LAST_INSERT_ID() from AIRINGS";
    q1=new QSqlQuery(sql);
    if(q1->first()) {
      modifyAffiliate(q1->value(0).toInt());
    }
    delete q1;
    sql=QString::asprintf("update AFFILIATES set IS_AFFILIATE=\"Y\" \
                           where ID=%d",affiliate_id);
    q1=new QSqlQuery(sql);
    delete q1;
  }
  delete q;

  return program_id;
}


int DvtFeed::modifyAffiliate(int airing_id)
{
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
  int program_id=-1;

  sql=QString::asprintf("select ID from PROGRAMS where PROGRAM_NAME=\"%s\"",
			slot_name.toUtf8().constData());
  q=new QSqlQuery(sql);
  if(q->first()) {
    program_id=q->value(0).toInt();
    sql=QString::asprintf("update AIRINGS set \
                           PROGRAM_ID=%d,\
                           AIR_TIME=\"%s\",\
                           AIR_LENGTH=%d,\
                           AIR_SUN=\"%s\",\
                           AIR_MON=\"%s\",\
                           AIR_TUE=\"%s\",\
                           AIR_WED=\"%s\",\
                           AIR_THU=\"%s\",\
                           AIR_FRI=\"%s\",\
                           AIR_SAT=\"%s\" \
                           where ID=%d",
			  program_id,
			  slot_start_time.toString("hh:mm:ss").
			  toUtf8().constData(),
			  slot_length,
			  DvtYesNo(slot_dow_active[6]).toUtf8().constData(),
			  DvtYesNo(slot_dow_active[0]).toUtf8().constData(),
			  DvtYesNo(slot_dow_active[1]).toUtf8().constData(),
			  DvtYesNo(slot_dow_active[2]).toUtf8().constData(),
			  DvtYesNo(slot_dow_active[3]).toUtf8().constData(),
			  DvtYesNo(slot_dow_active[4]).toUtf8().constData(),
			  DvtYesNo(slot_dow_active[5]).toUtf8().constData(),
			  airing_id);
    q1=new QSqlQuery(sql);
    delete q1;
  }
  delete q;

  return program_id;
}


void DvtFeed::clear()
{
  slot_name="";
  slot_start_time=QTime();
  slot_length=0;
  for(unsigned i=0;i<7;i++) {
    slot_dow_active[i]=true;
  }
}
