// add_provider.h
//
// Add a Davit Provider.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: add_provider.h,v 1.1 2007/03/21 13:38:37 fredg Exp $
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

#ifndef ADD_PROVIDER_H
#define ADD_PROVIDER_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qcombobox.h>


class AddProvider : public QDialog
{
 Q_OBJECT
 public:
  AddProvider(QString *bname,QWidget *parent=0,const char *name=0);
  ~AddProvider();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void okData();
  void cancelData();

 private:
  QLineEdit *add_business_name_edit;
  QString *add_business_name;
};


#endif  // ADD_PROVIDER_H
