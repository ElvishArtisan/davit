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
#include "globals.h"
#include "list_programs.h"

ListPrograms::ListPrograms(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
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
  list_generateaffadavit_dialog=new GenerateAffadavit(c,this);

  //
  // Programs List
  //
  list_programs_view=new TableView(this);
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
  list_affidavit_button=
    new QPushButton(tr("Generate")+"\n"+tr("Affidavit"),this);
  list_affidavit_button->setFont(buttonFont());
  list_affidavit_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivReportView));
  connect(list_affidavit_button,SIGNAL(clicked()),this,SLOT(affidavitData()));
  list_affidavit_button->hide();  // Disabled on 29 July 2025 as per mparadiso
  
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
  QString sql=QString("insert into `PROGRAMS` set ")+
    QString::asprintf("`PROVIDER_ID`=%d,",list_provider_id)+
    "`PROGRAM_NAME`="+DvtSqlQuery::escape(tr("[new program]"));
  int program_id=DvtSqlQuery::run(sql).toInt();
  if(list_editprogram_dialog->exec(program_id,true)) {
    QModelIndex index=list_programs_model->addProgram(program_id);
    if(index.isValid()) {
      list_programs_view->
	selectionModel()->select(index,QItemSelectionModel::ClearAndSelect|
				 QItemSelectionModel::Rows);
    }
  }
  else {
    sql=QString("delete from `PROGRAMS` where ")+
      QString::asprintf("`PROGRAMS`.`ID`=%d ",program_id);
    DvtSqlQuery::apply(sql);
  }
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
}


void ListPrograms::deleteData()
{
  QModelIndexList rows=list_programs_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  if(QMessageBox::question(this,
			   "Davit",
     tr("Are you sure you want to delete the program")+
			   " \""+
			   list_programs_model->programName(rows.first())+
			   "\"?\n\n"+
     tr("This will delete all affidavit records associated with this program."),
		 QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
    return;
  }
  int program_id=list_programs_model->programId(rows.first());

  QString sql=QString("delete from `FEEDS` where ")+
    QString::asprintf("`PROGRAM_ID`=%d",program_id);
  DvtSqlQuery::apply(sql);

  sql=QString("delete from `AIRINGS` where ")+
    QString::asprintf("`PROGRAM_ID`=%d",program_id);
  DvtSqlQuery::apply(sql);

  sql=QString("delete from `AIRED` where ")+
    QString::asprintf("`PROGRAM_ID`=%d",program_id);
  DvtSqlQuery::apply(sql);

  sql=QString("delete from `PROGRAMS` where ")+
    QString::asprintf("`PROGRAMS`.`ID`=%d ",program_id);
  DvtSqlQuery::apply(sql);
  list_programs_model->removeProgram(rows.first());
}


void ListPrograms::affidavitData()
{
  QModelIndexList rows=list_programs_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  list_generateaffadavit_dialog->
    execProgram(list_programs_model->programId(rows.first()));


  /*
  DvtListViewItem *item=
    (DvtListViewItem *)list_programs_list->selectedItem();
  
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
  list_affidavit_button->setGeometry(390,h-60,80,50);
  list_close_button->setGeometry(w-90,h-60,80,50);
}
