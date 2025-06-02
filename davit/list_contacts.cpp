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

#include <QSqlDatabase>
#include <QMessageBox>

#include <dvtconf.h>

#include "list_contacts.h"

ListContacts::ListContacts(DvtConfig *c,QWidget *parent)
  : DvtDialog(c,parent)
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
  list_contact_dialog=new EditContact(this);

  //
  // Contact List
  //
  /*
  list_contacts_list=new ContactListView(this);
  list_contacts_list->setAllColumnsShowFocus(true);
  list_contacts_list->setItemMargin(5);
  connect(list_contacts_list,
	  SIGNAL(doubleClicked(QListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(QListViewItem *,const QPoint &,int)));
  list_contacts_list->addColumn(tr("Name"));
  list_contacts_list->setColumnAlignment(0,AlignLeft);
  list_contacts_list->addColumn(tr("Title"));
  list_contacts_list->setColumnAlignment(1,AlignLeft);
  list_contacts_list->addColumn(tr("Phone"));
  list_contacts_list->setColumnAlignment(2,AlignCenter);
  list_contacts_list->addColumn(tr("FAX"));
  list_contacts_list->setColumnAlignment(3,AlignCenter);
  list_contacts_list->addColumn(tr("E-Mail"));
  list_contacts_list->setColumnAlignment(4,AlignCenter);
  list_contacts_list->addColumn(tr("Affidavit"));
  list_contacts_list->setColumnAlignment(5,AlignCenter);
  list_contacts_list->addColumn(tr("Prog Dir"));
  list_contacts_list->setColumnAlignment(6,AlignCenter);
  list_contacts_list->addColumn(tr("Gen Mgr"));
  list_contacts_list->setColumnAlignment(7,AlignCenter);
  list_contacts_list->addColumn(tr("Locked"));
  list_contacts_list->setColumnAlignment(8,AlignCenter);
  */
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

  RefreshList();
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
  /*
  Contact *contact=new Contact();
  if(list_contact_dialog->exec(contact)==0) {
    DvtListViewItem *item=new DvtListViewItem(list_contacts_list);
    UpdateItem(item,contact);
  }
  delete contact;
  */
}


void ListContacts::editData()
{
  /*
  DvtListViewItem *item=(DvtListViewItem *)list_contacts_list->selectedItem();
  if(item==NULL) {
    return;
  }
  Contact *contact=new Contact();
  contact->setName(item->text(0));
  contact->setTitle(item->text(1));
  contact->setPhone(item->text(2));
  contact->setFax(item->text(3));
  contact->setEmail(item->text(4));
  contact->setAffidavit(item->text(5)=="Y");
  contact->setProgramDirector(item->text(6)=="Y");
  contact->setGeneralManager(item->text(7)=="Y");
  contact->setLocked(item->text(8)=="Y");
  if(list_contact_dialog->exec(contact)==0) {
    UpdateItem(item,contact);
  }
  delete contact;
  */
}


void ListContacts::deleteData()
{
  /*
  QString sql;
  QSqlQuery *q;
  DvtListViewItem *item=(DvtListViewItem *)list_contacts_list->selectedItem();
  if(item==NULL) {
    return;
  }
  if(QMessageBox::question(this,"Davit",tr("Are you sure you want to delete this contact?"),QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
    return;
  }
  sql=QString().sprintf("delete from CONTACTS where ID=%d",item->id());
  q=new QSqlQuery(sql);
  delete q;
  delete item;
  */
}

/*
void ListContacts::doubleClickedData(QListViewItem *item,const QPoint &pt,int c)
{
  editData();
}
*/

void ListContacts::resizeEvent(QResizeEvent *e)
{
  //  list_contacts_list->setGeometry(0,0,size().width(),size().height()-40);
  list_add_button->setGeometry(10,size().height()-35,50,30);
  list_edit_button->setGeometry(70,size().height()-35,50,30);
  list_delete_button->setGeometry(130,size().height()-35,50,30);
}


