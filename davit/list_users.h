// list_users.h
//
// List Davit Users.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: list_users.h,v 1.1 2007/03/16 13:28:33 fredg Exp $
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

#ifndef LIST_USERS_H
#define LIST_USERS_H

#include <qdialog.h>
#include <qlistview.h>
#include <qpushbutton.h>


class ListUsers : public QDialog
{
 Q_OBJECT
 public:
  ListUsers(QWidget *parent=0,const char *name=0);
  ~ListUsers();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void addData();
  void editData();
  void deleteData();
  void doubleClickedData(QListViewItem *item,const QPoint &pt,int c);
  void closeData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  void DeleteUser (QString loginname);
  void RefreshList();
  void UpdateItem(QListViewItem *item);
  QListView *list_users_list;
  QPushButton *list_add_button;
  QPushButton *list_edit_button;
  QPushButton *list_delete_button;
  QPushButton *list_close_button;
};


#endif  // LIST_USERS_H
