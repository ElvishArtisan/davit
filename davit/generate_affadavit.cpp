// generate_affadavit.cpp
//
// Select a Set of Dates for an Affidavit
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
#include <dvtdatedialog.h>
#include <dvtdb.h>
#include <dvttextfile.h>

#include "generate_affadavit.h"

GenerateAffadavit::GenerateAffadavit(DvtConfig *c,QWidget *parent)
  : DvtDialog(c,parent)
{
  setModal(true);

  QString sql;
  QSqlQuery *q;

  QDate this_month=QDate::currentDate();
  QDate last_month=this_month.addMonths(-1);
  edit_affiliate_id=-1;
  edit_type=GenerateAffadavit::ReportAffiliate;

  //
  // Fix the Window Size
  //
  setMaximumSize(sizeHint());
  setMinimumSize(sizeHint());

  //
  // Date
  //
  edit_month_box=new QComboBox(this);
  connect(edit_month_box,SIGNAL(activated(int)),
	  this,SLOT(updateGenerateButtonData(int)));
  edit_month_box->insertItem(0,tr("January"));
  edit_month_box->insertItem(1,tr("February"));
  edit_month_box->insertItem(2,tr("March"));
  edit_month_box->insertItem(3,tr("April"));
  edit_month_box->insertItem(4,tr("May"));
  edit_month_box->insertItem(5,tr("June"));
  edit_month_box->insertItem(6,tr("July"));
  edit_month_box->insertItem(7,tr("August"));
  edit_month_box->insertItem(8,tr("September"));
  edit_month_box->insertItem(9,tr("October"));
  edit_month_box->insertItem(10,tr("November"));
  edit_month_box->insertItem(11,tr("December"));
  edit_month_box->setCurrentIndex(last_month.month()-1);
  edit_year_box=new QComboBox(this);
  connect(edit_year_box,SIGNAL(activated(int)),
	  this,SLOT(updateGenerateButtonData(int)));
  int count=0;
  for(int i=DVT_ORIGIN_YEAR;i<(last_month.year()+1);i++) {
    edit_year_box->insertItem(count++,QString::asprintf("%d",i));
  }
  edit_year_box->setCurrentIndex(edit_year_box->count()-1);
  edit_date_label=new QLabel(tr("Date:"),this);
  edit_date_label->setFont(labelFont());
  edit_date_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  
  //
  // Program List
  //
  edit_program_box=new QComboBox(this);
  connect(edit_program_box,SIGNAL(activated(int)),
	  this,SLOT(updateGenerateButtonData(int)));
  edit_program_label=new QLabel(tr("Program")+":",this);
  edit_program_label->setFont(labelFont());
  edit_program_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_program_box->insertItem(0,"[All Scheduled Programs]",0);
  sql=QString("select ")+
    "`ID`,"+            // 00
    "`PROGRAM_NAME` "+  // 01
    "from `PROGRAMS` order by `PROGRAM_NAME`";
  q=new QSqlQuery(sql);
  count=1;
  while(q->next()) {
    edit_program_box->
      insertItem(count++,q->value(1).toString(),q->value(0).toInt());
  }
  delete q;

  //
  // Airings List
  //
  edit_airings_check=new QCheckBox(this);
  edit_airings_label=new QLabel(tr("Include Airings List"),this);
  edit_airings_label->setFont(labelFont());
  edit_airings_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  

  //
  //  Generate Button
  //
  edit_generate_button=new QPushButton(this);
  edit_generate_button->setFont(buttonFont());
  edit_generate_button->setText(tr("Generate")+"\n"+tr("Affidavit"));
  connect(edit_generate_button,SIGNAL(clicked()),this,SLOT(generateData()));

  //
  //  Close Button
  //
  edit_close_button=new QPushButton(this);
  edit_close_button->setDefault(true);
  edit_close_button->setFont(buttonFont());
  edit_close_button->setText(tr("Close"));
  connect(edit_close_button,SIGNAL(clicked()),this,SLOT(closeData()));
}


GenerateAffadavit::~GenerateAffadavit()
{
}


QSize GenerateAffadavit::sizeHint() const
{
  return QSize(400,142);
}


QSizePolicy GenerateAffadavit::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int GenerateAffadavit::execAffiliate(int affiliate_id)
{
  edit_affiliate_id=affiliate_id;
  edit_type=GenerateAffadavit::ReportAffiliate;

  setWindowTitle("Davit - "+tr("Generate Affiliate Affidavit")+
		 QString::asprintf(" [ID: %d]",affiliate_id));
  updateGenerateButtonData(-1);

  return QDialog::exec();
}


