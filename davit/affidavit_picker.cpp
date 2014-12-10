// affidavit_picker.cpp
//
// Pick fields for an Affidavit Report
//
//   (C) Copyright 2014 Fred Gleason <fredg@paravelsystems.com>
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

#include <qsqldatabase.h>

#include "affidavit_picker.h"

AffidavitPicker::AffidavitPicker(Dvt::AffidavitStationFilter *stations,
				 Dvt::AffidavitSortType *sort,
				 int *pgm_id,QWidget *parent)
  : QDialog(parent,"",true)
{
  QString sql;
  QSqlQuery *q;

  report_stations=stations;
  report_sort_type=sort;
  report_program_id=pgm_id;

  setCaption("Davit - "+tr("Set Parameters"));

  //
  // Fonts
  //
  QFont label_font("helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  //
  // Station Selector
  //
  report_stations_box=new QComboBox(this);
  report_stations_box->insertItem(tr("All Dayparts"));
  report_stations_box->insertItem(tr("Weekdays"));
  report_stations_box->insertItem(tr("Weekends"));
  report_stations_box->insertItem(tr("By Program"));
  report_stations_label=new QLabel(report_stations_box,tr("Display")+":",this);
  report_stations_label->setFont(label_font);
  report_stations_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  connect(report_stations_box,SIGNAL(activated(int)),
	  this,SLOT(stationsSelectedData(int)));

  //
  // Program Selector
  //
  report_program_box=new QComboBox(this);
  sql=QString("select ID,PROGRAM_NAME from PROGRAMS order by PROGRAM_NAME");
  q=new QSqlQuery(sql);
  while(q->next()) {
    report_program_ids.push_back(q->value(0).toInt());
    report_program_box->insertItem(q->value(1).toString());
  }
  delete q;
  report_program_label=new QLabel(report_program_box,tr("Program")+":",this);
  report_program_label->setFont(label_font);
  report_program_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // Sort Selector
  //
  report_sort_box=new QComboBox(this);
  report_sort_box->insertItem(tr("By State"));
  report_sort_box->insertItem(tr("By Severity"));
  report_sort_label=new QLabel(report_sort_box,tr("Sort By")+":",this);
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

  //
  // Load Values
  //
  report_sort_box->setCurrentItem(*sort);
  report_stations_box->setCurrentItem(*stations);
  stationsSelectedData((int)(*sort));

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


void AffidavitPicker::stationsSelectedData(int num)
{
  report_program_label->setEnabled(num==Dvt::Program);
  report_program_box->setEnabled(num==Dvt::Program);
}


void AffidavitPicker::okData()
{
  *report_stations=
    (Dvt::AffidavitStationFilter)report_stations_box->currentItem();
  *report_sort_type=
    (Dvt::AffidavitSortType)report_sort_box->currentItem();
  *report_program_id=report_program_ids[report_program_box->currentItem()];
  done(0);
}


void AffidavitPicker::cancelData()
{
  done(-1);
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
