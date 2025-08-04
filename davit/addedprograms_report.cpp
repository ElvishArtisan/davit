// addedprograms_report.cpp
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

#include <dvt.h>
#include <dvtconf.h>
#include <dvtdb.h>
#include <spread_sheet.h>

#include "list_reports.h"
#include "pick_fields.h"

bool ListReports::AddedProgramsReport(Dvt::RemarkType type,SpreadSheet *sheet)
{
  int affiliate_id=0;
  int pgm_id=0;
  QDate start_date=QDate::currentDate().addMonths(-1);
  QDate end_date=QDate::currentDate();
  QString sql;
  DvtSqlQuery *q;
  DvtSqlQuery *q1;
  QString dow;
  QDateTime dt=QDateTime(QDate::currentDate(),QTime::currentTime());

  PickFields *r=new PickFields(&start_date,&end_date,&pgm_id,true,
			       &affiliate_id,true,NULL,true,NULL,
			       PickFields::NoMarket,config(),this);
  if(!r->exec()) {
    delete r;
    return false;
  }
  delete r;

  setBusyCursor();

  //
  // Report Header
  //
  SpreadTab *tab=sheet->addTab(sheet->tabs()+1);
  if(type==Dvt::RemarkProgramAdd) {
    tab->setName(tr("Added Programs"));
    tab->addCell(1,1)->setText(tr("Added Programs Report"));
  }
  else {
    tab->setName(tr("Deleted Programs"));
    tab->addCell(1,1)->setText(tr("Deleted Programs Report"));
  }
  tab->addCell(1,2)->setText(tr("Report Date")+": "+
			     dt.toString("MM/dd/yyyy hh:mm:ss"));
  tab->addCell(1,4)->setText(tr("CALL"));
  tab->addCell(2,4)->setText(tr("BAND"));
  tab->addCell(3,4)->setText(tr("FREQ"));
  tab->addCell(4,4)->setText(tr("ADDRESS"));
  tab->addCell(5,4)->setText(tr("CITY"));
  tab->addCell(6,4)->setText(tr("STATE"));
  tab->addCell(7,4)->setText(tr("MARKET"));
  tab->addCell(8,4)->setText(tr("PROGRAM DIRECTOR"));
  tab->addCell(9,4)->setText(tr("PHONE"));
  tab->addCell(10,4)->setText(tr("FAX"));
  tab->addCell(11,4)->setText(tr("E-MAIL"));

  //
  // Report Body
  //
  sql=QString("select ")+
    "`AFFILIATES`.`ID`,"+                 // 00
    "`AFFILIATES`.`STATION_CALL`,"+       // 01
    "`AFFILIATES`.`STATION_TYPE`,"+       // 02
    "`AFFILIATES`.`STATION_FREQUENCY`,"+  // 03
    "`AFFILIATES`.`ADDRESS1`,"+           // 04
    "`AFFILIATES`.`ADDRESS2`,"+           // 05
    "`AFFILIATES`.`CITY`,"+               // 06
    "`AFFILIATES`.`STATE`,"+              // 07
    "`AFFILIATES`.`ZIPCODE`,"+            // 08
    "`AFFILIATES`.`MARKET_NAME` "+        // 09
    "from `AFFILIATES` left join `AFFILIATE_REMARKS` "+
    "on (`AFFILIATES`.`ID`=`AFFILIATE_REMARKS`.`AFFILIATE_ID`) where ";
  if(affiliate_id>0) {
    sql+=QString::asprintf("(`AFFILIATES`.`ID`=%d)&&",affiliate_id);
  }
  if(pgm_id>0) {
    sql+=QString::asprintf("(`AFFILIATE_REMARKS`.`PROGRAM_ID`=%d)&&",
			   pgm_id);
  }
  sql+=QString::asprintf("(`AFFILIATE_REMARKS`.`EVENT_TYPE`=%d)&&",type);
  sql+="(`AFFILIATE_REMARKS`.`REMARK_DATETIME`>="+
    DvtSqlQuery::escape(start_date.toString("yyyy-MM-dd")+" 00:00:00")+")&&";
  sql+="(`AFFILIATE_REMARKS`.`REMARK_DATETIME`<="+
    DvtSqlQuery::escape(end_date.toString("yyyy-MM-dd")+" 23:59:59")+") ";
  sql+=" order by `AFFILIATES`.`STATION_CALL`";
  q=new DvtSqlQuery(sql);
  int row=5;
  while(q->next()) {
    sql=QString("select ")+
      "`PROGRAMS`.`PROGRAM_NAME`,"+
      "`AFFILIATE_REMARKS`.`REMARK_DATETIME` "+
      "from `AFFILIATE_REMARKS` right join `PROGRAMS` "+
      "on (`AFFILIATE_REMARKS`.`PROGRAM_ID`=`PROGRAMS.ID`) where "+
      QString::asprintf("(`AFFILIATE_REMARKS`.`EVENT_TYPE`=%d)&&",type)+
      QString::asprintf("(`AFFILIATE_REMARKS`.`AFFILIATE_ID`=%d)&&",
			q->value(0).toInt())+
      "(`AFFILIATE_REMARKS`.`REMARK_DATETIME`>="+
      DvtSqlQuery::escape(start_date.toString("yyyy-MM-dd")+" 00:00:00")+")&&"+
      "(`AFFILIATE_REMARKS`.`REMARK_DATETIME`<"+
      DvtSqlQuery::escape(end_date.addDays(1).toString("yyyy-MM-dd")+" 00:00:00")+")";
    if(pgm_id>0) {
      sql+=QString::asprintf("&&(`AFFILIATE_REMARKS`.`PROGRAM_ID`=%d)",pgm_id);
    }
    q1=new DvtSqlQuery(sql);
    if(q1->size()>0) {
      tab->addCell(1,row)->setText(q->value(1).toString());
      tab->addCell(2,row)->
	setText(DvtStationTypeString(q->value(2).toString()));
      tab->addCell(3,row)->
	setText(DvtFormatFrequency(q->value(3).toDouble()));
      if(q->value(5).toString().isEmpty()) {
	tab->addCell(4,row)->setText(q->value(4).toString());
      }
      else {
	tab->addCell(4,row)->setText(q->value(4).toString()+
				     ", "+q->value(5).toString());
      }
      tab->addCell(5,row)->setText(q->value(6).toString());
      tab->addCell(6,row)->setText(q->value(7).toString().toUpper());
      tab->addCell(7,row)->setText(q->value(9).toString());
      ContactFields(q->value(0).toInt(),
		    ListReports::ProgramDirectorContact,
		    ListReports::FieldName|
		    ListReports::FieldPhone|
		    ListReports::FieldFax|
		    ListReports::FieldEmail,tab,8,row);
      row++;
      tab->addCell(1,row);
      if(type==Dvt::RemarkProgramAdd) {
	tab->addCell(2,row)->setText(tr("PROGRAM"));
	tab->addCell(3,row)->setText(tr("ADDED ON"));
      }
      else {
	tab->addCell(2,row)->setText(tr("PROGRAM"));
	tab->addCell(3,row)->setText(tr("DELETED ON"));
      }
      row++;
      while(q1->next()) {
	tab->addCell(1,row);
	tab->addCell(2,row)->setText(q1->value(0).toString());
	tab->addCell(3,row)->setText(q1->value(1).toDateTime().
				     toString("MM/dd/yyyy"));
	row++;
      }
      row++;
    }
    delete q1;
  }
  delete q;

  return true;
}
