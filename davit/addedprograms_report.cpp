// addedprograms_report.cpp
//
// The RadioAmerica Affiliate Report for Davit
//
//   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: addedprograms_report.cpp,v 1.7 2011/02/21 23:41:18 pcvs Exp $
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
#include <qfontmetrics.h>

#include <dvt.h>
#include <dvtconf.h>
#include <spread_sheet.h>

#include <pick_fields.h>
#include <list_reports.h>

void ListReports::AddedProgramsReport(Dvt::RemarkType type)
{
  int affiliate_id=0;
  int pgm_id=0;
  QDate start_date=QDate::currentDate().addMonths(-1);
  QDate end_date=QDate::currentDate();
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
  FILE *f=NULL;
  QString outfile;
  QString dow;
  QDateTime dt=QDateTime(QDate::currentDate(),QTime::currentTime());

  PickFields *r=new PickFields(&start_date,&end_date,&pgm_id,true,
			       &affiliate_id,true,NULL,true,NULL,
			       PickFields::NoMarket,this);
  if(r->exec()!=0) {
    delete r;
    return;
  }
  delete r;

  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }
  fclose(f);

  //
  // Generate Fonts
  //
  QFont main_font("arial",10,QFont::Normal);
  QFontMetrics *fm=new QFontMetrics(main_font);

  //
  // Report Header
  //
  SpreadSheet *sheet=new SpreadSheet();
  SpreadTab *tab=sheet->addTab(1);
  if(type==Dvt::RemarkProgramAdd) {
    tab->setName(tr("Added Programs"));
    tab->addCell(1,1)->setText(tr("Added Programs Report"),fm);
  }
  else {
    tab->setName(tr("Deleted Programs"));
    tab->addCell(1,1)->setText(tr("Deleted Programs Report"),fm);
  }
  tab->addCell(1,2)->setText(tr("Report Date")+": "+
			     dt.toString("MM/dd/yyyy hh:mm:ss"),fm);
  tab->addCell(1,4)->setText(tr("CALL"),fm);
  tab->addCell(2,4)->setText(tr("BAND"),fm);
  tab->addCell(3,4)->setText(tr("FREQ"),fm);
  tab->addCell(4,4)->setText(tr("ADDRESS"),fm);
  tab->addCell(5,4)->setText(tr("CITY"),fm);
  tab->addCell(6,4)->setText(tr("STATE"),fm);
  tab->addCell(7,4)->setText(tr("MARKET"),fm);
  tab->addCell(8,4)->setText(tr("PROGRAM DIRECTOR"),fm);
  tab->addCell(9,4)->setText(tr("PHONE"),fm);
  tab->addCell(10,4)->setText(tr("FAX"),fm);
  tab->addCell(11,4)->setText(tr("E-MAIL"),fm);

  //
  // Report Body
  //
  sql="select AFFILIATES.ID,AFFILIATES.STATION_CALL,AFFILIATES.STATION_TYPE,\
       AFFILIATES.STATION_FREQUENCY,AFFILIATES.ADDRESS1,AFFILIATES.ADDRESS2, \
       AFFILIATES.CITY,AFFILIATES.STATE,AFFILIATES.ZIPCODE,		\
       AFFILIATES.MARKET_NAME \
       from AFFILIATES left join AFFILIATE_REMARKS \
       on (AFFILIATES.ID=AFFILIATE_REMARKS.AFFILIATE_ID) where";
  if(affiliate_id>0) {
    sql+=QString().sprintf("(AFFILIATES.ID=%d)&&",affiliate_id);
  }
  if(pgm_id>0) {
    sql+=QString().sprintf("(AFFILIATE_REMARKS.PROGRAM_ID=%d)&&",
			   pgm_id);
  }
  sql+=QString().sprintf("(AFFILIATE_REMARKS.EVENT_TYPE=%d)&&",type);
  sql+=QString().
    sprintf("(AFFILIATE_REMARKS.REMARK_DATETIME>=\"%s 00:00:00\")&&",
	    (const char *)start_date.toString("yyyy-MM-dd"));
  sql+=QString().
    sprintf("(AFFILIATE_REMARKS.REMARK_DATETIME<=\"%s 23:59:59\")",
	    (const char *)end_date.toString("yyyy-MM-dd"));
  sql+=" order by AFFILIATES.STATION_CALL";
  q=new QSqlQuery(sql);
  int row=5;
  while(q->next()) {
    sql=QString("select PROGRAMS.PROGRAM_NAME,")+
      "AFFILIATE_REMARKS.REMARK_DATETIME "+
      "from AFFILIATE_REMARKS right join PROGRAMS "+
      "on (AFFILIATE_REMARKS.PROGRAM_ID=PROGRAMS.ID) where "+
      QString().sprintf("(AFFILIATE_REMARKS.EVENT_TYPE=%d)&&",type)+
      QString().sprintf("(AFFILIATE_REMARKS.AFFILIATE_ID=%d)&&",
			q->value(0).toInt())+
      "(AFFILIATE_REMARKS.REMARK_DATETIME>=\""+
      start_date.toString("yyyy-MM-dd")+" 00:00:00\")&&"+
      "(AFFILIATE_REMARKS.REMARK_DATETIME<\""+
      end_date.addDays(1).toString("yyyy-MM-dd")+" 00:00:00\")";
    if(pgm_id>0) {
      sql+=QString().sprintf("&&(AFFILIATE_REMARKS.PROGRAM_ID=%d)",pgm_id);
    }
    q1=new QSqlQuery(sql);
    if(q1->size()>0) {
      tab->addCell(1,row)->setText(q->value(1).toString(),fm);
      tab->addCell(2,row)->
	setText(DvtStationTypeString(q->value(2).toString()),fm);
      tab->addCell(3,row)->
	setText(DvtFormatFrequency(q->value(3).toDouble()),fm);
      if(q->value(5).toString().isEmpty()) {
	tab->addCell(4,row)->setText(q->value(4).toString(),fm);
      }
      else {
	tab->addCell(4,row)->setText(q->value(4).toString()+
				     ", "+q->value(5).toString(),fm);
      }
      tab->addCell(5,row)->setText(q->value(6).toString(),fm);
      tab->addCell(6,row)->setText(q->value(7).toString().upper(),fm);
      tab->addCell(7,row)->setText(q->value(9).toString(),fm);
      ContactFields(q->value(0).toInt(),
		    ListReports::ProgramDirectorContact,
		    ListReports::FieldName|
		    ListReports::FieldPhone|
		    ListReports::FieldFax|
		    ListReports::FieldEmail,tab,8,row,fm);
      row++;
      tab->addCell(1,row);
      if(type==Dvt::RemarkProgramAdd) {
	tab->addCell(2,row)->setText(tr("PROGRAM"),fm);
	tab->addCell(3,row)->setText(tr("ADDED ON"),fm);
      }
      else {
	tab->addCell(2,row)->setText(tr("PROGRAM"),fm);
	tab->addCell(3,row)->setText(tr("DELETED ON"),fm);
      }
      row++;
      while(q1->next()) {
	tab->addCell(1,row);
	tab->addCell(2,row)->setText(q1->value(0).toString(),fm);
	tab->addCell(3,row)->setText(q1->value(1).toDateTime().
				     toString("MM/dd/yyyy"),fm);
	row++;
      }
      row++;
    }
    delete q1;
  }
  delete q;
  ForkViewer(outfile,sheet->write(SpreadObject::ExcelXmlFormat));
}
