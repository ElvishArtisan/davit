// pick_daypart.cpp
//
// Pick a daypart for a Davit report.
//
//   (C) Copyright 2008-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <QPushButton>

#include "pick_daypart.h"

PickDaypart::PickDaypart(QTime *start_time,QTime *end_time,bool dows[7],
			 DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  setModal(true);
  report_start_time=start_time;
  report_end_time=end_time;
  report_dows=dows;
  setWindowTitle(tr("Davit - Select Daypart"));

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());

  //
  // Start Time
  //
  report_start_edit=new QTimeEdit(this);
  report_start_edit->setGeometry(115,10,90,20);
  QLabel *label=new QLabel(tr("Start Time:"),this);
  label->setGeometry(10,10,100,20);
  label->setFont(labelFont());
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // End Time
  //
  report_end_edit=new QTimeEdit(this);
  report_end_edit->setGeometry(115,32,90,20);
  label=new QLabel(tr("End Time:"),this);
  label->setGeometry(10,32,100,20);
  label->setFont(labelFont());
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // Day of the Week Picker
  //
  report_dow_picker=new DayPicker(c,this);
  report_dow_picker->setGeometry(20,60,sizeHint().width()-40,60);

  //
  // OK Button
  //
  QPushButton *button=new QPushButton(tr("OK"),this);
  button->setFont(buttonFont());
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  button=new QPushButton(tr("Cancel"),this);
  button->setFont(buttonFont());
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Values
  //
  report_start_edit->setTime(*start_time);
  report_end_edit->setTime(*end_time);
  for(int i=0;i<7;i++) {
    report_dow_picker->setState(i+1,dows[i]);
  }
}


PickDaypart::~PickDaypart()
{
}


QSize PickDaypart::sizeHint() const
{
  return QSize(400,190);
}


QSizePolicy PickDaypart::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void PickDaypart::okData()
{
  *report_start_time=report_start_edit->time();
  *report_end_time=report_end_edit->time();
  for(int i=0;i<7;i++) {
    report_dows[i]=report_dow_picker->state(i+1);
  }

  done(0);
}


void PickDaypart::cancelData()
{
  done(-1);
}
