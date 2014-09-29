/*   dvtlistviewitem.cpp
 *
 *   A QListViewItem class for Davit
 *
 *   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
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

#include <dvtlistviewitem.h>


DvtListViewItem::DvtListViewItem(QListView *parent) 
  : QListViewItem(parent)
{
  item_id=-1;
}


int DvtListViewItem::id() const
{
  return item_id;
}


void DvtListViewItem::setId(int id)
{
  item_id=id;
}
