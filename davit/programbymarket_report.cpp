// programbymarket_report.cpp
//
// Generate a Program by Market report.
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
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
#include <state_conv.h>

#include <pick_fields.h>
#include <list_reports.h>


void ListReports::ProgramByMarketReport(PickFields::MarketType type)
{
  QString s;
  QString sql;
  QSqlQuery *q=NULL;
  QString where;
  QString outfile;
  FILE *f=NULL;

  //
  // Get Market
  //
  PickFields *d=new PickFields(NULL,NULL,NULL,false,NULL,false,NULL,false,NULL,type,this);
  if(d->exec()<0) {
    return;
  }
  QString market=d->selectedMarket();
  QString city=d->selectedCity();
  QString state=d->selectedStateCode();
  delete d;

  //
  // Generate Report
  //
  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }
  fprintf(f,"ID;PSCALC3\n");
  fprintf(f,"C;X1;Y1;K\"Programs in %s\n",(const char *)market);
  fprintf(f,"C;X1;Y2;K\"Report Date: %s\"\n",
	  (const char *)QDate::currentDate().toString("MMMM dd, yyyy"));
  fprintf(f,"C;X1;Y4;K\"CALL LETTERS\"\n");
  fprintf(f,"C;X2;Y4;K\"FREQUENCY\"\n");
  fprintf(f,"C;X3;Y4;K\"CITY\"\n");
  fprintf(f,"C;X4;Y4;K\"STATE\"\n");
  fprintf(f,"C;X5;Y4;K\"DMA MARKET\"\n");
  fprintf(f,"C;X6;Y4;K\"MSA MARKET\"\n");
  fprintf(f,"C;X7;Y4;K\"PROGRAM\"\n");
  sql=QString("select AFFILIATES.ID,AFFILIATES.STATION_CALL,")+
    "AFFILIATES.STATION_TYPE,AFFILIATES.STATION_FREQUENCY,"+
    "AFFILIATES.LICENSE_CITY,AFFILIATES.LICENSE_STATE,DMA_NAME,MARKET_NAME,"+
    "PROGRAMS.PROGRAM_NAME "+
    "from AFFILIATES left join AIRINGS "+
    "on AFFILIATES.ID=AIRINGS.AFFILIATE_ID "+
    "left join PROGRAMS on AIRINGS.PROGRAM_ID=PROGRAMS.ID "+
    "where (AFFILIATES.IS_AFFILIATE=\"Y\")&&";
  switch(type) {
  case PickFields::NoMarket:
    if(!city.isEmpty()) {
      sql+="(AFFILIATES.LICENSE_CITY=\""+city+"\")&&";
      fprintf(f,"C;X1;Y1;K\"Programs in %s\n",
	      (const char *)DvtFormatCityState(city,state));
    }
    else {
      fprintf(f,"C;X1;Y1;K\"Programs in %s\n",
	      (const char *)AbbreviationToState(state));
    }
    sql+="(AFFILIATES.LICENSE_STATE=\""+state+"\")";
    break;

  case PickFields::DmaMarket:
    sql+="(DMA_NAME=\""+market+"\")";
    fprintf(f,"C;X1;Y1;K\"Programs in %s\n",(const char *)market);
    break;

  case PickFields::MsaMarket:
    sql+="(MARKET_NAME=\""+market+"\")";
    fprintf(f,"C;X1;Y1;K\"Programs in %s\n",(const char *)market);
    break;
  }
  sql+=" order by AFFILIATES.LICENSE_STATE,AFFILIATES.LICENSE_CITY,";
  sql+="PROGRAMS.PROGRAM_NAME";
  q=new QSqlQuery(sql);
  int row=5;
  while(q->next()) {
    // Call Letters
    fprintf(f,"C;X1;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)DvtStationCallString(q->value(1).toString(),
						      q->value(2).toString()));
    fprintf(f,"\"\n");
    
    // Frequency
    fprintf(f,"C;X2;Y%d;K\"",row);
    fprintf(f,"%s",
	    (const char *)DvtFormatFrequency(q->value(3).toDouble()));
    fprintf(f,"\"\n");

    // City
    fprintf(f,"C;X3;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)q->value(4).toString());
    fprintf(f,"\"\n");

    // State
    fprintf(f,"C;X4;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)q->value(5).toString().upper());
    fprintf(f,"\"\n");

    // DMA Market
    fprintf(f,"C;X5;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)q->value(6).toString());
    fprintf(f,"\"\n");

    // MSA Market
    fprintf(f,"C;X6;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)q->value(7).toString());
    fprintf(f,"\"\n");

    // Program
    fprintf(f,"C;X7;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)q->value(8).toString());
    fprintf(f,"\"\n");

    row++;
  }
  delete q;
  fprintf(f,"E\n");
  fclose(f);
  ForkViewer(outfile);
}
