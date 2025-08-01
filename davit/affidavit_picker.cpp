// affidavit_picker.cpp
//
// Pick fields for an Affidavit Report
//
//   (C) Copyright 2014-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <dvtdb.h>

#include "affidavit_picker.h"

AffidavitPicker::AffidavitPicker(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  setModal(true);
  report_stations=NULL;
  report_sort_type=NULL;
  report_program_id=NULL;

  setWindowTitle("Davit - "+tr("Set Report Parameters"));

  //
  // Fonts
  //
  QFont label_font("helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  //
  // Station Selector
  //
  report_stations_box=new QComboBox(this);
  report_stations_box->insertItem(0,tr("All Dayparts"));
  report_stations_box->insertItem(1,tr("Weekdays"));
  report_stations_box->insertItem(2,tr("Weekends"));
  report_stations_box->insertItem(3,tr("By Program"));
  report_stations_label=new QLabel(tr("Display")+":",this);
  report_stations_label->setFont(label_font);
  report_stations_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  connect(report_stations_box,SIGNAL(activated(int)),
	  this,SLOT(stationsSelectedData(int)));

  //
  // Program Selector
  //
  report_program_box=new QComboBox(this);
  report_program_label=new QLabel(tr("Program")+":",this);
  report_program_label->setFont(label_font);
  report_program_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // Sort Selector
  //
  report_sort_box=new QComboBox(this);
  report_sort_box->insertItem(0,tr("By State"));
  report_sort_box->insertItem(1,tr("By Worst Offenders"));
  report_sort_label=new QLabel(tr("Sort By")+":",this);
  report_sort_label->setFont(label_font);
  report_sort_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // OK Button
  //
  report_ok_button=new QPushButton(tr("OK"),this);
  report_ok_button->setFont(label_font);
  connect(report_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  report_cancel_button=new QPushButton(tr("Cancel"),this);
  report_cancel_button->setFont(label_font);
  connect(report_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));

  setMinimumSize(sizeHint());
  setMaximumHeight(sizeHint().height());
}


AffidavitPicker::~AffidavitPicker()
{
}


QSize AffidavitPicker::sizeHint() const
{
  return QSize(400,155);
}


int AffidavitPicker::exec(Dvt::AffidavitStationFilter *stations,
			  Dvt::AffidavitSortType *sort,int *pgm_id)
{
  QString sql;
  DvtSqlQuery *q;

  report_program_box->clear();
  sql=QString("select ")+
    "`ID`,"+            // 00
    "`PROGRAM_NAME` "+  // 01
    "from `PROGRAMS` order by `PROGRAM_NAME`";
  q=new DvtSqlQuery(sql);
  int count=0;
  while(q->next()) {
    report_program_box->insertItem(count++,q->value(1).toString(),
				   q->value(0).toInt());
  }
  delete q;

  report_stations=stations;
  report_sort_type=sort;
  report_program_id=pgm_id;

  //
  // Load Values
  //
  report_sort_box->setCurrentIndex(*sort);
  report_stations_box->setCurrentIndex(*stations);
  stationsSelectedData((int)(*sort));


  
  return QDialog::exec();
}


void AffidavitPicker::stationsSelectedData(int num)
{
  report_program_label->setEnabled(num==Dvt::Program);
  report_program_box->setEnabled(num==Dvt::Program);
}


void AffidavitPicker::okData()
{
  *report_stations=
    (Dvt::AffidavitStationFilter)report_stations_box->currentIndex();
  *report_sort_type=
    (Dvt::AffidavitSortType)report_sort_box->currentIndex();
  *report_program_id=report_program_box->itemData(report_program_box->currentIndex()).toInt();

  done(true);
}


void AffidavitPicker::cancelData()
{
  done(false);
}


void AffidavitPicker::resizeEvent(QResizeEvent *e)
{
  report_stations_label->setGeometry(10,10,80,20);
  report_stations_box->setGeometry(95,10,size().width()-105,20);

  report_program_label->setGeometry(10,32,80,20);
  report_program_box->setGeometry(95,32,size().width()-105,20);

  report_sort_label->setGeometry(10,54,80,20);
  report_sort_box->setGeometry(95,54,size().width()-105,20);

  report_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  report_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}
