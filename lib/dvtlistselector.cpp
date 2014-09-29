//   dvtlistselector.cpp
//
//   A List Selector Widget.
//
//   (C) Copyright 2002,2008 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: dvtlistselector.cpp,v 1.2 2008/12/19 17:36:55 fredg Exp $
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
#include <qwidget.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qpixmap.h>

#include <dvtlistselector.h>


DvtListSelector::DvtListSelector(QWidget *parent,const char *name)
  : QHBox(parent,name)
{
  QFont font;

  //
  // Generate Font
  //
  font=QFont("Helvetica",10,QFont::Bold);
  font.setPixelSize(10);

  setSpacing(10);

  QVBox *source_box=new QVBox(this,"source_box");
  list_source_label=new QLabel(source_box,"list_source_label");
  list_source_label->setFont(font);
  list_source_label->setText(tr("Available Services"));
  list_source_label->setAlignment(AlignCenter);
  list_source_box=new QListBox(source_box,"list_source_box");

  QVBox *button_box=new QVBox(this,"button_box");
  list_add_button=new QPushButton(button_box,"list_add_button");
  list_add_button->setText(tr("Add >>"));
  list_add_button->setDisabled(true);
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));
  list_addall_button=new QPushButton(button_box,"list_addall_button");
  list_addall_button->setText(tr("Add All >>"));
  list_addall_button->setDisabled(true);
  connect(list_addall_button,SIGNAL(clicked()),this,SLOT(addAllData()));
  list_remove_button=new QPushButton(button_box,"list_remove_button");
  list_remove_button->setText(tr("<< Remove"));
  list_remove_button->setDisabled(true);
  connect(list_remove_button,SIGNAL(clicked()),this,SLOT(removeData()));
  list_removeall_button=new QPushButton(button_box,"list_removeall_button");
  list_removeall_button->setText(tr("<< Remove All"));
  list_removeall_button->setDisabled(true);
  connect(list_removeall_button,SIGNAL(clicked()),this,SLOT(removeAllData()));

  QVBox *dest_box=new QVBox(this,"dest_box");
  list_dest_label=new QLabel(dest_box,"list_dest_label");
  list_dest_label->setFont(font);
  list_dest_label->setText(tr("Active Services"));
  list_dest_label->setAlignment(AlignCenter);
  list_dest_box=new QListBox(dest_box,"list_dest_box");
}


uint DvtListSelector::sourceCount() const
{
  return list_source_box->count();
}


uint DvtListSelector::destCount() const
{
  return list_dest_box->count();
}


void DvtListSelector::sourceSetLabel(QString label)
{
  list_source_label->setText(label);
}


void DvtListSelector::destSetLabel(QString label)
{
  list_dest_label->setText(label);
}


void DvtListSelector::sourceInsertItem(const QString &text,int index)
{
  list_source_box->insertItem(text,index);
  list_source_box->sort();
  CheckButtons();
}


void DvtListSelector::destInsertItem(const QString &text,int index)
{
  list_dest_box->insertItem(text,index);
  list_dest_box->sort();
  CheckButtons();
}


void DvtListSelector::sourceRemoveItem(int index)
{
  list_source_box->removeItem(index);
  CheckButtons();
}


void DvtListSelector::destRemoveItem(int index)
{
  list_dest_box->removeItem(index);
  CheckButtons();
}


QString DvtListSelector::sourceText(int index) const
{
  return list_source_box->text(index);
}


QString DvtListSelector::destText(int index) const
{
  return list_dest_box->text(index);
}


void DvtListSelector::sourceChangeItem(const QString &text,int index)
{
  list_source_box->changeItem(text,index);
  list_source_box->sort();
}


void DvtListSelector::destChangeItem(const QString &text,int index)
{
  list_dest_box->changeItem(text,index);
  list_dest_box->sort();
}


int DvtListSelector::sourceNumItemsVisible() const
{
  return list_source_box->numItemsVisible();
}


int DvtListSelector::destNumItemsVisible() const
{
  return list_dest_box->numItemsVisible();
}


int DvtListSelector::sourceCurrentItem() const
{
  return list_source_box->currentItem();
}


int DvtListSelector::destCurrentItem() const
{
  return list_dest_box->currentItem();
}


QString DvtListSelector::sourceCurrentText() const
{
  return list_source_box->currentText();
}


QString DvtListSelector::destCurrentText() const
{
  return list_dest_box->currentText();
}


void DvtListSelector::sourceSetCurrentItem(int item)
{
  list_source_box->setCurrentItem(item);
}


void DvtListSelector::destSetCurrentItem(int item)
{
  list_dest_box->setCurrentItem(item);
}


QListBoxItem *DvtListSelector::sourceFindItem(const QString &text,
				      ComparisonFlags compare) const
{
  return list_source_box->findItem(text,compare);
}


QListBoxItem *DvtListSelector::destFindItem(const QString &text,
				      ComparisonFlags compare) const
{
  return list_dest_box->findItem(text,compare);
}


void DvtListSelector::clear()
{
  list_source_box->clear();
  list_dest_box->clear();
}


void DvtListSelector::addData()
{
  if(list_source_box->currentItem()>=0) {
    list_dest_box->
      insertItem(list_source_box->currentText());
    list_source_box->removeItem(list_source_box->currentItem());
    list_dest_box->sort();
    list_source_box->setCurrentItem(-1);
    CheckButtons();
  }
}


void DvtListSelector::addAllData()
{
  for(unsigned i=0;i<list_source_box->count();i++) {
    list_dest_box->
      insertItem(list_source_box->text(i));
  }    
  list_source_box->clear();
  list_dest_box->sort();
  CheckButtons();
}


void DvtListSelector::removeData()
{
  if(list_dest_box->currentItem()>=0) {
    list_source_box->
      insertItem(list_dest_box->currentText());
    list_dest_box->removeItem(list_dest_box->currentItem());
    list_source_box->sort();
    list_dest_box->setCurrentItem(-1);
    CheckButtons();
  }
}


void DvtListSelector::removeAllData()
{
  for(unsigned i=0;i<list_dest_box->count();i++) {
    list_source_box->
      insertItem(list_dest_box->text(i));
  }    
  list_dest_box->clear();
  list_source_box->sort();
  CheckButtons();
}


void DvtListSelector::CheckButtons()
{
  list_add_button->setDisabled(list_source_box->count()==0);
  list_addall_button->setDisabled(list_source_box->count()==0);
  list_remove_button->setDisabled(list_dest_box->count()==0);
  list_removeall_button->setDisabled(list_dest_box->count()==0);
}
