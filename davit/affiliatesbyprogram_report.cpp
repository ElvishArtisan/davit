// affiliatesbyprogram_report.cpp
//
// The AffiliatesByProgram Report for Davit
//
//   (C) Copyright 2008,2010 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: affiliatesbyprogram_report.cpp,v 1.14 2012/02/09 17:56:33 pcvs Exp $
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
#include <pick_daypart.h>
#include <list_reports.h>

void ListReports::AllAffiliatesReport()
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
  QString outfile;
  FILE *f=NULL;
  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }
  RenderAffiliateReport(f,where,tr("All Affiliates Report"),"",true,0);
  fclose(f);
  ForkViewer(outfile);
}


void ListReports::AllAffiliateContacts()
{
  QString sql;
  QSqlQuery *q;

  QString outfile;
  FILE *f=NULL;
  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }

  //
  // Report Header
  //
  fprintf(f,"\"All Affiliate Contacts Report\"\n");
  fprintf(f,"\"Report Date: %s\"\n",(const char *)
	  QDateTime(QDate::currentDate(),
		    QTime::currentTime()).toString("MM/dd/yyyy hh:mm:ss"));
  fprintf(f,"\n");
  fprintf(f,"\"CALL\",\"BAND\",\"FREQ\",\"CITY\",\"STATE\",\"MSA MARKET\",");
  fprintf(f,"\"MSA RANK\",\"DMA MARKET\",\"DMA RANK\",");
  fprintf(f,"\"PD NAME\",\"PD PHONE\",\"PD E-MAIL\"");
  fprintf(f,",\"AFFIDAVIT NAME\",\"AFFIDAVIT PHONE\",\"AFFIDAVIT E-MAIL\"");
  fprintf(f,",\"GM NAME\",\"GM PHONE\",\"GM E-MAIL\"");
  fprintf(f,"\n");

  sql="select ID,STATION_CALL,STATION_TYPE,STATION_FREQUENCY,CITY,\
              STATE,MARKET_NAME,MARKET_RANK,DMA_NAME,DMA_RANK from AFFILIATES \
              where IS_AFFILIATE=\"Y\" order by STATION_CALL,STATION_TYPE";
  q=new QSqlQuery(sql);
  while(q->next()) {
    /*
    if((prev_id>0)&&(q->value(19).toInt()!=prev_id)) {
      fprintf(f,"\n");
    }
    prev_id=q->value(16).toInt();
    */
    fprintf(f,(const char *)StringField(q->value(1).toString().upper()));
    fprintf(f,(const char *)TypeField(q->value(2)));
    fprintf(f,(const char *)FrequencyField(q->value(3)));
    fprintf(f,(const char *)StringField(q->value(4)));
    fprintf(f,(const char *)StringField(q->value(5)).upper());
    if(q->value(6).toString().isEmpty()) {
      fprintf(f,"\"\",");
      fprintf(f,"\"\",");
    }
    else {
      fprintf(f,(const char *)StringField(q->value(6)));
      if(q->value(7).toInt()>0) {
	fprintf(f,"\"%d\",",q->value(7).toInt());
      }
      else {
	fprintf(f,"\"\",");
      }
    }

    if(q->value(8).toString().isEmpty()) {
      fprintf(f,"\"\",");
      fprintf(f,"\"\",");
    }
    else {
      fprintf(f,(const char *)StringField(q->value(8)));
      if(q->value(9).toInt()>0) {
	fprintf(f,"\"%d\",",q->value(9).toInt());
      }
      else {
	fprintf(f,"\"\",");
      }
    }

    fprintf(f,"%s",(const char *)ContactFields(q->value(0).toInt(),
		    ListReports::ProgramDirectorContact,
		    ListReports::FieldName|ListReports::FieldPhone|
		    ListReports::FieldEmail));

    fprintf(f,"%s",(const char *)ContactFields(q->value(0).toInt(),
		    ListReports::AffidavitContact,
		    ListReports::FieldName|ListReports::FieldPhone|
		    ListReports::FieldEmail));

    fprintf(f,"%s",(const char *)ContactFields(q->value(0).toInt(),
		    ListReports::GeneralManagerContact,
		    ListReports::FieldName|ListReports::FieldPhone|
		    ListReports::FieldEmail));
    fprintf(f,"\n");
  }

  delete q;

  fclose(f);
  ForkViewer(outfile);
}


void ListReports::AffiliatesByNetworkReport()
{
  int network_id=0;
  QDate date;
  QString sql;
  QString where;
  QSqlQuery *q;
  FILE *f=NULL;
  QString outfile;
  QString network_name;
  QDateTime dt=QDateTime(QDate::currentDate(),QTime::currentTime());

  PickFields *r=new PickFields(NULL,NULL,NULL,false,NULL,false,&network_id,
			       false,0,PickFields::NoMarket,this);
  if(r->exec()!=0) {
    delete r;
    return;
  }
  delete r;

  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }
  sql=QString().sprintf("select NAME from NETWORKS where ID=%d",network_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    network_name=q->value(0).toString();
  }
  delete q;
  where=QString().sprintf("where (NETWORKS.ID=%d) ",network_id);
  where+=" order by AFFILIATES.STATION_CALL,AFFILIATES.STATION_TYPE";
  RenderAffiliateReport(f,where,tr("Affiliates by Network Report"),
			QString().sprintf("Network: %s",
					  (const char *)network_name),true,0);
  fclose(f);
  ForkViewer(outfile);
}


