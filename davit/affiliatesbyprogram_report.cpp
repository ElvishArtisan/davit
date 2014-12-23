// affiliatesbyprogram_report.cpp
//
// The AffiliatesByProgram Report for Davit
//
//   (C) Copyright 2008,2010,2014 Fred Gleason <fredg@paravelsystems.com>
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
#include <spread_sheet.h>

#include <pick_fields.h>
#include <pick_daypart.h>
#include <list_reports.h>

void ListReports::AllAffiliatesReport(SpreadSheet *sheet)
{
  //
  // Generate Where Clause
  //
  QString where="where (AFFILIATES.IS_AFFILIATE=\"Y\") \
                 order by AFFILIATES.STATION_CALL,AFFILIATES.STATION_TYPE,\
                 AIRINGS.AIR_TIME";

  //
  // Render Report
  //
  SpreadTab *tab=sheet->addTab(sheet->tabs()+1);
  tab->setName(tr("All Affiliates"));
  RenderAffiliateReport(tab,where,tr("All Affiliates Report"),"",true,0);
}


void ListReports::AllAffiliateContacts(SpreadSheet *sheet)
{
  QString sql;
  QSqlQuery *q;

  //
  // Generate Fonts
  //
  SpreadTab *tab=sheet->addTab(sheet->tabs()+1);
  tab->setName(tr("Affiliate Contacts"));

  //
  // Report Header
  //
  tab->addCell(1,1)->setText(tr("All Affiliate Contacts Report"));
  //  fprintf(f,"\"All Affiliate Contacts Report\"\n");
  tab->addCell(1,2)->
    setText(tr("Report Date")+": "+
	    QDateTime(QDate::currentDate(),
		      QTime::currentTime()).toString("MM/dd/yyyy hh:mm:ss"));
  tab->addCell(1,4)->setText(tr("CALL"));
  tab->addCell(2,4)->setText(tr("BAND"));
  tab->addCell(3,4)->setText(tr("FREQ"));
  tab->addCell(4,4)->setText(tr("CITY"));
  tab->addCell(5,4)->setText(tr("STATE"));
  tab->addCell(6,4)->setText(tr("MSA MARKET"));
  tab->addCell(7,4)->setText(tr("MSA RANK"));
  tab->addCell(8,4)->setText(tr("DMA MARKET"));
  tab->addCell(9,4)->setText(tr("DMA RANK"));
  tab->addCell(10,4)->setText(tr("PD NAME"));
  tab->addCell(11,4)->setText(tr("PD PHONE"));
  tab->addCell(12,4)->setText(tr("PD E-MAIL"));
  tab->addCell(13,4)->setText(tr("AFFIDAVIT NAME"));
  tab->addCell(14,4)->setText(tr("AFFIDAVIT PHONE"));
  tab->addCell(15,4)->setText(tr("AFFIDAVIT E-MAIL"));
  tab->addCell(16,4)->setText(tr("GM NAME"));
  tab->addCell(17,4)->setText(tr("GM PHONE"));
  tab->addCell(18,4)->setText(tr("GM E-MAIL"));

  int row=5;
  sql="select ID,STATION_CALL,STATION_TYPE,STATION_FREQUENCY,CITY,\
              STATE,MARKET_NAME,MARKET_RANK,DMA_NAME,DMA_RANK from AFFILIATES \
              where IS_AFFILIATE=\"Y\" order by STATION_CALL,STATION_TYPE";
  q=new QSqlQuery(sql);
  while(q->next()) {
    tab->addCell(1,row)->setText(q->value(1).toString());
    tab->addCell(2,row)->
      setText(DvtStationTypeString(q->value(2).toString()));
    tab->addCell(3,row)->
      setText(DvtFormatFrequency(q->value(3).toDouble()));
    tab->addCell(4,row)->setText(q->value(4).toString());
    tab->addCell(5,row)->setText(q->value(5).toString().upper());
    tab->addCell(6,row)->setText(q->value(6).toString());
    if(q->value(7).toInt()==0) {
      tab->addCell(7,row);
    }
    else {
      tab->addCell(7,row)->setText(q->value(7).toString());
    }
    tab->addCell(8,row)->setText(q->value(8).toString());
    if(q->value(9).toInt()==0) {
      tab->addCell(9,row);
    }
    else {
      tab->addCell(9,row)->setText(q->value(9).toString());
    }
    ContactFields(q->value(0).toInt(),
		  ListReports::ProgramDirectorContact,
		  ListReports::FieldName|ListReports::FieldPhone|
		  ListReports::FieldEmail,tab,10,row);
    ContactFields(q->value(0).toInt(),
		  ListReports::AffidavitContact,
		  ListReports::FieldName|ListReports::FieldPhone|
		  ListReports::FieldEmail,tab,13,row);
    ContactFields(q->value(0).toInt(),
		  ListReports::GeneralManagerContact,
		  ListReports::FieldName|ListReports::FieldPhone|
		  ListReports::FieldEmail,tab,16,row);
    row++;
  }

  delete q;
}


