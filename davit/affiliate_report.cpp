// affiliate_report.cpp
//
// Generate an Affiliate Affidavit Report
//
//   (C) Copyright 2002-2008 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: affiliate_report.cpp,v 1.8 2013/02/26 19:06:17 pcvs Exp $
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


void GenerateAffadavit::AffiliateReport(int id,const QDate &start_date,
					const QDate &end_date,bool pgms)
{
  QString s;
  QString e;
  QString p;
  QString sql;
  QSqlQuery *q;
  QDate date;
  QDateTime signature_datetime;
  QString sig_name;

  sql=QString().sprintf("select STATION_CALL,STATION_TYPE,STATION_FREQUENCY,\
                         ADDRESS1,ADDRESS2,CITY,STATE,ZIPCODE,PHONE,\
                         MARKET_NAME from AFFILIATES where ID=%d",id);
  q=new QSqlQuery(sql);
  if(!q->first()) {
    QMessageBox::warning(this,tr("Database Error"),
			 tr("Unable to fetch affiliate record!"));
    delete q;
    return;
  }

  //
  // Header
  //
  s+=Center("RadioAmerica Affiliate Affidavit");
  s+="\n";
  s+="\n";
  if(q->value(1).toString()=="F") {
    s+=Center(QString().sprintf("%s  %5.1lf FM",
				(const char *)q->value(0).toString(),
				q->value(2).toDouble()));
  }
  else {
    s+=Center(QString().sprintf("%s  %4.0lf AM",
				(const char *)q->value(0).toString(),
				q->value(2).toDouble()));
  }
  s+="\n";
  s+=Center(q->value(3).toString());
  s+="\n";
  if(!q->value(4).toString().isEmpty()) {
    s+=Center(q->value(4).toString());
    s+="\n";
  }
  s+=Center(QString().sprintf("%s, %s %s",
			      (const char *)q->value(5).toString(),
			      (const char *)q->value(6).toString(),
			      (const char *)q->value(7).toString()));
  s+="\n";
  s+=Center(q->value(8).toString());
  s+="\n";
  s+="\n";
  if(!q->value(9).toString().isEmpty()) {
    s+=Center(QString().sprintf("Market: %s",
				(const char *)q->value(9).toString()));
    s+="\n";
  }
  s+=Center(QString().sprintf("%s through %s",
			      (const char *)start_date.toString("MMMM d, yyyy"),
			      (const char *)end_date.toString("MMMM d, yyyy")));
  s+="\n";
  s+="\n";

  /*
  //
  // Programs Section
  //
  p+=Center("PROGRAMS");
  p+="\n";
  p+="          Program            Len   Mon   Tue   Wed   Thu   Fri   Sat   Sun";
  p+="\n";
  p+="          -----------------------------------------------------------------";
  p+="\n";
  sql=QString().sprintf("select AIRINGS.AIR_MON,AIRINGS.AIR_TUE,\
                           AIRINGS.AIR_WED,AIRINGS.AIR_THU,AIRINGS.AIR_FRI, \
                           AIRINGS.AIR_SAT,AIRINGS.AIR_SUN,		\
                           AIRINGS.AIR_TIME,AIRINGS.AIR_LENGTH,		\
                           PROGRAMS.PROGRAM_NAME,PROGRAMS.ID,		\
                           PROGRAMS.AIR_LENGTH from			\
                           AIRINGS right join PROGRAMS			\
                           on AIRINGS.PROGRAM_ID=PROGRAMS.ID		\
                           where AIRINGS.AFFILIATE_ID=%d		\
                           order by PROGRAMS.PROGRAM_NAME,AIRINGS.AIR_TIME",
			id);
  q1=new QSqlQuery(sql);
  while(q1->next()) {
    p+=QString().sprintf("          %-17s %s ",
			 (const char *)q1->value(9).toString().left(17),
			 (const char *)QTime().
			 addMSecs(q1->value(11).toInt()).
			 toString("mm:ss"));
    for(int i=0;i<7;i++) {
      if(q1->value(i).toString()=="Y") {
	p+=QString().sprintf("%5s ",(const char *)q1->value(7).toTime().
			     toString("h ap"));
	date=start_date;
	while(date<=end_date) {
	  if(date.dayOfWeek()==(i+1)) {
	    sql=QString().sprintf("select ORIGIN_DATETIME,CONTACT_NAME,\
                                   CONTACT_EMAIL from AIRED where \
                                   (AFFILIATE_ID=%d)&&		\
                                   (PROGRAM_ID=%d)&&			\
                                   (AIR_DATETIME>=\"%s 00:00:00\")&&\
                                   (AIR_DATETIME<\"%s 00:00:00\")",
				  id,
				  q1->value(10).toInt(),
				  (const char *)date.toString("yyyy-MM-dd"),
				  (const char *)date.addDays(1).
				  toString("yyyy-MM-dd"));
	    q2=new QSqlQuery(sql);
	    if(q2->first()) {
	      signature_datetime=q2->value(0).toDateTime();
	      sig_name=DvtFormatEmailAddress(q2->value(1).toString(),
					     q2->value(2).toString());
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
  if(pgms) {
    s+=p;
  }
  s+="\n";
  s+="\n";
  */

  /*
  //
  // Certification Section
  //
  s+=Center("STATION CERTIFICATION");
  s+="\n";
  if(signature_datetime.isValid()) {
    if(sig_name.isEmpty()) {
      //    if(signame.isEmpty()&&sigtitle.isEmpty()) {
      s+=QString().sprintf("          %s-%sM certifies that the\n",
			   (const char *)q->value(0).toString(),
			   (const char *)q->value(1).toString());
    }
    else {
      s+=QString().sprintf("          I, %s certify that the\n",
			   (const char *)sig_name);
    }
    s+=QString().sprintf("          Radio America programs were aired on %s at the times indicated.\n",
			 (const char *)q->value(0).toString());
    s+="          and all network commercials run within the RadioAmerica programs\n";
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
*/

  DvtTextFile(s);
}
