// daypicker.cpp
//
// A Widget for selecting days of the week.
//
//   (C) Copyright 2010-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include "daypicker.h"

DayPicker::DayPicker(DvtConfig *c,QWidget *parent)
  : Widget(c,parent)
{
  for(int i=0;i<7;i++) {
    dow_box[i]=new QCheckBox(this);
    dow_label[i]=new QLabel(this);
    dow_label[i]->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    //dow_label[i]->setBackgroundColor(red);
    //dow_label[i]->hide();
  }
  dow_label[0]->setText(tr("Monday"));
  dow_label[1]->setText(tr("Tuesday"));
  dow_label[2]->setText(tr("Wednesday"));
  dow_label[3]->setText(tr("Thursday"));
  dow_label[4]->setText(tr("Friday"));
  dow_label[5]->setText(tr("Saturday"));
  dow_label[6]->setText(tr("Sunday"));
}


DayPicker::~DayPicker()
{
}


QSize DayPicker::sizeHint() const
{
  return QSize(300,100);
}


QSizePolicy DayPicker::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


bool DayPicker::state(int dow) const
{
  return dow_box[dow-1]->isChecked();
}


void DayPicker::setState(int dow,bool state)
{
  dow_box[dow-1]->setChecked(state);
}


void DayPicker::resizeEvent(QResizeEvent *e)
{
  //
  // Weekdays
  //
  int step=size().width()/3;
  int hstep=size().height()/3;
  for(int i=0;i<3;i++) {
    dow_box[i]->setGeometry(step*i,2,15,15);
    dow_label[i]->setGeometry(step*i+20,0,size().width()/4,20);
  }
  step=size().width()/2;
  for(int i=0;i<2;i++) {
    dow_box[3+i]->setGeometry(60+step*i,2+hstep,15,15);
    dow_label[3+i]->setGeometry(60+step*i+20,hstep,size().width()/4,20);
  }

  //
  // Weekend
  //
  for(int i=0;i<2;i++) {
    dow_box[5+i]->setGeometry(60+step*i,2+2*hstep,15,15);
    dow_label[5+i]->setGeometry(60+step*i+20,2*hstep,size().width()/4,20);
  }
  
}
