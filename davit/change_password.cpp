// change_password.cpp
//
// Set Password widget for Davit.
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

#include <QLabel>
#include <QMessageBox>

#include "change_password.h"

ChangePassword::ChangePassword(DvtConfig *c,QWidget *parent)
  : DvtDialog(c,parent)
{
  setModal(true);
  setWindowTitle("Davit - "+tr("Change Password"));

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMaximumHeight(sizeHint().height());

  //
  // Password
  //
  passwd_password_1_edit=new QLineEdit(this);
  passwd_password_1_edit->setMaxLength(16);
  passwd_password_1_edit->setEchoMode(QLineEdit::Password);
  passwd_password_1_edit->setFocus();
  connect(passwd_password_1_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(textChangedData(const QString &)));
  passwd_password_1_label=new QLabel("Password:",this);
  passwd_password_1_label->setAlignment(Qt::AlignRight);
  passwd_password_1_label->setFont(labelFont());

  //
  // Confirm Password
  //
  passwd_password_2_edit=new QLineEdit(this);
  passwd_password_2_edit->setMaxLength(16);
  passwd_password_2_edit->setEchoMode(QLineEdit::Password);
  connect(passwd_password_2_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(textChangedData(const QString &)));
  passwd_password_2_label=new QLabel("Confirm:",this);
  passwd_password_2_label->setGeometry(10,34,75,19);
  passwd_password_2_label->setAlignment(Qt::AlignRight);
  passwd_password_2_label->setFont(labelFont());

  //
  // OK Button
  //
  passwd_ok_button=new QPushButton(this);
  passwd_ok_button->setFont(buttonFont());
  passwd_ok_button->setText("OK");
  passwd_ok_button->setDefault(true);
  connect(passwd_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  passwd_cancel_button=new QPushButton(this);
  passwd_cancel_button->setFont(buttonFont());
  passwd_cancel_button->setText("Cancel");
  connect(passwd_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));

}


ChangePassword::~ChangePassword()
{
  delete passwd_password_1_edit;
  delete passwd_password_2_edit;
}


QSize ChangePassword::sizeHint() const
{
  return QSize(250,125);
} 


QSizePolicy ChangePassword::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int ChangePassword::exec(QString *passwd)
{
  passwd_password=passwd;

  passwd_password_1_edit->setText(*passwd);
  passwd_password_2_edit->setText(*passwd);
  passwd_ok_button->setEnabled(true);

  return QDialog::exec();
}


void ChangePassword::textChangedData(const QString &text)
{
  passwd_ok_button->
    setEnabled(passwd_password_1_edit->text()==passwd_password_2_edit->text());
}


void ChangePassword::okData()
{
  if(passwd_password_1_edit->text()==passwd_password_2_edit->text()) {
    *passwd_password=passwd_password_1_edit->text();
    done(true);
  }
  else {
    QMessageBox::warning(this,"Davit - "+tr("Password Mismatch"),
			 tr("The passwords don't match, please try again!"));
  }
}


void ChangePassword::cancelData()
{
  done(false);
}


void ChangePassword::resizeEvent(QResizeEvent *e)
{
  int w=size().width();

  passwd_password_1_label->setGeometry(10,13,75,19);
  passwd_password_1_edit->setGeometry(90,11,w-105,19);
  passwd_password_2_label->setGeometry(10,32,75,19);
  passwd_password_2_edit->setGeometry(90,32,w-105,19);
  passwd_ok_button->setGeometry(w-220,60,100,55);
  passwd_cancel_button->setGeometry(w-110,60,100,55);
}

