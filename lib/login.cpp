// login.cpp
//
// Login widget for Davit.
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <QDialog>
#include <QString>
#include <QRadioButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QPainter>
#include <QEvent>
#include <QMessageBox>
#include <QButtonGroup>

#include "login.h"

Login::Login(QWidget *parent)
  : QDialog(parent)
{
  setModal(true);

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMaximumHeight(sizeHint().height());

  setWindowTitle("Davit - "+tr("Login"));

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);

  //
  // Login Name
  //
  login_name_edit=new QLineEdit(this);
  login_name_edit->setMaxLength(16);
  login_name_edit->setFocus();
  login_name_label=new QLabel(tr("User Name")+":",this);
  login_name_label->setFont(font);
  login_name_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // Login Password
  //
  login_password_edit=new QLineEdit(this);
  login_password_edit->setMaxLength(16);
  login_password_edit->setEchoMode(QLineEdit::Password);
  login_password_label=new QLabel(tr("Password")+":",this);
  login_password_label->setFont(font);
  login_password_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // OK Button
  //
  login_ok_button=new QPushButton(this);
  login_ok_button->setFont(font);
  login_ok_button->setText(tr("OK"));
  login_ok_button->setDefault(true);
  connect(login_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  login_cancel_button=new QPushButton(this);
  login_cancel_button->setFont(font);
  login_cancel_button->setText(tr("Cancel"));
  connect(login_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


Login::~Login()
{
  delete login_name_edit;
  delete login_password_edit;
}


QSize Login::sizeHint() const
{
  return QSize(230,125);
} 


QSizePolicy Login::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int Login::exec(QString *username,QString *password)
{
  login_name=username;
  login_name_edit->setText(*username);
  login_password=password;
  login_password_edit->setText(*password);

  return QDialog::exec();
}


void Login::okData()
{
  *login_name=login_name_edit->text();
  *login_password=login_password_edit->text();
  done(true);
}


void Login::cancelData()
{
  done(false);
}


void Login::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  login_name_edit->setGeometry(100,10,w-110,20);
  login_name_label->setGeometry(10,10,85,20);

  login_password_edit->setGeometry(100,31,w-110,19);
  login_password_label->setGeometry(10,31,85,19);

  login_ok_button->setGeometry(w-190,h-60,85,50);
  login_cancel_button->setGeometry(w-95,h-60,85,50);
}


void Login::closeEvent(QCloseEvent *e)
{
  cancelData();
}
