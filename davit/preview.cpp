// preview.cpp
//
// Preview a print job
//
//   (C) Copyright 2011 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: preview.cpp,v 1.2 2011/03/29 15:38:51 pcvs Exp $
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

#include <qpaintdevicemetrics.h>
#include <qsqldatabase.h>
#include <qpainter.h>
#include <qstringlist.h>

#include <dvtconf.h>
#include <dvt.h>

#include "preview.h"

Preview::Preview(QWidget *parent,const char *name)
  : QDialog(parent,name,false)
{
  setCaption(tr("Davit - Affidavit Preview"));

  //
  // Fonts
  //
  QFont font("helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont name_font("helvetica",12,QFont::Normal);
  name_font.setPixelSize(12);

  //
  // Printer Device
  //
  preview_printer=new QPrinter(QPrinter::ScreenResolution);
  preview_printer->setOrientation(QPrinter::Portrait);
  preview_printer->setPageSize(QPrinter::Letter);
  preview_printer->setFullPage(true);
  preview_printer->setNumCopies(1);
  preview_printer->setMinMax(1,1);
  preview_printer->setOptionEnabled(QPrinter::PrintToFile,true);
  preview_printer->setOptionEnabled(QPrinter::PrintSelection,false);
  preview_printer->setOptionEnabled(QPrinter::PrintPageRange,true);
  preview_pdm=new QPaintDeviceMetrics(preview_printer);

  //
  // Display Canvas
  //
  preview_label=new QLabel(this);
  preview_label->setGeometry(0,0,sizeHint().width(),sizeHint().height()-40);
  preview_label->setFrameStyle(QFrame::Box|QFrame::Plain);
  preview_label->setScaledContents(true);

  //
  // Print Button
  //
  preview_print_button=new QPushButton(tr("Print"),this);
  preview_print_button->
    setGeometry(sizeHint().width()-140,sizeHint().height()-35,60,30);
  preview_print_button->setFont(font);
  preview_print_button->setDisabled(true);
  connect(preview_print_button,SIGNAL(clicked()),this,SLOT(printData()));

  //
  // Setup Button
  //
  QPushButton *button=new QPushButton(tr("Setup"),this);
  button->setGeometry(10,sizeHint().height()-35,60,30);
  button->setFont(font);
  connect(button,SIGNAL(clicked()),this,SLOT(setupData()));
  QLabel *label=new QLabel("Print To:",this);
  label->setGeometry(80,sizeHint().height()-35,50,30);
  label->setFont(font);
  label->setAlignment(AlignVCenter|AlignRight);
  preview_printer_name_label=new QLabel("[none]",this);
  preview_printer_name_label->setGeometry(135,sizeHint().height()-35,160,30);
  preview_printer_name_label->setFont(name_font);
  if(!preview_printer->printerName().isEmpty()) {
    preview_printer_name_label->setText(preview_printer->printerName());
    preview_print_button->setEnabled(true);
  }

  //
  // Page Buttons
  //
  preview_prev_button=new DvtTransportButton(DvtTransportButton::Up,this);
  preview_prev_button->
    setGeometry(sizeHint().width()-230,sizeHint().height()-35,30,30);
  connect(preview_prev_button,SIGNAL(clicked()),this,SLOT(prevData()));
  preview_prev_button->setDisabled(true);

  preview_next_button=new DvtTransportButton(DvtTransportButton::Down,this);
  preview_next_button->
    setGeometry(sizeHint().width()-190,sizeHint().height()-35,30,30);
  connect(preview_next_button,SIGNAL(clicked()),this,SLOT(nextData()));
  preview_next_button->setDisabled(true);


  //
  // Cancel Button
  //
  button=new QPushButton(tr("Cancel"),this);
  button->setGeometry(sizeHint().width()-70,sizeHint().height()-35,60,30);
  button->setFont(font);
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


Preview::~Preview()
{
  delete preview_pdm;
}


QSize Preview::sizeHint() const
{
  return QSize(540,700);
}


QSizePolicy Preview::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int Preview::exec(int affiliate_id,const QDate &date,bool show_airings)
{
  return exec(affiliate_id,-1,date,show_airings);
}


int Preview::exec(int affiliate_id,int pgm_id,const QDate &date,
		  bool show_airings)
{
  preview_affiliate_id=affiliate_id;
  preview_program_id=pgm_id;
  preview_date=date;
  preview_show_airings=show_airings;

  Render(false);

  return QDialog::exec();
}


void Preview::setupData()
{
  preview_printer->setup(this);
  if(!preview_printer->printerName().isEmpty()) {
    preview_printer_name_label->setText(preview_printer->printerName());
    preview_print_button->setEnabled(true);
  }
  delete preview_pdm;
  preview_pdm=new QPaintDeviceMetrics(preview_printer);
  Render(false);
}


void Preview::prevData()
{
  preview_label->setPixmap(*preview_maps[--preview_current_map]);
  preview_prev_button->setEnabled(preview_current_map>0);
  preview_next_button->setEnabled(true);
}


void Preview::nextData()
{
  preview_label->setPixmap(*preview_maps[++preview_current_map]);
  preview_prev_button->setEnabled(true);
  preview_next_button->setEnabled(preview_current_map<(preview_maps.size()-1));
}


void Preview::printData()
{
  Render(true);
  done(0);
}


void Preview::cancelData()
{
  done(-1);
}


void Preview::Render(bool print)
{
  QString msg;
  QString station;
  QString signature;
  QPainter *p=NULL;

  //
  // Create Preview Pixmap
  //
  for(unsigned i=0;i<preview_maps.size();i++) {
    delete preview_maps[i];
  }
  preview_maps.clear();
  preview_maps.
    push_back(new QPixmap(preview_pdm->width(),preview_pdm->height()));
  preview_maps.back()->fill();

  //
  // Configure Draw Environment
  //
  QString sql;
  QSqlQuery *q;
  int ypos=preview_pdm->logicalDpiY();
  QFont title_font("helvetica",10,QFont::Bold);
  QFontMetrics title_metrics(title_font);
  QFont line_font("helvetica",10,QFont::Normal);
  QFontMetrics line_metrics(title_font);
  if(print) {
    p=new QPainter(preview_printer);
  }
  else {
    p=new QPainter(preview_maps.back());
  }
  p->setFont(title_font);
  p->setPen(Qt::black);
  p->setBrush(Qt::black);

  //
  // Generate Header
  //
  sql=QString().sprintf("select STATION_CALL,STATION_TYPE,ADDRESS1,ADDRESS2,\
                         CITY,STATE,ZIPCODE,DMA_NAME from AFFILIATES \
                         where ID=%d",
			preview_affiliate_id);
  q=new QSqlQuery(sql);
  if(!q->first()) {
    p->drawText(QRect(0,ypos,preview_pdm->width(),title_metrics.height()),
		AlignTop|AlignHCenter,
		"No Affiliate Data Found!");
    delete q;
    return;
  }
  else {
    station=DvtStationCallString(q->value(0).toString(),
				 q->value(1).toString());
    p->drawText(QRect(0,ypos,preview_pdm->width(),title_metrics.height()),
		AlignTop|AlignHCenter,
		"RadioAmerica Program Affidavit");
    p=NewLine(title_metrics.lineSpacing(),p,&ypos,print);
    p->drawText(QRect(0,ypos,preview_pdm->width(),title_metrics.height()),
		AlignTop|AlignHCenter,"Affiliate "+station);
    p=NewLine(title_metrics.lineSpacing(),p,&ypos,print);
    if(!q->value(2).toString().isEmpty()) {
      p->drawText(QRect(0,ypos,preview_pdm->width(),title_metrics.height()),
		  AlignTop|AlignHCenter,q->value(2).toString());
      p=NewLine(title_metrics.lineSpacing(),p,&ypos,print);
    }
    if(!q->value(3).toString().isEmpty()) {
      p->drawText(QRect(0,ypos,preview_pdm->width(),title_metrics.height()),
		  AlignTop|AlignHCenter,q->value(3).toString());
      p=NewLine(title_metrics.lineSpacing(),p,&ypos,print);
    }
    p->drawText(QRect(0,ypos,preview_pdm->width(),title_metrics.height()),
		AlignTop|AlignHCenter,q->value(4).toString()+", "+
		q->value(5).toString().upper()+" "+q->value(6).toString());
    p=NewLine(title_metrics.lineSpacing(),p,&ypos,print);
    p->drawText(QRect(0,ypos,preview_pdm->width(),title_metrics.height()),
		AlignTop|AlignHCenter,QString("DMA: ")+q->value(7).toString());
    p=NewLine(title_metrics.lineSpacing(),p,&ypos,print);
    p->drawText(QRect(0,ypos,preview_pdm->width(),title_metrics.height()),
		AlignTop|AlignHCenter,
		preview_date.toString("MMMM")+" 1 - "+
		preview_date.toString("MMMM")+" "+
		QString().sprintf("%d",preview_date.daysInMonth())+
		", "+preview_date.toString("yyyy"));
    p=NewLine(title_metrics.lineSpacing(),p,&ypos,print);
  }
  delete q;

  //
  // Generate Airings
  //
  p->setFont(line_font);
  sql=QString().sprintf("select AIRED.AIR_DATETIME,AIRED.AIR_LENGTH,\
                         AIRED.ORIGIN_DATETIME,AIRED.CONTACT_NAME,\
                         PROGRAMS.PROGRAM_NAME,AIRED.STATE \
                         from AIRED left join PROGRAMS \
                         on AIRED.PROGRAM_ID=PROGRAMS.ID \
                         where (AIRED.AFFILIATE_ID=%d)&&\
                         (AIRED.AIR_DATETIME>=\"%s-01 00:00:00\")&&\
                         (AIRED.AIR_DATETIME<\"%s-01 00:00:00\")&&\
                         ((AIRED.STATE=%d)||(AIRED.STATE=%d))",
			preview_affiliate_id,
			(const char *)preview_date.toString("yyyy-MM"),
			(const char *)preview_date.addMonths(1).
			toString("yyyy-MM"),
			Dvt::AiredStateConfirmed,
			Dvt::AiredStateModified);
  if(preview_program_id<0) {
    sql+="&&(PROGRAMS.ID>=0)";
  }
  else {
    sql+=QString().sprintf("&&(PROGRAMS.ID=%d)",preview_program_id);
  }
  sql+=" order by PROGRAMS.PROGRAM_NAME,AIRED.AIR_DATETIME";
  q=new QSqlQuery(sql);

  //
  // Certification
  //
  QString pgm_name="the Radio America Programs were";
  if(q->first()) {
    if(preview_program_id>=0) {
      pgm_name="\""+q->value(4).toString()+"\" was";
    }
    if(q->value(3).toString().isEmpty()) {
      msg=q->value(3).toString()+
	station+" certifies that "+pgm_name+"aired on "+station+" ";
    }
    else {
      msg="I, "+q->value(3).toString()+" certify that "+pgm_name+
	" aired on "+station+" ";
    }
    signature="Executed "+q->value(2).toDateTime().toString("h:mm:ss AP")+
      " on "+q->value(2).toDateTime().toString("dddd, MMMM d, yyyy");
    p=NewLine(title_metrics.lineSpacing(),p,&ypos,print);
    msg+="and all network commercials aired as per contract.";
    ypos=PrintText(ypos,&title_metrics,msg,p);
    p=NewLine(line_metrics.lineSpacing(),p,&ypos,print);
  }
  else {
    p->drawText(QRect(preview_pdm->logicalDpiX(),ypos,preview_pdm->width(),
		      title_metrics.height()),
		AlignTop|AlignHCenter,
		"No Affidavit Data Found!");
    delete q;
    return;
  }

  //
  // Listing Section
  //
  if(preview_show_airings) {
    if(preview_program_id<0) {
      //
      // Column Headers
      //
      int title_width=4*preview_pdm->width()/9;
      int date_width=
	line_metrics.width(preview_date.toString("MM/dd/yy"));
      int start_width=line_metrics.width("Start Time");
      int end_width=line_metrics.width("End Time");
      p->setFont(title_font);
      p->drawText(QRect(preview_pdm->logicalDpiX(),ypos,title_width,
			line_metrics.height()),AlignCenter,"Program");
      p->drawText(QRect(preview_pdm->logicalDpiX()+4*preview_pdm->width()/9,
			ypos,date_width,
			line_metrics.height()),AlignCenter,"Date");
      p->drawText(QRect(preview_pdm->logicalDpiX()+5*preview_pdm->width()/9,
			ypos,preview_pdm->width()/3,
			line_metrics.height()),
		  AlignLeft,"Start Time");
      p->drawText(QRect(preview_pdm->logicalDpiX()+6*preview_pdm->width()/9,
			ypos,preview_pdm->width()/3,line_metrics.height()),
		  AlignLeft,"End Time");
      p=NewLine(line_metrics.lineSpacing(),p,&ypos,print);

      //
      // Program Listings
      //
      QString star="";
      p->setFont(line_font);
      q->seek(-1);
      while(q->next()) {
	if(q->value(5).toInt()==Dvt::AiredStateModified) {
	  star=" *";
	}
	else {
	  star="";
	}
	p->drawText(QRect(preview_pdm->logicalDpiX(),
			  ypos,4*preview_pdm->width()/9,line_metrics.height()),
		    AlignLeft,q->value(4).toString()+star);
	p->drawText(QRect(preview_pdm->logicalDpiX()+4*preview_pdm->width()/9,
			  ypos,preview_pdm->width()/3,line_metrics.height()),
		    AlignLeft,q->value(0).toDateTime().
		    toString("MM/dd/yy"));
	p->drawText(QRect(preview_pdm->logicalDpiX()+5*preview_pdm->width()/9,
			  ypos,start_width,line_metrics.height()),
		    AlignCenter,q->value(0).toDateTime().toString("h:mm AP"));
	p->drawText(QRect(preview_pdm->logicalDpiX()+6*preview_pdm->width()/9,
			  ypos,end_width,line_metrics.height()),
		    AlignCenter,q->value(0).toDateTime().
		    addSecs(q->value(1).toInt()).toString("h:mm AP"));
	p=NewLine(line_metrics.lineSpacing(),p,&ypos,print);
      }
    }
    else {
      //
      // Column Headers
      //
      int date_width=
	line_metrics.width(preview_date.toString("dddd, MMMM 00, yyyy"));
      int start_width=line_metrics.width("Start Time");
      int end_width=line_metrics.width("End Time");
      p->setFont(title_font);
      p->drawText(QRect(preview_pdm->logicalDpiX(),ypos,date_width,
			line_metrics.height()),AlignCenter,"Date");
      p->drawText(QRect(preview_pdm->logicalDpiX()+preview_pdm->width()/2,
			ypos,preview_pdm->width()/3,
			line_metrics.height()),
		  AlignLeft,"Start Time");
      p->drawText(QRect(preview_pdm->logicalDpiX()+2*preview_pdm->width()/3,
			ypos,preview_pdm->width()/3,line_metrics.height()),
		  AlignLeft,"End Time");
      p=NewLine(line_metrics.lineSpacing(),p,&ypos,print);

      //
      // Program Listings
      //
      p->setFont(line_font);
      q->seek(-1);
      while(q->next()) {
	p->drawText(QRect(preview_pdm->logicalDpiX(),
			  ypos,preview_pdm->width()/3,line_metrics.height()),
		    AlignLeft,q->value(0).toDateTime().
		    toString("dddd, MMMM d, yyyy"));
	p->drawText(QRect(preview_pdm->logicalDpiX()+preview_pdm->width()/2,
			  ypos,start_width,line_metrics.height()),
		    AlignCenter,q->value(0).toDateTime().toString("h:mm AP"));
	p->drawText(QRect(preview_pdm->logicalDpiX()+2*preview_pdm->width()/3,
			  ypos,end_width,line_metrics.height()),
		    AlignCenter,q->value(0).toDateTime().
		    addSecs(q->value(1).toInt()).toString("h:mm AP"));
	ypos+=line_metrics.lineSpacing();
      }
      delete q;
      p=NewLine(line_metrics.lineSpacing(),p,&ypos,print);
    }
  }
  ypos+=line_metrics.lineSpacing();

  //
  // Exception Section
  //
  if(!preview_show_airings) {
    sql=QString().sprintf("select AIRED.AIR_DATETIME,AIRED.AIR_LENGTH,\
                           AIRED.ORIGIN_DATETIME,AIRED.CONTACT_NAME,\
                           PROGRAMS.PROGRAM_NAME \
                           from AIRED left join PROGRAMS \
                           on AIRED.PROGRAM_ID=PROGRAMS.ID \
                           where (AIRED.AFFILIATE_ID=%d)&&\
                           (AIRED.AIR_DATETIME>=\"%s-01 00:00:00\")&&\
                           (AIRED.AIR_DATETIME<\"%s-01 00:00:00\")&&\
                           (AIRED.STATE=%d)",
			  preview_affiliate_id,
			  (const char *)preview_date.toString("yyyy-MM"),
			  (const char *)preview_date.addMonths(1).
			  toString("yyyy-MM"),
			  Dvt::AiredStateDenied);
    if(preview_program_id<0) {
      sql+="&&(PROGRAMS.ID>=0)";
    }
    else {
      sql+=QString().sprintf("&&(PROGRAMS.ID=%d)",preview_program_id);
    }
    sql+=" order by PROGRAMS.PROGRAM_NAME,AIRED.AIR_DATETIME";
    q=new QSqlQuery(sql);
    if(q->first()) {
      p->setFont(title_font);
      p->drawText(QRect(0,ypos,preview_pdm->width(),title_metrics.height()),
		  AlignTop|AlignHCenter,
		  "Exceptions");
      p=NewLine(title_metrics.lineSpacing(),p,&ypos,print);
      p->setFont(line_font);
      p->drawText(QRect(0,ypos,preview_pdm->width(),title_metrics.height()),
		  AlignTop|AlignHCenter,
		  "The following program instances were NOT aired:");
      p=NewLine(title_metrics.lineSpacing(),p,&ypos,print);
      q->seek(-1);
      
      //
      // Column Headers
      //
      int title_width=4*preview_pdm->width()/9;
      int date_width=
	line_metrics.width(preview_date.toString("MM/dd/yy"));
      int start_width=line_metrics.width("Start Time");
      int end_width=line_metrics.width("End Time");
      p->setFont(title_font);
      p->drawText(QRect(preview_pdm->logicalDpiX(),ypos,title_width,
			line_metrics.height()),AlignCenter,"Program");
      p->drawText(QRect(preview_pdm->logicalDpiX()+4*preview_pdm->width()/9,
			ypos,date_width,
			line_metrics.height()),AlignCenter,"Date");
      p->drawText(QRect(preview_pdm->logicalDpiX()+5*preview_pdm->width()/9,
			ypos,preview_pdm->width()/3,
			line_metrics.height()),
		  AlignLeft,"Start Time");
      p->drawText(QRect(preview_pdm->logicalDpiX()+6*preview_pdm->width()/9,
			ypos,preview_pdm->width()/3,line_metrics.height()),
		  AlignLeft,"End Time");
      p=NewLine(line_metrics.lineSpacing(),p,&ypos,print);
      
      //
      // Program Listings
      //
      p->setFont(line_font);
      q->seek(-1);
      while(q->next()) {
	p->drawText(QRect(preview_pdm->logicalDpiX(),
			  ypos,4*preview_pdm->width()/9,line_metrics.height()),
		    AlignLeft,q->value(4).toString());
	p->drawText(QRect(preview_pdm->logicalDpiX()+4*preview_pdm->width()/9,
			  ypos,preview_pdm->width()/3,line_metrics.height()),
		    AlignLeft,q->value(0).toDateTime().
		    toString("MM/dd/yy"));
	p->drawText(QRect(preview_pdm->logicalDpiX()+5*preview_pdm->width()/9,
			  ypos,start_width,line_metrics.height()),
		    AlignCenter,q->value(0).toDateTime().toString("h:mm AP"));
	p->drawText(QRect(preview_pdm->logicalDpiX()+6*preview_pdm->width()/9,
			  ypos,end_width,line_metrics.height()),
		    AlignCenter,q->value(0).toDateTime().
		    addSecs(q->value(1).toInt()).toString("h:mm AP"));
	p=NewLine(line_metrics.lineSpacing(),p,&ypos,print);
      }
      p=NewLine(line_metrics.lineSpacing(),p,&ypos,print);
    }
    delete q;
  }

  //
  // Signature
  //
  p->drawText(QRect(preview_pdm->logicalDpiX(),ypos,preview_pdm->width(),
		    line_metrics.height()),AlignLeft,signature);

  //
  // Finish Up
  //
  delete p;
  preview_printer->setMinMax(1,preview_maps.size());
  if(!print) {
    preview_label->setPixmap(*preview_maps.front());
    preview_current_map=0;
    preview_prev_button->setDisabled(true);
    preview_next_button->setEnabled(preview_maps.size()>1);
  }
}


QPainter *Preview::NewLine(int spacing,QPainter *p,int *ypos,bool print)
{
  *ypos+=spacing;
  if(*ypos>preview_pdm->height()-preview_pdm->logicalDpiY()) {
    if(print) {
      preview_printer->newPage();
    }
    else {
      p->end();
      delete p;
      preview_maps.push_back(new QPixmap(preview_pdm->width(),
					 preview_pdm->height()));
      preview_maps.back()->fill();
      p=new QPainter(preview_maps.back());
    }
    *ypos=preview_pdm->logicalDpiY();
  }

  return p;
}


int Preview::PrintText(int ypos,QFontMetrics *fm,const QString &str,QPainter *p)
{
  QString line;
  QStringList strs=strs.split(" ",str);

  for(unsigned i=0;i<strs.size();i++) {
    if(fm->width(line+" "+strs[i])<
       preview_pdm->width()-2*preview_pdm->logicalDpiX()) {
      line+=" "+strs[i];
    }
    else {
      p->drawText(QRect(preview_pdm->logicalDpiX(),ypos,preview_pdm->width(),
			fm->height()),AlignLeft,line);
      line=strs[i];
      ypos+=fm->height();
    }
  }
  if(!line.isEmpty()) {
    p->drawText(QRect(preview_pdm->logicalDpiX(),ypos,preview_pdm->width(),
		      fm->height()),AlignLeft,line);
    ypos+=fm->height();
  }

  return ypos;
}
