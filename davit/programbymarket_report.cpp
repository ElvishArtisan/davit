// programbymarket_report.cpp
//
// Generate a Program by Market report.
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
#include <QMessageBox>

#include <dvtconf.h>
#include <dvtdb.h>
#include <state_conv.h>

#include "list_reports.h"
#include "pick_fields.h"
#include "textfile.h"

bool ListReports::ProgramByMarketReport(PickFields::MarketType type,
					SpreadSheet *sheet)
{
  QString s;
  QString sql;
  DvtSqlQuery *q=NULL;
  QString where;
  QString outfile;

  //
  // Get Market
  //
  PickFields *d=new PickFields(NULL,NULL,NULL,false,NULL,false,NULL,false,NULL,type,this);
  if(d->exec()<0) {
    return false;
  }
  QString market=d->selectedMarket();
  QString city=d->selectedCity();
  QString state=d->selectedStateCode();
  delete d;

  //
  // Generate Report
  //
  setBusyCursor();
  SpreadTab *tab=sheet->addTab(sheet->tabs()+1);
  tab->setName(tr("Programs by")+" "+market);
  tab->addCell(1,1)->setText(tr("Programs in")+" "+market);
  tab->addCell(1,2)->setText(tr("Report Date")+": "+
			     QDate::currentDate().toString("MMMM dd, yyyy"));
  tab->addCell(1,4)->setText(tr("CALL LETTERS"));
  tab->addCell(2,4)->setText(tr("FREQUENCY"));
  tab->addCell(3,4)->setText(tr("CITY"));
  tab->addCell(4,4)->setText(tr("STATE"));
  tab->addCell(5,4)->setText(tr("DMA MARKET"));
  tab->addCell(6,4)->setText(tr("MSA MARKET"));
  tab->addCell(7,4)->setText(tr("PROGRAM"));
  sql=QString("select ")+
    "`AFFILIATES`.`ID`,"+
    "`AFFILIATES`.`STATION_CALL`,"+
    "`AFFILIATES`.`STATION_TYPE`,"+
    "`AFFILIATES`.`STATION_FREQUENCY`,"+
    "`AFFILIATES`.`LICENSE_CITY`,"+
    "`AFFILIATES`.`LICENSE_STATE`,"+
    "`DMA_NAME`,"+
    "`MARKET_NAME`,"+
    "`PROGRAMS`.`PROGRAM_NAME` "+
    "from `AFFILIATES` left join `AIRINGS` "+
    "on `AFFILIATES`.`ID`=`AIRINGS`.`AFFILIATE_ID` "+
    "left join `PROGRAMS` on `AIRINGS`.`PROGRAM_ID`=`PROGRAMS`.`ID` "+
    "where (`AFFILIATES`.`IS_AFFILIATE`='Y')";
  switch(type) {
  case PickFields::NoMarket:
    break;

  case PickFields::StateMarket:
    if(state!="aa") {
      if(!city.isEmpty()) {
	sql+=QString("&&(`AFFILIATES`.`LICENSE_CITY`=")+
	  DvtSqlQuery::escape(city)+")";
	tab->addCell(1,1)->setText(tr("Programs in")+" "+
				   DvtFormatCityState(city,state));
      }
      else {
	tab->addCell(1,1)->setText(tr("Programs in")+" "+
				   AbbreviationToState(state));
      }
      sql+=QString("&&(`AFFILIATES`.`LICENSE_STATE`=")+
	DvtSqlQuery::escape(state)+")";
    }
    break;

  case PickFields::DmaMarket:
    sql+=QString("&&(DMA_NAME=")+DvtSqlQuery::escape(market)+")";
    tab->addCell(1,1)->setText(tr("Programs in")+" "+market);
    break;

  case PickFields::MsaMarket:
    sql+=QString("&&(MARKET_NAME=")+DvtSqlQuery::escape(market)+")";
    tab->addCell(1,1)->setText(tr("Programs in")+" "+market);
    break;
  }
  sql+=" order by `AFFILIATES`.`LICENSE_STATE`,`AFFILIATES`.`LICENSE_CITY`,";
  sql+="`PROGRAMS`.`PROGRAM_NAME`";
  q=new DvtSqlQuery(sql);
  int row=5;
  while(q->next()) {
    // Call Letters
    tab->addCell(1,row)->
      setText(DvtStationCallString(q->value(1).toString(),
				   q->value(2).toString()));

    // Frequency
    tab->addCell(2,row)->
      setText(DvtFormatFrequency(q->value(3).toDouble()));

    // City
    tab->addCell(3,row)->setText(q->value(4).toString());

    // State
    tab->addCell(4,row)->setText(q->value(5).toString().toUpper());

    // DMA Market
    tab->addCell(5,row)->setText(q->value(6).toString());

    // MSA Market
    tab->addCell(6,row)->setText(q->value(7).toString());

    // Program
    tab->addCell(7,row)->setText(q->value(8).toString());

    row++;
  }
  delete q;

  return true;
}
