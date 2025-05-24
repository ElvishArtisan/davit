// add_program.cpp
//
// Add a Davit Program.
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

#include "add_program.h"


AddProgram::AddProgram(QString *bname,QString *pname,QWidget *parent)
  : QDialog(parent)
{
  setModal(true);
  QString sql;
  QSqlQuery *q;

  add_business_name=bname;
  add_program_name=pname;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setWindowTitle("Davit - Add Program");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // Business Name
  //
  add_business_name_box=new QComboBox(this);
  add_business_name_box->setGeometry(110,10,sizeHint().width()-120,20);
  add_business_name_box->setFont(font);
  sql="select BUSINESS_NAME from PROVIDERS order by BUSINESS_NAME";
  q=new QSqlQuery(sql);
  int count=0;
  while(q->next()) {
    add_business_name_box->insertItem(count++,q->value(0).toString());
    if(*bname==q->value(0).toString()) {
      add_business_name_box->setCurrentIndex(add_business_name_box->count()-1);
    }
  }
  delete q;
  QLabel *label=new QLabel("Provider:",this);
  label->setGeometry(10,10,95,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Program Name
  //
  add_program_name_edit=new QLineEdit(this);
  add_program_name_edit->setGeometry(110,32,sizeHint().width()-120,20);
  add_program_name_edit->setFont(font);
  add_program_name_edit->setMaxLength(64);
  label=new QLabel("Program Name:",this);
  label->setGeometry(10,32,95,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

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


AddProgram::~AddProgram()
{
}


QSize AddProgram::sizeHint() const
{
  return QSize(400,142);
} 


QSizePolicy AddProgram::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void AddProgram::okData()
{
  QString sql=
    QString().sprintf("select PROGRAM_NAME from PROGRAMS \
                       where PROGRAM_NAME=\"%s\"",
		      add_program_name_edit->text().toUtf8().constData());
  QSqlQuery *q=new QSqlQuery(sql);
  if(q->first()) {
    QMessageBox::warning(this,"Program Exists",
			 "That program already exists!");
    delete q;
    return;
  }
  delete q;
  *add_business_name=add_business_name_box->currentText();
  *add_program_name=add_program_name_edit->text();
  done(0);
}


void AddProgram::cancelData()
{
  done(-1);
}
