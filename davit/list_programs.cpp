// list_programs.cpp
//
// List Davit Programs.
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

#include "add_program.h"
#include "edit_program.h"
#include "generate_affadavit.h"
#include "globals.h"
#include "list_programs.h"

ListPrograms::ListPrograms(DvtConfig *c,QWidget *parent)
  : DvtDialog(c,parent)
{
  setModal(true);
  //  list_provider_id=provider_id;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  //
  // Dialogs
  //
  list_editprogram_dialog=new EditProgram(config,this);

  //
  // Programs List
  //
  list_programs_view=new DvtTableView(this);
  list_programs_model=new ProgramListModel(this);
  list_programs_model->setFont(defaultFont());
  list_programs_model->setPalette(palette());
  list_programs_view->setModel(list_programs_model);
  connect(list_programs_view,SIGNAL(doubleClicked(const QModelIndex &)),
	  this,SLOT(doubleClickedData(const QModelIndex &)));

  //
  //  Add Button
  //
  list_add_button=new QPushButton(this);
  list_add_button->setFont(buttonFont());
  list_add_button->setText(tr("Add"));
  list_add_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  //
  //  Edit Button
  //
  list_edit_button=new QPushButton(this);
  list_edit_button->setFont(buttonFont());
  list_edit_button->setText(tr("Edit"));
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  //  Delete Button
  //
  list_delete_button=new QPushButton(this);
  list_delete_button->setFont(buttonFont());
  list_delete_button->setText(tr("Delete"));
  list_delete_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  //  Affidavit Button
  //
  list_affadavit_button=new QPushButton(this);
  list_affadavit_button->setFont(buttonFont());
  list_affadavit_button->setText(tr("Generate")+"\n"+tr("Affidavit"));
  list_affadavit_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivReportView));
  connect(list_affadavit_button,SIGNAL(clicked()),this,SLOT(affadavitData()));

  //
  //  Close Button
  //
  list_close_button=new QPushButton(this);
  list_close_button->setDefault(true);
  list_close_button->setFont(buttonFont());
  list_close_button->setText(tr("Close"));
  connect(list_close_button,SIGNAL(clicked()),this,SLOT(closeData()));
}


ListPrograms::~ListPrograms()
{
}


QSize ListPrograms::sizeHint() const
{
  return QSize(640,480);
} 


QSizePolicy ListPrograms::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int ListPrograms::exec(int provider_id)
{
  list_provider_id=provider_id;

  setWindowTitle("Davit - "+tr("Programs"));
  list_programs_model->refresh(provider_id);
  list_programs_view->resizeColumnsToContents();

  return QDialog::exec();
}


void ListPrograms::addData()
{
  /*
  QString bname;
  QString pname;
  int pid=0;
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;

  AddProgram *add=new AddProgram(&bname,&pname,this,"add");
  if(add->exec()==0) {
    sql=
      QString().sprintf("select ID from PROVIDERS where BUSINESS_NAME=\"%s\"",
			(const char *)bname);
    q=new QSqlQuery(sql);
    if(q->next()) {
      sql=QString().sprintf("insert into PROGRAMS set PROVIDER_ID=%d,\
                             PROGRAM_NAME=\"%s\"",q->value(0).toInt(),
			    (const char *)pname);
      q1=new QSqlQuery(sql);
      delete q1;
      sql="select LAST_INSERT_ID() from PROGRAMS";
      q1=new QSqlQuery(sql);
      if(q1->first()) {
	pid=q1->value(0).toInt();
      }
      delete q1;
      EditProgram *edit=new EditProgram(pname,this,"edit");
      if(edit->exec()==0) {
	DvtListViewItem *item=new DvtListViewItem(list_programs_view);
	item->setText(0,pname);
	UpdateItem(item);
	list_programs_view->setSelected(item,true);
	list_programs_view->ensureItemVisible(item);
      }
      else {
	DeleteProgram(pid);
      }
      delete edit;
    }
  }
  delete add;
  */
}


void ListPrograms::editData()
{
  QModelIndexList rows=list_programs_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  if(list_editprogram_dialog->
     exec(list_programs_model->programId(rows.first()),false)) {
    list_programs_model->refresh(rows.first());
  }
  /*
  DvtListViewItem *item=(DvtListViewItem *)list_programs_view->selectedItem();
  
  if(item==NULL) {
    return;
  }
  EditProgram *edit=new EditProgram(item->text(0),this,"edit");
  if(edit->exec()==0) {
    UpdateItem(item);
  }
  delete edit;
  */
}


