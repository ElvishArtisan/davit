// add_user.cpp
//
// Add a Davit User.
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

#include <QMessageBox>

#include <dvtdb.h>

#include "add_user.h"

AddUser::AddUser(DvtConfig *c,QWidget *parent)
  : DvtDialog(c,parent)
{
  add_loginname=NULL;

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());

  setWindowTitle("Davit - "+tr("Add User"));

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
  add_loginname_edit=new QLineEdit(this);
  add_loginname_edit->setGeometry(110,10,80,20);
  add_loginname_edit->setFont(font);
  add_loginname_edit->setMaxLength(8);
  add_loginname_label=new QLabel(tr("Login Name")+":",this);
  add_loginname_label->setGeometry(10,10,95,20);
  add_loginname_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  add_loginname_label->setFont(labelFont());

  //
  //  OK Button
  //
  add_ok_button=new QPushButton(this);
  add_ok_button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  add_ok_button->setDefault(true);
  add_ok_button->setFont(label_font);
  add_ok_button->setText(tr("OK"));
  connect(add_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  add_cancel_button=new QPushButton(this);
  add_cancel_button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  add_cancel_button->setFont(label_font);
  add_cancel_button->setText("&Cancel");
  connect(add_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
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


int AddUser::exec(QString *username)
{
  add_loginname=username;
  add_loginname_edit->setText(*username);
  add_loginname_edit->selectAll();

  return QDialog::exec();
}


void AddUser::okData()
{
  QString sql=
    QString::asprintf("select USER_NAME from USERS where USER_NAME=\"%s\"",
		      add_loginname_edit->text().toUtf8().constData());
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  if(q->first()) {
    QMessageBox::warning(this,"User Exists",
			 "That user already exists!");
    delete q;
    return;
  }
  delete q;
  *add_loginname=add_loginname_edit->text();
  done(true);
}


void AddUser::cancelData()
{
  done(false);
}
