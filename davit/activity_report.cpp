// activity_report.cpp
//
// The RadioAmerica Affiliate Report for Davit
//
//   (C) Copyright 2008-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <QDateTime>
#include <QFontMetrics>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <dvt.h>
#include <dvtconf.h>

#include "list_reports.h"
#include "pick_fields.h"

bool ListReports::ActivityReport(SpreadSheet *sheet)
{
  int affiliate_id=0;
  QDate start_date=QDate::currentDate().addMonths(-1);
  QDate end_date=QDate::currentDate();
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
  QString dow;
  int row=5;
  QDateTime dt=QDateTime(QDate::currentDate(),QTime::currentTime());

  PickFields *r=new PickFields(&start_date,&end_date,NULL,true,
			       &affiliate_id,true,NULL,true,NULL,
			       PickFields::NoMarket,this);
  if(r->exec()!=0) {
    delete r;
    return false;
  }
  delete r;

  SpreadTab *tab=sheet->addTab(sheet->tabs()+1);
  tab->setName(tr("Affiliate Activity"));
  tab->addCell(1,1)->setText(tr("Affiliate Activity Report"));
  tab->addCell(1,2)->setText(tr("Report Date")+": "+
			     dt.toString("MM/dd/yyyy hh:mm:ss"));
  tab->addCell(1,4)->setText(tr("CALL"));
  tab->addCell(2,4)->setText(tr("PROGRAM DIRECTOR"));
  tab->addCell(3,4)->setText(tr("PHONE"));
  tab->addCell(4,4)->setText(tr("DATE"));
  tab->addCell(5,4)->setText(tr("USER NAME"));
  tab->addCell(6,4)->setText(tr("REMARKS"));
  sql="select AFFILIATES.ID,AFFILIATES.STATION_CALL,AFFILIATES.STATION_TYPE,\
       CONTACTS.NAME,CONTACTS.PHONE \
       from AFFILIATES left join CONTACTS \
       on AFFILIATES.ID=CONTACTS.AFFILIATE_ID \
       where (CONTACTS.PROGRAM_DIRECTOR=\"Y\")";
  if(affiliate_id>0) {
    sql+=QString().sprintf("&& (AFFILIATES.ID=%d)",affiliate_id);
  }
  sql+=" order by AFFILIATES.STATION_CALL";
  q=new QSqlQuery(sql);
  while(q->next()) {
    sql=QString().sprintf("select AFFILIATE_REMARKS.REMARK_DATETIME,\
                           AFFILIATE_REMARKS.USER_NAME,\
                           AFFILIATE_REMARKS.REMARK \
                           from AFFILIATE_REMARKS \
                           where (AFFILIATE_REMARKS.AFFILIATE_ID=%d)&&",
			  q->value(0).toInt());
    sql+=QString().
      sprintf("(AFFILIATE_REMARKS.REMARK_DATETIME>=\"%s 00:00:00\")&&",
	      start_date.toString("yyyy-MM-dd").toUtf8().constData());
    sql+=QString().
      sprintf("(AFFILIATE_REMARKS.REMARK_DATETIME<=\"%s 23:59:59\")",
	      end_date.toString("yyyy-MM-dd").toUtf8().constData());
    sql+=" order by AFFILIATE_REMARKS.REMARK_DATETIME";
    q1=new QSqlQuery(sql);
    if(q1->size()>0) {
      tab->addCell(1,row)->
	setText(DvtStationCallString(q->value(1).toString(),
				     q->value(2).toString()));
      tab->addCell(2,row)->setText(q->value(3).toString());
      tab->addCell(3,row)->
	setText(DvtFormatPhoneNumber(q->value(4).toString()));
      row++;
      while(q1->next()) {
	tab->addCell(1,row);
	tab->addCell(2,row);
	tab->addCell(3,row);
	tab->addCell(4,row)->
	  setText(q1->value(0).toDateTime().toString("MM/dd/yyyy"));
	tab->addCell(5,row)->setText(q1->value(1).toString());
	tab->addCell(5,row)->setText(q1->value(2).toString());
	row++;
      }
    }
    delete q1;
  }
  delete q;

  return true;
}
