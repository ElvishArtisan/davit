// duplicateaffiliate_report.cpp
//
// Generate a list of affiliate records containing duplicate
// STATION_CALL / STATION_TYPE values.
//
//   (C) Copyright 2010-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <vector>

#include <QFile>
#include <QFontMetrics>
#include <QMessageBox>

#include <dvtconf.h>
#include <dvtdb.h>
#include <spread_sheet.h>

#include "list_reports.h"

bool ListReports::DuplicateAffiliateReport(SpreadSheet *sheet)
{
  QString s;
  QString sql;
  DvtSqlQuery *q;
  int row=4;
  std::vector<int> ids;
  QString str;

  //
  // Generate Report
  //
  setBusyCursor();
  SpreadTab *tab=sheet->addTab(sheet->tabs()+1);
  tab->setName(tr("Duplicate Affiliates"));

  tab->addCell(1,1)->setText(tr("Duplicate Affiliate Report"));
  tab->addCell(1,2)->setText(tr("Report Date")+": "+
			     QDate::currentDate().toString("MM/dd/yyyy"));
  tab->addCell(1,3)->setText(tr("CALL"));
  tab->addCell(2,3)->setText(tr("CITY OF LICENSE"));
  tab->addCell(3,3)->setText("FREQUENCY");
  tab->addCell(4,3)->setText(tr("BUSINESS NAME"));
  tab->addCell(5,3)->setText(tr("BIA CODE"));
  tab->addCell(6,3)->setText(tr("ID"));
  tab->addCell(7,3)->setText(tr("IS AFFILIATE"));
  sql=QString("select ")+
    "`ID`,"+                 // 00
    "`STATION_CALL`,"+       // 01
    "`STATION_TYPE`,"+       // 02
    "`LICENSE_CITY`,"+       // 03
    "`LICENSE_STATE`,"+      // 04
    "`STATION_FREQUENCY`,"+  // 05
    "`BUSINESS_NAME`,"+      // 06
    "`BIA_CODE`,"+           // 07
    "`IS_AFFILIATE` "+       // 08
    "from `AFFILIATES` "+
    "order by `STATION_CALL` desc";
  q=new DvtSqlQuery(sql);
  if(q->first()) {
    int last_id=q->value(0).toInt();
    QString last_call=q->value(1).toString();
    QString last_type=q->value(2).toString();
    QString last_city=q->value(3).toString();
    QString last_state=q->value(4).toString();
    double last_frequency=q->value(5).toDouble();
    QString last_business_name=q->value(6).toString();
    QString last_bia_code=q->value(7).toString();
    QString last_is_affiliate=q->value(8).toString();
    while(q->next()) {
      if((q->value(1).toString().left(3)!="NEW")&&
	 (q->value(1).toString()==last_call)&&(q->value(2)==last_type)) {
	tab->addCell(1,row)->
	  setText(DvtStationCallString(q->value(1).toString(),
				       q->value(2).toString()));
	tab->addCell(2,row)->setText(DvtFormatCityState(q->value(3).toString(),
					       	q->value(4).toString()));
	tab->addCell(3,row)->
	  setText(QString::asprintf("%4.0lf",q->value(5).toDouble()));
	tab->addCell(4,row)->setText(q->value(6).toString());
	tab->addCell(5,row)->setText(q->value(7).toString());
	tab->addCell(6,row)->
	  setText(QString::asprintf("%d",q->value(0).toInt()));
	tab->addCell(7,row)->setText(q->value(8).toString());
	row++;

	tab->addCell(1,row)->
	  setText(DvtStationCallString(last_call,last_type));
	tab->addCell(2,row)->setText(DvtFormatCityState(last_city,last_state));
	tab->addCell(3,row)->
	  setText(QString::asprintf("%4.0lf",last_frequency));
	tab->addCell(4,row)->setText(last_business_name);
	tab->addCell(5,row)->setText(last_bia_code);
	tab->addCell(6,row)->setText(QString::asprintf("%d",last_id));
	tab->addCell(7,row)->setText(last_is_affiliate);
	row++;

	row++;
      }
      last_call=q->value(1).toString();
      last_type=q->value(2).toString();
      last_city=q->value(3).toString();
      last_state=q->value(4).toString();
      last_frequency=q->value(5).toDouble();
      last_business_name=q->value(6).toString();
      last_bia_code=q->value(7).toString();
      last_is_affiliate=q->value(8).toString();
    }
  }
  delete q;

  return true;
}


