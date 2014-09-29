// add_affiliate.h
//
// Add a Davit Affiliate.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: add_affiliate.h,v 1.2 2008/01/29 16:55:33 fredg Exp $
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

#ifndef ADD_AFFILIATE_H
#define ADD_AFFILIATE_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qcombobox.h>


class AddAffiliate : public QDialog
{
 Q_OBJECT
 public:
  AddAffiliate(QString *call,QString *type,
	       QWidget *parent=0,const char *name=0);
  ~AddAffiliate();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void okData();
  void cancelData();

 private:
  QLineEdit *add_call_edit;
  QComboBox *add_type_box;
  QString *add_call;
  QString *add_type;
};


#endif  // ADD_AFFILIATE_H
