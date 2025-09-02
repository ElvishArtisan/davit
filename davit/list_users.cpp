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

#include <QMessageBox>

#include <dvtconfig.h>
#include <dvtconf.h>

#include "add_user.h"
#include "edit_user.h"
#include "globals.h"
#include "list_users.h"

ListUsers::ListUsers(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());

  setWindowTitle("Davit - "+tr("List Users"));

  //
  // Dialogs
  //
  list_adduser_dialog=new AddUser(c,this);
  list_edituser_dialog=new EditUser(c,this);

  //
  // Users List
  //
  list_users_view=new TableView(this);
  list_users_model=new UserListModel(this);
  list_users_model->setFont(defaultFont());
  list_users_model->setPalette(palette());
  list_users_view->setModel(list_users_model);
  connect(list_users_view,SIGNAL(doubleClicked(const QModelIndex &)),
	  this,SLOT(doubleClickedData(const QModelIndex &)));
  connect(list_users_view->selectionModel(),
     SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)),
     this,
     SLOT(selectionChangedData(const QItemSelection &,const QItemSelection &)));

  //
  //  Add Button
  //
  list_add_button=new QPushButton(this);
  list_add_button->setFont(buttonFont());
  list_add_button->setText(tr("Add"));
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  //
  //  Edit Button
  //
  list_edit_button=new QPushButton(this);
  list_edit_button->setFont(buttonFont());
  list_edit_button->setText(tr("Edit"));
  list_edit_button->setDisabled(true);
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  //  Delete Button
  //
  list_delete_button=new QPushButton(this);
  list_delete_button->setFont(buttonFont());
  list_delete_button->setText(tr("Delete"));
  list_delete_button->setDisabled(true);
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  //  Close Button
  //
  list_close_button=new QPushButton(this);
  list_close_button->setDefault(true);
  list_close_button->setFont(buttonFont());
  list_close_button->setText(tr("Close"));
  connect(list_close_button,SIGNAL(clicked()),this,SLOT(closeData()));

  list_users_model->refresh();
  list_users_view->resizeColumnsToContents();
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
  QString username;
  QString sql;

  if(list_adduser_dialog->exec(&username)) {
    sql=QString("insert into `USERS` set ")+
      "`USER_NAME`="+DvtSqlQuery::escape(username)+","+
      "`USER_PASSWORD`=password('')";
    DvtSqlQuery::apply(sql);
    if(list_edituser_dialog->exec(username)) {
      QModelIndex index=list_users_model->addUser(username);
      if(index.isValid()) {
	list_users_view->selectRow(index.row());
	list_users_view->scrollTo(index,QAbstractItemView::PositionAtCenter);
      }
    }
    else {
      sql=QString("delete from `USERS` where ")+
	"`USER_NAME`="+DvtSqlQuery::escape(username);
      DvtSqlQuery::apply(sql);
    }
  }
}


void ListUsers::editData()
{
  QModelIndexList rows=list_users_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  if(list_edituser_dialog->exec(list_users_model->userName(rows.first()))) {
    list_users_model->refresh(rows.first());
  }
}


void ListUsers::deleteData()
{
  QString sql;
  QString warning;
  QString str;
  QModelIndexList rows=list_users_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  QString username=list_users_model->userName(rows.first());
  if(global_dvtuser->name()==username) {
    QMessageBox::warning(this,"Davit - "+tr("Delete User"),
			 tr("You cannot delete yourself!"));
    return;
  }
  if(QMessageBox::question(this,"Davit - "+tr("Delete User"),
			   tr("Are you sure you want to delete the user")+
			   " \""+username+"\"?",
			   QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
    return;
  }
  sql=QString("delete from `USERS` where ")+
    "`USER_NAME`="+DvtSqlQuery::escape(username);
  DvtSqlQuery::apply(sql);
  list_users_model->removeUser(rows.first());
}


void ListUsers::doubleClickedData(const QModelIndex &index)
{
  editData();
}


void ListUsers::selectionChangedData(const QItemSelection &before,
					  const QItemSelection &after)
{
  QModelIndexList rows=list_users_view->selectionModel()->selectedRows();

  list_edit_button->setDisabled(rows.size()!=1);
  list_delete_button->setDisabled(rows.size()!=1);
}


void ListUsers::closeData()
{
  done(0);
}


void ListUsers::resizeEvent(QResizeEvent *e)
{
  list_users_view->setGeometry(10,10,size().width()-20,size().height()-80);
  list_add_button->setGeometry(10,size().height()-60,80,50);
  list_edit_button->setGeometry(100,size().height()-60,80,50);
  list_delete_button->setGeometry(190,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}
