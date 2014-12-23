// affiliatesbyprogram_report.cpp
//
// The RadioAmerica Affiliate Report for Davit
//
//   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: raaffiliate_report.cpp,v 1.12 2011/09/27 21:00:29 pcvs Exp $
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

#include <qdatetime.h>
#include <qsqldatabase.h>

#include <dvtconf.h>

#include <pick_fields.h>
#include <list_reports.h>

void ListReports::RAAffiliateReport()
{
  int affiliate_id=0;
  int pgm_id=0;
  QDate date;
  QString sql;
  QSqlQuery *q;
  FILE *f=NULL;
  QString outfile;
  QString dow;
  QDateTime dt=QDateTime(QDate::currentDate(),QTime::currentTime());

  PickFields *r=
    new PickFields(NULL,NULL,&pgm_id,true,&affiliate_id,true,NULL,true,
		   NULL,PickFields::NoMarket,this);
  if(r->exec()!=0) {
    delete r;
    return;
  }
  delete r;

  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }
  fclose(f);
  QFont main_font("arial",10,QFont::Normal);
  QFontMetrics *fm=new QFontMetrics(main_font);
  SpreadSheet *sheet=new SpreadSheet();
  SpreadTab *tab=sheet->addTab(1);
  tab->setName(tr("RadioAmerica Affiliates"));

  tab->addCell(1,1)->setText(tr("RadioAmerica Affiliates Report"),fm);
  tab->addCell(1,2)->setText(tr("Report Date")+": "+
			     dt.toString("MM/dd/yyyy hh:mm:ss"),fm);
  int col=1;
  int row=4;

  tab->addCell(col++,row)->setText(tr("CALL"),fm);
  tab->addCell(col++,row)->setText(tr("BAND"),fm);
  tab->addCell(col++,row)->setText(tr("FREQ"),fm);
  tab->addCell(col++,row)->setText(tr("ADDRESS1"),fm);
  tab->addCell(col++,row)->setText(tr("CITY"),fm);
  tab->addCell(col++,row)->setText(tr("STATE"),fm);
  tab->addCell(col++,row)->setText(tr("MSA MARKET"),fm);
  tab->addCell(col++,row)->setText(tr("MSA RANK"),fm);
  tab->addCell(col++,row)->setText(tr("DMA MARKET"),fm);
  tab->addCell(col++,row)->setText(tr("DMA RANK"),fm);
  tab->addCell(col++,row)->setText(tr("PD NAME"),fm);
  tab->addCell(col++,row)->setText(tr("PD PHONE"),fm);
  tab->addCell(col++,row)->setText(tr("PD FAX"),fm);
  tab->addCell(col++,row)->setText(tr("PD E-MAIL"),fm);
  tab->addCell(col++,row)->setText(tr("AFFIDAVIT NAME"),fm);
  tab->addCell(col++,row)->setText(tr("AFFIDAVIT PHONE"),fm);
  tab->addCell(col++,row)->setText(tr("PROGRAM"),fm);
  tab->addCell(col++,row)->setText(tr("AIRING"),fm);
  row++;
  /*
  sql=QString("select ID,STATION_CALL,STATION_TYPE,STATION_FREQUENCY,")+
    "ADDRESS1,ADDRESS2,CITY,STATE,ZIPCODE,MARKET_NAME,MARKET_RANK,DMA_NAME,"+
    "DMA_RANK from AFFILIATES";
  */
  sql=QString("select AFFILIATES.ID,AFFILIATES.STATION_CALL,")+
    "AFFILIATES.STATION_TYPE,AFFILIATES.STATION_FREQUENCY,"+
    "AFFILIATES.ADDRESS1,AFFILIATES.ADDRESS2,AFFILIATES.CITY,"+
    "AFFILIATES.STATE,AFFILIATES.ZIPCODE,AFFILIATES.MARKET_NAME,"+
    "AFFILIATES.MARKET_RANK,AFFILIATES.DMA_NAME,AFFILIATES.DMA_RANK,"+
    "PROGRAMS.PROGRAM_NAME,AIRINGS.AIR_TIME,AIRINGS.AIR_LENGTH,"+
    "AIRINGS.AIR_SUN,AIRINGS.AIR_MON,AIRINGS.AIR_TUE,AIRINGS.AIR_WED,"+
    "AIRINGS.AIR_THU,AIRINGS.AIR_FRI,AIRINGS.AIR_SAT "+
    "from AFFILIATES left join AIRINGS "+
    "on AFFILIATES.ID=AIRINGS.AFFILIATE_ID "+
    "right join PROGRAMS on PROGRAMS.ID=AIRINGS.PROGRAM_ID "+
    "where AFFILIATES.STATION_CALL is not null ";
  if(affiliate_id>0) {
    sql+=QString().sprintf("&&(AFFILIATES.ID=%d)",affiliate_id);
  }
  if(pgm_id>0) {
    sql+=QString().sprintf("&&(PROGRAMS.ID=%d)",pgm_id);
  }
  sql+=" order by AFFILIATES.STATE,AFFILIATES.CITY,AFFILIATES.STATION_CALL";
  q=new QSqlQuery(sql);
  while(q->next()) {
    col=1;
    tab->addCell(col++,row)->setText(q->value(1).toString(),fm);
    tab->addCell(col++,row)->
      setText(DvtStationTypeString(q->value(2).toString()),fm);
    tab->addCell(col++,row)->
      setText(DvtFormatFrequency(q->value(3).toDouble()),fm);
    tab->addCell(col++,row)->setText(q->value(4).toString(),fm);
    tab->addCell(col++,row)->setText(q->value(6).toString(),fm);
    tab->addCell(col++,row)->setText(q->value(7).toString().upper(),fm);
    tab->addCell(col++,row)->setText(q->value(9).toString(),fm);
    tab->addCell(col++,row)->
      setText(DvtMarketRankString(q->value(10).toInt()),fm);
    tab->addCell(col++,row)->setText(q->value(11).toString(),fm);
    tab->addCell(col++,row)->
      setText(DvtMarketRankString(q->value(12).toInt()),fm);
    ContactFields(q->value(0).toInt(),
		  ListReports::ProgramDirectorContact,
		  ListReports::FieldName|
		  ListReports::FieldPhone|
		  ListReports::FieldFax|
		  ListReports::FieldEmail,tab,col,row,fm);
    col+=4;
    ContactFields(q->value(0).toInt(),
		  ListReports::AffidavitContact,
		  ListReports::FieldName|
		  ListReports::FieldPhone,tab,col,row,fm);
    col+=2;
    tab->addCell(col++,row)->setText(q->value(13).toString(),fm);
    dow="";
    if(q->value(16).toString()=="Y") {
      dow+="Su";
    }
    if(q->value(17).toString()=="Y") {
      dow+="Mo";
    }
    if(q->value(18).toString()=="Y") {
      dow+="Tu";
    }
    if(q->value(19).toString()=="Y") {
      dow+="We";
    }
    if(q->value(20).toString()=="Y") {
      dow+="Th";
    }
    if(q->value(21).toString()=="Y") {
      dow+="Fr";
    }
    if(q->value(22).toString()=="Y") {
      dow+="Sa";
    }
    dow+=(", "+q->value(14).toTime().toString("h ap")+"-"+
	  q->value(14).toTime().
	  addSecs(q->value(15).toInt()).toString("h ap"));
    tab->addCell(col++,row)->setText(dow,fm);
    row++;
  }
  delete q;
  delete fm;
  ForkViewer(outfile,sheet->write(SpreadObject::ExcelXmlFormat));
}
