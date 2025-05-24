// list_programs.h
//
// List Davit Programs.
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

#ifndef LIST_PROGRAMS_H
#define LIST_PROGRAMS_H

#include <QDialog>
#include <QListView>
#include <QPushButton>

#include <dvtlistviewitem.h>

class ListPrograms : public QDialog
{
 Q_OBJECT
 public:
  ListPrograms(int provider_id=-1,QWidget *parent=0);
  ~ListPrograms();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void addData();
  void editData();
  void deleteData();
  void affadavitData();
  //  void doubleClickedData(QListViewItem *item,const QPoint &pt,int c);
  void closeData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  void DeleteProgram(int pid);
  void RefreshList();
  void UpdateItem(DvtListViewItem *item);
  //  QListView *list_programs_list;
  QPushButton *list_add_button;
  QPushButton *list_edit_button;
  QPushButton *list_delete_button;
  QPushButton *list_affadavit_button;
  QPushButton *list_close_button;
  int list_provider_id;
};


#endif  // LIST_PROGRAMS_H
