/*   contactlistview.h
 *
 *   A QListViewItem class for the ListContacts widget.
 *
 *   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef CONTACTLISTVIEW_H
#define CONTACTLISTVIEW_H

#include <QListView>

//#include <qpopupmenu.h>

//#include <dvtlistviewitem.h>

class ContactListView : public QListView
{
  Q_OBJECT;
 public:
  ContactListView(QWidget *parent);

 private slots:
  void aboutToShowData();
  void mailContactData();

 protected:
  void contentsMousePressEvent(QMouseEvent *e);

 private:
  //  QPopupMenu *item_menu;
  //  DvtListViewItem *item_menu_item;
  QWidget *item_parent;
};


#endif  // CONTACTLISTVIEW_H
