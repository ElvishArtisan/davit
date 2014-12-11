// missingaffidavitcontact_report.cpp
//
// Generate an Affiliate Missing Affidavit Contact List Report
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

#include <pick_fields.h>
#include <list_reports.h>


void ListReports::MissingAffidavitContactReport()
{
  QString s;
  QString sql;
  QSqlQuery *q=NULL;
  QSqlQuery *q1=NULL;
  QString where;
  QString outfile;
  FILE *f=NULL;
  int row=4;

  //
  // Generate Report
  //
  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }
  fprintf(f,"ID;PSCALC3\n");
  fprintf(f,"C;X1;Y1;K\"Missing Affidavit Contacts\n");
  fprintf(f,"C;X2;Y1;K\"Report Date: %s\"\n",
	  (const char *)QDate::currentDate().toString("MMMM dd, yyyy"));
  fprintf(f,"C;X1;Y3;K\"CALL LETTERS\"\n");
  fprintf(f,"C;X2;Y3;K\"STATION PHONE\"\n");
  sql=QString("select ID,STATION_CALL,STATION_TYPE,PHONE from AFFILIATES ")+
    "where IS_AFFILIATE=\"Y\"";
  q=new QSqlQuery(sql);
  while(q->next()) {
    sql=QString("select ID from CONTACTS where ")+
      QString().sprintf("(AFFILIATE_ID=%d)&&",q->value(0).toInt())+
      "(CONTACTS.AFFIDAVIT=\"Y\")&&"+
      "((CONTACTS.EMAIL!=\"\")&&(CONTACTS.EMAIL is not null))||"+
      "((CONTACTS.PHONE!=\"\")&&(CONTACTS.PHONE is null))";
    q1=new QSqlQuery(sql);
    if(!q1->first()) {
      // Call Letters
      fprintf(f,"C;X1;Y%d;K\"",row);
      fprintf(f,"%s",(const char *)DvtStationCallString(q->value(1).toString(),
						      q->value(2).toString()));
      fprintf(f,"\"\n");
    
      // Phone
      fprintf(f,"C;X2;Y%d;K\"",row);
      fprintf(f,"%s",
	      (const char *)DvtFormatPhoneNumber(q->value(3).toString()));
      fprintf(f,"\"\n");

      row++;
    }
    delete q1;
  }
  delete q;
  fprintf(f,"E\n");
  fclose(f);
  ForkViewer(outfile);
}