void GenerateAffadavit::updateGenerateButtonData(int index)
{
  //
  // Validate Date
  //
  QDate start_date=QDate(edit_year_box->
		    currentText().toInt(),edit_month_box->currentIndex()+1,1);
  QDate end_date=start_date.addMonths(1).addDays(-1);
  if(end_date>=QDate::currentDate()) {
    edit_generate_button->setDisabled(true);
    return;
  }
  
  //
  // Check for Affidavit Records
  //
  edit_generate_button->setEnabled(HasAffidavits());
}


void GenerateAffadavit::generateData()
{
  QPrintPreviewDialog *dialog=new QPrintPreviewDialog(this);
  connect(dialog,SIGNAL(paintRequested(QPrinter *)),
	  this,SLOT(printerPaint(QPrinter *)));
  dialog->exec();
  delete dialog;
}


bool GenerateAffadavit::HasAffidavits() const
{
  QString sql;
  DvtSqlQuery *q=NULL;
  QDate date=SelectedDate();

  int program_id=
    edit_program_box->itemData(edit_program_box->currentIndex()).toInt();
  sql=QString("select ")+
    "`PROGRAMS`.`ID` "+
    "from `AIRED` left join `PROGRAMS` "+
    "on `AIRED`.`PROGRAM_ID`=`PROGRAMS`.`ID` where ";
  if(program_id>0) {
    sql+=QString::asprintf("(`PROGRAMS`.`ID`=%d)&&",program_id);
  }
  sql+=QString::asprintf("(`AIRED`.`AFFILIATE_ID`=%d)&&",edit_affiliate_id)+
    "(`AIRED`.`AIR_DATETIME`>="+
    DvtSqlQuery::escape(date.toString("yyyy-MM")+"-01 00:00:00")+")&&"+
    "(`AIRED`.`AIR_DATETIME`<"+
    DvtSqlQuery::escape(date.addMonths(1).toString("yyyy-MM")+"-01 00:00:00")+
    ")&&"+
    QString::asprintf("((`AIRED`.`STATE`=%d)||",Dvt::AiredStateConfirmed)+
    QString::asprintf("(`AIRED`.`STATE`=%d)) ",Dvt::AiredStateDenied)+
    "order by `AIRED`.`AIR_DATETIME` ";
  q=new DvtSqlQuery(sql);
  bool ret=q->first();
  delete q;

  return ret;
}


QDate GenerateAffadavit::SelectedDate() const
{
  return QDate(edit_year_box->currentText().toInt(),
	       edit_month_box->currentIndex()+1,1);
}


QString GenerateAffadavit::Center(const QString &s)
{
  QString r;
  int margin=(AFFADAVIT_WIDTH-s.length())/2;

  for(int i=0;i<margin;i++) {
    r+=" ";
  }
  return r+s;
}


void GenerateAffadavit::closeData()
{
  done(true);
}


