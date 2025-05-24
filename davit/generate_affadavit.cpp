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
#include <QLabel>
#include <QMessageBox>
//#include <QPrinter>
#include <QPainter>
//#include <QPaintDeviceMetrics>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <dvt.h>
#include <dvtdatedialog.h>
#include <dvttextfile.h>
#include <dvtconf.h>

#include "generate_affadavit.h"
#include "preview.h"

GenerateAffadavit::GenerateAffadavit(ReportType type,int id,QWidget *parent)
  : QDialog(parent)
{
  setModal(true);

  QString sql;
  QSqlQuery *q;

  QDate this_month=QDate::currentDate();
  QDate last_month=this_month.addMonths(-1);
  edit_id=id;
  edit_type=type;
  setWindowTitle(tr("Generate Affidavit"));

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
  edit_year_box->setGeometry(220,10,70,19);
  connect(edit_year_box,SIGNAL(activated(int)),
	  this,SLOT(validateDateData(int)));
  int count=0;
  for(int i=DVT_ORIGIN_YEAR;i<(last_month.year()+1);i++) {
    edit_year_box->insertItem(count++,QString::asprintf("%d",i));
  }
  edit_year_box->setCurrentIndex(edit_year_box->count()-1);
  QLabel *label=new QLabel(tr("&Date:"),this);
  label->setGeometry(40,10,70,19);
  label->setFont(bold_font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  
  //
  // Program List
  //
  edit_program_box=new QComboBox(this);
  edit_program_box->setGeometry(85,35,sizeHint().width()-95,19);
  label=new QLabel(tr("&Program:"),this);
  label->setGeometry(10,35,70,19);
  label->setFont(bold_font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_program_box->insertItem(0,"[All Scheduled Programs]");
  sql="select PROGRAM_NAME from PROGRAMS order by PROGRAM_NAME";
  q=new QSqlQuery(sql);
  count=1;
  while(q->next()) {
    edit_program_box->insertItem(count++,q->value(0).toString());
  }
  delete q;

  //
  // Airings List
  //
  edit_airings_check=new QCheckBox(this);
  edit_airings_check->setGeometry(85,59,15,15);
  label=new QLabel(tr("Include Airings List"),this);
  label->setGeometry(105,55,200,19);
  label->setFont(bold_font);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  

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
		    currentText().toInt(),edit_month_box->currentIndex()+1,1);
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
		   edit_month_box->currentIndex()+1,1);

  if(edit_program_box->currentIndex()==0) {  // All Programs
    Preview *p=new Preview(this);
    p->exec(edit_id,date,edit_airings_check->isChecked());
    delete p;
    return;
  }

  //
  // Check for Affidavit Data
  //
  sql=QString::asprintf("select PROGRAMS.ID from AIRED left join PROGRAMS \
                         on AIRED.PROGRAM_ID=PROGRAMS.ID \
                         where (PROGRAMS.PROGRAM_NAME=\"%s\")&&\
                         (AIRED.AFFILIATE_ID=%d)&&\
                         (AIRED.AIR_DATETIME>=\"%s-01 00:00:00\")&&\
                         (AIRED.AIR_DATETIME<\"%s-01 00:00:00\")&&\
                         ((AIRED.STATE=%d)||(AIRED.STATE=%d)) \
                         order by AIRED.AIR_DATETIME",
			DvtEscapeString(edit_program_box->
					currentText()).toUtf8().constData(),
			edit_id,
			date.toString("yyyy-MM").toUtf8().constData(),
			date.addMonths(1).toString("yyyy-MM").
			toUtf8().constData(),
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
