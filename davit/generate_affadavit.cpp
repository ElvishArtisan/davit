// generate_affadavit.cpp
//
// Select a Set of Dates for an Affidavit
//
//   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: generate_affadavit.cpp,v 1.10 2011/03/29 15:38:51 pcvs Exp $
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

#include <qlabel.h>
#include <qmessagebox.h>
#include <qfile.h>
#include <qsqldatabase.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qfontmetrics.h>
#include <qpaintdevicemetrics.h>

#include <dvt.h>
#include <dvtdatedialog.h>
#include <dvttextfile.h>
#include <dvtconf.h>

#include "preview.h"
#include "generate_affadavit.h"

GenerateAffadavit::GenerateAffadavit(ReportType type,int id,
				     QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  QString sql;
  QSqlQuery *q;

  QDate this_month=QDate::currentDate();
  QDate last_month=this_month.addMonths(-1);
  QDate start_date=QDate(last_month.year(),last_month.month(),1);
  QDate end_date=QDate(last_month.year(),last_month.month(),
		       QDate(this_month.year(),
			     this_month.month(),1).addDays(-1).day());

  edit_id=id;
  edit_type=type;
  setCaption(tr("Generate Affidavit"));

  //
  // Fix the Window Size
  //
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  //
  // Create Fonts
  //
  QFont bold_font=QFont("Helvetica",12,QFont::Bold);
  bold_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // Date
  //
  edit_month_box=new QComboBox(this);
  edit_month_box->setGeometry(115,10,100,19);
  connect(edit_month_box,SIGNAL(activated(int)),
	  this,SLOT(validateDateData(int)));
  edit_month_box->insertItem(tr("January"));
  edit_month_box->insertItem(tr("February"));
  edit_month_box->insertItem(tr("March"));
  edit_month_box->insertItem(tr("April"));
  edit_month_box->insertItem(tr("May"));
  edit_month_box->insertItem(tr("June"));
  edit_month_box->insertItem(tr("July"));
  edit_month_box->insertItem(tr("August"));
  edit_month_box->insertItem(tr("September"));
  edit_month_box->insertItem(tr("October"));
  edit_month_box->insertItem(tr("November"));
  edit_month_box->insertItem(tr("December"));
  edit_month_box->setCurrentItem(last_month.month()-1);
  edit_year_box=new QComboBox(this);
  edit_year_box->setGeometry(220,10,70,19);
  connect(edit_year_box,SIGNAL(activated(int)),
	  this,SLOT(validateDateData(int)));
  for(int i=DVT_ORIGIN_YEAR;i<(last_month.year()+1);i++) {
    edit_year_box->insertItem(QString().sprintf("%d",i));
  }
  edit_year_box->setCurrentItem(edit_year_box->count()-1);
  QLabel *label=new QLabel(edit_month_box,tr("&Date:"),this);
  label->setGeometry(40,10,70,19);
  label->setFont(bold_font);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  
  //
  // Program List
  //
  edit_program_box=new QComboBox(this);
  edit_program_box->setGeometry(85,35,sizeHint().width()-95,19);
  label=new QLabel(edit_program_box,tr("&Program:"),this);
  label->setGeometry(10,35,70,19);
  label->setFont(bold_font);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);
  edit_program_box->insertItem("[All Scheduled Programs]");
  sql="select PROGRAM_NAME from PROGRAMS order by PROGRAM_NAME";
  q=new QSqlQuery(sql);
  while(q->next()) {
    edit_program_box->insertItem(q->value(0).toString());
  }
  delete q;

  //
  // Airings List
  //
  edit_airings_check=new QCheckBox(this);
  edit_airings_check->setGeometry(85,59,15,15);
  label=new QLabel(edit_airings_check,tr("Include Airings List"),this);
  label->setGeometry(105,55,200,19);
  label->setFont(bold_font);
  label->setAlignment(AlignLeft|AlignVCenter|ShowPrefix);
  

  //
  //  Generate Button
  //
  edit_generate_button=new QPushButton(this);
  edit_generate_button->setGeometry(10,sizeHint().height()-60,80,50);
  edit_generate_button->setFont(bold_font);
  edit_generate_button->setText(tr("&Generate\nAffidavit"));
  connect(edit_generate_button,SIGNAL(clicked()),this,SLOT(generateData()));

  //
  //  Close Button
  //
  QPushButton *button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(bold_font);
  button->setText(tr("C&lose"));
  connect(button,SIGNAL(clicked()),this,SLOT(closeData()));
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


void GenerateAffadavit::validateDateData(int index)
{
  QDate start_date=QDate(edit_year_box->
		    currentText().toInt(),edit_month_box->currentItem()+1,1);
  QDate end_date=start_date.addMonths(1).addDays(-1);
  edit_generate_button->setEnabled(end_date<QDate::currentDate());
}


void GenerateAffadavit::generateData()
{
  QString sql;
  QSqlQuery *q;
  int pgm_id=-1;

  //
  // Generate Date
  //
  QDate date=QDate(edit_year_box->currentText().toInt(),
		   edit_month_box->currentItem()+1,1);

  if(edit_program_box->currentItem()==0) {  // All Programs
    Preview *p=new Preview(this);
    p->exec(edit_id,date,edit_airings_check->isChecked());
    delete p;
    return;
  }

  //
  // Check for Affidavit Data
  //
  sql=QString().sprintf("select PROGRAMS.ID from AIRED left join PROGRAMS \
                         on AIRED.PROGRAM_ID=PROGRAMS.ID \
                         where (PROGRAMS.PROGRAM_NAME=\"%s\")&&\
                         (AIRED.AFFILIATE_ID=%d)&&\
                         (AIRED.AIR_DATETIME>=\"%s-01 00:00:00\")&&\
                         (AIRED.AIR_DATETIME<\"%s-01 00:00:00\")&&\
                         ((AIRED.STATE=%d)||(AIRED.STATE=%d)) \
                         order by AIRED.AIR_DATETIME",
			(const char *)DvtEscapeString(edit_program_box->
						      currentText()),
			edit_id,
			(const char *)date.toString("yyyy-MM"),
			(const char *)date.addMonths(1).toString("yyyy-MM"),
			Dvt::AiredStateConfirmed,
			Dvt::AiredStateDenied);
  q=new QSqlQuery(sql);
  if(q->first()) {
    pgm_id=q->value(0).toInt();
  }
  else {
    QMessageBox::information(this,"Davit",
			     tr("No affidavit data found for this program!"));
    delete q;
    return;
  }
  delete q;

  Preview *p=new Preview(this);
  p->exec(edit_id,pgm_id,date,edit_airings_check->isChecked());
  delete p;
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
  done(0);
}