void ListReports::AffiliatesByNetworkReport(SpreadSheet *sheet)
{
  int network_id=0;
  QDate date;
  QString sql;
  QString where;
  QSqlQuery *q;
  QString network_name;
  QDateTime dt=QDateTime(QDate::currentDate(),QTime::currentTime());

  PickFields *r=new PickFields(NULL,NULL,NULL,false,NULL,false,&network_id,
			       false,0,PickFields::NoMarket,this);
  if(r->exec()!=0) {
    delete r;
    return;
  }
  delete r;

  SpreadTab *tab=sheet->addTab(sheet->tabs()+1);
  tab->setName(tr("Affiliates by Network"));
  sql=QString().sprintf("select NAME from NETWORKS where ID=%d",network_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    network_name=q->value(0).toString();
  }
  delete q;
  where=QString().sprintf("where (NETWORKS.ID=%d) ",network_id);
  where+=" order by AFFILIATES.STATION_CALL,AFFILIATES.STATION_TYPE";
  RenderAffiliateReport(tab,where,tr("Affiliates by Network Report"),
			QString().sprintf("Network: %s",
					  (const char *)network_name),true,0);
}


void ListReports::AffiliatesByProgramReport(int contacts,SpreadSheet *sheet)
{
  int pgm_id=0;
  QDate date;
  QString sql;
  QString where;
  QSqlQuery *q;
  QString program_name;
  QDateTime dt=QDateTime(QDate::currentDate(),QTime::currentTime());
  PickFields::SortField sort=(PickFields::SortField)(PickFields::SortAffiliate|
						     PickFields::SortCityState|
						     PickFields::SortMarket);

  PickFields *r=new PickFields(NULL,NULL,&pgm_id,false,NULL,false,NULL,false,
			       &sort,PickFields::NoMarket,this);
  if(r->exec()!=0) {
    delete r;
    return;
  }
  delete r;

  SpreadTab *tab=sheet->addTab(sheet->tabs()+1);
  tab->setName(tr("Affiliates by Program"));
  sql=QString().sprintf("select PROGRAM_NAME from PROGRAMS where ID=%d",pgm_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    program_name=q->value(0).toString();
  }
  delete q;
  where=QString().sprintf("where (PROGRAMS.ID=%d) ",pgm_id);
  switch(sort) {
    case PickFields::SortAffiliate:
      where+=" order by AFFILIATES.STATION_CALL,AFFILIATES.STATION_TYPE";
      break;

    case PickFields::SortCityState:
      where+=" order by AFFILIATES.STATE,AFFILIATES.CITY";
      break;

    case PickFields::SortMarket:
      where+=
	" order by AFFILIATES.MARKET_NAME,AFFILIATES.STATE,AFFILIATES.CITY";
      break;
  }
  RenderAffiliateReport(tab,where,tr("Affiliates by Program Report"),
			QString().sprintf("Program: %s",
					  (const char *)program_name),false,
			contacts);
}


