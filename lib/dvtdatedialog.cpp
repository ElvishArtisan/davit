// dvtdatedialog.cpp
//
// A Dialog Box for using an DvtDatePicker widget.
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


#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QRect>
#include <QPoint>
#include <QPainter>
#include <QString>
#include <QMessageBox>
#include <QLineEdit>
#include <QLabel>
#include <QSignalMapper>

#include "dvtconf.h"
#include "dvtdatedialog.h"

//
// Global Classes
//
DvtDateDialog::DvtDateDialog(int low_year,int high_year,QWidget *parent)
  : QDialog(parent)
{
  setModal(true);
  QFont font;

  font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);

  setWindowTitle(tr("Select Date"));

  //
  // Datepicker
  //
  date_picker=new DvtDatePicker(low_year,high_year,this);
  date_picker->setGeometry(10,10,
			   date_picker->sizeHint().width(),
			   date_picker->sizeHint().height());

  //
  // OK Button
  //
  QPushButton *button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-130,sizeHint().height()-40,50,30);
  button->setFont(font);
  button->setText(tr("&OK"));
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-65,sizeHint().height()-40,55,30);
  button->setFont(font);
  button->setText(tr("&Cancel"));
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


DvtDateDialog::~DvtDateDialog()
{
}


QSize DvtDateDialog::sizeHint() const
{
  return QSize(date_picker->sizeHint().width()+20,
	       date_picker->sizeHint().height()+60);
}


QSizePolicy DvtDateDialog::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int DvtDateDialog::exec(QDate *date)
{
  date_date=date;
  date_picker->setDate(*date_date);
  return QDialog::exec();
}


void DvtDateDialog::okData()
{
  *date_date=date_picker->date();
  done(0);
}


void DvtDateDialog::cancelData()
{
  done(-1);
}
