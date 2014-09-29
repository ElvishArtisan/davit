// add_provider.cpp
//
// Edit a Davit Provider.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: add_provider.cpp,v 1.2 2007/11/19 16:53:29 fredg Exp $
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

#include <add_provider.h>


AddProvider::AddProvider(QString *bname,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  add_business_name=bname;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Davit - Add Provider");

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
  add_business_name_edit=new QLineEdit(this,"add_business_name_edit");
  add_business_name_edit->setGeometry(110,10,sizeHint().width()-120,20);
  add_business_name_edit->setFont(font);
  add_business_name_edit->setMaxLength(64);
  QLabel *label=new QLabel(add_business_name_edit,"Business Name:",
			   this,"add_business_name_label");
  label->setGeometry(10,10,95,20);
  label->setAlignment(AlignRight|AlignVCenter);
  label->setFont(label_font);

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


AddProvider::~AddProvider()
{
}


QSize AddProvider::sizeHint() const
{
  return QSize(400,110);
} 


QSizePolicy AddProvider::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void AddProvider::okData()
{
  QString sql=
    QString().sprintf("select BUSINESS_NAME from PROVIDERS \
                       where BUSINESS_NAME=\"%s\"",
		      (const char *)add_business_name_edit->text());
  QSqlQuery *q=new QSqlQuery(sql);
  if(q->first()) {
    QMessageBox::warning(this,"Provider Exists",
			 "That provider already exists!");
    delete q;
    return;
  }
  delete q;
  *add_business_name=add_business_name_edit->text();
  done(0);
}


void AddProvider::cancelData()
{
  done(-1);
}
