// add_remark.cpp
//
// Add a Davit Remark.
//
//   (C) Copyright 2008-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <QMessageBox>

#include <dvtdb.h>

#include "add_remark.h"

AddRemark::AddRemark(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  setModal(true);
  add_remark=NULL;

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());

  setWindowTitle("Davit - "+tr("Add Remark"));

  //
  // Station Call
  //
  add_remark_edit=new QTextEdit(this);
  add_remark_edit->setAcceptRichText(false);

  //
  //  OK Button
  //
  add_ok_button=new QPushButton(this);
  add_ok_button->setDefault(true);
  add_ok_button->setFont(buttonFont());
  add_ok_button->setText("&OK");
  connect(add_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  add_cancel_button=new QPushButton(this);
  add_cancel_button->setFont(buttonFont());
  add_cancel_button->setText("&Cancel");
  connect(add_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


AddRemark::~AddRemark()
{
}


QSize AddRemark::sizeHint() const
{
  return QSize(400,300);
} 


QSizePolicy AddRemark::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int AddRemark::exec(QString *remark)
{
  add_remark_edit->setText(*remark);
  add_remark=remark;

  return QDialog::exec();
}


void AddRemark::resizeEvent(QResizeEvent *e)
{
  add_remark_edit->setGeometry(10,10,size().width()-20,size().height()-80);
  add_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  add_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void AddRemark::okData()
{
  *add_remark=add_remark_edit->toPlainText().trimmed();

  done(true);
}


void AddRemark::cancelData()
{
  done(false);
}
