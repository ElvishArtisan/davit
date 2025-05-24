// edit_feed.cpp
//
// Edit a Davit Feed.
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
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

#include <math.h>

#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "edit_feed.h"

EditFeed::EditFeed(DvtFeed *feed,QWidget *parent)
  : QDialog(parent)
{
  setModal(true);
  edit_feed=feed;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setWindowTitle("Davit - Edit Show Time");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont day_font=QFont("Helvetica",10,QFont::Normal);
  day_font.setPixelSize(10);

  //
  // Monday Button
  //
  edit_mon_button=new QCheckBox(this);
  edit_mon_button->setGeometry(22,10,20,20);
  edit_mon_label=new QLabel(tr("Monday"),this);
  edit_mon_label->setGeometry(42,10,115,20);
  edit_mon_label->setFont(day_font);
  edit_mon_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Tuesday Button
  //
  edit_tue_button=new QCheckBox(this);
  edit_tue_button->setGeometry(99,10,20,20);
  edit_tue_label=new QLabel(tr("Tuesday"),this);
  edit_tue_label->setGeometry(119,10,115,20);
  edit_tue_label->setFont(day_font);
  edit_tue_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Wednesday Button
  //
  edit_wed_button=new QCheckBox(this);
  edit_wed_button->setGeometry(181,10,20,20);
  edit_wed_label=new QLabel(tr("Wednesday"),this);
  edit_wed_label->setGeometry(201,10,115,20);
  edit_wed_label->setFont(day_font);
  edit_wed_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Thursday Button
  //
  edit_thu_button=new QCheckBox(this);
  edit_thu_button->setGeometry(283,10,20,20);
  edit_thu_label=new QLabel(tr("Thursday"),this);
  edit_thu_label->setGeometry(303,10,115,20);
  edit_thu_label->setFont(day_font);
  edit_thu_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Friday Button
  //
  edit_fri_button=new QCheckBox(this);
  edit_fri_button->setGeometry(370,10,20,20);
  edit_fri_label=new QLabel(tr("Friday"),this);
  edit_fri_label->setGeometry(390,10,35,20);
  edit_fri_label->setFont(day_font);
  edit_fri_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Saturday Button
  //
  edit_sat_button=new QCheckBox(this);
  edit_sat_button->setGeometry(142,35,20,20);
  edit_sat_label=new QLabel(tr("Saturday"),this);
  edit_sat_label->setGeometry(162,35,60,20);
  edit_sat_label->setFont(day_font);
  edit_sat_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Sunday Button
  //
  edit_sun_button=new QCheckBox(this);
  edit_sun_button->setGeometry(246,35,20,20);
  edit_sun_label=new QLabel(tr("Sunday"),this);
  edit_sun_label->setGeometry(266,35,60,20);
  edit_sun_label->setFont(day_font);
  edit_sun_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Start Time
  //
  edit_starttime_edit=new QTimeEdit(this);
  edit_starttime_edit->setGeometry(112,60,90,20);
  edit_starttime_label=new QLabel(tr("&Starting at:"),this);
  edit_starttime_label->setGeometry(22,60,85,20);
  edit_starttime_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_starttime_label->setFont(label_font);

  //
  //  OK Button
  //
  QPushButton *button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(label_font);
  button->setText("&OK");
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText("&Cancel");
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Data
  //
  edit_starttime_edit->setTime(feed->startTime());
  edit_sun_button->setChecked(feed->dowActive(7));
  edit_mon_button->setChecked(feed->dowActive(1));
  edit_tue_button->setChecked(feed->dowActive(2));
  edit_wed_button->setChecked(feed->dowActive(3));
  edit_thu_button->setChecked(feed->dowActive(4));
  edit_fri_button->setChecked(feed->dowActive(5));
  edit_sat_button->setChecked(feed->dowActive(6));
}


EditFeed::~EditFeed()
{
}


QSize EditFeed::sizeHint() const
{
  return QSize(450,160);
} 


QSizePolicy EditFeed::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditFeed::okData()
{
  edit_feed->setStartTime(edit_starttime_edit->time());
  edit_feed->setDowActive(7,edit_sun_button->isChecked());
  edit_feed->setDowActive(1,edit_mon_button->isChecked());
  edit_feed->setDowActive(2,edit_tue_button->isChecked());
  edit_feed->setDowActive(3,edit_wed_button->isChecked());
  edit_feed->setDowActive(4,edit_thu_button->isChecked());
  edit_feed->setDowActive(5,edit_fri_button->isChecked());
  edit_feed->setDowActive(6,edit_sat_button->isChecked());
  done(0);
}


void EditFeed::cancelData()
{
  done(-1);
}