void ListReports::AffiliatesByDaypartReport(SpreadSheet *sheet)
{
  QString where;
  QString outfile;
  QString subtitle;
  QTime start_time(6,0,0);
  QTime end_time(19,59,59);
  bool dows[7]={true,true,true,true,true,false,false};

  PickDaypart *r=new PickDaypart(&start_time,&end_time,dows);
  if(r->exec()!=0) {
    delete r;
    return;
  }
  delete r;

  SpreadTab *tab=sheet->addTab(sheet->tabs()+1);
  tab->setName(tr("Affiliates by Daypart"));

  subtitle=QString().sprintf("Start Time: %s, End Time: %s, Days Selected: ",
			     (const char *)start_time.toString("hh:mm:ss"),
			     (const char *)end_time.toString("hh:mm:ss"));
  where=QString().sprintf("where (AIRINGS.AIR_TIME>=\"%s\")&&\
                          (AIRINGS.AIR_TIME<\"%s\")",
			  (const char *)start_time.toString("hh:mm:ss"),
			  (const char *)end_time.toString("hh:mm:ss"));
  if(dows[0]) {
    where+="&&(AIRINGS.AIR_MON=\"Y\")";
    subtitle+="Mo";
  }
  if(dows[1]) {
    where+="&&(AIRINGS.AIR_TUE=\"Y\")";
    subtitle+="Tu";
  }
  if(dows[2]) {
    where+="&&(AIRINGS.AIR_WED=\"Y\")";
    subtitle+="We";
  }
  if(dows[3]) {
    where+="&&(AIRINGS.AIR_THU=\"Y\")";
    subtitle+="Th";
  }
  if(dows[4]) {
    where+="&&(AIRINGS.AIR_FRI=\"Y\")";
    subtitle+="Fr";
  }
  if(dows[5]) {
    where+="&&(AIRINGS.AIR_SAT=\"Y\")";
    subtitle+="Sa";
  }
  if(dows[6]) {
    where+="&&(AIRINGS.AIR_SUN=\"Y\")";
    subtitle+="Su";
  }
  where+=" order by AFFILIATES.STATION_CALL,AFFILIATES.STATION_TYPE";
  RenderAffiliateReport(tab,where,tr("Affiliates by Daypart Report"),
			subtitle,true,0);
}


