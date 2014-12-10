// missingaffidavit_report.cpp
//
// Generate an Affiliate Missing Affidavit List Report
//
//   (C) Copyright 2010-2014 Fred Gleason <fredg@paravelsystems.com>
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

#include <affidavit_picker.h>
#include <list_reports.h>


void ListReports::MissingAffidavitReport()
{
  QString s;
  QString sql;
  QSqlQuery *q=NULL;
  QString where;
  QString outfile;
  FILE *f=NULL;
  std::vector<int> affiliate_ids;
  Dvt::AffidavitStationFilter filter=Dvt::All;
  Dvt::AffidavitSortType sort_type=Dvt::ByCount;
  int program_id=-1;

  //
  // Get Parameters
  //
  QDate end_date=QDate::currentDate();
  end_date.setYMD(end_date.year(),end_date.month(),1);
  QDate start_date=end_date.addYears(-1);
  AffidavitPicker *d=new AffidavitPicker(&filter,&sort_type,&program_id,this);
  if(d->exec()!=0) {
    delete d;
    return;
  }
  delete d;

  //
  // Generate Report
  //

  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }
  fprintf(f,"ID;PSCALC3\n");
  fprintf(f,"C;X1;Y1;K\"Affiliates Missing Affidavits\n");
  fprintf(f,"C;X1;Y2;K\"Report Date: %s\"\n",
	  (const char *)QDate::currentDate().toString("MMMM dd, yyyy"));
  switch(filter) {
  case Dvt::All:
    fprintf(f,"C;X1;Y3;K\"All Programs\"\n");
    break;

  case Dvt::Weekday:
    fprintf(f,"C;X1;Y3;K\"All Weekday Programs\"\n");
    break;

  case Dvt::Weekend:
    fprintf(f,"C;X1;Y3;K\"All Weekend Programs\"\n");
    break;

  case Dvt::Program:
    sql=QString("select PROGRAM_NAME from PROGRAMS ")+
      QString().sprintf("where ID=%d",program_id);
    q=new QSqlQuery(sql);
    if(q->first()) {
      fprintf(f,"C;X1;Y3;K\"%s\"\n",(const char *)q->value(0).toString());
    }
    else {
      fprintf(f,"C;X1;Y3;K\"INTERNAL ERROR!\"\n");
    }
    delete q;
    break;
  }
  fprintf(f,"C;X1;Y5;K\"CALL LETTERS\"\n");
  fprintf(f,"C;X2;Y5;K\"NAME\"\n");
  fprintf(f,"C;X3;Y5;K\"PHONE\"\n");
  fprintf(f,"C;X4;Y5;K\"E-MAIL\"\n");
  fprintf(f,"C;X5;Y5;K\"MISSING\"\n");
  int row=6;

  DvtAffidavitNeeded(&affiliate_ids,start_date,end_date,filter,program_id);
  sql=QString("select AFFILIATES.ID,AFFILIATES.STATION_CALL,")+
    "AFFILIATES.STATION_TYPE,CONTACTS.NAME,CONTACTS.PHONE,CONTACTS.EMAIL "+
    "from AFFILIATES left join CONTACTS "+
    "on AFFILIATES.ID=CONTACTS.AFFILIATE_ID "+
    "where (CONTACTS.AFFIDAVIT=\"Y\")&&(";
  for(unsigned i=0;i<affiliate_ids.size();i++) {
    sql+=QString().sprintf("(AFFILIATES.ID=%d)||",affiliate_ids[i]);
  }
  sql=sql.left(sql.length()-2)+")";
  q=new QSqlQuery(sql);
  while(q->next()) {
    // Call Letters
    fprintf(f,"C;X1;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)DvtStationCallString(q->value(1).toString(),
						      q->value(2).toString()));
    fprintf(f,"\"\n");

    // Contact Name
    fprintf(f,"C;X2;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)q->value(3).toString());
    fprintf(f,"\"\n");

    // Contact Phone
    fprintf(f,"C;X3;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)DvtFormatPhoneNumber(q->value(4).toString()));
    fprintf(f,"\"\n");

    // Contact E-Mail Address
    fprintf(f,"C;X4;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)q->value(5).toString());
    fprintf(f,"\"\n");

    // Missing Months
    std::vector<QDate> dates;
    QString date_str="";
    if(DvtAffidavitNeededDates(&dates,q->value(0).toInt(),
			       start_date,end_date)) {
      for(unsigned i=0;i<dates.size();i++) {
	date_str+=dates[i].toString("M/yy")+", ";
      }
      date_str=date_str.left(date_str.length()-2);
    }
    fprintf(f,"C;X5;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)date_str);
    fprintf(f,"\"\n");

    row++;
  }
  delete q;
  fprintf(f,"E\n");
  fclose(f);
  ForkViewer(outfile);
}
