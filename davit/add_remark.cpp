// add_remark.cpp
//
// Edit a Davit Remark.
//
//   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: add_remark.cpp,v 1.1 2008/12/13 00:44:27 fredg Exp $
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

#include <qlabel.h>
#include <qsqldatabase.h>
#include <qmessagebox.h>

#include <math.h>

#include <add_remark.h>


AddRemark::AddRemark(QString *remark,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  add_remark=remark;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setCaption("Davit - Add Remark");

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
  add_remark_edit=new QTextEdit(this,"add_remark_edit");
  add_remark_edit->setFont(font);
  add_remark_edit->setTextFormat(Qt::PlainText);

  //
  //  OK Button
  //
  add_ok_button=new QPushButton(this,"add_ok_button");
  add_ok_button->setDefault(true);
  add_ok_button->setFont(label_font);
  add_ok_button->setText("&OK");
  connect(add_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  add_cancel_button=new QPushButton(this,"add_cancel_button");
  add_cancel_button->setFont(label_font);
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


void AddRemark::resizeEvent(QResizeEvent *e)
{
  add_remark_edit->setGeometry(10,10,size().width()-20,size().height()-80);
  add_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  add_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void AddRemark::okData()
{
  *add_remark=add_remark_edit->text().simplifyWhiteSpace().stripWhiteSpace();
  done(0);
}


void AddRemark::cancelData()
{
  done(-1);
}