void GenerateAffadavit::printerPaint(QPrinter *printer)
{
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
  //  int ypos=preview_pdm->logicalDpiY();
  int ypos=0;
  QFont title_font("helvetica",10,QFont::Bold);
  QFontMetrics title_metrics(title_font);
  QFont line_font("helvetica",10,QFont::Normal);
  QFontMetrics line_metrics(title_font);
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
    NewLine(title_metrics.lineSpacing(),printer,p,&ypos);
    p->drawText(QRect(0,ypos,w,title_metrics.height()),
		Qt::AlignTop|Qt::AlignHCenter,"Affiliate "+station);
    NewLine(title_metrics.lineSpacing(),printer,p,&ypos);
    if(!q->value(2).toString().isEmpty()) {
      p->drawText(QRect(0,ypos,w,title_metrics.height()),
		  Qt::AlignTop|Qt::AlignHCenter,q->value(2).toString());
      NewLine(title_metrics.lineSpacing(),printer,p,&ypos);
    }
    if(!q->value(3).toString().isEmpty()) {
      p->drawText(QRect(0,ypos,w,title_metrics.height()),
		  Qt::AlignTop|Qt::AlignHCenter,q->value(3).toString());
      NewLine(title_metrics.lineSpacing(),printer,p,&ypos);
    }
    p->drawText(QRect(0,ypos,w,title_metrics.height()),
		Qt::AlignTop|Qt::AlignHCenter,q->value(4).toString()+", "+
		q->value(5).toString().toUpper()+" "+q->value(6).toString());
    NewLine(title_metrics.lineSpacing(),printer,p,&ypos);
    p->drawText(QRect(0,ypos,w,title_metrics.height()),
		Qt::AlignTop|Qt::AlignHCenter,QString("DMA: ")+q->value(7).toString());
    NewLine(title_metrics.lineSpacing(),printer,p,&ypos);
    p->drawText(QRect(0,ypos,w,title_metrics.height()),
		Qt::AlignTop|Qt::AlignHCenter,
		date.toString("MMMM")+" 1 - "+
		date.toString("MMMM")+" "+
		QString::asprintf("%d",date.daysInMonth())+
		", "+date.toString("yyyy"));
    NewLine(title_metrics.lineSpacing(),printer,p,&ypos);
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
  printf("Airings SQL: %s\n",sql.toUtf8().constData());
  q=new DvtSqlQuery(sql);

  //
  // Certification
  //
  QString pgm_name=tr("the Radio America Programs were");
  if(q->first()) {
    if(program_id>=0) {
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
    NewLine(title_metrics.lineSpacing(),printer,p,&ypos);
    msg+=tr("and all network commercials aired as per contract.");
    ypos=PrintText(ypos,&title_metrics,msg,p);
    NewLine(line_metrics.lineSpacing(),printer,p,&ypos);
  }
  else {
    p->drawText(QRect(0,ypos,w,title_metrics.height()),
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
	line_metrics.horizontalAdvance(date.toString("MM/dd/yy"));
      int start_width=line_metrics.horizontalAdvance("Start Time");
      int end_width=line_metrics.horizontalAdvance("End Time");
      p->setFont(title_font);
      p->drawText(QRect(0,ypos,title_width,
			line_metrics.height()),Qt::AlignCenter,"Program");
      p->drawText(QRect(0+4*w/9,
			ypos,date_width,
			line_metrics.height()),Qt::AlignCenter,"Date");
      p->drawText(QRect(0+5*w/9,
			ypos,w/3,
			line_metrics.height()),
		  Qt::AlignLeft,"Start Time");
      p->drawText(QRect(0+6*w/9,
			ypos,w/3,line_metrics.height()),
		  Qt::AlignLeft,"End Time");
      NewLine(line_metrics.lineSpacing(),printer,p,&ypos);

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
	p->drawText(QRect(0,
			  ypos,4*w/9,line_metrics.height()),
		    Qt::AlignLeft,q->value(4).toString()+star);
	p->drawText(QRect(0+4*w/9,
			  ypos,w/3,line_metrics.height()),
		    Qt::AlignLeft,q->value(0).toDateTime().
		    toString("MM/dd/yy"));
	p->drawText(QRect(0+5*w/9,
			  ypos,start_width,line_metrics.height()),
		    Qt::AlignCenter,q->value(0).toDateTime().toString("h:mm AP"));
	p->drawText(QRect(0+6*w/9,
			  ypos,end_width,line_metrics.height()),
		    Qt::AlignCenter,q->value(0).toDateTime().
		    addSecs(q->value(1).toInt()).toString("h:mm AP"));
	NewLine(line_metrics.lineSpacing(),printer,p,&ypos);
      }
    }
    else {
      //
      // Column Headers
      //
      int date_width=
	line_metrics.horizontalAdvance(date.toString("dddd, MMMM 00, yyyy"));
      int start_width=line_metrics.horizontalAdvance("Start Time");
      int end_width=line_metrics.horizontalAdvance("End Time");
      p->setFont(title_font);
      p->drawText(QRect(0,ypos,date_width,
			line_metrics.height()),Qt::AlignCenter,"Date");
      p->drawText(QRect(0+w/2,
			ypos,w/3,
			line_metrics.height()),
		  Qt::AlignLeft,"Start Time");
      p->drawText(QRect(0+2*w/3,
			ypos,w/3,line_metrics.height()),
		  Qt::AlignLeft,"End Time");
      NewLine(line_metrics.lineSpacing(),printer,p,&ypos);

      //
      // Program Listings
      //
      p->setFont(line_font);
      q->seek(-1);
      while(q->next()) {
	p->drawText(QRect(0,
			  ypos,w/3,line_metrics.height()),
		    Qt::AlignLeft,q->value(0).toDateTime().
		    toString("dddd, MMMM d, yyyy"));
	p->drawText(QRect(0+w/2,
			  ypos,start_width,line_metrics.height()),
		    Qt::AlignCenter,q->value(0).toDateTime().toString("h:mm AP"));
	p->drawText(QRect(0+2*w/3,
			  ypos,end_width,line_metrics.height()),
		    Qt::AlignCenter,q->value(0).toDateTime().
		    addSecs(q->value(1).toInt()).toString("h:mm AP"));
	ypos+=line_metrics.lineSpacing();
      }
      delete q;
      NewLine(line_metrics.lineSpacing(),printer,p,&ypos);
    }
  }
  ypos+=line_metrics.lineSpacing();

  //
  // Exception Section
  //
  if(!edit_airings_check->isChecked()) {
    //    printer->newPage();
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
      p->setFont(title_font);
      p->drawText(QRect(0,ypos,w,title_metrics.height()),
		  Qt::AlignTop|Qt::AlignHCenter,
		  "Exceptions");
      NewLine(title_metrics.lineSpacing(),printer,p,&ypos);
      p->setFont(line_font);
      p->drawText(QRect(0,ypos,w,title_metrics.height()),
		  Qt::AlignTop|Qt::AlignHCenter,
		  "The following program instances were NOT aired:");
      NewLine(title_metrics.lineSpacing(),printer,p,&ypos);
      q->seek(-1);
      
      //
      // Column Headers
      //
      int title_width=4*w/9;
      int date_width=
	line_metrics.horizontalAdvance(date.toString("MM/dd/yy"));
      int start_width=line_metrics.horizontalAdvance("Start Time");
      int end_width=line_metrics.horizontalAdvance("End Time");
      p->setFont(title_font);
      p->drawText(QRect(0,ypos,title_width,
			line_metrics.height()),Qt::AlignCenter,"Program");
      p->drawText(QRect(0+4*w/9,
			ypos,date_width,
			line_metrics.height()),Qt::AlignCenter,"Date");
      p->drawText(QRect(0+5*w/9,
			ypos,w/3,
			line_metrics.height()),
		  Qt::AlignLeft,"Start Time");
      p->drawText(QRect(0+6*w/9,
			ypos,w/3,line_metrics.height()),
		  Qt::AlignLeft,"End Time");
      NewLine(line_metrics.lineSpacing(),printer,p,&ypos);
      
      //
      // Program Listings
      //
      p->setFont(line_font);
      q->seek(-1);
      while(q->next()) {
	p->drawText(QRect(0,
			  ypos,4*w/9,line_metrics.height()),
		    Qt::AlignLeft,q->value(4).toString());
	p->drawText(QRect(0+4*w/9,
			  ypos,w/3,line_metrics.height()),
		    Qt::AlignLeft,q->value(0).toDateTime().
		    toString("MM/dd/yy"));
	p->drawText(QRect(0+5*w/9,
			  ypos,start_width,line_metrics.height()),
		    Qt::AlignCenter,q->value(0).toDateTime().toString("h:mm AP"));
	p->drawText(QRect(0+6*w/9,
			  ypos,end_width,line_metrics.height()),
		    Qt::AlignCenter,q->value(0).toDateTime().
		    addSecs(q->value(1).toInt()).toString("h:mm AP"));
	NewLine(line_metrics.lineSpacing(),printer,p,&ypos);
      }
      NewLine(line_metrics.lineSpacing(),printer,p,&ypos);
    }
    delete q;
  }

  //
  // Signature
  //
  p->drawText(QRect(0,ypos,w,line_metrics.height()),Qt::AlignLeft,signature);

  delete p;
}


