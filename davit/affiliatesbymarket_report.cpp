// affiliatesbymarket_report.cpp
//
// Generate an Affiliates by DMA|MSA Market report.
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: affiliatesbymarket_report.cpp,v 1.2 2013/02/26 19:06:17 pcvs Exp $
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

#include <qfile.h>
#include <qsqldatabase.h>
#include <qmessagebox.h>

#include <dvttextfile.h>
#include <dvtconf.h>

#include <list_reports.h>

void ListReports::AffiliatesByMarketReport(PickFields::MarketType type)
{
  int pgm_id;
  QString market;

  //
  // Get Parameters
  //
  PickFields *r=new PickFields(NULL,NULL,&pgm_id,false,NULL,false,NULL,false,
			       NULL,type,this);
  if(r->exec()!=0) {
    delete r;
    return;
  }
  market=r->selectedMarket();
  delete r;

  //
  // Generate Report
  //
  QString s;
  QString sql;
  QString field_name;
  QSqlQuery *q;
  QString outfile;
  FILE *f=NULL;
  int row=6;
  std::vector<int> ids;

  //
  // Generate Report
  //
  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }
  fprintf(f,"ID;PSCALC3\n");

  switch(type) {
  case PickFields::DmaMarket:
    fprintf(f,"C;X1;Y1;K\"Affiliates by Program/DMA Market Report\"\n");
    field_name="DMA_NAME";
    break;

  case PickFields::MsaMarket:
    fprintf(f,"C;X1;Y1;K\"Affiliates by Program/MSA Market Report\"\n");
    field_name="MARKET_NAME";
    break;

  case PickFields::NoMarket:
    break;
  }
  fprintf(f,"C;X2;Y1;K\"Report Date: %s\"\n",
	  (const char *)QDate::currentDate().toString("MMMM dd, yyyy"));
  sql=QString().sprintf("select PROGRAM_NAME from PROGRAMS where ID=%d",
			pgm_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    fprintf(f,"C;X2;Y2;K\"Program: %s\"\n",
	    (const char *)q->value(0).toString());
  }
  delete q;
  fprintf(f,"C;X2;Y3;K\"Market: %s\"\n",(const char *)market);
  fprintf(f,"C;X1;Y5;K\"AFFILIATE\"\n");
  fprintf(f,"C;X2;Y5;K\"START\"\n");
  fprintf(f,"C;X3;Y5;K\"LENGTH\"\n");
  fprintf(f,"C;X4;Y5;K\"SUN\"\n");
  fprintf(f,"C;X5;Y5;K\"MON\"\n");
  fprintf(f,"C;X6;Y5;K\"TUE\"\n");
  fprintf(f,"C;X7;Y5;K\"WED\"\n");
  fprintf(f,"C;X8;Y5;K\"THU\"\n");
  fprintf(f,"C;X9;Y5;K\"FRI\"\n");
  fprintf(f,"C;X10;Y5;K\"SAT\"\n");

  sql=QString().sprintf("select AIRINGS.AIR_SUN,\
                                AIRINGS.AIR_MON,\
                                AIRINGS.AIR_TUE,\
                                AIRINGS.AIR_WED,\
                                AIRINGS.AIR_THU,\
                                AIRINGS.AIR_FRI,\
                                AIRINGS.AIR_SAT,\
                                AIRINGS.AIR_TIME,\
                                AIRINGS.AIR_LENGTH,\
                                AFFILIATES.STATION_CALL,\
                                AFFILIATES.STATION_TYPE,\
                                AFFILIATES.LICENSE_CITY,\
                                AFFILIATES.LICENSE_STATE \
                                from AFFILIATES left join AIRINGS \
                                on (AFFILIATES.ID=AIRINGS.AFFILIATE_ID) \
                                where (PROGRAM_ID=%d)&&(%s=\"%s\")",
			pgm_id,
			(const char *)field_name,
			(const char *)market);
  q=new QSqlQuery(sql);
  while(q->next()) {
    // Affiliate
    fprintf(f,"C;X1;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)q->value(9).toString());
    if(q->value(10).toString().lower()=="a") {
      fprintf(f,"%s","-AM");
    }
    else {
      if(q->value(10).toString().lower()=="f") {
	fprintf(f,"%s","-FM");
      }
    }
    fprintf(f," -- %s, %s\"\n",(const char *)q->value(11).toString(),
	    (const char *)q->value(12).toString());
    fprintf(f,"\n");

    //
    // Start time
    //
    fprintf(f,"C;X2;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)q->value(7).toTime().toString("hh:mm:ss"));
    fprintf(f,"\n");

    //
    // Length
    //
    fprintf(f,"C;X3;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)DvtGetTimeLength(q->value(8).toInt()*1000,
						  false,false));
    fprintf(f,"\n");

    //
    // Days of Week
    //
    for(int i=0;i<7;i++) {
      fprintf(f,"C;X%d;Y%d;K\"",i+4,row);
      if(q->value(i).toString()=="Y") {
	fprintf(f,"X");
      }
      fprintf(f,"\n");
    }

    row++;
  }
  delete q;


  /*
  sql="select ID,STATION_CALL,STATION_TYPE from AFFILIATES where (";
  GetActiveAffiliates(&ids);
  for(unsigned i=0;i<ids.size();i++) {
    sql+=QString().sprintf("(ID=%d)||",ids[i]);
  }
  sql=sql.left(sql.length()-2);
  sql+=") order by STATION_CALL desc";
  q=new QSqlQuery(sql);
  while(q->next()) {
    // Call Letters
    fprintf(f,"C;X1;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)q->value(1).toString());
    if(q->value(2).toString().lower()=="a") {
      fprintf(f,"%s","-AM");
    }
    else {
      if(q->value(2).toString().lower()=="f") {
	fprintf(f,"%s","-FM");
      }
    }
    fprintf(f,"\"\n");

    // E-Mail Address
    fprintf(f,"C;X2;Y%d;K\"",row++);
    fprintf(f,"%s",
	    (const char *)GetEmailContactList(q->value(0).toInt(),true,false));
    fprintf(f,"\"\n");
  }
  delete q;
  */
  fprintf(f,"E\n");
  fclose(f);
  ForkViewer(outfile);
}



