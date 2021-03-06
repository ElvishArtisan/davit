// add_affiliate.cpp
//
// Edit a Davit Affiliate.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: add_affiliate.cpp,v 1.3 2008/01/29 16:55:33 fredg Exp $
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
#include <qlabel.h>
#include <qsqldatabase.h>
#include <qmessagebox.h>

#include <math.h>

#include <add_affiliate.h>


AddAffiliate::AddAffiliate(QString *call,QString *type,
			   QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  add_call=call;
  add_type=type;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Davit - Add Affiliate");

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
  add_call_edit=new QLineEdit(this,"add_call_edit");
  add_call_edit->setGeometry(110,10,80,20);
  add_call_edit->setFont(font);
  add_call_edit->setMaxLength(8);
  QLabel *label=
    new QLabel(add_call_edit,"Station Call:",this,"add_call_label");
  label->setGeometry(10,10,95,20);
  label->setAlignment(AlignRight|AlignVCenter);
  label->setFont(label_font);

  //
  // Station Type
  //
  add_type_box=new QComboBox(this,"add_type_edit");
  add_type_box->setGeometry(110,32,80,20);
  label=new QLabel(add_type_box,"Type:",this,"add_type_label");
  label->setGeometry(10,32,95,20);
  label->setAlignment(AlignRight|AlignVCenter);
  label->setFont(label_font);
  add_type_box->insertItem("AM");
  add_type_box->insertItem("FM");
  add_type_box->insertItem("Internet");

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
		      (const char *)add_call_edit->text(),
		      (const char *)add_type_box->currentText().left(1));
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
