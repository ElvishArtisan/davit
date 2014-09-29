// pick_daypart.cpp
//
// Pick a daypart for a Davit report.
//
//   (C) Copyright 2008,2010 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: pick_daypart.cpp,v 1.1 2010/11/05 16:22:39 pcvs Exp $
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

#include <qpushbutton.h>

#include <pick_daypart.h>

PickDaypart::PickDaypart(QTime *start_time,QTime *end_time,bool dows[7],
			 QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  report_start_time=start_time;
  report_end_time=end_time;
  report_dows=dows;
  setCaption(tr("Davit - Select Daypart"));

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  //
  // Generate Fonts
  //
  QFont label_font("helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  //
  // Start Time
  //
  report_start_edit=new QTimeEdit(this);
  report_start_edit->setGeometry(115,10,90,20);
  QLabel *label=new QLabel(report_start_edit,tr("Start Time:"),this);
  label->setGeometry(10,10,100,20);
  label->setFont(label_font);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  // End Time
  //
  report_end_edit=new QTimeEdit(this);
  report_end_edit->setGeometry(115,32,90,20);
  label=new QLabel(report_end_edit,tr("End Time:"),this);
  label->setGeometry(10,32,100,20);
  label->setFont(label_font);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  // Day of the Week Picker
  //
  report_dow_picker=new DayPicker(this);
  report_dow_picker->setGeometry(20,60,sizeHint().width()-40,60);

  //
  // OK Button
  //
  QPushButton *button=new QPushButton(tr("OK"),this);
  button->setFont(label_font);
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  button=new QPushButton(tr("Cancel"),this);
  button->setFont(label_font);
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
