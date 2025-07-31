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
#include <dvtdb.h>

#include "datedialog.h"
#include "generate_affadavit.h"
#include "textfile.h"

GenerateAffadavit::GenerateAffadavit(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  setModal(true);

  QString sql;
  QSqlQuery *q;

  QDate this_month=QDate::currentDate();
  QDate last_month=this_month.addMonths(-1);
  edit_affiliate_id=-1;
  edit_program_id=-1;
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
  edit_program_id=-1;
  edit_type=GenerateAffadavit::ReportAffiliate;

  setWindowTitle("Davit - "+tr("Generate Affiliate Affidavit")+
		 QString::asprintf(" [ID: %d]",affiliate_id));
  edit_program_box->setCurrentIndex(0);
  edit_program_box->setEnabled(true);

  updateGenerateButtonData(-1);

  return QDialog::exec();
}


int GenerateAffadavit::execProgram(int program_id)
{
  edit_program_id=program_id;
  edit_affiliate_id=-1;
  edit_type=GenerateAffadavit::ReportProgram;

  setWindowTitle("Davit - "+tr("Generate Program Affidavit")+
		 QString::asprintf(" [ID: %d]",program_id));
  for(int i=0;i<edit_program_box->count();i++) {
    if(edit_program_box->itemData(i).toInt()==program_id) {
      edit_program_box->setCurrentIndex(i);
      break;
    }
  }
  edit_program_box->setEnabled(false);

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
  switch(edit_type) {
  case GenerateAffadavit::ReportAffiliate:
    connect(dialog,SIGNAL(paintRequested(QPrinter *)),
	    this,SLOT(paintAffiliateReport(QPrinter *)));
    break;

  case GenerateAffadavit::ReportProgram:
    connect(dialog,SIGNAL(paintRequested(QPrinter *)),
	    this,SLOT(paintProgramReport(QPrinter *)));
    break;
  }

  dialog->exec();
  delete dialog;
}


void GenerateAffadavit::closeData()
{
  done(true);
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
  if(edit_affiliate_id>0) {
    sql+=QString::asprintf("(`AIRED`.`AFFILIATE_ID`=%d)&&",edit_affiliate_id);
  }
  if(edit_program_id>0) {
    sql+=QString::asprintf("(`AIRED`.`PROGRAM_ID`=%d)&&",edit_program_id);
  }
  sql+="(`AIRED`.`AIR_DATETIME`>="+
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
  printf("HasAffidavits: %d\nSQL: %s\n",ret,sql.toUtf8().constData());

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


void GenerateAffadavit::NewLine(QPrinter *printer,QPainter *p,int *ypos)
{
  //  int w=p->device()->width();
  int h=p->device()->height();

  *ypos+=p->fontMetrics().lineSpacing();
  if(*ypos>h) {
    printer->newPage();
    *ypos=p->fontMetrics().lineSpacing();;
  }
}


int GenerateAffadavit::PrintText(int ypos,const QString &str,QPainter *p)
{
  int w=p->device()->width();
  //  int h=p->device()->height();

  QString line;
  QStringList strs=str.split(" ");

  for(int i=0;i<strs.size();i++) {
    if(p->fontMetrics().horizontalAdvance(line+" "+strs[i])<w) {
      line+=" "+strs[i];
    }
    else {
      p->drawText(QRect(0,ypos,w,p->fontMetrics().height()),Qt::AlignLeft,line);
      line=strs[i];
      ypos+=p->fontMetrics().height();
    }
  }
  if(!line.isEmpty()) {
    p->drawText(QRect(0,ypos,w,p->fontMetrics().height()),Qt::AlignLeft,line);
    ypos+=p->fontMetrics().height();
  }

  return ypos;
}