void GenerateAffadavit::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  edit_date_label->setGeometry(40,10,70,19);
  edit_month_box->setGeometry(115,10,100,19);
  edit_year_box->setGeometry(220,10,70,19);

  edit_program_label->setGeometry(10,35,70,19);
  edit_program_box->setGeometry(85,35,w-95,19);
  edit_airings_check->setGeometry(85,59,15,15);
  edit_airings_label->setGeometry(105,55,200,19);

  edit_generate_button->setGeometry(10,h-60,80,50);
  edit_close_button->setGeometry(w-90,h-60,80,50);
}


void GenerateAffadavit::closeEvent(QCloseEvent *e)
{
  closeData();
}


void GenerateAffadavit::NewLine(int spacing,QPrinter *printer,QPainter *p,
				int *ypos)
{
  //  int w=p->device()->width();
  int h=p->device()->height();

  *ypos+=spacing;
  if(*ypos>h) {
    printer->newPage();
    *ypos=spacing;
  }
}


int GenerateAffadavit::PrintText(int ypos,QFontMetrics *fm,const QString &str,QPainter *p)
{
  int w=p->device()->width();
  //  int h=p->device()->height();

  QString line;
  QStringList strs=str.split(" ");

  for(int i=0;i<strs.size();i++) {
    if(fm->horizontalAdvance(line+" "+strs[i])<w) {
      line+=" "+strs[i];
    }
    else {
      p->drawText(QRect(0,ypos,w,fm->height()),Qt::AlignLeft,line);
      line=strs[i];
      ypos+=fm->height();
    }
  }
  if(!line.isEmpty()) {
    p->drawText(QRect(0,ypos,w,fm->height()),Qt::AlignLeft,line);
    ypos+=fm->height();
  }

  return ypos;
}
