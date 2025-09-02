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

#include "contactlistmodel.h"
#include "dialog.h"
#include "edit_contact.h"
#include "tableview.h"

class ListContacts : public Dialog
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
  void selectionChangedData(const QItemSelection &now,
			    const QItemSelection &prev);

 protected:
  void resizeEvent(QResizeEvent *e);
  void closeEvent(QCloseEvent *);
  
 private:
  TableView *list_contacts_view;
  ContactListModel *list_contacts_model;
  QPushButton *list_add_button;
  QPushButton *list_edit_button;
  QPushButton *list_delete_button;
  int list_id;
  EditContact *list_editcontact_dialog;
};


#endif  // LIST_CONTACTS_H
