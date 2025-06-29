// list_airings.h
//
// List Davit Airings.
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

#ifndef LIST_AIRINGS_H
#define LIST_AIRINGS_H

#include <QListView>
#include <QPushButton>

#include <dvtdialog.h>
#include <dvttableview.h>

#include "airinglistmodel.h"
#include "edit_airing.h"

class ListAirings : public DvtDialog
{
 Q_OBJECT
 public:
  ListAirings(DvtConfig *c,QWidget *parent=0);
  ~ListAirings();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  void setAffiliateId(int id);
  void show();
  void hide();

 signals:
  void remarkAdded();
  void visibilityChanged(bool visible);

 private slots:
  void addData();
  void editData();
  void deleteData();
  void doubleClickedData(const QModelIndex &index);
  void selectionChangedData(const QItemSelection &now,
			    const QItemSelection &prev);

 protected:
  void resizeEvent(QResizeEvent *e);
  void closeEvent(QCloseEvent *);

 private:
  AiringListModel *list_airings_model;
  DvtTableView *list_airings_view;
  QPushButton *list_add_button;
  QPushButton *list_edit_button;
  QPushButton *list_delete_button;
  int list_affiliate_id;
  EditAiring *list_editairing_dialog;
};


#endif  // LIST_AIRINGS_H
