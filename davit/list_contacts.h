// list_contacts.h
//
// List Davit Contacts.
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

#ifndef LIST_CONTACTS_H
#define LIST_CONTACTS_H

#include <QListView>
#include <QPushButton>

#include <dvtdialog.h>
#include <dvttableview.h>

#include "contactlistmodel.h"
#include "edit_contact.h"

class ListContacts : public DvtDialog
{
 Q_OBJECT
 public:
  ListContacts(DvtConfig *c,QWidget *parent=0);
  ~ListContacts();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  void setAffiliateId(int id);
  void show();
  void hide();

 signals:
  void visibilityChanged(bool visible);

 private slots:
  void addData();
  void editData();
  void deleteData();
  void doubleClickedData(const QModelIndex &index);

 protected:
  void resizeEvent(QResizeEvent *e);
  void closeEvent(QCloseEvent *);
  
 private:
  void RefreshList();
  //  void UpdateItem(DvtListViewItem *item,Contact *contact);
  //  ContactListView *list_contacts_list;
  DvtTableView *list_contacts_view;
  ContactListModel *list_contacts_model;
  QPushButton *list_add_button;
  QPushButton *list_edit_button;
  QPushButton *list_delete_button;
  int list_id;
  EditContact *list_contact_dialog;
};


#endif  // LIST_CONTACTS_H
