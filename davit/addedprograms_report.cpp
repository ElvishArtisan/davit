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

#include <dvt.h>
#include <dvtconf.h>

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
  if(type==Dvt::RemarkProgramAdd) {
    fprintf(f,"\"Added Programs Report\"\n");
  }
  else {
    fprintf(f,"\"Deleted Programs Report\"\n");
  }
  fprintf(f,"\"Report Date: %s\"\n",(const char *)dt.toString("MM/dd/yyyy hh:mm:ss"));
  fprintf(f,"\n");
  fprintf(f,"\"CALL\",\"BAND\",\"FREQ\",\"ADDRESS\",\"CITY\",\"STATE\",\"MARKET\",\"PROGRAM DIRECTOR\",\"PHONE\",\"FAX\",\"E-MAIL\"\n");
  sql="select AFFILIATES.ID,AFFILIATES.STATION_CALL,AFFILIATES.STATION_TYPE,\
       AFFILIATES.STATION_FREQUENCY,AFFILIATES.ADDRESS1,AFFILIATES.ADDRESS2,\
       AFFILIATES.CITY,AFFILIATES.STATE,AFFILIATES.ZIPCODE,\
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
  while(q->next()) {
    sql=QString().sprintf("select PROGRAMS.PROGRAM_NAME,\
                           AFFILIATE_REMARKS.REMARK_DATETIME \
                           from AFFILIATE_REMARKS right join PROGRAMS \
                           on (AFFILIATE_REMARKS.PROGRAM_ID=PROGRAMS.ID)\
                           where (AFFILIATE_REMARKS.EVENT_TYPE=%d)&&\
                           (AFFILIATE_REMARKS.AFFILIATE_ID=%d)",
			  type,
			  q->value(0).toInt());
    if(pgm_id>0) {
      sql+=QString().sprintf("&&(AFFILIATE_REMARKS.PROGRAM_ID=%d)",pgm_id);
    }
    q1=new QSqlQuery(sql);
    if(q1->size()>0) {
      fprintf(f,"\"%s\",",(const char *)q->value(1).toString());

      fprintf(f,(const char *)TypeField(q->value(2)));
      fprintf(f,(const char *)FrequencyField(q->value(3)));
      fprintf(f,(const char *)AddressField(q->value(4),q->value(5)));
      fprintf(f,(const char *)StringField(q->value(6)));
      fprintf(f,(const char *)StringField(q->value(7)).upper());
      fprintf(f,(const char *)StringField(q->value(9)));
      fprintf(f,(const char *)ContactFields(q->value(0).toInt(),
					    ListReports::ProgramDirectorContact,
					    ListReports::FieldName|
					    ListReports::FieldPhone|
					    ListReports::FieldFax|
					    ListReports::FieldEmail));
      fprintf(f,"\n");
      if(type==Dvt::RemarkProgramAdd) {
	fprintf(f,"\"\",\"PROGRAM\",\"ADDED ON\"\n");
      }
      else {
	fprintf(f,"\"\",\"PROGRAM\",\"DELETED ON\"\n");
      }
      while(q1->next()) {
	fprintf(f,"\"\",");
	fprintf(f,"\"%s\",",(const char *)q1->value(0).toString());
	fprintf(f,"\"%s\"",(const char *)q1->value(1).toDateTime().
		toString("MM/dd/yyyy"));
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
