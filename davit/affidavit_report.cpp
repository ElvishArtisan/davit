// affidavit_report.cpp
//
// Generate an Affiliate Affidavit Contact List Report
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: affidavit_report.cpp,v 1.7 2014/01/02 19:49:12 pcvs Exp $
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


void ListReports::AffidavitReport()
{
  QString s;
  QString sql;
  QSqlQuery *q;
  QString outfile;
  FILE *f=NULL;
  int row=4;
  std::vector<int> ids;
  QString str;

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
  fprintf(f,"C;X3;Y3;K\"AFFIDAVIT CONTACT NAME\"\n");
  fprintf(f,"C;X4;Y3;K\"AFFIDAVIT CONTACT PHONE\"\n");
  sql="select ID,STATION_CALL,STATION_TYPE from AFFILIATES \
       where (AFFIDAVIT_ACTIVE=\"Y\") order by STATION_CALL desc";
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
    fprintf(f,"C;X2;Y%d;K\"",row);
    fprintf(f,"%s",
	    (const char *)GetEmailContactList(q->value(0).toInt(),true,false));
    fprintf(f,"\"\n");


    // Name
    fprintf(f,"C;X3;Y%d;K\"",row);
    str=GetNameContactList(q->value(0).toInt(),true,false);
    if(!str.isEmpty()) {
      fprintf(f,"%s",(const char *)str);
    }
    fprintf(f,"\"\n");


    // Phone Number
    fprintf(f,"C;X4;Y%d;K\"",row++);
    str=GetPhoneNumberContactList(q->value(0).toInt(),true,false);
    if(!str.isEmpty()) {
      fprintf(f,"%s",(const char *)str);
    }
    fprintf(f,"\"\n");
  }
  delete q;
  fprintf(f,"E\n");
  fclose(f);
  ForkViewer(outfile);
}
