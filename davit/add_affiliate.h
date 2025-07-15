// add_affiliate.h
//
// Add a Davit Affiliate.
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

#ifndef ADD_AFFILIATE_H
#define ADD_AFFILIATE_H

#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <dvtdialog.h>

class AddAffiliate : public DvtDialog
{
 Q_OBJECT
 public:
  AddAffiliate(DvtConfig *c,QWidget *parent=0);
  ~AddAffiliate();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(QString *station_call,QString *station_type);

 private slots:
  void okData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);
  void closeEvent(QCloseEvent *e);

 private:
  QLabel *add_call_label;
  QLineEdit *add_call_edit;
  QLabel *add_type_label;
  QComboBox *add_type_box;
  QPushButton *add_ok_button;
  QPushButton *add_cancel_button;
  QString *add_call;
  QString *add_type;
};


#endif  // ADD_AFFILIATE_H
