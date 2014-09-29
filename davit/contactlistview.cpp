/*   contactlistview.cpp
 *
 *   A QListView class for the ListContacts widget.
 *
 *   (C) Copyright 2002-2004,2010 Fred Gleason <fredg@paravelsystems.com>
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

#include <qheader.h>

#include <dvtconf.h>

#include <globals.h>
#include <contactlistview.h>

ContactListView::ContactListView(QWidget *parent) 
  : QListView(parent)
{
  item_parent=parent;
  item_menu_item=NULL;

  //
  // Right Button Menu
  //
  item_menu=new QPopupMenu(this);
  connect(item_menu,SIGNAL(aboutToShow()),this,SLOT(aboutToShowData()));
  item_menu->
    insertItem(tr("Email this contact"),this,SLOT(mailContactData()),0,0);
}


void ContactListView::aboutToShowData()
{
  item_menu->setItemEnabled(0,(item_menu_item!=NULL)&&
			    DvtIsEmailAddress(item_menu_item->text(4)));
}


void ContactListView::mailContactData()
{
  mail_dialog->exec(item_menu_item->text(4),"","",global_dvtuser->email(),
		    global_dvtuser->email(),"","");
}


void ContactListView::contentsMousePressEvent(QMouseEvent *e)
{
  QListView::contentsMousePressEvent(e);
  item_menu_item=(DvtListViewItem *)selectedItem();
  switch(e->button()) {
      case QMouseEvent::RightButton:
	item_menu->setGeometry(e->globalX()+2,
			       e->globalY(),
			       item_menu->sizeHint().width(),
			       item_menu->sizeHint().height());
	item_menu->exec();
	break;

      default:
	e->ignore();
	break;
  }
}

