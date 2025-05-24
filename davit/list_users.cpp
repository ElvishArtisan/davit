// list_users.cpp
//
// List Davit Users.
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

#include <dvtconfig.h>
#include <dvtconf.h>

#include "add_user.h"
#include "edit_user.h"
#include "globals.h"
#include "list_users.h"

ListUsers::ListUsers(QWidget *parent)
  : QDialog(parent)
{
  setModal(true);

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setWindowTitle("Davit - Users");

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Users List
  //
  /*
  list_users_list=new QListView(this);
  list_users_list->setMargin(5);
  list_users_list->setAllColumnsShowFocus(true);
  connect(list_users_list,
	  SIGNAL(doubleClicked(QListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(QListViewItem *,const QPoint &,int)));
  list_users_list->addColumn("Full Name");
  list_users_list->setColumnAlignment(0,Qt::AlignLeft|Qt::AlignVCenter);
  list_users_list->addColumn("User Name");
  list_users_list->setColumnAlignment(1,Qt::AlignLeft|Qt::AlignVCenter);
  list_users_list->addColumn("Description");
  list_users_list->setColumnAlignment(2,Qt::AlignLeft|Qt::AlignVCenter);
  list_users_list->addColumn("Phone Number");
  list_users_list->setColumnAlignment(3,Qt::AlignCenter);
  list_users_list->addColumn("E-Mail Address");
  list_users_list->setColumnAlignment(4,Qt::AlignCenter);
  list_users_list->addColumn("Administrator");
  list_users_list->setColumnAlignment(5,Qt::AlignCenter);
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


ListUsers::~ListUsers()
{
}


QSize ListUsers::sizeHint() const
{
  return QSize(640,480);
} 


QSizePolicy ListUsers::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListUsers::addData()
{
  /*
  QString lname;
  QString sql;
  QSqlQuery *q;

  AddUser *add=new AddUser(&lname,this);
  if(add->exec()==0) {
    sql=QString().sprintf("insert into USERS set USER_NAME=\"%s\",\
                           USER_PASSWORD=password(\"\")",
			  (const char *)lname);
    q=new QSqlQuery(sql);
    delete q;
    EditUser *edit=new EditUser(lname,this,"edit");
    if(edit->exec()==0) {
      QListViewItem *item=new QListViewItem(list_users_list);
      item->setText(1,lname);
      UpdateItem(item);
      list_users_list->setSelected(item,true);
      list_users_list->ensureItemVisible(item);
    }
    else {
      DeleteUser(lname);
    }
    delete edit;
  }
  delete add;
  */
}


void ListUsers::editData()
{
  /*
  QListViewItem *item=list_users_list->selectedItem();
  
  if(item==NULL) {
    return;
  }
  EditUser *edit=new EditUser(item->text(1),this,"edit");
  if(edit->exec()==0) {
    UpdateItem(item);
  }
  delete edit;
  */
}


void ListUsers::deleteData()
{
  /*
  QListViewItem *item=list_users_list->selectedItem();

  if(item==NULL) {
    return;
  }
  if(item->text(1)==global_dvtuser->name()) {
    QMessageBox::information(this,"Same User",
			     "You cannot delete the current user!");
    return;
  }
  if(QMessageBox::question(this,"Delete User",
      QString().sprintf("Are you sure you want to delete the user \"%s\"",
			   (const char *)item->text(0)),
			   QMessageBox::Yes,
			   QMessageBox::No)==QMessageBox::No) {
    return;
  }
  DeleteUser(item->text(1));
  delete item;
  */
}

/*
void ListUsers::doubleClickedData(QListViewItem *item,const QPoint &pt,
				      int c)
{
  editData();
}
*/

void ListUsers::closeData()
{
  done(0);
}


void ListUsers::resizeEvent(QResizeEvent *e)
{
  //  list_users_list->setGeometry(10,10,size().width()-20,size().height()-80);
  list_add_button->setGeometry(10,size().height()-60,80,50);
  list_edit_button->setGeometry(100,size().height()-60,80,50);
  list_delete_button->setGeometry(190,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListUsers::DeleteUser (QString lname)
{
  QString sql;
  QSqlQuery *q;

  sql=QString::asprintf("delete from USERS where USER_NAME=\"%s\"",
			lname.toUtf8().constData());
  q=new QSqlQuery(sql);
  delete q;
}


void ListUsers::RefreshList()
{
  /*
  QSqlQuery *q;
  QListViewItem *item;

  list_users_list->clear();
  q=new QSqlQuery("select FULL_NAME,USER_NAME,DESCRIPTION,PHONE_NUMBER,\
                   EMAIL,ADMIN_PRIV from USERS");
  while (q->next()) {
    item=new QListViewItem(list_users_list);
    item->setText(0,q->value(0).toString());
    item->setText(1,q->value(1).toString());
    item->setText(2,q->value(2).toString());
    item->setText(3,DvtFormatPhoneNumber(q->value(3).toString()));
    item->setText(4,q->value(4).toString());
    if(q->value(5).toString().lower()=="y") {
      item->setText(5,tr("Yes"));
    }
    else {
      item->setText(5,tr("No"));
    }
  }
  delete q;
  */
}

/*
void ListUsers::UpdateItem(QListViewItem *item)
{
  QSqlQuery *q=new 
    QSqlQuery(QString::asprintf("select FULL_NAME,DESCRIPTION,PHONE_NUMBER,\
                                 EMAIL,ADMIN_PRIV from USERS \
                                 where USER_NAME=\"%s\"",
				(const char *)item->text(1)));
  if(q->first()) {
    item->setText(0,q->value(0).toString());
    item->setText(2,q->value(1).toString());
    item->setText(3,DvtFormatPhoneNumber(q->value(2).toString()));
    item->setText(4,q->value(3).toString());
    if(q->value(4).toString().lower()=="y") {
      item->setText(5,tr("Yes"));
    }
    else {
      item->setText(5,tr("No"));
    }
  }
  delete q;
}
*/
