// list_networks.h
//
// List Davit Networks.
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

#ifndef LIST_NETWORKS_H
#define LIST_NETWORKS_H

#include <QPushButton>

#include "dialog.h"
#include "edit_network.h"
#include "networklistmodel.h"
#include "tableview.h"

class ListNetworks : public Dialog
{
 Q_OBJECT
 public:
  ListNetworks(DvtConfig *c,QWidget *parent=0);
  ~ListNetworks();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void addData();
  void editData();
  void deleteData();
  void doubleClickedData(const QModelIndex &index);
  void closeData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  TableView *list_networks_view;
  NetworkListModel *list_networks_model;
  QPushButton *list_add_button;
  QPushButton *list_edit_button;
  QPushButton *list_delete_button;
  QPushButton *list_close_button;
  EditNetwork *list_editnetwork_dialog;
};


#endif  // LIST_NETWORKS_H
