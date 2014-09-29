// arbitron_report.cpp
//
// The Arbitron Report for Davit
//
//   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: arbitron_report.cpp,v 1.11 2011/02/21 23:41:18 pcvs Exp $
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

void ListReports::ArbitronReport()
{
  int pgm_id=0;
  QDate date;
  QString sql;
  QSqlQuery *q;
  FILE *f=NULL;
  QString outfile;
  QString dow;
  int row=1;
  QDateTime dt=QDateTime(QDate::currentDate(),QTime::currentTime());

  PickFields *r=
    new PickFields(NULL,NULL,&pgm_id,false,NULL,false,NULL,false,NULL,
		   PickFields::NoMarket,this);
  if(r->exec()!=0) {
    delete r;
    return;
  }
  delete r;

  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }
  fprintf(f,"ID;PSCALC3\n");
  sql=QString().sprintf("select PROGRAM_NAME from PROGRAMS where ID=%d",pgm_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    fprintf(f,"C;X1;Y%d;K\"%s\"\n",row++,(const char *)q->value(0).toString());
  }
  delete q;
  fprintf(f,"C;X1;Y%d;K\"Arbitron Listing - %s\"\n",row++,
	  (const char *)dt.toString("MM/dd/yyyy hh:mm:ss"));
  sql=QString().sprintf("select AFFILIATES.STATION_CALL,\
                         AFFILIATES.STATION_TYPE,AIRINGS.AIR_TIME,\
                         AIRINGS.AIR_LENGTH,AIRINGS.AIR_SUN,AIRINGS.AIR_MON,\
                         AIRINGS.AIR_TUE,AIRINGS.AIR_WED,AIRINGS.AIR_THU,\
                         AIRINGS.AIR_FRI,AIRINGS.AIR_SAT \
                         from AFFILIATES left join AIRINGS \
                         on (AFFILIATES.ID=AIRINGS.AFFILIATE_ID) \
                         left join PROGRAMS on (AIRINGS.PROGRAM_ID=PROGRAMS.ID)\
                         where PROGRAMS.ID=%d",pgm_id);
  q=new QSqlQuery(sql);
  fprintf(f,"C;X1;Y%d;K\"Listings Generated: %d\"\n",row++,q->size());
  while(q->next()) {
    fprintf(f,"C;X1;Y%d;K\"",row++);
    fprintf(f,"%s",(const char *)q->value(0).toString().upper());
    if(q->value(1).toString().lower()=="a") {
      fprintf(f,"%s","-AM,");
    }
    else {
      if(q->value(1).toString().lower()=="f") {
	fprintf(f,"%s","-FM,");
      }
    }
    if((q->value(5).toString()=="Y")&&(q->value(6).toString()=="Y")&&
       (q->value(7).toString()=="Y")&&(q->value(8).toString()=="Y")&&
       (q->value(9).toString()=="Y")) {
      dow="MF";
    }
    else {
      dow="";
      if(q->value(5).toString()=="Y") {
	dow+="M";
      }
      if(q->value(6).toString()=="Y") {
	dow+="T";
      }
      if(q->value(7).toString()=="Y") {
	dow+="W";
      }
      if(q->value(8).toString()=="Y") {
	dow+="R";
      }
      if(q->value(9).toString()=="Y") {
	dow+="F";
      }
      if(q->value(10).toString()=="Y") {
	dow+="SA";
      }
      if(q->value(4).toString()=="Y") {
	dow+="SU";
      }
    }
    if(!dow.isEmpty()) {
      if(q->value(2).toTime().hour()<12) {
	dow+=QString().sprintf("%d",q->value(2).toTime().hour());
	dow+="a-";
      }
      else {
	dow+=QString().sprintf("%d",q->value(2).toTime().hour()-12);
	dow+="p-";
      }
      if(q->value(2).toTime().hour()<12) {
	dow+=QString().sprintf("%d",q->value(2).toTime().hour()+1);
	dow+="a";
      }
      else {
	dow+=QString().sprintf("%d",q->value(2).toTime().hour()-11);
	dow+="p";
      }
      fprintf(f," %s",(const char *)dow);
    }
    fprintf(f,"\"\n");
  }
  delete q;
  fprintf(f,"E\n");
  fclose(f);
  ForkViewer(outfile);
}
