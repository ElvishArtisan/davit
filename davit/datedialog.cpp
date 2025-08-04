// datedialog.cpp
//
// A Dialog Box for using a DatePicker widget.
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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

#include "dvtconf.h"
#include "datedialog.h"

//
// Global Classes
//
DateDialog::DateDialog(int low_year,int high_year,DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  setModal(true);

  setWindowTitle("Davit - "+tr("Select Date"));

  //
  // Datepicker
  //
  date_picker=new DatePicker(low_year,high_year,config(),this);

  //
  // OK Button
  //
  date_ok_button=new QPushButton(this);
  date_ok_button->setFont(buttonFont());
  date_ok_button->setText(tr("OK"));
  connect(date_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  date_cancel_button=new QPushButton(this);
  date_cancel_button->setFont(buttonFont());
  date_cancel_button->setText(tr("Cancel"));
  connect(date_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


DateDialog::~DateDialog()
{
}


QSize DateDialog::sizeHint() const
{
  return QSize(date_picker->sizeHint().width()+20,
	       date_picker->sizeHint().height()+60);
}


QSizePolicy DateDialog::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int DateDialog::exec(QDate *date)
{
  date_date=date;
  date_picker->setDate(*date_date);
  return QDialog::exec();
}


void DateDialog::okData()
{
  *date_date=date_picker->date();
  done(true);
}


void DateDialog::cancelData()
{
  done(false);
}


void DateDialog::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  date_picker->setGeometry(10,10,w,h-50);

  date_ok_button->setGeometry(w-130,h-40,55,30);
  date_cancel_button->setGeometry(w-65,h-40,55,30);
}
