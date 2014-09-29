// add_user.cpp
//
// Edit a Davit User.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: add_user.cpp,v 1.2 2007/11/19 16:53:29 fredg Exp $
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

#include <add_user.h>


AddUser::AddUser(QString *loginname,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  add_loginname=loginname;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Davit - Add User");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // Login Name
  //
  add_loginname_edit=new QLineEdit(this,"add_loginname_edit");
  add_loginname_edit->setGeometry(110,10,80,20);
  add_loginname_edit->setFont(font);
  add_loginname_edit->setMaxLength(8);
  QLabel *label=
    new QLabel(add_loginname_edit,"Login Name:",this,"add_loginname_label");
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


AddUser::~AddUser()
{
}


QSize AddUser::sizeHint() const
{
  return QSize(200,110);
} 


QSizePolicy AddUser::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void AddUser::okData()
{
  QString sql=
    QString().sprintf("select USER_NAME from USERS where USER_NAME=\"%s\"",
		      (const char *)add_loginname_edit->text());
  QSqlQuery *q=new QSqlQuery(sql);
  if(q->first()) {
    QMessageBox::warning(this,"User Exists",
			 "That user already exists!");
    delete q;
    return;
  }
  delete q;
  *add_loginname=add_loginname_edit->text();
  done(0);
}


void AddUser::cancelData()
{
  done(-1);
}
