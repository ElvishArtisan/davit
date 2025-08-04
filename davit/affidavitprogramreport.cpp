// affidavitprogramreport.cpp
//
// Paint an Program Affidavit report
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <QFile>
#include <QFontMetrics>
#include <QMessageBox>
#include <QPainter>
#include <QPrintPreviewDialog>

#include <dvt.h>
#include <dvtconf.h>
#include <dvtdb.h>

#include "datedialog.h"
#include "generate_affadavit.h"
#include "textfile.h"

void GenerateAffadavit::paintProgramReport(QPrinter *printer)
{
  QMarginsF margins(20.0,20.0,20.0,20.0);
  printer->setPageMargins(margins);

  int program_id=
    edit_program_box->itemData(edit_program_box->currentIndex()).toInt();

  printer->setFullPage(false);
  QPainter *painter=new QPainter(printer);
  int w=painter->device()->width();
  painter->setPen(Qt::black);
  painter->setBrush(Qt::black);
  int ypos=0;

  QString s;
  QString e;
  QString p;
  QString sql;
  DvtSqlQuery *q;
  DvtSqlQuery *q1;
  QDate date;
  QDateTime signature_datetime;
  QString signame;
  QString sigtitle;
  sql=QString("select ")+
    "`PROVIDERS`.`BUSINESS_NAME`,"+  // 00
    "`PROGRAMS`.`PROGRAM_NAME`,"+    // 01
    "`PROVIDERS`.`ADDRESS1`,"+       // 02
    "`PROVIDERS`.`ADDRESS2`,"+       // 03
    "`PROVIDERS`.`CITY`,"+           // 04
    "`PROVIDERS`.`STATE`,"+          // 05
    "`PROVIDERS`.`ZIPCODE`,"+        // 06
    "`PROVIDERS`.`CONTACT_PHONE`,"+  // 07
    "`PROVIDERS`.`CONTACT_NAME`,"+   // 08
    "`PROGRAMS`.`AIR_LENGTH` "+      // 09
    "from `PROVIDERS` left join `PROGRAMS` "+
    "on `PROVIDERS`.`ID`=`PROGRAMS`.`PROVIDER_ID` where "+
    QString::asprintf("`PROGRAMS`.`ID`=%d",program_id);
  q=new DvtSqlQuery(sql);
  if(!q->first()) {
    QMessageBox::warning(this,"Davit - "+tr("Database Error"),
			 tr("Unable to fetch program record!"));
    delete q;
    return;
  }
  //
  // Header
  //
  painter->setFont(bigLabelFont());
  painter->drawText(QRect(0,ypos,w,bigLabelFontMetrics()->height()),
		    Qt::AlignTop|Qt::AlignHCenter,
		    tr("RadioAmerica Program Affidavit"));
  NewLine(printer,painter,&ypos);
  NewLine(printer,painter,&ypos);
  painter->drawText(QRect(0,ypos,w,bigLabelFontMetrics()->height()),
		    Qt::AlignTop|Qt::AlignHCenter,
		    q->value(0).toString());
  NewLine(printer,painter,&ypos);
  painter->drawText(QRect(0,ypos,w,bigLabelFontMetrics()->height()),
		    Qt::AlignTop|Qt::AlignHCenter,
		    q->value(1).toString());
  NewLine(printer,painter,&ypos);
  painter->drawText(QRect(0,ypos,w,bigLabelFontMetrics()->height()),
		    Qt::AlignTop|Qt::AlignHCenter,
		    q->value(2).toString());
  NewLine(printer,painter,&ypos);
  if(!q->value(3).toString().isEmpty()) {
    painter->drawText(QRect(0,ypos,w,bigLabelFontMetrics()->height()),
		      Qt::AlignTop|Qt::AlignHCenter,
		      q->value(3).toString());
    NewLine(printer,painter,&ypos);
  }
  painter->drawText(QRect(0,ypos,w,bigLabelFontMetrics()->height()),
		    Qt::AlignTop|Qt::AlignHCenter,
		    q->value(4).toString()+", "+
		    q->value(5).toString()+" "+
		    q->value(6).toString());
  NewLine(printer,painter,&ypos);
  painter->drawText(QRect(0,ypos,w,bigLabelFontMetrics()->height()),
		    Qt::AlignTop|Qt::AlignHCenter,
		    q->value(7).toString());
  NewLine(printer,painter,&ypos);
  NewLine(printer,painter,&ypos);
  
  //
  // Affiliates Section
  //
  if(edit_airings_check->isChecked()) {
    painter->drawText(QRect(0,ypos,w,bigLabelFontMetrics()->height()),
		      Qt::AlignTop|Qt::AlignHCenter,
		      tr("AFFILIATES"));
    NewLine(printer,painter,&ypos);
    painter->drawText(QRect(0,ypos,100,bigLabelFontMetrics()->height()),
		      Qt::AlignTop|Qt::AlignLeft,
		      tr("Affiliate"));
  //  painter->drawText(QRect(0,ypos,w,bigLabelFontMetrics()->height()),
  //		    Qt::AlignTop|Qt::AlignRight,
  //		    tr("Len   Mon   Tue   Wed   Thu   Fri   Sat   Sun"));
    painter->drawText(QRect(150,ypos,100,bigLabelFontMetrics()->height()),
		      Qt::AlignTop|Qt::AlignLeft,
		      tr("Time"));
    painter->drawText(QRect(230,ypos,100,bigLabelFontMetrics()->height()),
		      Qt::AlignTop|Qt::AlignLeft,
		      tr("Len"));
    painter->drawText(QRect(284,ypos,100,bigLabelFontMetrics()->height()),
		      Qt::AlignTop|Qt::AlignLeft,
		      tr("Mon"));
    painter->drawText(QRect(338,ypos,100,bigLabelFontMetrics()->height()),
		      Qt::AlignTop|Qt::AlignLeft,
		      tr("Tue"));
    painter->drawText(QRect(392,ypos,100,bigLabelFontMetrics()->height()),
		      Qt::AlignTop|Qt::AlignLeft,
		      tr("Wed"));
    painter->drawText(QRect(446,ypos,100,bigLabelFontMetrics()->height()),
		      Qt::AlignTop|Qt::AlignLeft,
		      tr("Thu"));
    painter->drawText(QRect(500,ypos,100,bigLabelFontMetrics()->height()),
		      Qt::AlignTop|Qt::AlignLeft,
		      tr("Fri"));
    painter->drawText(QRect(554,ypos,100,bigLabelFontMetrics()->height()),
		      Qt::AlignTop|Qt::AlignLeft,
		      tr("Sat"));
    painter->drawText(QRect(608,ypos,100,bigLabelFontMetrics()->height()),
		      Qt::AlignTop|Qt::AlignLeft,
		      tr("Sun"));
    NewLine(printer,painter,&ypos);
    painter->drawLine(0,ypos,w,ypos);
  //  painter->drawText(QRect(0,ypos,w,bigLabelFontMetrics()->height()),
  //		    Qt::AlignTop|Qt::AlignHCenter,
  //		    "-----------------------------------------------------------------");
  //  NewLine(printer,painter,&ypos);
    sql=QString("select ")+
      "`AIRINGS`.`AIR_MON`,"+          // 00
      "`AIRINGS`.`AIR_TUE`,"+          // 01
      "`AIRINGS`.`AIR_WED`,"+          // 02
      "`AIRINGS`.`AIR_THU`,"+          // 03
      "`AIRINGS`.`AIR_FRI`,"+          // 04
      "`AIRINGS`.`AIR_SAT`,"+          // 05
      "`AIRINGS`.`AIR_SUN`,"+          // 06
      "`AIRINGS`.`AIR_TIME`,"+         // 07

      "`AIRINGS`.`AIR_LENGTH`,"+       // 08
      "`AFFILIATES`.`STATION_CALL`,"+  // 09
      "`AFFILIATES`.`STATION_TYPE`,"+  // 10
      "`AFFILIATES`.`ID` "+            // 10+1
      "from `AIRINGS` right join `AFFILIATES` "+
      "on `AIRINGS`.`AFFILIATE_ID`=`AFFILIATES`.`ID` where "+
      QString::asprintf("`AIRINGS`.`PROGRAM_ID`=%d ",program_id)+
      "order by `AFFILIATES`.`STATION_CALL`,`AIRINGS`.`AIR_TIME`";
    printf("SQL1: %s\n",sql.toUtf8().constData());
    painter->setFont(printerFont());
    q1=new DvtSqlQuery(sql);
    while(q1->next()) {
      QString station=DvtStationCallString(q1->value(9).toString(),
					   q1->value(10).toString());
      painter->drawText(QRect(0,ypos,w,printerFontMetrics()->height()),
			Qt::AlignTop|Qt::AlignLeft,
			station);
      painter->drawText(QRect(150,ypos,100,printerFontMetrics()->height()),
			Qt::AlignTop|Qt::AlignLeft,
			q1->value(7).toTime().toString("hh:mm:ss"));
      painter->drawText(QRect(230,ypos,100,printerFontMetrics()->height()),
			Qt::AlignTop|Qt::AlignLeft,
			QTime(0,0,0).addSecs(q1->value(8).toInt()).
			toString("mm:ss"));
      if(q1->value(0).toString()=="Y") {
	painter->drawText(QRect(284,ypos,100,printerFontMetrics()->height()),
			  Qt::AlignTop|Qt::AlignLeft,
			  tr("Mo"));
      }
      if(q1->value(1).toString()=="Y") {
	painter->drawText(QRect(338,ypos,100,printerFontMetrics()->height()),
			  Qt::AlignTop|Qt::AlignLeft,
			  tr("Tu"));
      }
      if(q1->value(2).toString()=="Y") {
	painter->drawText(QRect(392,ypos,100,printerFontMetrics()->height()),
			  Qt::AlignTop|Qt::AlignLeft,
			  tr("We"));
      }
      if(q1->value(3).toString()=="Y") {
	painter->drawText(QRect(446,ypos,100,printerFontMetrics()->height()),
			  Qt::AlignTop|Qt::AlignLeft,
			  tr("Th"));
      }
      if(q1->value(4).toString()=="Y") {
	painter->drawText(QRect(500,ypos,100,printerFontMetrics()->height()),
			  Qt::AlignTop|Qt::AlignLeft,
			  tr("Fr"));
      }
      if(q1->value(5).toString()=="Y") {
	painter->drawText(QRect(554,ypos,100,printerFontMetrics()->height()),
			  Qt::AlignTop|Qt::AlignLeft,
			  tr("Sa"));
      }
      if(q1->value(6).toString()=="Y") {
	painter->drawText(QRect(608,ypos,100,printerFontMetrics()->height()),
			  Qt::AlignTop|Qt::AlignLeft,
			  tr("Su"));
      }

      NewLine(printer,painter,&ypos);
    }
    delete q1;
  }

  //
  // Certification Section
  //
  painter->setFont(bigLabelFont());
  painter->drawText(QRect(0,ypos,w,bigLabelFontMetrics()->height()),
		    Qt::AlignTop|Qt::AlignHCenter,
		    tr("PROVIDER CERTIFICATION"));
  NewLine(printer,painter,&ypos);



  /*
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
    s+=QString::asprintf("          %s certifies that the %s\n",
			 signame.toUtf8().constData(),
			 q->value(1).toString().toUtf8().constData());
    s+=QString::asprintf("          program was aired on RadioAmerica at the times indicated.\n");
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
    s+=QString::asprintf("                                    Executed %s\n",
			 signature_datetime.
			 toString("MM/dd/yyyy - h:mm:ss ap").
			 toUtf8().constData());
    s+="\n";
  }
  else {
    s+="\n";
    s+=Center("No affidavit data found.");
  }
  s+="\n";

  delete q;

  DvtTextFile(s);
  */
  delete painter;
}

