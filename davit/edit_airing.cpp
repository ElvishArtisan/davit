// edit_airing.cpp
//
// Edit a Davit Airing.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
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

#include <qpushbutton.h>
#include <qlabel.h>
#include <qsqldatabase.h>
#include <qpainter.h>
#include <qmessagebox.h>

#include <math.h>

#include <edit_airing.h>

EditAiring::EditAiring(DvtFeed *feed,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  QString sql;
  QSqlQuery *q;

  edit_feed=feed;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Davit - Edit Show Time");

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
  // Program Name
  //
  edit_program_box=new QComboBox(this,"program_name_label");
  edit_program_box->setGeometry(85,10,sizeHint().width()-95,20);
  QLabel *label=new QLabel(tr("Program:"),this,"label");
  label->setGeometry(10,10,70,20);
  label->setFont(label_font);
  label->setAlignment(AlignRight|AlignVCenter|ShowPrefix);

  //
  // Monday Button
  //
  edit_mon_button=new QCheckBox(this,"edit_mon_button");
  edit_mon_button->setGeometry(22,37,20,20);
  edit_mon_label=new QLabel(edit_mon_button,
		   tr("Monday"),this,"edit_mon_label");
  edit_mon_label->setGeometry(42,37,115,20);
  edit_mon_label->setFont(day_font);
  edit_mon_label->setAlignment(AlignLeft|AlignVCenter|ShowPrefix);

  //
  // Tuesday Button
  //
  edit_tue_button=new QCheckBox(this,"edit_tue_button");
  edit_tue_button->setGeometry(99,37,20,20);
  edit_tue_label=new QLabel(edit_tue_button,
		   tr("Tuesday"),this,"edit_tue_label");
  edit_tue_label->setGeometry(119,37,115,20);
  edit_tue_label->setFont(day_font);
  edit_tue_label->setAlignment(AlignLeft|AlignVCenter|ShowPrefix);

  //
  // Wednesday Button
  //
  edit_wed_button=new QCheckBox(this,"edit_wed_button");
  edit_wed_button->setGeometry(181,37,20,20);
  edit_wed_label=new QLabel(edit_wed_button,
		   tr("Wednesday"),this,"edit_wed_label");
  edit_wed_label->setGeometry(201,37,115,20);
  edit_wed_label->setFont(day_font);
  edit_wed_label->setAlignment(AlignLeft|AlignVCenter|ShowPrefix);

  //
  // Thursday Button
  //
  edit_thu_button=new QCheckBox(this,"edit_thu_button");
  edit_thu_button->setGeometry(283,37,20,20);
  edit_thu_label=new QLabel(edit_thu_button,
		   tr("Thursday"),this,"edit_thu_label");
  edit_thu_label->setGeometry(303,37,115,20);
  edit_thu_label->setFont(day_font);
  edit_thu_label->setAlignment(AlignLeft|AlignVCenter|ShowPrefix);

  //
  // Friday Button
  //
  edit_fri_button=new QCheckBox(this,"edit_fri_button");
  edit_fri_button->setGeometry(370,37,20,20);
  edit_fri_label=new QLabel(edit_fri_button,
		   tr("Friday"),this,"edit_fri_label");
  edit_fri_label->setGeometry(390,37,35,20);
  edit_fri_label->setFont(day_font);
  edit_fri_label->setAlignment(AlignLeft|AlignVCenter|ShowPrefix);

  //
  // Saturday Button
  //
  edit_sat_button=new QCheckBox(this,"edit_sat_button");
  edit_sat_button->setGeometry(142,57,20,20);
  edit_sat_label=new QLabel(edit_sat_button,
		   tr("Saturday"),this,"edit_sat_label");
  edit_sat_label->setGeometry(162,57,60,20);
  edit_sat_label->setFont(day_font);
  edit_sat_label->setAlignment(AlignLeft|AlignVCenter|ShowPrefix);

  //
  // Sunday Button
  //
  edit_sun_button=new QCheckBox(this,"edit_sun_button");
  edit_sun_button->setGeometry(246,57,20,20);
  edit_sun_label=new QLabel(edit_sun_button,
		   tr("Sunday"),this,"edit_sun_label");
  edit_sun_label->setGeometry(266,57,60,20);
  edit_sun_label->setFont(day_font);
  edit_sun_label->setAlignment(AlignLeft|AlignVCenter|ShowPrefix);

  //
  // Start Time
  //
  edit_starttime_edit=new QTimeEdit(this,"edit_starttime_edit");
  edit_starttime_edit->setGeometry(112,82,90,20);
  edit_starttime_label=new QLabel(edit_starttime_edit,tr("&Starting at:"),
		   this,"edit_starttime_label");
  edit_starttime_label->setGeometry(22,82,85,20);
  edit_starttime_label->setAlignment(AlignRight|AlignVCenter);
  edit_starttime_label->setFont(label_font);

  //
  // End Time
  //
  edit_endtime_edit=new QTimeEdit(this,"edit_endtime_edit");
  edit_endtime_edit->setGeometry(342,82,90,20);
  edit_endtime_label=new QLabel(edit_endtime_edit,tr("&Ending at:"),
		   this,"edit_endtime_label");
  edit_endtime_label->setGeometry(252,82,85,20);
  edit_endtime_label->setAlignment(AlignRight|AlignVCenter);
  edit_endtime_label->setFont(label_font);

  //
  //  OK Button
  //
  QPushButton *button=new QPushButton(this,"ok_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(label_font);
  button->setText("&OK");
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this,"cancel_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText("&Cancel");
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Data
  //
  sql="select PROGRAM_NAME from PROGRAMS order by PROGRAM_NAME";
  q=new QSqlQuery(sql);
  while(q->next()) {
    edit_program_box->insertItem(q->value(0).toString());
    if(feed->name()==q->value(0).toString()) {
      edit_program_box->setCurrentItem(edit_program_box->count()-1);
    }
  }
  delete q;
  edit_starttime_edit->setTime(feed->startTime());
  if(feed->endTime()==QTime(23,59,59)) {
    edit_endtime_edit->setTime(QTime(0,0,0));
  }
  else {
    edit_endtime_edit->setTime(feed->endTime());
  }
  edit_sun_button->setChecked(feed->dowActive(7));
  edit_mon_button->setChecked(feed->dowActive(1));
  edit_tue_button->setChecked(feed->dowActive(2));
  edit_wed_button->setChecked(feed->dowActive(3));
  edit_thu_button->setChecked(feed->dowActive(4));
  edit_fri_button->setChecked(feed->dowActive(5));
  edit_sat_button->setChecked(feed->dowActive(6));
}


EditAiring::~EditAiring()
{
}


QSize EditAiring::sizeHint() const
{
  return QSize(450,182);
} 


QSizePolicy EditAiring::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditAiring::okData()
{
  if((edit_starttime_edit->time()>=edit_endtime_edit->time())&&
     (edit_endtime_edit->time()!=QTime(0,0,0))) {
    QMessageBox::warning(this,tr("Invalid Times"),
			 tr("The program cannot end before it starts!"));
    return;
  }
  edit_feed->setName(edit_program_box->currentText());
  edit_feed->setStartTime(edit_starttime_edit->time());
  if(edit_endtime_edit->time()==QTime(0,0,0)) {
    edit_feed->setLength(edit_starttime_edit->time().
			 secsTo(QTime(23,59,59)));
  }
  else {
    edit_feed->setLength(edit_starttime_edit->time().
			 secsTo(edit_endtime_edit->time()));
  }
  edit_feed->setDowActive(7,edit_sun_button->isChecked());
  edit_feed->setDowActive(1,edit_mon_button->isChecked());
  edit_feed->setDowActive(2,edit_tue_button->isChecked());
  edit_feed->setDowActive(3,edit_wed_button->isChecked());
  edit_feed->setDowActive(4,edit_thu_button->isChecked());
  edit_feed->setDowActive(5,edit_fri_button->isChecked());
  edit_feed->setDowActive(6,edit_sat_button->isChecked());
  done(0);
}


void EditAiring::cancelData()
{
  done(-1);
}
