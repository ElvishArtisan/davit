// change_password.h
//
// Set Password Widget for Davit.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: change_password.h,v 1.1 2007/03/16 13:28:40 fredg Exp $
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

#ifndef CHANGE_PASSWORD_H
#define CHANGE_PASSWORD_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qpixmap.h>
#include <qradiobutton.h>


class ChangePassword : public QDialog
{
  Q_OBJECT
  public:
   ChangePassword(QString *password,QWidget *parent=0,const char *name=0);
   ~ChangePassword();
   QSize sizeHint() const;
   QSizePolicy sizePolicy() const;

  protected:

  private slots:
   void okData();
   void cancelData();

  private:
   QLineEdit *passwd_password_1_edit;
   QLineEdit *passwd_password_2_edit;
   QString *passwd_password;
};


#endif  // CHANGE_PASSWORD_H
