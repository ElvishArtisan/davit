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
#include <QSqlDatabase>
#include <QSqlQuery>

#include "add_affiliate.h"


AddAffiliate::AddAffiliate(QString *call,QString *type,QWidget *parent)
  : QDialog(parent)
{
  setModal(true);
  add_call=call;
  add_type=type;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setWindowTitle("Davit - Add Affiliate");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // Station Call
  //
  add_call_edit=new QLineEdit(this);
  add_call_edit->setGeometry(110,10,80,20);
  add_call_edit->setFont(font);
  add_call_edit->setMaxLength(8);
  QLabel *label=new QLabel("Station Call:",this);
  label->setGeometry(10,10,95,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Station Type
  //
  add_type_box=new QComboBox(this);
  add_type_box->setGeometry(110,32,80,20);
  label=new QLabel("Type:",this);
  label->setGeometry(10,32,95,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  add_type_box->insertItem(0,"AM");
  add_type_box->insertItem(1,"FM");
  add_type_box->insertItem(2,"Internet");

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


void AddAffiliate::okData()
{
  QString sql=
    QString().sprintf("select STATION_CALL from AFFILIATES \
                       where (STATION_CALL=\"%s\")&&(STATION_TYPE=\"%s\")",
		      add_call_edit->text().toUtf8().constData(),
		      add_type_box->currentText().left(1).toUtf8().constData());
  QSqlQuery *q=new QSqlQuery(sql);
  if(q->first()) {
    QMessageBox::warning(this,"Affiliate Exists",
			 "That affiliate already exists!");
    delete q;
    return;
  }
  delete q;
  *add_call=add_call_edit->text();
  *add_type=add_type_box->currentText().left(1);
  done(0);
}


void AddAffiliate::cancelData()
{
  done(-1);
}
