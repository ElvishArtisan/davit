// edit_airing.cpp
//
// Edit a Davit Airing.
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
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>

#include <dvtconf.h>
#include <dvtdb.h>

#include "edit_airing.h"

EditAiring::EditAiring(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  edit_airing_id=0;

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());

  setWindowTitle("Davit - Edit Show Time");

  //
  // Program Name
  //
  edit_program_box=new QComboBox(this);
  edit_program_label=new QLabel(tr("Program:"),this);
  edit_program_label->setFont(labelFont());
  edit_program_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  connect(edit_program_box,SIGNAL(currentIndexChanged(int)),
	  this,SLOT(programIndexChangedData(int)));
  
  //
  // Monday Button
  //
  edit_mon_button=new QCheckBox(this);
  edit_mon_label=new QLabel(tr("Monday"),this);
  edit_mon_label->setFont(subLabelFont());
  edit_mon_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Tuesday Button
  //
  edit_tue_button=new QCheckBox(this);
  edit_tue_label=new QLabel(tr("Tuesday"),this);
  edit_tue_label->setFont(subLabelFont());
  edit_tue_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Wednesday Button
  //
  edit_wed_button=new QCheckBox(this);
  edit_wed_label=new QLabel(tr("Wednesday"),this);
  edit_wed_label->setFont(subLabelFont());
  edit_wed_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Thursday Button
  //
  edit_thu_button=new QCheckBox(this);
  edit_thu_label=new QLabel(tr("Thursday"),this);
  edit_thu_label->setFont(subLabelFont());
  edit_thu_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Friday Button
  //
  edit_fri_button=new QCheckBox(this);
  edit_fri_label=new QLabel(tr("Friday"),this);
  edit_fri_label->setFont(subLabelFont());
  edit_fri_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Saturday Button
  //
  edit_sat_button=new QCheckBox(this);
  edit_sat_label=new QLabel(tr("Saturday"),this);
  edit_sat_label->setFont(subLabelFont());
  edit_sat_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Sunday Button
  //
  edit_sun_button=new QCheckBox(this);
  edit_sun_label=new QLabel(tr("Sunday"),this);
  edit_sun_label->setFont(subLabelFont());
  edit_sun_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Start Time
  //
  edit_starttime_edit=new QTimeEdit(this);
  edit_starttime_edit->setDisplayFormat("hh:mm:ss");
  edit_starttime_label=new QLabel(tr("Starting at:"),this);
  edit_starttime_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_starttime_label->setFont(labelFont());

  //
  // End Time
  //
  edit_endtime_edit=new QTimeEdit(this);
  edit_endtime_edit->setDisplayFormat("hh:mm:ss");
  edit_endtime_label=new QLabel(tr("Ending at:"),this);
  edit_endtime_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_endtime_label->setFont(labelFont());

  //
  //  OK Button
  //
  edit_ok_button=new QPushButton(this);
  edit_ok_button->setDefault(true);
  edit_ok_button->setFont(buttonFont());
  edit_ok_button->setText("&OK");
  connect(edit_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  edit_cancel_button=new QPushButton(this);
  edit_cancel_button->setFont(buttonFont());
  edit_cancel_button->setText("&Cancel");
  connect(edit_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
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


int EditAiring::exec(int airing_id,bool new_entry)
{
  edit_airing_id=airing_id;

  QString sql=QString("select ")+
    "`ID`,"+            // 00
    "`PROGRAM_NAME` "+  // 01
    "from `PROGRAMS` order by `PROGRAM_NAME`";
  int count=0;
  edit_program_box->clear();

  DvtSqlQuery *q=new DvtSqlQuery(sql);
  while(q->next()) {
    edit_program_box->
      insertItem(count++,q->value(1).toString(),q->value(0).toInt());
  }
  delete q;
  edit_program_box->insertItem(0,tr("[select program]"),0);
  edit_program_box->removeItem(1);

  sql=QString("select ")+
    "`AIRINGS`.`PROGRAM_ID`,"+    // 00
    "`AIRINGS`.`AFFILIATE_ID`,"+  // 01
    "`AIRINGS`.`AIR_MON`,"+       // 02
    "`AIRINGS`.`AIR_TUE`,"+       // 03
    "`AIRINGS`.`AIR_WED`,"+       // 04
    "`AIRINGS`.`AIR_THU`,"+       // 05
    "`AIRINGS`.`AIR_FRI`,"+       // 06
    "`AIRINGS`.`AIR_SAT`,"+       // 07
    "`AIRINGS`.`AIR_SUN`,"+       // 08
    "`AIRINGS`.`AIR_TIME`,"+      // 09
    "`AIRINGS`.`AIR_LENGTH`,"+    // 10
    "`PROGRAMS`.`PROGRAM_NAME` "+ // 11
    "from `AIRINGS` left join `PROGRAMS` "+
    "on `AIRINGS`.`PROGRAM_ID`=`PROGRAMS`.`ID` where "+
    QString::asprintf("`AIRINGS`.`ID`=%d ",airing_id);
  q=new DvtSqlQuery(sql);
  if(q->first()) {
    for(int i=0;i<edit_program_box->count();i++) {
      if(edit_program_box->itemData(i).toInt()==q->value(0).toInt()) {
	edit_program_box->setCurrentIndex(i);
      }
    }
    edit_mon_button->setChecked(q->value(2).toString()=="Y");
    edit_tue_button->setChecked(q->value(3).toString()=="Y");
    edit_wed_button->setChecked(q->value(4).toString()=="Y");
    edit_thu_button->setChecked(q->value(5).toString()=="Y");
    edit_fri_button->setChecked(q->value(6).toString()=="Y");
    edit_sat_button->setChecked(q->value(7).toString()=="Y");
    edit_sun_button->setChecked(q->value(8).toString()=="Y");
    edit_starttime_edit->setTime(q->value(9).toTime());
    edit_endtime_edit->
      setTime(q->value(9).toTime().addSecs(q->value(10).toInt()));
    delete q;
  }
  programIndexChangedData(edit_program_box->currentIndex());

  return QDialog::exec();
}


void EditAiring::programIndexChangedData(int index)
{
  edit_ok_button->setDisabled(index==0);
}


void EditAiring::okData()
{
  if((edit_starttime_edit->time()>=edit_endtime_edit->time())&&
     (edit_endtime_edit->time()!=QTime(0,0,0))) {
    QMessageBox::warning(this,tr("Invalid Times"),
			 tr("The program cannot end before it starts!"));
    return;
  }

  QString sql=QString("update `AIRINGS` set ")+
    QString::asprintf("`PROGRAM_ID`=%d,",
		      edit_program_box->itemData(edit_program_box->currentIndex()).toInt())+
    "`AIR_TIME`="+DvtSqlQuery::escape(edit_starttime_edit->time().toString("hh:mm:ss"))+","+
    QString::asprintf("`AIR_LENGTH`=%d,",edit_starttime_edit->time().secsTo(edit_endtime_edit->time()))+
    "`AIR_MON`="+
    DvtSqlQuery::escape(DvtYesNo(edit_mon_button->isChecked()))+","+
    "`AIR_TUE`="+
    DvtSqlQuery::escape(DvtYesNo(edit_tue_button->isChecked()))+","+
    "`AIR_WED`="+
    DvtSqlQuery::escape(DvtYesNo(edit_wed_button->isChecked()))+","+
    "`AIR_THU`="+
    DvtSqlQuery::escape(DvtYesNo(edit_thu_button->isChecked()))+","+
    "`AIR_FRI`="+
    DvtSqlQuery::escape(DvtYesNo(edit_fri_button->isChecked()))+","+
    "`AIR_SAT`="+
    DvtSqlQuery::escape(DvtYesNo(edit_sat_button->isChecked()))+","+
    "`AIR_SUN`="+
    DvtSqlQuery::escape(DvtYesNo(edit_sun_button->isChecked()))+" "+
    QString::asprintf("where `ID`=%d ",edit_airing_id);
  DvtSqlQuery::apply(sql);

  done(true);
}


void EditAiring::cancelData()
{
  done(false);
}


void EditAiring::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  edit_program_box->setGeometry(85,10,w-95,20);
  edit_program_label->setGeometry(10,10,70,20);

  edit_mon_button->setGeometry(22,37,20,20);
  edit_mon_label->setGeometry(42,37,115,20);

  edit_tue_button->setGeometry(99,37,20,20);
  edit_tue_label->setGeometry(119,37,115,20);

  edit_wed_button->setGeometry(181,37,20,20);
  edit_wed_label->setGeometry(201,37,115,20);

  edit_thu_button->setGeometry(283,37,20,20);
  edit_thu_label->setGeometry(303,37,115,20);

  edit_fri_button->setGeometry(370,37,20,20);
  edit_fri_label->setGeometry(390,37,35,20);

  edit_sat_button->setGeometry(142,57,20,20);
  edit_sat_label->setGeometry(162,57,60,20);

  edit_sun_button->setGeometry(246,57,20,20);
  edit_sun_label->setGeometry(266,57,60,20);

  edit_starttime_edit->setGeometry(112,82,90,20);
  edit_starttime_label->setGeometry(22,82,85,20);

  edit_endtime_edit->setGeometry(342,82,90,20);
  edit_endtime_label->setGeometry(252,82,85,20);

  edit_ok_button->setGeometry(w-180,h-60,80,50);
  edit_cancel_button->setGeometry(w-90,h-60,80,50);
}


void EditAiring::closeEvent(QCloseEvent *e)
{
  cancelData();
}
