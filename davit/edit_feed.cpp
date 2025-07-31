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

#include <dvtdb.h>

#include "edit_feed.h"

EditFeed::EditFeed(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  setModal(true);
  edit_feed_id=-1;

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());

  //
  // Monday Button
  //
  edit_mon_button=new QCheckBox(this);
  edit_mon_button->setGeometry(22,10,20,20);
  edit_mon_label=new QLabel(tr("Monday"),this);
  edit_mon_label->setGeometry(42,10,115,20);
  edit_mon_label->setFont(subLabelFont());
  edit_mon_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Tuesday Button
  //
  edit_tue_button=new QCheckBox(this);
  edit_tue_button->setGeometry(99,10,20,20);
  edit_tue_label=new QLabel(tr("Tuesday"),this);
  edit_tue_label->setGeometry(119,10,115,20);
  edit_tue_label->setFont(subLabelFont());
  edit_tue_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Wednesday Button
  //
  edit_wed_button=new QCheckBox(this);
  edit_wed_button->setGeometry(181,10,20,20);
  edit_wed_label=new QLabel(tr("Wednesday"),this);
  edit_wed_label->setGeometry(201,10,115,20);
  edit_wed_label->setFont(subLabelFont());
  edit_wed_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Thursday Button
  //
  edit_thu_button=new QCheckBox(this);
  edit_thu_button->setGeometry(283,10,20,20);
  edit_thu_label=new QLabel(tr("Thursday"),this);
  edit_thu_label->setGeometry(303,10,115,20);
  edit_thu_label->setFont(subLabelFont());
  edit_thu_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Friday Button
  //
  edit_fri_button=new QCheckBox(this);
  edit_fri_button->setGeometry(370,10,20,20);
  edit_fri_label=new QLabel(tr("Friday"),this);
  edit_fri_label->setGeometry(390,10,35,20);
  edit_fri_label->setFont(subLabelFont());
  edit_fri_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Saturday Button
  //
  edit_sat_button=new QCheckBox(this);
  edit_sat_button->setGeometry(142,35,20,20);
  edit_sat_label=new QLabel(tr("Saturday"),this);
  edit_sat_label->setGeometry(162,35,60,20);
  edit_sat_label->setFont(subLabelFont());
  edit_sat_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Sunday Button
  //
  edit_sun_button=new QCheckBox(this);
  edit_sun_button->setGeometry(246,35,20,20);
  edit_sun_label=new QLabel(tr("Sunday"),this);
  edit_sun_label->setGeometry(266,35,60,20);
  edit_sun_label->setFont(subLabelFont());
  edit_sun_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Start Time
  //
  edit_starttime_edit=new QTimeEdit(this);
  edit_starttime_edit->setDisplayFormat("hh:mm:ss");
  edit_starttime_edit->setGeometry(112,60,90,20);
  edit_starttime_label=new QLabel(tr("Starting at:"),this);
  edit_starttime_label->setGeometry(22,60,85,20);
  edit_starttime_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_starttime_label->setFont(labelFont());

  //
  //  OK Button
  //
  QPushButton *button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(buttonFont());
  button->setText("OK");
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(buttonFont());
  button->setText("Cancel");
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));
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


int EditFeed::exec(int feed_id,bool new_entry)
{
  edit_feed_id=feed_id;
  
  setWindowTitle("Davit - "+tr("Edit Feed Slot")+
		 QString::asprintf(" [ID: %d]",feed_id));;

  QString sql=QString("select ")+
    "`AIR_SUN`,"+   // 00
    "`AIR_MON`,"+   // 01
    "`AIR_TUE`,"+   // 02
    "`AIR_WED`,"+   // 03
    "`AIR_THU`,"+   // 04
    "`AIR_FRI`,"+   // 05
    "`AIR_SAT`,"+   // 06
    "`AIR_TIME` "+  // 07
    "from `FEEDS` where "+
    QString::asprintf("`ID`=%d",edit_feed_id);
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  if(q->first()) {
    edit_sun_button->setChecked(q->value(0).toString()=="Y");
    edit_mon_button->setChecked(q->value(1).toString()=="Y");
    edit_tue_button->setChecked(q->value(2).toString()=="Y");
    edit_wed_button->setChecked(q->value(3).toString()=="Y");
    edit_thu_button->setChecked(q->value(4).toString()=="Y");
    edit_fri_button->setChecked(q->value(5).toString()=="Y");
    edit_sat_button->setChecked(q->value(6).toString()=="Y");
    edit_starttime_edit->setTime(q->value(7).toTime());
  }
  delete q;

  return QDialog::exec();
}


void EditFeed::okData()
{
  QString sql=QString("update `FEEDS` set ")+
    "`AIR_SUN`="+CheckboxState(edit_sun_button)+","+
    "`AIR_MON`="+CheckboxState(edit_mon_button)+","+
    "`AIR_TUE`="+CheckboxState(edit_tue_button)+","+
    "`AIR_WED`="+CheckboxState(edit_wed_button)+","+
    "`AIR_THU`="+CheckboxState(edit_thu_button)+","+
    "`AIR_FRI`="+CheckboxState(edit_fri_button)+","+
    "`AIR_SAT`="+CheckboxState(edit_sat_button)+","+
    "`AIR_TIME`="+
    DvtSqlQuery::escape(edit_starttime_edit->time().toString("hh:mm:ss"))+
    " where "+
    QString::asprintf("`ID`=%d",edit_feed_id);
  DvtSqlQuery::apply(sql);

  done(true);
}


void EditFeed::cancelData()
{
  done(false);
}


QString EditFeed::CheckboxState(QCheckBox *check) const
{
  QString ret="'N'";

  if(check->isChecked()) {
    ret="'Y'";
  }
  
  return ret;
}