void ListContacts::RefreshList()
{
  /*
  QString sql;
  QSqlQuery *q;
  DvtListViewItem *item=NULL;

  sql=QString().sprintf("select ID,NAME,TITLE,PHONE,FAX,EMAIL,AFFIDAVIT,\
                         PROGRAM_DIRECTOR,GENERAL_MANAGER,LOCKED \
                         from CONTACTS where AFFILIATE_ID=%d",list_id);
  q=new QSqlQuery(sql);
  while(q->next()) {
    item=new DvtListViewItem(list_contacts_list);
    item->setId(q->value(0).toInt());
    item->setText(0,q->value(1).toString());
    item->setText(1,q->value(2).toString());
    item->setText(2,DvtFormatPhoneNumber(q->value(3).toString()));
    item->setText(3,DvtFormatPhoneNumber(q->value(4).toString()));
    item->setText(4,q->value(5).toString());
    item->setText(5,q->value(6).toString());
    item->setText(6,q->value(7).toString());
    item->setText(7,q->value(8).toString());
    item->setText(8,q->value(9).toString());
  }
  delete q;
  */
}

/*
void ListContacts::UpdateItem(DvtListViewItem *item,Contact *contact)
{
  QString sql;
  QSqlQuery *q;

  if(item->id()>0) {
    sql=QString().sprintf("update CONTACTS set \
                           NAME=\"%s\",\
                           TITLE=\"%s\",\
                           PHONE=\"%s\",\
                           FAX=\"%s\",\
                           EMAIL=\"%s\",\
                           AFFIDAVIT=\"%s\",\
                           PROGRAM_DIRECTOR=\"%s\",\
                           GENERAL_MANAGER=\"%s\",\
                           LOCKED=\"%s\" \
                           where ID=%d",
			  (const char *)DvtEscapeString(contact->name()),
			  (const char *)DvtEscapeString(contact->title()),
			  (const char *)DvtEscapeString(contact->phone()),
			  (const char *)DvtEscapeString(contact->fax()),
			  (const char *)DvtEscapeString(contact->email()),
			  (const char *)DvtYesNo(contact->affidavit()),
			  (const char *)DvtYesNo(contact->programDirector()),
			  (const char *)DvtYesNo(contact->generalManager()),
			  (const char *)DvtYesNo(contact->locked()),
			  item->id());
    q=new QSqlQuery(sql);
    delete q;
  }
  else {
    sql=QString().sprintf("insert into CONTACTS set \
                           NAME=\"%s\",\
                           TITLE=\"%s\",\
                           PHONE=\"%s\",\
                           FAX=\"%s\",\
                           EMAIL=\"%s\",\
                           AFFIDAVIT=\"%s\",\
                           PROGRAM_DIRECTOR=\"%s\",\
                           GENERAL_MANAGER=\"%s\",\
                           LOCKED=\"%s\",\
                           AFFILIATE_ID=%d",
			  (const char *)DvtEscapeString(contact->name()),
			  (const char *)DvtEscapeString(contact->title()),
			  (const char *)DvtEscapeString(contact->phone()),
			  (const char *)DvtEscapeString(contact->fax()),
			  (const char *)DvtEscapeString(contact->email()),
			  (const char *)DvtYesNo(contact->affidavit()),
			  (const char *)DvtYesNo(contact->programDirector()),
			  (const char *)DvtYesNo(contact->generalManager()),
			  (const char *)DvtYesNo(contact->locked()),
			  list_id);
    printf("SQL: %s\n",(const char *)sql);
    q=new QSqlQuery(sql);
    delete q;
    sql="select LAST_INSERT_ID() from CONTACTS";
    q=new QSqlQuery(sql);
    if(q->first()) {
      item->setId(q->value(0).toInt());
    }
    delete q;
  }
  item->setText(0,contact->name());
  item->setText(1,contact->title());
  item->setText(2,DvtFormatPhoneNumber(contact->phone()));
  item->setText(3,DvtFormatPhoneNumber(contact->fax()));
  item->setText(4,contact->email());
  item->setText(5,DvtYesNo(contact->affidavit()));
  item->setText(6,DvtYesNo(contact->programDirector()));
  item->setText(7,DvtYesNo(contact->generalManager()));
  item->setText(8,DvtYesNo(contact->locked()));
}
*/
