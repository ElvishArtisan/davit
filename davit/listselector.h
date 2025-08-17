//   listselector.h
//
//   An listselector widget with word wrap.
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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

#ifndef LISTSELECTOR_H
#define LISTSELECTOR_H

#include <QLabel>
#include <QListWidget>
#include <QPushButton>

#include "widget.h"

class ListSelectorItem : public QListWidgetItem
{
 public:
  ListSelectorItem(QListWidget *owner,int airing_id,const QString &text);
  int airingId() const;

 private:
  int d_airing_id;
};




class ListSelector : public Widget
{
  Q_OBJECT
 public:
  ListSelector(DvtConfig *c,QWidget *parent=0);
  uint sourceCount() const;
  uint destCount() const;
  void sourceSetLabel(QString label);
  void destSetLabel(QString label);
  void sourceInsertItem(int id,const QString &text);
  void destInsertItem(int id,const QString &text);
  void sourceRemoveItem(int index);
  void destRemoveItem(int index);
  QString sourceText(int index) const;
  int sourceAiringId(int index) const;
  QString destText(int index) const;
  int destAiringId(int index) const;
  int sourceCurrentItem() const;
  int destCurrentItem() const;
  QString sourceCurrentText() const;
  QString destCurrentText() const;
  void sourceSetCurrentItem(int item);
  void destSetCurrentItem(int item);
  void clear();

 signals:
  void destItemsQuantityChanged(int quan);

 private slots:
  void addData();
  void addAllData();
  void removeData();
  void removeAllData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  void CheckButtons();
  QListWidget *list_source_box;
  QLabel *list_source_label;
  QListWidget *list_dest_box;
  int list_dest_items_quantity;
  QLabel *list_dest_label;
  QPushButton *list_add_button;
  QPushButton *list_addall_button;
  QPushButton *list_remove_button;
  QPushButton *list_removeall_button;
};


#endif  // LISTSELECTOR_H
