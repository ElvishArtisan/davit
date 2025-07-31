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

#include <QMessageBox>

#include <dvtconf.h>
#include <dvtconfig.h>

#include "edit_network.h"
#include "globals.h"
#include "list_networks.h"

ListNetworks::ListNetworks(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  setModal(true);

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setWindowTitle("Davit - "+tr("Networks"));

  //
  // Edit Network Dialog
  //
  list_editnetwork_dialog=new EditNetwork(c,this);

  //
  // Networks List
  //
  list_networks_view=new TableView(this);
  list_networks_model=new NetworkListModel(this);
  list_networks_model->setFont(defaultFont());
  list_networks_model->setPalette(palette());
  list_networks_view->setModel(list_networks_model);
  connect(list_networks_view,SIGNAL(doubleClicked(const QModelIndex &)),
	  this,SLOT(doubleClickedData(const QModelIndex &)));

  //
  //  Add Button
  //
  list_add_button=new QPushButton(this);
  list_add_button->setFont(buttonFont());
  list_add_button->setText("&Add");
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  //
  //  Edit Button
  //
  list_edit_button=new QPushButton(this);
  list_edit_button->setFont(buttonFont());
  list_edit_button->setText("&Edit");
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  //  Delete Button
  //
  list_delete_button=new QPushButton(this);
  list_delete_button->setFont(buttonFont());
  list_delete_button->setText("&Delete");
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  //  Close Button
  //
  list_close_button=new QPushButton(this);
  list_close_button->setDefault(true);
  list_close_button->setFont(buttonFont());
  list_close_button->setText("&Close");
  connect(list_close_button,SIGNAL(clicked()),this,SLOT(closeData()));

  list_networks_model->refresh();
  list_networks_view->resizeColumnsToContents();
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
  QString sql=QString("insert into `NETWORKS` set ")+
    "`NAME`="+DvtSqlQuery::escape(tr("[new network]"));
  int network_id=DvtSqlQuery::run(sql).toInt();
  if(list_editnetwork_dialog->exec(network_id,true)) {
    QModelIndex index=list_networks_model->addNetwork(network_id);
    if(index.isValid()) {
      list_networks_view->
	selectionModel()->select(index,QItemSelectionModel::ClearAndSelect|
				 QItemSelectionModel::Rows);
    }
  }
  else {
    sql=QString("delete from `NETWORKS` where ")+
      QString::asprintf("`NETWORKS`.`ID`=%d ",network_id);
    DvtSqlQuery::apply(sql);
  }
}


void ListNetworks::editData()
{
  QModelIndexList rows=list_networks_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  if(list_editnetwork_dialog->
     exec(list_networks_model->networkId(rows.first()),false)) {
    list_networks_model->refresh(rows.first());
  }
}


void ListNetworks::deleteData()
{
  QModelIndexList rows=list_networks_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  if(QMessageBox::question(this,
	     "Davit",tr("Are you sure you want to delete the network for")+
			   " \""+
			   list_networks_model->networkName(rows.first())+
			   "\"?",
		 QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
    return;
  }
  int network_id=list_networks_model->networkId(rows.first());
  QString sql=QString("delete from `NETWORKS` where ")+
    QString::asprintf("`NETWORKS`.`ID`=%d ",network_id);
  DvtSqlQuery::apply(sql);
  list_networks_model->removeNetwork(rows.first());
}


void ListNetworks::doubleClickedData(const QModelIndex &index)
{
  editData();
}


void ListNetworks::closeData()
{
  done(true);
}


void ListNetworks::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  list_networks_view->setGeometry(10,10,w-20,h-80);
  list_add_button->setGeometry(10,h-60,80,50);
  list_edit_button->setGeometry(100,h-60,80,50);
  list_delete_button->setGeometry(190,h-60,80,50);
  list_close_button->setGeometry(w-90,h-60,80,50);
}