/*
void ListReports::AffidavitReport()
{
  QString s;
  QString sql;
  QSqlQuery *q;
  QString outfile;
  FILE *f=NULL;
  int row=4;
  std::vector<int> ids;

  //
  // Generate Report
  //
  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }
  fprintf(f,"ID;PSCALC3\n");
  fprintf(f,"C;X1;Y1;K\"Affiliate Affidavit Contacts\"\n");
  fprintf(f,"C;X2;Y1;K\"Report Date: %s\"\n",
	  (const char *)QDate::currentDate().toString("MMMM dd, yyyy"));
  fprintf(f,"C;X1;Y3;K\"CALL LETTERS\"\n");
  fprintf(f,"C;X2;Y3;K\"AFFIDAVIT CONTACT E-MAIL\"\n");
  sql="select ID,STATION_CALL,STATION_TYPE from AFFILIATES where (";
  GetActiveAffiliates(&ids);
  for(unsigned i=0;i<ids.size();i++) {
    sql+=QString().sprintf("(ID=%d)||",ids[i]);
  }
  sql=sql.left(sql.length()-2);
  sql+=") order by STATION_CALL desc";
  q=new QSqlQuery(sql);
  while(q->next()) {
    // Call Letters
    fprintf(f,"C;X1;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)q->value(1).toString());
    if(q->value(2).toString().lower()=="a") {
      fprintf(f,"%s","-AM");
    }
    else {
      if(q->value(2).toString().lower()=="f") {
	fprintf(f,"%s","-FM");
      }
    }
    fprintf(f,"\"\n");

    // E-Mail Address
    fprintf(f,"C;X2;Y%d;K\"",row++);
    fprintf(f,"%s",
	    (const char *)GetEmailContactList(q->value(0).toInt(),true,false));
    fprintf(f,"\"\n");
  }
  delete q;
  fprintf(f,"E\n");
  fclose(f);
  ForkViewer(outfile);
}
*/
