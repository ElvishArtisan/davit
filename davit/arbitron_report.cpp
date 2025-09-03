// arbitron_report.cpp
//
// The Arbitron Report for Davit
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

#include <dvtconf.h>
#include <dvtdb.h>

#include "list_reports.h"
#include "pick_fields.h"

bool ListReports::ArbitronReport(SpreadSheet *sheet)
{
  int pgm_id=0;
  QDate date;
  QString sql;
  DvtSqlQuery *q;
  QString dow;
  int row=1;
  QDateTime dt=QDateTime(QDate::currentDate(),QTime::currentTime());

  PickFields *r=
    new PickFields(NULL,NULL,&pgm_id,false,NULL,false,NULL,false,NULL,
		   PickFields::NoMarket,config(),this);
  if(!r->exec()) {
    delete r;
    return false;
  }
  delete r;

  setBusyCursor();
  SpreadTab *tab=sheet->addTab(sheet->tabs()+1);
  tab->setName(tr("Arbitron"));
  sql=QString("select ")+
    "`PROGRAM_NAME` "+  // 00
    "from `PROGRAMS` where "+
    QString::asprintf("`ID`=%d",pgm_id);
  q=new DvtSqlQuery(sql);
  if(q->first()) {
    tab->addCell(1,row++)->setText(q->value(0).toString());
  }
  delete q;
  tab->addCell(1,row++)->setText(tr("Arbitron Listing")+" - "+
				 dt.toString("MM/dd/yyyy hh:mm:ss"));
  sql=QString("select ")+
    "`AFFILIATES`.`STATION_CALL`,"+  // 00
    "`AFFILIATES`.`STATION_TYPE`,"+  // 01
    "`AIRINGS`.`AIR_TIME`,"+         // 02
    "`AIRINGS`.`AIR_LENGTH`,"+       // 03
    "`AIRINGS`.`AIR_SUN`,"+          // 04
    "`AIRINGS`.`AIR_MON`,"+          // 05
    "`AIRINGS`.`AIR_TUE`,"+          // 06
    "`AIRINGS`.`AIR_WED`,"+          // 07
    "`AIRINGS`.`AIR_THU`,"+          // 08
    "`AIRINGS`.`AIR_FRI`,"+          // 09
    "`AIRINGS`.`AIR_SAT` "+          // 10
    "from `AFFILIATES` left join `AIRINGS` "+
    "on (`AFFILIATES`.`ID`=`AIRINGS`.`AFFILIATE_ID`) "+
    "left join `PROGRAMS` on (`AIRINGS`.`PROGRAM_ID`=`PROGRAMS`.`ID`) where "+
    QString::asprintf("`PROGRAMS`.`ID`=%d",pgm_id);
  q=new DvtSqlQuery(sql);
  tab->addCell(1,row++)->setText(tr("Listings Generated")+": "+
				 QString::asprintf("%d",q->size()));
  while(q->next()) {
    QString str=DvtStationCallString(q->value(0).toString(),
				     q->value(1).toString());
    if((q->value(5).toString()=="Y")&&(q->value(6).toString()=="Y")&&
       (q->value(7).toString()=="Y")&&(q->value(8).toString()=="Y")&&
       (q->value(9).toString()=="Y")) {
      dow="MF";
    }
    else {
      dow="";
      if(q->value(5).toString()=="Y") {
	dow+="M";
      }
      if(q->value(6).toString()=="Y") {
	dow+="T";
      }
      if(q->value(7).toString()=="Y") {
	dow+="W";
      }
      if(q->value(8).toString()=="Y") {
	dow+="R";
      }
      if(q->value(9).toString()=="Y") {
	dow+="F";
      }
      if(q->value(10).toString()=="Y") {
	dow+="SA";
      }
      if(q->value(4).toString()=="Y") {
	dow+="SU";
      }
    }
    if(!dow.isEmpty()) {
      if(q->value(2).toTime().hour()<12) {
	dow+=QString::asprintf("%d",q->value(2).toTime().hour());
	dow+="a-";
      }
      else {
	dow+=QString::asprintf("%d",q->value(2).toTime().hour()-12);
	dow+="p-";
      }
      if(q->value(2).toTime().hour()<12) {
	dow+=QString::asprintf("%d",q->value(2).toTime().hour()+1);
	dow+="a";
      }
      else {
	dow+=QString::asprintf("%d",q->value(2).toTime().hour()-11);
	dow+="p";
      }
      tab->addCell(1,row++)->setText(str+" "+dow);
    }
  }
  delete q;

  return true;
}