/*
void GenerateAffadavit::ProgramReport(int id,const QDate &start_date,
				      const QDate &end_date,bool affiliates)
{
  QString s;
  QString e;
  QString p;
  QString sql;
  DvtSqlQuery *q;
  DvtSqlQuery *q1;
  DvtSqlQuery *q2;
  QDate date;
  QDateTime signature_datetime;
  QString signame;
  QString sigtitle;

  sql=QString::asprintf("select PROVIDERS.BUSINESS_NAME,PROGRAMS.PROGRAM_NAME,\
                         PROVIDERS.ADDRESS1,PROVIDERS.ADDRESS2,PROVIDERS.CITY,\
                         PROVIDERS.STATE,PROVIDERS.ZIPCODE,\
                         PROVIDERS.CONTACT_PHONE,PROVIDERS.CONTACT_NAME, \
                         PROGRAMS.AIR_LENGTH from PROVIDERS left join PROGRAMS \
                         on PROVIDERS.ID=PROGRAMS.PROVIDER_ID \
                         where PROGRAMS.ID=%d",id);
  q=new DvtSqlQuery(sql);
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
  s+=Center(QString::asprintf("%s, %s %s",
			      q->value(4).toString().toUtf8().constData(),
			      q->value(5).toString().toUtf8().constData(),
			      q->value(6).toString().toUtf8().constData()));
  s+="\n";
  s+=Center(q->value(7).toString());
  s+="\n";
  s+="\n";
  s+=Center(QString::asprintf("%s through %s",
	      start_date.toString("MMMM d, yyyy").toUtf8().constData(),
	      end_date.toString("MMMM d, yyyy").toUtf8().constData()));
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
  sql=QString::asprintf("select AIRINGS.AIR_MON,AIRINGS.AIR_TUE,\
                           AIRINGS.AIR_WED,AIRINGS.AIR_THU,AIRINGS.AIR_FRI, \
                           AIRINGS.AIR_SAT,AIRINGS.AIR_SUN,		\
                           AIRINGS.AIR_TIME,AIRINGS.AIR_LENGTH,		\
                           AFFILIATES.STATION_CALL,AFFILIATES.ID from	\
                           AIRINGS left join AFFILIATES			\
                           on AIRINGS.AFFILIATE_ID=AFFILIATES.ID	\
                           where AIRINGS.PROGRAM_ID=%d		\
                           order by AFFILIATES.STATION_CALL,AIRINGS.AIR_TIME",
			id);
  q1=new DvtSqlQuery(sql);
  while(q1->next()) {
    p+=QString::asprintf("          %-17s %s  ",
			 q1->value(9).toString().left(17).toUtf8().constData(),
			 QTime().addMSecs(q->value(9).toInt()).
			 toString("mm:ss").toUtf8().constData());
    for(int i=0;i<7;i++) {
      if(q1->value(i).toString()=="Y") {
	p+=QString::asprintf("%5s ",q1->value(7).toTime().toString("h ap").
			     toUtf8().constData());
	date=start_date;
	while(date<=end_date) {
	  if(date.dayOfWeek()==(i+1)) {
	    sql=QString::asprintf("select ORIGIN_DATETIME from AIRED where \
                                     (PROGRAM_ID=%d)&&\
                                     (AFFILIATE_ID=%d)&&\
                                     (AIR_DATETIME=\"%s %s\")",
				  id,
				  q1->value(10).toInt(),
				  date.toString("yyyy-MM-dd").toUtf8().constData(),
				  q1->value(7).toTime().
				  toString("hh:mm:ss").toUtf8().constData());
	    q2=new DvtSqlQuery(sql);
	    if(q2->first()) {
	      signature_datetime=q2->value(0).toDateTime();
	    }
	    else {
	      e+=QString::asprintf("          %-40s -- %5s %s",
				   q1->value(9).toString().left(40).
				   toUtf8().constData(),
				   date.toString("MM/dd/yyyy").
				   toUtf8().constData(),
				   q1->value(7).toTime().toString("hh:mm:ss").
				   toUtf8().constData());
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
    s+=QString::asprintf("          %s certifies that the %s\n",
			 signame.toUtf8().constData(),
			 q->value(1).toString().toUtf8().constData());
    s+=QString::asprintf("          program was aired on RadioAmerica at the times indicated.\n");
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
    s+=QString::asprintf("                                    Executed %s\n",
			 signature_datetime.
			 toString("MM/dd/yyyy - h:mm:ss ap").
			 toUtf8().constData());
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
*/
