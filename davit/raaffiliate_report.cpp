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
  QSqlQuery *q1;
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
  fprintf(f,"\"RadioAmerica Affiliates Report\"\n");
  fprintf(f,"\"Report Date: %s\"\n",(const char *)dt.toString("MM/dd/yyyy hh:mm:ss"));
  fprintf(f,"\n");
  fprintf(f,"\"CALL\",\"BAND\",\"FREQ\",\"ADDRESS1\",\"CITY\",\"STATE\",\"MSA MARKET\",\"MSA RANK\",\"DMA MARKET\",\"DMA RANK\",\"PD NAME\",\"PD PHONE\",\"PD FAX\",\"PD E-MAIL\",\"AFFIDAVIT NAME\",\"AFFIDAVIT E-MAIL\"\n");
  sql="select ID,STATION_CALL,STATION_TYPE,STATION_FREQUENCY,ADDRESS1,\
       ADDRESS2,CITY,STATE,ZIPCODE,MARKET_NAME,MARKET_RANK,DMA_NAME,DMA_RANK \
       from AFFILIATES";
  if(affiliate_id>0) {
    sql+=QString().sprintf(" where ID=%d",affiliate_id);
  }
  sql+=" order by STATE,CITY,STATION_CALL";
  q=new QSqlQuery(sql);
  while(q->next()) {
    sql=QString().sprintf("select PROGRAMS.PROGRAM_NAME,AIRINGS.AIR_TIME,\
                         AIRINGS.AIR_LENGTH,AIRINGS.AIR_SUN,AIRINGS.AIR_MON,\
                         AIRINGS.AIR_TUE,AIRINGS.AIR_WED,AIRINGS.AIR_THU,\
                         AIRINGS.AIR_FRI,AIRINGS.AIR_SAT \
                         from AIRINGS right join PROGRAMS \
                         on (AIRINGS.PROGRAM_ID=PROGRAMS.ID)\
                         where (AIRINGS.AFFILIATE_ID=%d)",q->value(0).toInt());
    if(pgm_id>0) {
      sql+=QString().sprintf("&&(PROGRAMS.ID=%d)",pgm_id);
    }
    q1=new QSqlQuery(sql);
    if(q1->size()>0) {
      fprintf(f,"\"%s\",",(const char *)q->value(1).toString());

      fprintf(f,(const char *)TypeField(q->value(2)));
      fprintf(f,(const char *)FrequencyField(q->value(3)));
      fprintf(f,(const char *)AddressField(q->value(4),q->value(5)));
      fprintf(f,(const char *)StringField(q->value(6)));
      fprintf(f,(const char *)StringField(q->value(7)));
      if(q->value(9).toString().isEmpty()) {
	fprintf(f,(const char *)EmptyField());
	fprintf(f,(const char *)EmptyField());
      }
      else {
	fprintf(f,(const char *)StringField(q->value(9)));
	fprintf(f,(const char *)StringField(q->value(10)));
      }
      if(q->value(11).toString().isEmpty()) {
	fprintf(f,(const char *)EmptyField());
	fprintf(f,(const char *)EmptyField());
      }
      else {
	fprintf(f,(const char *)StringField(q->value(11)));
	fprintf(f,(const char *)StringField(q->value(12)));
      }
      fprintf(f,(const char *)ContactFields(q->value(0).toInt(),
					    ListReports::ProgramDirectorContact,
					    ListReports::FieldName|
					    ListReports::FieldPhone|
					    ListReports::FieldFax|
					    ListReports::FieldEmail));
      fprintf(f,(const char *)ContactFields(q->value(0).toInt(),
					    ListReports::AffidavitContact,
					    ListReports::FieldName|
					    ListReports::FieldEmail));
      fprintf(f,"\n");
      fprintf(f,"\"\",\"PROGRAM\",\"AIRING\"\n");
      while(q1->next()) {
	dow="";
	if(q1->value(3).toString()=="Y") {
	  dow+="Su";
	}
	if(q1->value(4).toString()=="Y") {
	  dow+="Mo";
	}
	if(q1->value(5).toString()=="Y") {
	  dow+="Tu";
	}
	if(q1->value(6).toString()=="Y") {
	  dow+="We";
	}
	if(q1->value(7).toString()=="Y") {
	  dow+="Th";
	}
	if(q1->value(8).toString()=="Y") {
	  dow+="Fr";
	}
	if(q1->value(9).toString()=="Y") {
	  dow+="Sa";
	}
	if(!dow.isEmpty()) {
	  fprintf(f,"\"\",\"%s\",",(const char *)q1->value(0).toString());
	  dow+=(", "+q1->value(1).toTime().toString("h ap")+"-"+
	    q1->value(1).toTime().
		addSecs(q1->value(2).toInt()).toString("h ap"));
	  fprintf(f,"\"%s\"",(const char *)dow);
	}
	fprintf(f,"\n");
      }
      fprintf(f,"\n");
    }
    delete q1;
  }
  delete q;
  fclose(f);
  ForkViewer(outfile);
}