void ListReports::AffiliatesByProgramReport(int contacts)
{
  int pgm_id=0;
  QDate date;
  QString sql;
  QString where;
  QSqlQuery *q;
  FILE *f=NULL;
  QString outfile;
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

  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }
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
  RenderAffiliateReport(f,where,tr("Affiliates by Program Report"),
			QString().sprintf("Program: %s",
					  (const char *)program_name),false,
			contacts);
  fclose(f);
  ForkViewer(outfile);
}


void ListReports::AffiliatesByDaypartReport()
{
  FILE *f=NULL;
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

  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }

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
  RenderAffiliateReport(f,where,tr("Affiliates by Daypart Report"),
			subtitle,true,0);
  fclose(f);
  ForkViewer(outfile);
}


void ListReports::RenderAffiliateReport(FILE *f,const QString &where,
					const QString &title,const QString &sub,
					bool show_program_name,int contacts)
{
  QSqlQuery *q;
  QString dow;
  QString sql;
  int prev_id=-1;

  fprintf(f,"\"%s\"\n",(const char *)title);
  if(!sub.isEmpty()) {
    fprintf(f,"\"%s\"\n",(const char *)sub);
  }
  fprintf(f,"\"Report Date: %s\"\n",(const char *)
	  QDateTime(QDate::currentDate(),
		    QTime::currentTime()).toString("MM/dd/yyyy hh:mm:ss"));
  fprintf(f,"\n");
  fprintf(f,"\"CALL\",\"BAND\",\"FREQ\",\"CITY\",\"STATE\",\"MSA MARKET\",");
  fprintf(f,"\"MSA RANK\",\"DMA MARKET\",\"DMA RANK\",");
  if(show_program_name) {
    fprintf(f,"\"PROGRAM\",");
  }
  fprintf(f,"\"DAY OF WEEK\",\"TIME\"");
  if((contacts&ListReports::ProgramDirectorContact)!=0) {
    fprintf(f,",\"PD NAME\",\"PD PHONE\",\"PD E-MAIL\"");
  }
  if((contacts&ListReports::AffidavitContact)!=0) {
    fprintf(f,",\"AFFIDAVIT NAME\",\"AFFIDAVIT PHONE\",\"AFFIDAVIT E-MAIL\"");
  }
  fprintf(f,"\n");

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
    if((prev_id>0)&&(q->value(19).toInt()!=prev_id)) {
      fprintf(f,"\n");
    }
    prev_id=q->value(16).toInt();
    fprintf(f,(const char *)StringField(q->value(0).toString().upper()));
    fprintf(f,(const char *)TypeField(q->value(1)));
    fprintf(f,(const char *)FrequencyField(q->value(2)));
    fprintf(f,(const char *)StringField(q->value(3)));
    fprintf(f,(const char *)StringField(q->value(4)).upper());
    if(q->value(5).toString().isEmpty()) {
      fprintf(f,"\"\",");
      fprintf(f,"\"\",");
    }
    else {
      fprintf(f,(const char *)StringField(q->value(5)));
      if(q->value(6).toInt()>0) {
	fprintf(f,"\"%d\",",q->value(6).toInt());
      }
      else {
	fprintf(f,"\"\",");
      }
    }

    if(q->value(7).toString().isEmpty()) {
      fprintf(f,"\"\",");
      fprintf(f,"\"\",");
    }
    else {
      fprintf(f,(const char *)StringField(q->value(7)));
      if(q->value(8).toInt()>0) {
	fprintf(f,"\"%d\",",q->value(8).toInt());
      }
      else {
	fprintf(f,"\"\",");
      }
    }

    if(show_program_name) {
      fprintf(f,(const char *)StringField(q->value(18)));
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
      fprintf(f,"\"%s\"",(const char *)dow);
      dow=(q->value(9).toTime().toString("h ap")+"-"+
	    q->value(9).toTime().addSecs(q->value(10).toInt()).toString("h ap"));
      fprintf(f,",\"%s\",",(const char *)dow);
    }
    if((contacts&ListReports::ProgramDirectorContact)!=0) {
      fprintf(f,"%s",(const char *)ContactFields(q->value(19).toInt(),
		    ListReports::ProgramDirectorContact,
		    ListReports::FieldName|ListReports::FieldPhone|
		    ListReports::FieldEmail));
    }
    if((contacts&ListReports::AffidavitContact)!=0) {
      fprintf(f,"%s",(const char *)ContactFields(q->value(19).toInt(),
		    ListReports::AffidavitContact,
		    ListReports::FieldName|ListReports::FieldPhone|
		    ListReports::FieldEmail));
    }

    fprintf(f,"\n");
  }
  delete q;
}
