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
#include <QSqlDatabase>
#include <QMessageBox>

#include <dvtconfig.h>

#include "add_provider.h"
#include "edit_provider.h"
#include "globals.h"
#include "list_providers.h"

ListProviders::ListProviders(QWidget *parent)
  : QDialog(parent)
{
  setModal(true);
  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setWindowTitle("Davit - Providers");

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Providers List
  //
  /*
  list_providers_list=new QListView(this,"list_providers_list");
  list_providers_list->setMargin(5);
  list_providers_list->setAllColumnsShowFocus(true);
  connect(list_providers_list,
	  SIGNAL(doubleClicked(QListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(QListViewItem *,const QPoint &,int)));
  list_providers_list->addColumn("Business Name");
  list_providers_list->setColumnAlignment(0,AlignLeft|AlignVCenter);
  list_providers_list->addColumn("Contact");
  list_providers_list->setColumnAlignment(1,AlignCenter);
  list_providers_list->addColumn("Phone");
  list_providers_list->setColumnAlignment(2,AlignLeft|AlignVCenter);
  list_providers_list->addColumn("E-Mail");
  list_providers_list->setColumnAlignment(3,AlignLeft|AlignVCenter);
  */
  //
  //  Add Button
  //
  list_add_button=new QPushButton(this);
  list_add_button->setFont(font);
  list_add_button->setText("&Add");
  list_add_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivProviderEdit));
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
  list_delete_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivProviderEdit));
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


void ListProviders::addData()
{
  /*
  QString bname;
  QString sql;
  QSqlQuery *q;
  int pid=0;

  AddProvider *add=new AddProvider(&bname,this);
  if(add->exec()==0) {
    sql=QString().sprintf("insert into PROVIDERS set BUSINESS_NAME=\"%s\"",
			  (const char *)bname);
    q=new QSqlQuery(sql);
    delete q;
    sql="select LAST_INSERT_ID() from PROVIDERS";
    q=new QSqlQuery(sql);
    if(q->first()) {
      pid=q->value(0).toInt();
    }
    delete q;
    EditProvider *edit=new EditProvider(bname,this,"edit");
    if(edit->exec()==0) {
      DvtListViewItem *item=new DvtListViewItem(list_providers_list);
      item->setText(0,bname);
      UpdateItem(item);
      list_providers_list->setSelected(item,true);
      list_providers_list->ensureItemVisible(item);
    }
    else {
      DeleteProvider(pid);
    }
    delete edit;
  }
  delete add;
  */
}


void ListProviders::editData()
{
  /*
  DvtListViewItem *item=(DvtListViewItem *)list_providers_list->selectedItem();
  
  if(item==NULL) {
    return;
  }
  EditProvider *edit=new EditProvider(item->text(0),this,"edit");
  if(edit->exec()==0) {
    UpdateItem(item);
  }
  delete edit;
  */
}


void ListProviders::deleteData()
{
  /*
  DvtListViewItem *item=(DvtListViewItem *)list_providers_list->selectedItem();

  if(item==NULL) {
    return;
  }
  if(QMessageBox::question(this,"Delete Provider",
      QString().sprintf("Are you sure you want to delete the provider \"%s\"",
			   (const char *)item->text(0)),
			   QMessageBox::Yes,
			   QMessageBox::No)==QMessageBox::No) {
    return;
  }
  DeleteProvider(item->id());
  delete item;
  */
}

/*
void ListProviders::doubleClickedData(QListViewItem *item,const QPoint &pt,
				      int c)
{
  editData();
}
*/

void ListProviders::closeData()
{
  done(0);
}


void ListProviders::resizeEvent(QResizeEvent *e)
{
  //  list_providers_list->
  //    setGeometry(10,10,size().width()-20,size().height()-80);
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

  list_providers_list->clear();
  q=new QSqlQuery("select BUSINESS_NAME,CONTACT_NAME,\
                   CONTACT_PHONE,CONTACT_EMAIL,ID from PROVIDERS");
  while (q->next()) {
    item=new DvtListViewItem(list_providers_list);
    item->setId(q->value(4).toInt());
    item->setText(0,q->value(0).toString());
    item->setText(1,q->value(1).toString());
    item->setText(2,q->value(2).toString());
    item->setText(3,q->value(3).toString());
  }
  delete q;
  */
}


void ListProviders::UpdateItem(DvtListViewItem *item)
{
  /*
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
  */
}
