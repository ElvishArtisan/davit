// add_affiliate.cpp
//
// Edit a Davit Affiliate.
//
//   (C) Copyright 2007-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <math.h>

#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

#include <dvtdb.h>

#include "add_affiliate.h"


AddAffiliate::AddAffiliate(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  setModal(true);

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());

  //
  // Station Call
  //
  add_call_edit=new QLineEdit(this);
  add_call_edit->setMaxLength(10);
  add_call_label=new QLabel(tr("Station Call")+":",this);
  add_call_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  add_call_label->setFont(labelFont());

  //
  // Station Type
  //
  add_type_box=new QComboBox(this);
  add_type_label=new QLabel(tr("Type")+":",this);
  add_type_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  add_type_label->setFont(labelFont());
  add_type_box->insertItem(0,"AM");
  add_type_box->insertItem(1,"FM");
  add_type_box->insertItem(2,"Internet");

  //
  //  OK Button
  //
  add_ok_button=new QPushButton(this);
  add_ok_button->setDefault(true);
  add_ok_button->setFont(buttonFont());
  add_ok_button->setText("OK");
  connect(add_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  add_cancel_button=new QPushButton(this);
  add_cancel_button->setFont(buttonFont());
  add_cancel_button->setText("Cancel");
  connect(add_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


AddAffiliate::~AddAffiliate()
{
}


QSize AddAffiliate::sizeHint() const
{
  return QSize(200,132);
} 


QSizePolicy AddAffiliate::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int AddAffiliate::exec(QString *station_call,QString *station_type)
{
  add_call=station_call;
  add_type=station_type;

  setWindowTitle("Davit - "+tr("Add Affiliate"));

  add_call_edit->setText(tr("[new call]"));
  add_call_edit->selectAll();
  
  return QDialog::exec();
}


void AddAffiliate::okData()
{
  QString sql=
    QString("select ")+
    "`STATION_CALL` "+  // 00
    "from `AFFILIATES` where "+
    "(`STATION_CALL`="+DvtSqlQuery::escape(add_call_edit->text())+")&&"+
    "(`STATION_TYPE`="+
    DvtSqlQuery::escape(add_type_box->currentText().left(1))+")";
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  if(q->first()) {
    QMessageBox::warning(this,"Davit - "+tr("Affiliate Exists"),
			 tr("That affiliate already exists!"));
    delete q;
    return;
  }
  delete q;
  *add_call=add_call_edit->text();
  *add_type=add_type_box->currentText().left(1);
  done(true);
}


void AddAffiliate::cancelData()
{
  done(false);
}


void AddAffiliate::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  add_call_edit->setGeometry(110,10,80,20);
  add_call_label->setGeometry(10,10,95,20);
  add_type_box->setGeometry(110,32,80,20);
  add_type_label->setGeometry(10,32,95,20);
  add_ok_button->setGeometry(w-180,h-60,80,50);
  add_cancel_button->setGeometry(w-90,h-60,80,50);
}


void AddAffiliate::closeEvent(QCloseEvent *e)
{
  cancelData();
}
