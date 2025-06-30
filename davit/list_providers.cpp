// list_providers.cpp
//
// List Davit Providers.
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

#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

#include <dvtdb.h>

#include "add_provider.h"
#include "edit_provider.h"
#include "globals.h"
#include "list_programs.h"
#include "list_providers.h"

ListProviders::ListProviders(DvtConfig *c,ListPrograms *listprograms_dialog,
			     QWidget *parent)
  : DvtDialog(c,parent)
{
  setModal(true);

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());

  setWindowTitle("Davit - Providers");

  //
  // Dialogs
  //
  list_editprovider_dialog=new EditProvider(config,listprograms_dialog,this);

  //
  // Providers List
  //
  list_providers_view=new DvtTableView(this);
  list_providers_model=new ProviderListModel(this);
  list_providers_model->setFont(defaultFont());
  list_providers_model->setPalette(palette());
  list_providers_view->setModel(list_providers_model);
  connect(list_providers_view,SIGNAL(doubleClicked(const QModelIndex &)),
	  this,SLOT(doubleClickedData(const QModelIndex &)));

  //
  //  Add Button
  //
  list_add_button=new QPushButton(this);
  list_add_button->setFont(buttonFont());
  list_add_button->setText("Add");
  list_add_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  //
  //  Edit Button
  //
  list_edit_button=new QPushButton(this);
  list_edit_button->setFont(buttonFont());
  list_edit_button->setText("Edit");
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  //  Delete Button
  //
  list_delete_button=new QPushButton(this);
  list_delete_button->setFont(buttonFont());
  list_delete_button->setText("Delete");
  list_delete_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  //  Close Button
  //
  list_close_button=new QPushButton(this);
  list_close_button->setDefault(true);
  list_close_button->setFont(buttonFont());
  list_close_button->setText("&Close");
  connect(list_close_button,SIGNAL(clicked()),this,SLOT(closeData()));

  RefreshList();
}


ListProviders::~ListProviders()
{
}


QSize ListProviders::sizeHint() const
{
  return QSize(640,480);
} 


QSizePolicy ListProviders::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int ListProviders::exec()
{
  list_providers_model->refresh();
  list_providers_view->resizeColumnsToContents();

  return QDialog::exec();
}


void ListProviders::addData()
{
  QString sql=QString("insert into `PROVIDERS` set ")+
    "`BUSINESS_NAME`="+DvtSqlQuery::escape(tr("[new provider]"));
  int provider_id=DvtSqlQuery::run(sql).toInt();
  if(list_editprovider_dialog->exec(provider_id,true)) {
    QModelIndex index=list_providers_model->addProvider(provider_id);
    if(index.isValid()) {
      list_providers_view->
	selectionModel()->select(index,QItemSelectionModel::ClearAndSelect|
				 QItemSelectionModel::Rows);
    }
  }
  else {
    sql=QString("delete from `PROVIDERS` where ")+
      QString::asprintf("`PROVIDERS`.`ID`=%d ",provider_id);
    DvtSqlQuery::apply(sql);
  }
}


void ListProviders::editData()
{
  QModelIndexList rows=list_providers_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  if(list_editprovider_dialog->
     exec(list_providers_model->providerId(rows.first()),false)) {
    list_providers_model->refresh(rows.first());
  }
}


void ListProviders::deleteData()
{
  QModelIndexList rows=list_providers_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  QString sql=QString("select ")+
    "`ID` "+
    "from `PROGRAMS` where "+
    QString::asprintf("`PROVIDER_ID`=%d",
		      list_providers_model->providerId(rows.first()));
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  if(q->first()) {
    QMessageBox::information(this,"Davit - "+tr("Error"),
			     tr("You must delete or migrate all programs off this provider before it can be deleted."));
    delete q;
    return;
  }
  delete q;

  if(QMessageBox::question(this,
	     "Davit",tr("Are you sure you want to delete the provider")+
			   " \""+
			   list_providers_model->providerName(rows.first())+
			   "\"?",
		 QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
    return;
  }
  int provider_id=list_providers_model->providerId(rows.first());
  sql=QString("delete from `PROVIDERS` where ")+
    QString::asprintf("`PROVIDERS`.`ID`=%d ",provider_id);
  DvtSqlQuery::apply(sql);
  list_providers_model->removeProvider(rows.first());
}


void ListProviders::doubleClickedData(const QModelIndex &index)
{
  editData();
}


void ListProviders::closeData()
{
  done(0);
}


void ListProviders::resizeEvent(QResizeEvent *e)
{
  list_providers_view->setGeometry(10,10,size().width()-20,size().height()-80);
  list_add_button->setGeometry(10,size().height()-60,80,50);
  list_edit_button->setGeometry(100,size().height()-60,80,50);
  list_delete_button->setGeometry(190,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListProviders::DeleteProvider(int pid)
{
  /*
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;

  sql=QString().sprintf("select ID from PROGRAMS where PROVIDER_ID=%d",pid);
  q=new QSqlQuery(sql);
  while(q->next()) {
    sql=QString().sprintf("delete from FEEDS where PROGRAM_ID=%d",
			  q->value(0).toInt());
    q1=new QSqlQuery(sql);
    delete q1;

    sql=QString().sprintf("delete from AIRINGS where PROGRAM_ID=%d",
			  q->value(0).toInt());
    q1=new QSqlQuery(sql);
    delete q1;

    sql=QString().sprintf("delete from AIRED where PROGRAM_ID=%d",
			  q->value(0).toInt());
    q1=new QSqlQuery(sql);
    delete q1;
  }
  delete q;

  sql=QString().sprintf("delete from PROGRAMS where PROVIDER_ID=%d",pid);
  q=new QSqlQuery(sql);
  delete q;

  sql=QString().sprintf("delete from PROVIDERS where ID=%d",pid);
  q=new QSqlQuery(sql);
  delete q;
  */
}


void ListProviders::RefreshList()
{
  /*
  QSqlQuery *q;
  DvtListViewItem *item;

  list_providers_view->clear();
  q=new QSqlQuery("select BUSINESS_NAME,CONTACT_NAME,\
                   CONTACT_PHONE,CONTACT_EMAIL,ID from PROVIDERS");
  while (q->next()) {
    item=new DvtListViewItem(list_providers_view);
    item->setId(q->value(4).toInt());
    item->setText(0,q->value(0).toString());
    item->setText(1,q->value(1).toString());
    item->setText(2,q->value(2).toString());
    item->setText(3,q->value(3).toString());
  }
  delete q;
  */
}


/*
void ListProviders::UpdateItem(DvtListViewItem *item)
{
  QString sql;
  QSqlQuery *q;

  sql=QString().sprintf("select CONTACT_NAME,CONTACT_PHONE,CONTACT_EMAIL,ID \
                         from PROVIDERS where BUSINESS_NAME=\"%s\"",
			(const char *)item->text(0));
  q=new QSqlQuery(sql);
  if(q->first()) {
    item->setId(q->value(3).toInt());
    item->setText(1,q->value(0).toString());
    item->setText(2,q->value(1).toString());
    item->setText(3,q->value(2).toString());
  }
  delete q;
}
*/
