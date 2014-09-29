// missingaffidavit_report.cpp
//
// Generate an Affiliate Missing Affidavit Contact List Report
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: missingaffidavit_report.cpp,v 1.8 2011/03/29 17:51:27 pcvs Exp $
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


void ListReports::MissingAffidavitReport()
{
  QString s;
  QString sql;
  QSqlQuery *q=NULL;
  QDate date;
  QString where;
  QString outfile;
  FILE *f=NULL;
  int row=4;
  std::vector<int> affiliate_ids;

  //
  // Get Date
  //
  date=QDate::currentDate();
  date.setYMD(date.year(),date.addMonths(-1).month(),1);
  PickFields *d=
    new PickFields(&date,NULL,NULL,false,NULL,false,NULL,false,NULL,
		   PickFields::NoMarket,this);
  if(d->exec()!=0) {
    delete d;
    return;
  }
  delete d;
  date.setYMD(date.year(),date.month(),1);


  //
  // Generate Report
  //
  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }
  fprintf(f,"ID;PSCALC3\n");
  fprintf(f,"C;X1;Y1;K\"Affiliates Missing Affidavits for %s\"\n",
	  (const char *)date.toString("MMMM, yyyy"));
  fprintf(f,"C;X2;Y1;K\"Report Date: %s\"\n",
	  (const char *)QDate::currentDate().toString("MMMM dd, yyyy"));
  fprintf(f,"C;X1;Y3;K\"CALL LETTERS\"\n");
  fprintf(f,"C;X2;Y3;K\"AFFIDAVIT CONTACT E-MAIL\"\n");

  
  affiliate_ids.push_back(0);
  sql=QString().sprintf("select AFFILIATE_ID from AIRED \
                         where (STATE=%d)&&\
                         (AIR_DATETIME>=\"%s-01 00:00:00\")&&\
                         (AIR_DATETIME<\"%s-01 00:00:00\") \
                         order by AFFILIATE_ID",
			Dvt::AiredStateScheduled,
			(const char *)date.toString("yyyy-MM"),
			(const char *)date.addMonths(1).
			toString("yyyy-MM"));
  q=new QSqlQuery(sql);
  while(q->next()) {
    if(q->value(0).toInt()!=affiliate_ids.back()) {
      affiliate_ids.push_back(q->value(0).toInt());
    }
  }
  delete q;
  affiliate_ids.erase(affiliate_ids.begin());

  for(unsigned i=0;i<affiliate_ids.size();i++) {
    // Call Letters
    fprintf(f,"C;X1;Y%d;K\"",row);
    fprintf(f,"%s",(const char *)DvtStationCallString(affiliate_ids[i]));
    fprintf(f,"\"\n");
    
    // E-Mail Address
    fprintf(f,"C;X2;Y%d;K\"",row++);
    fprintf(f,"%s",(const char *)GetEmailContactList(affiliate_ids[i],
						     true,false));
    fprintf(f,"\"\n");
  }
  fprintf(f,"E\n");
  fclose(f);
  ForkViewer(outfile);
}
