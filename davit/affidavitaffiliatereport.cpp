// affadavitaffiliatereport.cpp
//
// Paint an Affiliate Affidavit report
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

void GenerateAffadavit::paintAffiliateReport(QPrinter *printer)
{
  QMarginsF margins(20.0,20.0,20.0,20.0);
  printer->setPageMargins(margins);

  QDate date=QDate(edit_year_box->currentText().toInt(),
		   edit_month_box->currentIndex()+1,1);
  int program_id=
    edit_program_box->itemData(edit_program_box->currentIndex()).toInt();
  QString msg;
  QString station;
  QString signature;

  printer->setFullPage(false);
  QPainter *p=new QPainter(printer);
  int w=p->device()->width();

  //
  // Configure Draw Environment
  //
  QString sql;
  DvtSqlQuery *q=NULL;
  int ypos=0;
  p->setPen(Qt::black);
  p->setBrush(Qt::black);

  //
  // Generate Header
  //
  sql=QString("select ")+
    "`STATION_CALL`,"+  // 00
    "`STATION_TYPE`,"+  // 01
    "`ADDRESS1`,"+      // 02
    "`ADDRESS2`,"+      // 03
    "`CITY`,"+          // 04
    "`STATE`,"+         // 05
    "`ZIPCODE`,"+       // 06
    "`DMA_NAME` "+      // 07
    "from `AFFILIATES` where "+
    QString::asprintf("`ID`=%d",edit_affiliate_id);
  q=new DvtSqlQuery(sql);
  if(!q->first()) {
    p->setFont(hugeButtonFont());
    p->drawText(QRect(0,0,w,hugeButtonFontMetrics()->height()),
		Qt::AlignCenter|Qt::AlignTop,
    		tr("No Affiliate Data Found!"));
  }
  else {
    p->setFont(bigLabelFont());
    station=DvtStationCallString(q->value(0).toString(),
				 q->value(1).toString());
    ypos=100;
    p->drawText(QRect(0,ypos,w,bigLabelFontMetrics()->height()),
		Qt::AlignTop|Qt::AlignHCenter,
		tr("RadioAmerica Program Affidavit"));
    NewLine(printer,p,&ypos);
    p->drawText(QRect(0,ypos,w,p->fontMetrics().height()),
		Qt::AlignTop|Qt::AlignHCenter,"Affiliate "+station);
    NewLine(printer,p,&ypos);
    if(!q->value(2).toString().isEmpty()) {
      p->drawText(QRect(0,ypos,w,p->fontMetrics().height()),
		  Qt::AlignTop|Qt::AlignHCenter,q->value(2).toString());
      NewLine(printer,p,&ypos);
    }
    if(!q->value(3).toString().isEmpty()) {
      p->drawText(QRect(0,ypos,w,p->fontMetrics().height()),
		  Qt::AlignTop|Qt::AlignHCenter,q->value(3).toString());
      NewLine(printer,p,&ypos);
    }
    p->drawText(QRect(0,ypos,w,p->fontMetrics().height()),
		Qt::AlignTop|Qt::AlignHCenter,q->value(4).toString()+", "+
		q->value(5).toString().toUpper()+" "+q->value(6).toString());
    NewLine(printer,p,&ypos);
    p->drawText(QRect(0,ypos,w,p->fontMetrics().height()),
		Qt::AlignTop|Qt::AlignHCenter,QString("DMA: ")+q->value(7).toString());
    NewLine(printer,p,&ypos);
    p->drawText(QRect(0,ypos,w,p->fontMetrics().height()),
		Qt::AlignTop|Qt::AlignHCenter,
		date.toString("MMMM")+" 1 - "+
		date.toString("MMMM")+" "+
		QString::asprintf("%d",date.daysInMonth())+
		", "+date.toString("yyyy"));
    NewLine(printer,p,&ypos);
  }

  //
  // Generate Airings
  //
  p->setFont(labelFont());
  sql=QString("select ")+
    "`AIRED`.`AIR_DATETIME`,"+     // 00
    "`AIRED`.`AIR_LENGTH`,"+       // 01
    "`AIRED`.`ORIGIN_DATETIME`,"+  // 02
    "`AIRED`.`CONTACT_NAME`,"+     // 03
    "`PROGRAMS`.`PROGRAM_NAME`,"+  // 04
    "`AIRED`.`STATE` "+
    "from `AIRED` left join `PROGRAMS` "+
    "on `AIRED`.`PROGRAM_ID`=`PROGRAMS`.`ID` where "+
    QString::asprintf("(`AIRED`.`AFFILIATE_ID`=%d)&&",edit_affiliate_id)+
    "(`AIRED`.`AIR_DATETIME`>="+
    DvtSqlQuery::escape(date.toString("yyyy-MM")+"-01 00:00:00")+")&&"+
    "(`AIRED`.`AIR_DATETIME`<"+
    DvtSqlQuery::escape(date.addMonths(1).toString("yyyy-MM")+"-01 00:00:00")+
    ")&&"+
    QString::asprintf("((`AIRED`.`STATE`=%d)||",Dvt::AiredStateConfirmed)+
    QString::asprintf("(`AIRED`.`STATE`=%d))",Dvt::AiredStateModified);
  if(program_id<=0) {
    sql+="&&(`PROGRAMS`.`ID`>=0)";
  }
  else {
    sql+=QString::asprintf("&&(`PROGRAMS`.`ID`=%d)",program_id);
  }
  sql+=" order by `PROGRAMS`.`PROGRAM_NAME`,`AIRED`.`AIR_DATETIME`";
  q=new DvtSqlQuery(sql);

  //
  // Certification
  //
  QString pgm_name=tr("the Radio America Programs were");
  if(q->first()) {
    if(program_id>0) {
      pgm_name="\""+q->value(4).toString()+"\" was";
    }
    if(q->value(3).toString().isEmpty()) {
      msg=q->value(3).toString()+
	station+" "+tr("certifies that")+" "+pgm_name+tr("aired on")+" "+
	station+" ";
    }
    else {
      msg="I, "+q->value(3).toString()+" "+tr("certify that")+" "+pgm_name+
	" "+tr("aired on")+" "+station+" ";
    }
    signature=tr("Executed")+" "+
      q->value(2).toDateTime().toString("h:mm:ss AP")+
      " "+tr("on")+" "+q->value(2).toDateTime().toString("dddd, MMMM d, yyyy");
    NewLine(printer,p,&ypos);
    msg+=tr("and all network commercials aired as per contract.");
    p->setFont(defaultFont());
    ypos=PrintText(ypos,msg,p);
    NewLine(printer,p,&ypos);
  }
  else {
    p->drawText(QRect(0,ypos,w,p->fontMetrics().height()),
		Qt::AlignTop|Qt::AlignHCenter,
		tr("No Affidavit Data Found!"));
    delete q;
    delete p;
    return;
  }

  //
  // Listing Section
  //
  if(edit_airings_check->isChecked()) {
    if(program_id<=0) {
      //
      // Column Headers
      //
      int title_width=4*w/9;
      int date_width=
	p->fontMetrics().horizontalAdvance(date.toString("MM/dd/yy"));
      int start_width=p->fontMetrics().horizontalAdvance("Start Time");
      int end_width=p->fontMetrics().horizontalAdvance("End Time");
      p->setFont(labelFont());
      p->drawText(QRect(0,ypos,title_width,
			p->fontMetrics().height()),Qt::AlignCenter,"Program Name");
      p->drawText(QRect(0+4*w/9,
			ypos,date_width,
			p->fontMetrics().height()),Qt::AlignCenter,"Air Date");
      p->drawText(QRect(0+5*w/9,
			ypos,w/3,
			p->fontMetrics().height()),
		  Qt::AlignLeft,"Start Time");
      p->drawText(QRect(0+6*w/9,
			ypos,w/3,p->fontMetrics().height()),
		  Qt::AlignLeft,"End Time");
      NewLine(printer,p,&ypos);

      //
      // Program Listings
      //
      QString star="";
      p->setFont(defaultFont());
      q->seek(-1);
      while(q->next()) {
	if(q->value(5).toInt()==Dvt::AiredStateModified) {
	  star=" *";
	}
	else {
	  star="";
	}
	p->drawText(QRect(0,
			  ypos,4*w/9,p->fontMetrics().height()),
		    Qt::AlignLeft,q->value(4).toString()+star);
	p->drawText(QRect(0+4*w/9,
			  ypos,w/3,p->fontMetrics().height()),
		    Qt::AlignLeft,q->value(0).toDateTime().
		    toString("MM/dd/yy"));
	p->drawText(QRect(0+5*w/9,
			  ypos,start_width,p->fontMetrics().height()),
		    Qt::AlignRight,q->value(0).toDateTime().toString("h:mm AP"));
	p->drawText(QRect(0+6*w/9,
			  ypos,end_width,p->fontMetrics().height()),
		    Qt::AlignRight,q->value(0).toDateTime().
		    addSecs(q->value(1).toInt()).toString("h:mm AP"));
	NewLine(printer,p,&ypos);
      }
    }
    else {
      //
      // Column Headers
      //
      int date_width=
	p->fontMetrics().horizontalAdvance(date.toString("dddd, MMMM 00, yyyy"));
      int start_width=p->fontMetrics().horizontalAdvance("Start Time");
      int end_width=p->fontMetrics().horizontalAdvance("End Time");
      p->setFont(labelFont());
      p->drawText(QRect(0,ypos,date_width,
			p->fontMetrics().height()),Qt::AlignCenter,"Date");
      p->drawText(QRect(0+w/2,
			ypos,w/3,
			p->fontMetrics().height()),
		  Qt::AlignLeft,"Start Time");
      p->drawText(QRect(0+2*w/3,
			ypos,w/3,p->fontMetrics().height()),
		  Qt::AlignLeft,"End Time");
      NewLine(printer,p,&ypos);

      //
      // Program Listings
      //
      p->setFont(defaultFont());
      q->seek(-1);
      while(q->next()) {
	p->drawText(QRect(0,
			  ypos,w/3,p->fontMetrics().height()),
		    Qt::AlignLeft,q->value(0).toDateTime().
		    toString("dddd, MMMM d, yyyy"));
	p->drawText(QRect(0+w/2,
			  ypos,start_width,p->fontMetrics().height()),
		    Qt::AlignCenter,q->value(0).toDateTime().toString("h:mm AP"));
	p->drawText(QRect(0+2*w/3,
			  ypos,end_width,p->fontMetrics().height()),
		    Qt::AlignCenter,q->value(0).toDateTime().
		    addSecs(q->value(1).toInt()).toString("h:mm AP"));
	ypos+=p->fontMetrics().lineSpacing();
      }
      delete q;
      NewLine(printer,p,&ypos);
    }
  }
  ypos+=p->fontMetrics().lineSpacing();

  //
  // Exception Section
  //
  if(!edit_airings_check->isChecked()) {
    sql=QString("select ")+
      "`AIRED`.`AIR_DATETIME`,"+     // 00
      "`AIRED`.`AIR_LENGTH`,"+       // 01
      "`AIRED`.`ORIGIN_DATETIME`,"+  // 02
      "`AIRED`.`CONTACT_NAME`,"+     // 03
      "`PROGRAMS`.`PROGRAM_NAME` "+  // 04
      "from `AIRED` left join `PROGRAMS` "+
      "on `AIRED`.`PROGRAM_ID`=`PROGRAMS`.`ID` where "+
      QString::asprintf("(`AIRED`.`AFFILIATE_ID`=%d)&&",edit_affiliate_id)+
      "(`AIRED`.`AIR_DATETIME`>="+
      DvtSqlQuery::escape(date.toString("yyyy-MM")+
			  "-01 00:00:00")+")&&"+
      "(`AIRED`.`AIR_DATETIME`<"+
      DvtSqlQuery::escape(date.addMonths(1).toString("yyyy-MM")+
			  "-01 00:00:00")+")&&"+
      QString::asprintf("(`AIRED`.`STATE`=%d)",Dvt::AiredStateDenied);
    if(program_id<0) {
      sql+="&&(`PROGRAMS`.`ID`>=0)";
    }
    else {
      sql+=QString::asprintf("&&(PROGRAMS.ID=%d)",program_id);
    }
    sql+=" order by PROGRAMS.PROGRAM_NAME,AIRED.AIR_DATETIME";
    q=new DvtSqlQuery(sql);
    if(q->first()) {
      p->setFont(labelFont());
      p->drawText(QRect(0,ypos,w,p->fontMetrics().height()),
		  Qt::AlignTop|Qt::AlignHCenter,
		  "Exceptions");
      NewLine(printer,p,&ypos);
      p->setFont(defaultFont());
      p->drawText(QRect(0,ypos,w,p->fontMetrics().height()),
		  Qt::AlignTop|Qt::AlignHCenter,
		  "The following program instances were NOT aired:");
      NewLine(printer,p,&ypos);
      q->seek(-1);
      
      //
      // Column Headers
      //
      int title_width=4*w/9;
      int date_width=
	p->fontMetrics().horizontalAdvance(date.toString("MM/dd/yy"));
      int start_width=p->fontMetrics().horizontalAdvance("Start Time");
      int end_width=p->fontMetrics().horizontalAdvance("End Time");
      p->setFont(labelFont());
      p->drawText(QRect(0,ypos,title_width,
			p->fontMetrics().height()),Qt::AlignCenter,"Program");
      p->drawText(QRect(0+4*w/9,
			ypos,date_width,
			p->fontMetrics().height()),Qt::AlignCenter,"Date");
      p->drawText(QRect(0+5*w/9,
			ypos,w/3,
			p->fontMetrics().height()),
		  Qt::AlignLeft,"Start Time");
      p->drawText(QRect(0+6*w/9,
			ypos,w/3,p->fontMetrics().height()),
		  Qt::AlignLeft,"End Time");
      NewLine(printer,p,&ypos);
      
      //
      // Program Listings
      //
      p->setFont(defaultFont());
      q->seek(-1);
      while(q->next()) {
	p->drawText(QRect(0,
			  ypos,4*w/9,p->fontMetrics().height()),
		    Qt::AlignLeft,q->value(4).toString());
	p->drawText(QRect(0+4*w/9,
			  ypos,w/3,p->fontMetrics().height()),
		    Qt::AlignLeft,q->value(0).toDateTime().
		    toString("MM/dd/yy"));
	p->drawText(QRect(0+5*w/9,
			  ypos,start_width,p->fontMetrics().height()),
		    Qt::AlignCenter,q->value(0).toDateTime().toString("h:mm AP"));
	p->drawText(QRect(0+6*w/9,
			  ypos,end_width,p->fontMetrics().height()),
		    Qt::AlignCenter,q->value(0).toDateTime().
		    addSecs(q->value(1).toInt()).toString("h:mm AP"));
	NewLine(printer,p,&ypos);
      }
      NewLine(printer,p,&ypos);
    }
    delete q;
  }

  //
  // Signature
  //
  p->drawText(QRect(0,ypos,w,p->fontMetrics().height()),Qt::AlignLeft,signature);

  delete p;
}
