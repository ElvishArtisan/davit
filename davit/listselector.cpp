// listselector.cpp
//
// A List Selector Widget.
//
// (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
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
//

#include <stdlib.h>
#include <stdio.h>

#include <QWidget>
#include <QString>
#include <QPixmap>

#include "listselector.h"

ListSelectorItem::ListSelectorItem(QListWidget *owner,int airing_id,
				   const QString &text)
  : QListWidgetItem(owner)
{
  setText(text);
  d_airing_id=airing_id;
}

int ListSelectorItem::airingId() const
{
  return d_airing_id;
}



ListSelector::ListSelector(DvtConfig *c,QWidget *parent)
  : Widget(c,parent)
{
  QFont font;
  list_dest_items_quantity=0;

  list_source_label=new QLabel(this);
  list_source_label->setFont(font);
  list_source_label->setText(tr("Available Services"));
  list_source_label->setAlignment(Qt::AlignCenter);
  list_source_box=new QListWidget(this);
  list_source_box->setSelectionMode(QAbstractItemView::ExtendedSelection);
  list_source_box->setSortingEnabled(true);

  list_add_button=new QPushButton(this);
  list_add_button->setText(tr("Add >>"));
  list_add_button->setDisabled(true);
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));
  list_addall_button=new QPushButton(this);
  list_addall_button->setText(tr("Add All >>"));
  list_addall_button->setDisabled(true);
  connect(list_addall_button,SIGNAL(clicked()),this,SLOT(addAllData()));
  list_remove_button=new QPushButton(this);
  list_remove_button->setText(tr("<< Remove"));
  list_remove_button->setDisabled(true);
  connect(list_remove_button,SIGNAL(clicked()),this,SLOT(removeData()));
  list_removeall_button=new QPushButton(this);
  list_removeall_button->setText(tr("<< Remove All"));
  list_removeall_button->setDisabled(true);
  connect(list_removeall_button,SIGNAL(clicked()),this,SLOT(removeAllData()));

  list_dest_label=new QLabel(this);
  list_dest_label->setFont(font);
  list_dest_label->setText(tr("Active Services"));
  list_dest_label->setAlignment(Qt::AlignCenter);
  list_dest_box=new QListWidget(this);
  list_dest_box->setSelectionMode(QAbstractItemView::ExtendedSelection);
  list_dest_box->setSortingEnabled(true);
}


uint ListSelector::sourceCount() const
{
  return list_source_box->count();
}


uint ListSelector::destCount() const
{
  return list_dest_box->count();
}


void ListSelector::sourceSetLabel(QString label)
{
  list_source_label->setText(label);
}


void ListSelector::destSetLabel(QString label)
{
  list_dest_label->setText(label);
}


void ListSelector::sourceInsertItem(int id,const QString &text)
{
  new ListSelectorItem(list_source_box,id,text);
  CheckButtons();
}


void ListSelector::destInsertItem(int id,const QString &text)
{
  new ListSelectorItem(list_dest_box,id,text);
  CheckButtons();
}


void ListSelector::sourceRemoveItem(int index)
{
  ListSelectorItem *item=NULL;
  if((item=(ListSelectorItem *)list_source_box->takeItem(index))!=NULL) {
    delete item;
  }
  CheckButtons();
}


void ListSelector::destRemoveItem(int index)
{
  ListSelectorItem *item=NULL;
  if((item=(ListSelectorItem *)list_dest_box->takeItem(index))!=NULL) {
    delete item;
  }
  CheckButtons();
}


int ListSelector::sourceAiringId(int index) const
{
  return list_source_box->item(index)->data(Qt::UserRole).toInt();
}


QString ListSelector::sourceText(int index) const
{
  return list_source_box->item(index)->data(Qt::DisplayRole).toString();
}


int ListSelector::destAiringId(int index) const
{
  return ((ListSelectorItem *)list_dest_box->item(index))->airingId();
}


QString ListSelector::destText(int index) const
{
  return list_dest_box->item(index)->data(Qt::DisplayRole).toString();
}


int ListSelector::sourceCurrentItem() const
{
  return list_source_box->currentRow();
}


int ListSelector::destCurrentItem() const
{
  return list_dest_box->currentRow();
}


QString ListSelector::sourceCurrentText() const
{
  return list_source_box->currentItem()->data(Qt::DisplayRole).toString();
}


QString ListSelector::destCurrentText() const
{
  return list_dest_box->currentItem()->data(Qt::DisplayRole).toString();
}


void ListSelector::sourceSetCurrentItem(int item)
{
  list_source_box->setCurrentRow(item);
}


void ListSelector::destSetCurrentItem(int item)
{
  list_dest_box->setCurrentRow(item);
}


void ListSelector::clear()
{
  list_source_box->clear();
  list_dest_box->clear();
}


void ListSelector::addData()
{
  QList<QListWidgetItem *> items=list_source_box->selectedItems();
  for(int i=0;i<items.size();i++) {
    QListWidgetItem *item=
      list_source_box->takeItem(list_source_box->row(items.at(i)));
    list_dest_box->addItem(item);
  }
  list_dest_box->sortItems();
  list_source_box->setCurrentItem(NULL);
  CheckButtons();
}


void ListSelector::addAllData()
{
  int rows=list_source_box->count();
  for(int i=0;i<rows;i++) {
    list_dest_box->addItem(list_source_box->takeItem(0));
  }
  list_dest_box->sortItems();
  list_source_box->setCurrentItem(NULL);
  CheckButtons();
}


void ListSelector::removeData()
{
  QList<QListWidgetItem *> items=list_dest_box->selectedItems();
  for(int i=0;i<items.size();i++) {
    list_source_box->
      addItem(list_dest_box->takeItem(list_dest_box->row(items.at(i))));
  }
  list_source_box->sortItems();
  list_dest_box->setCurrentItem(NULL);
  CheckButtons();
}


void ListSelector::removeAllData()
{
  int rows=list_dest_box->count();
  for(int i=0;i<rows;i++) {
    list_source_box->addItem(list_dest_box->takeItem(0));
  }
  list_source_box->sortItems();
  list_dest_box->setCurrentItem(NULL);
  CheckButtons();
}


void ListSelector::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  list_source_label->setGeometry(0,0,2*w/5,20);
  list_source_box->setGeometry(0,20,2*w/5,h-20);

  list_dest_label->setGeometry(3*w/5,0,2*w/5,20);
  list_dest_box->setGeometry(3*w/5,20,2*w/5,h-20);

  list_add_button->setGeometry(2*w/5+10,40,w/5-20,25);
  list_addall_button->setGeometry(2*w/5+10,40+30,w/5-20,25);

  list_remove_button->setGeometry(2*w/5+10,h-45-30,w/5-20,25);
  list_removeall_button->setGeometry(2*w/5+10,h-45,w/5-20,25);
}


void ListSelector::CheckButtons()
{
  list_add_button->setDisabled(list_source_box->count()==0);
  list_addall_button->setDisabled(list_source_box->count()==0);
  list_remove_button->setDisabled(list_dest_box->count()==0);
  list_removeall_button->setDisabled(list_dest_box->count()==0);

  if(list_dest_box->count()!=list_dest_items_quantity) {
    list_dest_items_quantity=list_dest_box->count();
    emit destItemsQuantityChanged(list_dest_items_quantity);
  }
}
