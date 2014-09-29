// program_report.cpp
//
// Generate a Program Affidavit Report
//
//   (C) Copyright 2002-2008 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: program_report.cpp,v 1.2 2011/03/29 18:43:49 pcvs Exp $
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

#include <qfile.h>
#include <qsqldatabase.h>
#include <qmessagebox.h>

#include <dvttextfile.h>
#include <dvtconf.h>

#include <generate_affadavit.h>


void GenerateAffadavit::ProgramReport(int id,const QDate &start_date,
				      const QDate &end_date,bool affiliates)
{
  QString s;
  QString e;
  QString p;
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
  QSqlQuery *q2;
  QDate date;
  QDateTime signature_datetime;
  QString signame;
  QString sigtitle;

  sql=QString().sprintf("select PROVIDERS.BUSINESS_NAME,PROGRAMS.PROGRAM_NAME,\
                         PROVIDERS.ADDRESS1,PROVIDERS.ADDRESS2,PROVIDERS.CITY,\
                         PROVIDERS.STATE,PROVIDERS.ZIPCODE,\
                         PROVIDERS.CONTACT_PHONE,PROVIDERS.CONTACT_NAME, \
                         PROGRAMS.AIR_LENGTH from PROVIDERS left join PROGRAMS \
                         on PROVIDERS.ID=PROGRAMS.PROVIDER_ID \
                         where PROGRAMS.ID=%d",id);
  q=new QSqlQuery(sql);
  if(!q->first()) {
    QMessageBox::warning(this,tr("Database Error"),
			 tr("Unable to fetch program record!"));
    delete q;
    return;
  }

  //
  // Header
  //
  s+=Center("RadioAmerica Program Affidavit");
  s+="\n";
  s+="\n";
  s+=Center(q->value(0).toString());
  s+="\n";
  s+=Center(q->value(1).toString());
  s+="\n";
  s+=Center(q->value(2).toString());
  s+="\n";
  if(!q->value(3).toString().isEmpty()) {
    s+=Center(q->value(3).toString());
    s+="\n";
  }
  s+=Center(QString().sprintf("%s, %s %s",
			      (const char *)q->value(4).toString(),
			      (const char *)q->value(5).toString(),
			      (const char *)q->value(6).toString()));
  s+="\n";
  s+=Center(q->value(7).toString());
  s+="\n";
  s+="\n";
  s+=Center(QString().sprintf("%s through %s",
			      (const char *)start_date.toString("MMMM d, yyyy"),
			      (const char *)end_date.toString("MMMM d, yyyy")));
  s+="\n";
  s+="\n";

  //
  // Affiliates Section
  //
  p+=Center("AFFILIATES");
  p+="\n";
  p+="          Affiliates         Len   Mon   Tue   Wed   Thu   Fri   Sat   Sun";
  p+="\n";
  p+="          -----------------------------------------------------------------";
  p+="\n";
  sql=QString().sprintf("select AIRINGS.AIR_MON,AIRINGS.AIR_TUE,\
                           AIRINGS.AIR_WED,AIRINGS.AIR_THU,AIRINGS.AIR_FRI, \
                           AIRINGS.AIR_SAT,AIRINGS.AIR_SUN,		\
                           AIRINGS.AIR_TIME,AIRINGS.AIR_LENGTH,		\
                           AFFILIATES.STATION_CALL,AFFILIATES.ID from	\
                           AIRINGS left join AFFILIATES			\
                           on AIRINGS.AFFILIATE_ID=AFFILIATES.ID	\
                           where AIRINGS.PROGRAM_ID=%d		\
                           order by AFFILIATES.STATION_CALL,AIRINGS.AIR_TIME",
			id);
  q1=new QSqlQuery(sql);
  while(q1->next()) {
    p+=QString().sprintf("          %-17s %s  ",
			 (const char *)q1->value(9).toString().left(17),
			 (const char *)QTime().
			 addMSecs(q->value(9).toInt()).
			 toString("mm:ss"));
    for(int i=0;i<7;i++) {
      if(q1->value(i).toString()=="Y") {
	p+=QString().sprintf("%5s ",(const char *)q1->value(7).toTime().
			     toString("h ap"));
	date=start_date;
	while(date<=end_date) {
	  if(date.dayOfWeek()==(i+1)) {
	    sql=QString().sprintf("select ORIGIN_DATETIME from AIRED where \
                                     (PROGRAM_ID=%d)&&\
                                     (AFFILIATE_ID=%d)&&\
                                     (AIR_DATETIME=\"%s %s\")",
				  id,
				  q1->value(10).toInt(),
				  (const char *)date.toString("yyyy-MM-dd"),
				  (const char *)q1->value(7).toTime().
				  toString("hh:mm:ss"));
	    q2=new QSqlQuery(sql);
	    if(q2->first()) {
	      signature_datetime=q2->value(0).toDateTime();
	    }
	    else {
	      e+=QString().sprintf("          %-40s -- %5s %s",
				   (const char *)q1->value(9).
				   toString().left(40),
				   (const char *)date.toString("MM/dd/yyyy"),
				   (const char *)q1->value(7).toTime().
				   toString("hh:mm:ss"));
	      e+="\n";
	    }
	    delete q2;
	  }
	  date=date.addDays(1);
	}
      }
      else {
	p+="      ";
      }
    }
    p+="\n";
  }
  delete q1;
  if(affiliates) {
    s+=p;
  }
  s+="\n";
  s+="\n";

  //
  // Certification Section
  //
  s+=Center("PROVIDER CERTIFICATION");
  s+="\n";
  if(signature_datetime.isValid()) {
    signame=q->value(8).toString();
    if(signame.isEmpty()) {
      signame=q->value(0).toString();
    }
    s+=QString().sprintf("          %s certifies that the %s\n",
			 (const char *)signame,
			 (const char *)q->value(1).toString());
    s+=QString().
      sprintf("          program was aired on RadioAmerica at the times indicated.\n");
    s+="          and all network commercials run within the programs\n";
    s+="          as per contract";
    if(e.isEmpty()) {
      s+=".";
    }
    else {
      s+=", except as shown in the EXCEPTIONS section below.";
    }
    s+="\n";
    s+="\n";
    
    //
    // Exception Section
    //
    if(!e.isEmpty()) {
      s+=Center("EXCEPTIONS");
      s+="\n";
      s+=e;
      s+="\n";
    }

    //
    // Signature Section
    //
    s+=QString().sprintf("                                    Executed %s\n",
			 (const char *)signature_datetime.
			 toString("MM/dd/yyyy - h:mm:ss ap"));
    s+="\n";
  }
  else {
    s+="\n";
    s+=Center("No affidavit data found.");
  }
  s+="\n";

  delete q;

  DvtTextFile(s);
}
