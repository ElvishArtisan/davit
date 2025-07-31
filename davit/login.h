// login.h
//
// Login Widget for Davit.
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

#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPixmap>
#include <QPushButton>
#include <QRadioButton>


class Login : public QDialog
{
 Q_OBJECT
 public:
 Login(QWidget *parent=0);
  ~Login();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(QString *username,QString *password);

 private slots:
  void okData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);
  void closeEvent(QCloseEvent *e);

 private:
  QString *login_name;
  QLabel *login_name_label;
  QLineEdit *login_name_edit;
  QString *login_password;
  QLabel *login_password_label;
  QLineEdit *login_password_edit;
  QPushButton *login_ok_button;
  QPushButton *login_cancel_button;
};


#endif  // LOGIN_H
