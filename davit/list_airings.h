// list_airings.h
//
// List Davit Airings.
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: list_airings.h,v 1.1 2011/01/29 00:32:34 pcvs Exp $
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

#include <qdialog.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlistview.h>

#include <dvtlistviewitem.h>
#include <dvtfeed.h>

#include <edit_contact.h>

class ListAirings : public QDialog
{
 Q_OBJECT
 public:
  ListAirings(int id,QWidget *parent=0,const char *name=0);
  ~ListAirings();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  void show();
  void hide();

 signals:
  void remarkAdded();
  void visibilityChanged(bool visible);

 private slots:
  void addData();
  void editData();
  void deleteData();
  void doubleClickedData(QListViewItem *item,const QPoint &pt,int c);

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  void RefreshList();
  void RefreshItem(DvtListViewItem *item,DvtFeed *airing);
  QListView *list_airings_list;
  QPushButton *list_add_button;
  QPushButton *list_edit_button;
  QPushButton *list_delete_button;
  int list_id;
  EditContact *list_contact_dialog;
};


#endif  // LIST_AIRINGS_H