void ListReports::RenderAffiliateReport(SpreadTab *tab,const QString &where,
					const QString &title,const QString &sub,
					bool show_program_name,int contacts)
{
  QSqlQuery *q;
  QString dow;
  QString sql;
  int prev_id=-1;
  int row=1;

  tab->addCell(1,row++)->setText(title);
  if(!sub.isEmpty()) {
    tab->addCell(1,row++)->setText(sub);
  }
  tab->addCell(1,row++)->
    setText(tr("Report Date")+": "+
	    QDateTime(QDate::currentDate(),
		      QTime::currentTime()).toString("MM/dd/yyyy hh:mm:ss"));
  row++;
  tab->addCell(1,row)->setText(tr("CALL"));
  tab->addCell(2,row)->setText(tr("BAND"));
  tab->addCell(3,row)->setText(tr("FREQ"));
  tab->addCell(4,row)->setText(tr("CITY"));
  tab->addCell(5,row)->setText(tr("STATE"));
  tab->addCell(6,row)->setText(tr("MSA MARKET"));
  tab->addCell(7,row)->setText(tr("MSA RANK"));
  tab->addCell(8,row)->setText(tr("DMA MARKET"));
  tab->addCell(9,row)->setText(tr("DMA RANK"));
  int col=10;
  if(show_program_name) {
    tab->addCell(col++,row)->setText(tr("PROGRAM"));
  }
  tab->addCell(col++,row)->setText(tr("DAY OF WEEK"));
  tab->addCell(col++,row)->setText(tr("TIME"));
  if((contacts&ListReports::ProgramDirectorContact)!=0) {
    tab->addCell(col++,row)->setText(tr("PD NAME"));
    tab->addCell(col++,row)->setText(tr("PD PHONE"));
    tab->addCell(col++,row)->setText(tr("PD E-MAIL"));
  }
  if((contacts&ListReports::AffidavitContact)!=0) {
    tab->addCell(col++,row)->setText(tr("AFFIDAVIT NAME"));
    tab->addCell(col++,row)->setText(tr("AFFIDAVIT PHONE"));
    tab->addCell(col++,row)->setText(tr("AFFIDAVIT E-MAIL"));
  }
  row++;

  sql=QString().sprintf("select AFFILIATES.STATION_CALL,\
                         AFFILIATES.STATION_TYPE,AFFILIATES.STATION_FREQUENCY,\
                         AFFILIATES.CITY,AFFILIATES.STATE,\
                         AFFILIATES.MARKET_NAME,AFFILIATES.MARKET_RANK,\
                         AFFILIATES.DMA_NAME,AFFILIATES.DMA_RANK,\
                         AIRINGS.AIR_TIME,\
                         AIRINGS.AIR_LENGTH,AIRINGS.AIR_SUN,AIRINGS.AIR_MON,\
                         AIRINGS.AIR_TUE,AIRINGS.AIR_WED,AIRINGS.AIR_THU,\
                         AIRINGS.AIR_FRI,AIRINGS.AIR_SAT,PROGRAMS.PROGRAM_NAME,\
                         AFFILIATES.ID \
                         from AFFILIATES left join AIRINGS \
                         on (AFFILIATES.ID=AIRINGS.AFFILIATE_ID) \
                         left join PROGRAMS on (AIRINGS.PROGRAM_ID=PROGRAMS.ID)\
                         left join NETWORKS on \
                         (AFFILIATES.SECOND_NETWORK_ID=NETWORKS.ID)\
                         %s",(const char *)where);
  //printf("SQL: %s\n",(const char *)sql);
  q=new QSqlQuery(sql);
  while(q->next()) {
    col=1;
    if((prev_id>0)&&(q->value(19).toInt()!=prev_id)) {
      row++;
    }
    prev_id=q->value(16).toInt();
    tab->addCell(col++,row)->setText(q->value(0).toString().upper());
    tab->addCell(col++,row)->
      setText(DvtStationTypeString(q->value(1).toString()));
    tab->addCell(col++,row)->
      setText(DvtFormatFrequency(q->value(2).toDouble()));
    tab->addCell(col++,row)->setText(q->value(3).toString());
    tab->addCell(col++,row)->setText(q->value(4).toString().upper());
    tab->addCell(col++,row)->setText(q->value(5).toString());
    tab->addCell(col++,row)->
      setText(DvtMarketRankString(q->value(6).toInt()));
    tab->addCell(col++,row)->setText(q->value(7).toString());
    tab->addCell(col++,row)->
      setText(DvtMarketRankString(q->value(8).toInt()));
    if(show_program_name) {
      tab->addCell(col++,row)->setText(q->value(18).toString());
    }
    dow="";
    if((q->value(11).toString()=="N")&&
       (q->value(12).toString()=="Y")&&
       (q->value(13).toString()=="Y")&&
       (q->value(14).toString()=="Y")&&
       (q->value(15).toString()=="Y")&&
       (q->value(16).toString()=="Y")&&
       (q->value(17).toString()=="N")) {
      dow="M-F";
    }
    else {
      if(q->value(11).toString()=="Y") {
	dow+="Su";
      }
      if(q->value(12).toString()=="Y") {
	dow+="Mo";
      }
      if(q->value(13).toString()=="Y") {
	dow+="Tu";
      }
      if(q->value(14).toString()=="Y") {
	dow+="We";
      }
      if(q->value(15).toString()=="Y") {
	dow+="Th";
      }
      if(q->value(16).toString()=="Y") {
	dow+="Fr";
      }
      if(q->value(17).toString()=="Y") {
	dow+="Sa";
      }
    }
    if(!dow.isEmpty()) {
      tab->addCell(col++,row)->setText(dow);
      dow=(q->value(9).toTime().toString("h ap")+"-"+
	   q->value(9).toTime().addSecs(q->value(10).toInt()).
	   toString("h ap"));
      tab->addCell(col++,row)->setText(dow);
    }
    if((contacts&ListReports::ProgramDirectorContact)!=0) {
      ContactFields(q->value(10).toInt(),
		    ListReports::ProgramDirectorContact,
		    ListReports::FieldName|ListReports::FieldPhone|
		    ListReports::FieldEmail,
		    tab,col,row);
      col+=3;
    }
    if((contacts&ListReports::AffidavitContact)!=0) {
      ContactFields(q->value(10).toInt(),
		    ListReports::AffidavitContact,
		    ListReports::FieldName|ListReports::FieldPhone|
		    ListReports::FieldEmail,
		    tab,col,row);
    }
    row++;
  }
  delete q;
}
