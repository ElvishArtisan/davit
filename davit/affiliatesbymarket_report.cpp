// affiliatesbymarket_report.cpp
//
// Generate an Affiliates by DMA|MSA Market report.
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

#include "list_reports.h"
#include "textfile.h"

bool ListReports::AffiliatesByMarketReport(PickFields::MarketType type,
					   SpreadSheet *sheet)
{
  int pgm_id;
  QString market;

  //
  // Get Parameters
  //
  PickFields *r=new PickFields(NULL,NULL,&pgm_id,false,NULL,false,NULL,false,
			       NULL,type,config(),this);
  if(r->exec()!=0) {
    delete r;
    return false;
  }
  market=r->selectedMarket();
  delete r;

  //
  // Generate Report
  //
  setBusyCursor();

  QString s;
  QString sql;
  QString field_name;
  DvtSqlQuery *q;
  int row=6;
  std::vector<int> ids;

  //
  // Generate Report
  //
  SpreadTab *tab=sheet->addTab(sheet->tabs()+1);

  switch(type) {
  case PickFields::DmaMarket:
    tab->setName(tr("Affiliates by Program/DMA"));
    tab->addCell(1,1)->
      setText(tr("Affiliates by Program/DMA Market Report"));
    field_name="DMA_NAME";
    break;

  case PickFields::MsaMarket:
    tab->setName(tr("Affiliates by Program/MSA"));
    tab->addCell(1,1)->
      setText(tr("Affiliates by Program/MSA Market Report"));
    field_name="MARKET_NAME";
    break;

  case PickFields::NoMarket:
  case PickFields::StateMarket:
    break;
  }
  tab->addCell(2,1)->setText(tr("Report Date")+": "+
			     QDate::currentDate().toString("MMMM dd, yyyy"));
  sql=QString("select ")+
    "`PROGRAM_NAME` "+  // 00
    "from `PROGRAMS` where "+
    QString::asprintf("`ID`=%d",pgm_id);
  q=new DvtSqlQuery(sql);
  if(q->first()) {
    tab->addCell(2,2)->setText(tr("Program")+": "+q->value(0).toString());
  }
  delete q;
  tab->addCell(2,3)->setText(tr("Market")+": "+market);
  tab->addCell(1,5)->setText(tr("AFFILIATE"));
  tab->addCell(2,5)->setText(tr("START"));
  tab->addCell(3,5)->setText(tr("LENGTH"));
  tab->addCell(4,5)->setText(tr("SUN"));
  tab->addCell(5,5)->setText(tr("MON"));
  tab->addCell(6,5)->setText(tr("TUE"));
  tab->addCell(7,5)->setText(tr("WED"));
  tab->addCell(8,5)->setText(tr("THU"));
  tab->addCell(9,5)->setText(tr("FRI"));
  tab->addCell(10,5)->setText(tr("SAT"));
  sql=QString("select ")+
    "`AIRINGS`.`AIR_SUN`,"+
    "`AIRINGS`.`AIR_MON`,"+
    "`AIRINGS`.`AIR_TUE`,"+
    "`AIRINGS`.`AIR_WED`,"+
    "`AIRINGS`.`AIR_THU`,"+
    "`AIRINGS`.`AIR_FRI`,"+
    "`AIRINGS`.`AIR_SAT`,"+
    "`AIRINGS`.`AIR_TIME`,"+
    "`AIRINGS`.`AIR_LENGTH`,"+
    "`AFFILIATES`.`STATION_CALL`,"+
    "`AFFILIATES`.`STATION_TYPE`,"+
    "`AFFILIATES`.`LICENSE_CITY`,"+
    "`AFFILIATES`.`LICENSE_STATE` "+
    "from `AFFILIATES` left join `AIRINGS` "+
    "on `AFFILIATES`.`ID`=`AIRINGS`.`AFFILIATE_ID` where "+
    QString::asprintf("(`PROGRAM_ID`=%d)&&",pgm_id)+
    "(`"+field_name+"`="+DvtSqlQuery::escape(market)+")";
  q=new DvtSqlQuery(sql);
  while(q->next()) {
    // Affiliate
    tab->addCell(1,row)->
      setText(DvtStationCallString(q->value(9).toString(),
				   q->value(10).toString()));

    //
    // Start time
    //
    tab->addCell(2,row)->setText(q->value(7).toTime().toString("hh:mm:ss"));

    //
    // Length
    //
    tab->addCell(3,row)->setText(DvtGetTimeLength(q->value(8).toInt()*1000,
						  false,false));

    //
    // Days of Week
    //
    for(int i=0;i<7;i++) {
      if(q->value(i).toString()=="Y") {
	tab->addCell(4+i,row)->setText("X");
      }
      else {
	tab->addCell(4+i,row);
      }
    }

    row++;
  }
  delete q;

  return true;
}
