// activity_report.cpp
//
// The RadioAmerica Affiliate Report for Davit
//
//   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: activity_report.cpp,v 1.4 2011/03/29 18:10:33 pcvs Exp $
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

void ListReports::ActivityReport()
{
  int affiliate_id=0;
  QDate start_date=QDate::currentDate().addMonths(-1);
  QDate end_date=QDate::currentDate();
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
  FILE *f=NULL;
  QString outfile;
  QString dow;
  QDateTime dt=QDateTime(QDate::currentDate(),QTime::currentTime());

  PickFields *r=new PickFields(&start_date,&end_date,NULL,true,
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
  fprintf(f,"\"Affiliate Activity Report\"\n");
  fprintf(f,"\"Report Date: %s\"\n",(const char *)dt.toString("MM/dd/yyyy hh:mm:ss"));
  fprintf(f,"\n");
  fprintf(f,"\"CALL\",\"PROGRAM DIRECTOR\",\"PHONE\",\"DATE\",\"USER NAME\",\"REMARKS\"\n");
  sql="select AFFILIATES.ID,AFFILIATES.STATION_CALL,AFFILIATES.STATION_TYPE,\
       CONTACTS.NAME,CONTACTS.PHONE \
       from AFFILIATES left join CONTACTS \
       on AFFILIATES.ID=CONTACTS.AFFILIATE_ID \
       where (CONTACTS.PROGRAM_DIRECTOR=\"Y\")";
  if(affiliate_id>0) {
    sql+=QString().sprintf("&& (AFFILIATES.ID=%d)",affiliate_id);
  }
  sql+=" order by AFFILIATES.STATION_CALL";
  q=new QSqlQuery(sql);
  while(q->next()) {
    sql=QString().sprintf("select AFFILIATE_REMARKS.REMARK_DATETIME,\
                           AFFILIATE_REMARKS.USER_NAME,\
                           AFFILIATE_REMARKS.REMARK \
                           from AFFILIATE_REMARKS \
                           where (AFFILIATE_REMARKS.AFFILIATE_ID=%d)&&",
			  q->value(0).toInt());
    sql+=QString().
      sprintf("(AFFILIATE_REMARKS.REMARK_DATETIME>=\"%s 00:00:00\")&&",
	      (const char *)start_date.toString("yyyy-MM-dd"));
    sql+=QString().
      sprintf("(AFFILIATE_REMARKS.REMARK_DATETIME<=\"%s 23:59:59\")",
	      (const char *)end_date.toString("yyyy-MM-dd"));
    sql+=" order by AFFILIATE_REMARKS.REMARK_DATETIME";
    q1=new QSqlQuery(sql);
    if(q1->size()>0) {
      fprintf(f,"\"%s",(const char *)q->value(1).toString());
      if(q->value(2).toString().lower()=="a") {
	fprintf(f,"-AM\",");
      }
      else {
	if(q->value(2).toString().lower()=="f") {
	  fprintf(f,"-FM\",");
	}
	else {
	  fprintf(f,"\",");
	}
      }
      fprintf(f,(const char *)PhoneField(q->value(3)));
      fprintf(f,",\"****\",\"****\",\"****\"");
      fprintf(f,"\n");
      while(q1->next()) {
	fprintf(f,"\"****\",\"****\",\"****\",");
	fprintf(f,"\"%s\",",(const char *)
		q1->value(0).toDateTime().toString("MM/dd/yyyy"));
	fprintf(f,(const char *)StringField(q1->value(1)));
	fprintf(f,(const char *)StringField(q1->value(2)));
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
