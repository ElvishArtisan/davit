// showaffidavits.h
//
// Show an affiliates affidavit status.
//
//   (C) Copyright 2010-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef SHOWAFFIDAVITS_H
#define SHOWAFFIDAVITS_H

#include <QListView>
#include <QPushButton>
#include <QWidget>

#include <dvtlistviewitem.h>

#include "edit_contact.h"
#include "maildialog.h"

class ShowAffidavits : public QWidget
{
 Q_OBJECT
 public:
  ShowAffidavits(int id,QWidget *parent);
  ~ShowAffidavits();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  void setAffiliateStatus(bool state);

 signals:
  void remarksUpdated();

 private slots:
  void mailClickedData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  QPushButton *show_mail_button;
  QListView *show_list;
  int show_id;
};


#endif  // SHOWAFFIDAVITS_H
