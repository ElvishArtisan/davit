// list_networks.cpp
//
// List Davit Networks.
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

#include <math.h>

#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <dvtconf.h>
#include <dvtconfig.h>

#include "edit_network.h"
#include "globals.h"
#include "list_networks.h"

ListNetworks::ListNetworks(QWidget *parent)
  : QDialog(parent)
{
  setModal(true);

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setWindowTitle("Davit - Networks");

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Networks List
  //
  /*
  list_networks_list=new QListView(this,"list_networks_list");
  list_networks_list->setMargin(5);
  list_networks_list->setAllColumnsShowFocus(true);
  connect(list_networks_list,
	  SIGNAL(doubleClicked(QListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(QListViewItem *,const QPoint &,int)));
  list_networks_list->addColumn("Name");
  list_networks_list->setColumnAlignment(0,Qt::AlignLeft|Qt::AlignVCenter);
  */
  //
  //  Add Button
  //
  list_add_button=new QPushButton(this);
  list_add_button->setFont(font);
  list_add_button->setText("&Add");
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  //
  //  Edit Button
  //
  list_edit_button=new QPushButton(this);
  list_edit_button->setFont(font);
  list_edit_button->setText("&Edit");
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  //  Delete Button
  //
  list_delete_button=new QPushButton(this);
  list_delete_button->setFont(font);
  list_delete_button->setText("&Delete");
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  //  Close Button
  //
  list_close_button=new QPushButton(this);
  list_close_button->setDefault(true);
  list_close_button->setFont(font);
  list_close_button->setText("&Close");
  connect(list_close_button,SIGNAL(clicked()),this,SLOT(closeData()));

  RefreshList();
}


ListNetworks::~ListNetworks()
{
}


QSize ListNetworks::sizeHint() const
{
  return QSize(640,480);
} 


QSizePolicy ListNetworks::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListNetworks::addData()
{
  /*
  QString network="[new network]";
  QString sql;
  QSqlQuery *q;

  EditNetwork *add=new EditNetwork(&network,this);
  if(add->exec()==0) {
    sql=QString().sprintf("insert into NETWORKS set NAME=\"%s\"",
			  (const char *)DvtEscapeString(network));
    q=new QSqlQuery(sql);
    delete q;
    DvtListViewItem *item=
      (DvtListViewItem *)new QListViewItem(list_networks_list);
    item->setText(0,network);
    list_networks_list->setSelected(item,true);
    list_networks_list->ensureItemVisible(item);
  }
  delete add;
  */
}


void ListNetworks::editData()
{
  /*
  DvtListViewItem *item=(DvtListViewItem *)list_networks_list->selectedItem();
  
  if(item==NULL) {
    return;
  }
  QString name=item->text(0);
  EditNetwork *edit=new EditNetwork(&name,this,"edit");
  if(edit->exec()==0) {
    UpdateItem(item,name);
  }
  delete edit;
  */
}


void ListNetworks::deleteData()
{
  /*
  QString sql;
  QSqlQuery *q;
  QListViewItem *item=list_networks_list->selectedItem();

  if(item==NULL) {
    return;
  }
  if(QMessageBox::information(this,"Davit",
			      "Are you sure you want to delete this network?",
			      QMessageBox::Yes,QMessageBox::No)!=
     QMessageBox::Yes) {

    return;
  }			     
  sql=QString().sprintf("select ID from NETWORKS where NAME=\"%s\"",
			(const char *)DvtEscapeString(item->text(0)));
  q=new QSqlQuery(sql);
  if(q->first()) {
    int id=q->value(0).toInt();
    delete q;
    sql=QString().sprintf("update AFFILIATES set SECOND_NETWORK_ID=NULL \
                           where SECOND_NETWORK_ID=%d",id);
    q=new QSqlQuery(sql);
    delete q;
    sql=QString().sprintf("delete from NETWORKS where ID=%d",id);
    q=new QSqlQuery(sql);
  }
  delete q;
  delete item;
  */
}

/*
void ListNetworks::doubleClickedData(QListViewItem *item,const QPoint &pt,
				      int c)
{
  editData();
}
*/

void ListNetworks::closeData()
{
  done(0);
}


void ListNetworks::resizeEvent(QResizeEvent *e)
{
  //  list_networks_list->setGeometry(10,10,size().width()-20,size().height()-80);
  list_add_button->setGeometry(10,size().height()-60,80,50);
  list_edit_button->setGeometry(100,size().height()-60,80,50);
  list_delete_button->setGeometry(190,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListNetworks::RefreshList()
{
  /*
  QSqlQuery *q;
  DvtListViewItem *item;

  list_networks_list->clear();
  q=new QSqlQuery("select ID,NAME from NETWORKS");
  while (q->next()) {
    item=new DvtListViewItem(list_networks_list);
    item->setId(q->value(0).toInt());
    item->setText(0,q->value(1).toString());
  }
  delete q;
  */
}


void ListNetworks::UpdateItem(DvtListViewItem *item,const QString &name)
{
  /*
  item->setText(0,name);
  QString sql=QString().sprintf("update NETWORKS set NAME=\"%s\" where ID=%d",
				(const char *)name,item->id());
  QSqlQuery *q=new QSqlQuery(sql);
  delete q;
  */
}