void ListPrograms::deleteData()
{
  /*
  DvtListViewItem *item=(DvtListViewItem *)list_programs_view->selectedItem();

  if(item==NULL) {
    return;
  }
  if(QMessageBox::question(this,"Delete Program",
      QString().sprintf("Are you sure you want to delete the program \"%s\"",
			   (const char *)item->text(0)),
			   QMessageBox::Yes,
			   QMessageBox::No)==QMessageBox::No) {
    return;
  }
  DeleteProgram(item->id());
  delete item;
  */
}


void ListPrograms::affadavitData()
{
  /*
  DvtListViewItem *item=
    (DvtListViewItem *)list_programs_view->selectedItem();
  
  if(item==NULL) {
    return;
  }
  GenerateAffadavit *edit=
    new GenerateAffadavit(GenerateAffadavit::ReportProgram,item->id(),
			  this,"edit");
  edit->exec();
  delete edit;
  */
}


void ListPrograms::doubleClickedData(const QModelIndex &index)
{
  editData();
}


void ListPrograms::closeData()
{
  done(true);
}


void ListPrograms::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  list_programs_view->setGeometry(10,10,w-20,h-80);
  list_add_button->setGeometry(10,h-60,80,50);
  list_edit_button->setGeometry(100,h-60,80,50);
  list_delete_button->setGeometry(190,h-60,80,50);
  list_affadavit_button->setGeometry(380,h-60,80,50);
  list_close_button->setGeometry(w-90,h-60,80,50);
}


void ListPrograms::DeleteProgram(int pid)
{
  QString sql;
  QSqlQuery *q;

  sql=QString::asprintf("delete from PROGRAMS where ID=%d",pid);
  q=new QSqlQuery(sql);
  delete q;

  sql=QString::asprintf("delete from FEEDS where PROGRAM_ID=%d",pid);
  q=new QSqlQuery(sql);
  delete q;

  sql=QString::asprintf("delete from AIRINGS where PROGRAM_ID=%d",pid);
  q=new QSqlQuery(sql);
  delete q;

  sql=QString::asprintf("delete from AIRED where PROGRAM_ID=%d",pid);
  q=new QSqlQuery(sql);
  delete q;
}


void ListPrograms::RefreshList()
{
  /*
  QString sql;
  QSqlQuery *q;
  DvtListViewItem *item;

  list_programs_view->clear();
  sql="select PROGRAMS.PROGRAM_NAME,PROVIDERS.BUSINESS_NAME,\
              PROGRAMS.CONTACT_NAME,PROGRAMS.CONTACT_PHONE,\
              PROGRAMS.CONTACT_EMAIL,PROGRAMS.ID \
              from PROVIDERS left join PROGRAMS \
              on PROVIDERS.ID=PROGRAMS.PROVIDER_ID";
  if(list_provider_id>=0) {
    sql+=QString::asprintf(" where PROGRAMS.PROVIDER_ID=%d",list_provider_id);
  }
  q=new QSqlQuery(sql);
  while (q->next()) {
    item=new DvtListViewItem(list_programs_view);
    item->setId(q->value(5).toInt());
    item->setText(0,q->value(0).toString());
    item->setText(1,q->value(1).toString());
    item->setText(2,q->value(2).toString());
    item->setText(3,q->value(3).toString());
    item->setText(4,q->value(4).toString());
  }
  delete q;
  */
}


/*
void ListPrograms::UpdateItem(DvtListViewItem *item)
{
  QString sql;
  QSqlQuery *q;

  sql=QString::asprintf("select CONTACT_NAME,CONTACT_PHONE,CONTACT_EMAIL,ID \
                         from PROGRAMS where PROGRAM_NAME=\"%s\"",
			(const char *)item->text(0));
  q=new QSqlQuery(sql);
  if(q->first()) {
    item->setId(q->value(3).toInt());
    item->setText(2,q->value(0).toString());
    item->setText(3,q->value(1).toString());
    item->setText(4,q->value(2).toString());
  }
  delete q;
}
*/
