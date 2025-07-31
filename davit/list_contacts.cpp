// list_contacts.cpp
//
// List Davit Contacts.
//
//   (C) Copyright 2010-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <QMessageBox>

#include <dvtconf.h>

#include "list_contacts.h"

ListContacts::ListContacts(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  setModal(false);
  list_id=-1;

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  //
  // Edit Contact Dialog
  //
  list_editcontact_dialog=new EditContact(c,this);

  //
  // Contact List
  //
  list_contacts_view=new TableView(this);
  list_contacts_model=new ContactListModel(this);
  list_contacts_model->setFont(defaultFont());
  list_contacts_model->setPalette(palette());
  list_contacts_view->setModel(list_contacts_model);
  connect(list_contacts_view,SIGNAL(doubleClicked(const QModelIndex &)),
	  this,SLOT(doubleClickedData(const QModelIndex &)));

  list_add_button=new QPushButton(this);
  list_add_button->setFont(label_font);
  list_add_button->setText(tr("Add"));
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  list_edit_button=new QPushButton(this);
  list_edit_button->setFont(label_font);
  list_edit_button->setText(tr("Edit"));
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  list_delete_button=new QPushButton(this);
  list_delete_button->setFont(label_font);
  list_delete_button->setText(tr("Delete"));
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  list_contacts_model->refresh();
  list_contacts_view->resizeColumnsToContents();
}


ListContacts::~ListContacts()
{
}


QSize ListContacts::sizeHint() const
{
  return QSize(615,300);
}


void ListContacts::setAffiliateId(int id)
{
  list_id=id;
  setWindowTitle("Davit - "+tr("Contacts for ")+DvtStationCallString(id));
  list_contacts_model->setAffiliateId(id);
  list_contacts_view->resizeColumnsToContents();
}


void ListContacts::show()
{
  QDialog::show();
  emit visibilityChanged(true);
}


void ListContacts::hide()
{
  QDialog::hide();
  emit visibilityChanged(false);
}


QSizePolicy ListContacts::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListContacts::addData()
{
  QString sql=QString("insert into `CONTACTS` set ")+
    QString::asprintf("`AFFILIATE_ID`=%d,",list_id)+
    "`NAME`="+DvtSqlQuery::escape(tr("[new contact]"));
  int contact_id=DvtSqlQuery::run(sql).toInt();
  if(list_editcontact_dialog->exec(contact_id,true)) {
    QModelIndex index=list_contacts_model->addContact(contact_id);
    if(index.isValid()) {
      list_contacts_view->
	selectionModel()->select(index,QItemSelectionModel::ClearAndSelect|
				 QItemSelectionModel::Rows);
    }
  }
  else {
    sql=QString("delete from `CONTACTS` where ")+
      QString::asprintf("`CONTACTS`.`ID`=%d ",contact_id);
    DvtSqlQuery::apply(sql);
  }
}


void ListContacts::editData()
{
  QModelIndexList rows=list_contacts_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  if(list_editcontact_dialog->
     exec(list_contacts_model->contactId(rows.first()),false)) {
    list_contacts_model->refresh(rows.first());
  }
}


void ListContacts::deleteData()
{
  QModelIndexList rows=list_contacts_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  if(QMessageBox::question(this,
	     "Davit",tr("Are you sure you want to delete the contact for")+
			   " \""+
			   list_contacts_model->contactName(rows.first())+
			   "\"?",
		 QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
    return;
  }
  int contact_id=list_contacts_model->contactId(rows.first());
  QString sql=QString("delete from `CONTACTS` where ")+
    QString::asprintf("`CONTACTS`.`ID`=%d ",contact_id);
  DvtSqlQuery::apply(sql);
  list_contacts_model->removeContact(rows.first());
}


void ListContacts::doubleClickedData(const QModelIndex &index)
{
  editData();
}


void ListContacts::resizeEvent(QResizeEvent *e)
{
  list_contacts_view->setGeometry(0,0,size().width(),size().height()-40);
  list_add_button->setGeometry(10,size().height()-35,50,30);
  list_edit_button->setGeometry(70,size().height()-35,50,30);
  list_delete_button->setGeometry(130,size().height()-35,50,30);
}


void ListContacts::closeEvent(QCloseEvent *)
{
  hide();
}
