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
#include <QSqlDatabase>
#include <QSqlQuery>

#include <dvtconfig.h>

#include "add_program.h"
#include "edit_program.h"
#include "generate_affadavit.h"
#include "globals.h"
#include "list_programs.h"

ListPrograms::ListPrograms(int provider_id,QWidget *parent)
  : QDialog(parent)
{
  setModal(true);
  list_provider_id=provider_id;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setWindowTitle("Davit - Programs");

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Programs List
  //
  /*
  list_programs_list=new QListView(this);
  list_programs_list->setItemMargin(5);
  list_programs_list->setAllColumnsShowFocus(true);
  connect(list_programs_list,
	  SIGNAL(doubleClicked(QListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(QListViewItem *,const QPoint &,int)));
  list_programs_list->addColumn("Program Name");
  list_programs_list->setColumnAlignment(0,AlignLeft|AlignVCenter);
  list_programs_list->addColumn("Provider");
  list_programs_list->setColumnAlignment(1,AlignLeft|AlignVCenter);
  list_programs_list->addColumn("Contact");
  list_programs_list->setColumnAlignment(2,AlignCenter);
  list_programs_list->addColumn("Phone");
  list_programs_list->setColumnAlignment(3,AlignLeft|AlignVCenter);
  list_programs_list->addColumn("E-Mail");
  list_programs_list->setColumnAlignment(4,AlignLeft|AlignVCenter);
  */
  //
  //  Add Button
  //
  list_add_button=new QPushButton(this);
  list_add_button->setFont(font);
  list_add_button->setText("&Add");
  list_add_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivProgramEdit));
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
    setEnabled(global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  //  Affidavit Button
  //
  list_affadavit_button=new QPushButton(this);
  list_affadavit_button->setFont(font);
  list_affadavit_button->setText("&Generate\nAffidavit");
  list_affadavit_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivReportView));
  connect(list_affadavit_button,SIGNAL(clicked()),this,SLOT(affadavitData()));

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
	DvtListViewItem *item=new DvtListViewItem(list_programs_list);
	item->setText(0,pname);
	UpdateItem(item);
	list_programs_list->setSelected(item,true);
	list_programs_list->ensureItemVisible(item);
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
  /*
  DvtListViewItem *item=(DvtListViewItem *)list_programs_list->selectedItem();
  
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
  DvtListViewItem *item=(DvtListViewItem *)list_programs_list->selectedItem();

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

/*
void ListPrograms::doubleClickedData(QListViewItem *item,const QPoint &pt,
				      int c)
{
  editData();
}
*/

void ListPrograms::closeData()
{
  done(0);
}


void ListPrograms::resizeEvent(QResizeEvent *e)
{
  //  list_programs_list->
  //    setGeometry(10,10,size().width()-20,size().height()-80);
  list_add_button->setGeometry(10,size().height()-60,80,50);
  list_edit_button->setGeometry(100,size().height()-60,80,50);
  list_delete_button->setGeometry(190,size().height()-60,80,50);
  list_affadavit_button->setGeometry(380,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListPrograms::DeleteProgram(int pid)
{
  QString sql;
  QSqlQuery *q;

  sql=QString().sprintf("delete from PROGRAMS where ID=%d",pid);
  q=new QSqlQuery(sql);
  delete q;

  sql=QString().sprintf("delete from FEEDS where PROGRAM_ID=%d",pid);
  q=new QSqlQuery(sql);
  delete q;

  sql=QString().sprintf("delete from AIRINGS where PROGRAM_ID=%d",pid);
  q=new QSqlQuery(sql);
  delete q;

  sql=QString().sprintf("delete from AIRED where PROGRAM_ID=%d",pid);
  q=new QSqlQuery(sql);
  delete q;
}


void ListPrograms::RefreshList()
{
  /*
  QString sql;
  QSqlQuery *q;
  DvtListViewItem *item;

  list_programs_list->clear();
  sql="select PROGRAMS.PROGRAM_NAME,PROVIDERS.BUSINESS_NAME,\
              PROGRAMS.CONTACT_NAME,PROGRAMS.CONTACT_PHONE,\
              PROGRAMS.CONTACT_EMAIL,PROGRAMS.ID \
              from PROVIDERS left join PROGRAMS \
              on PROVIDERS.ID=PROGRAMS.PROVIDER_ID";
  if(list_provider_id>=0) {
    sql+=QString().sprintf(" where PROGRAMS.PROVIDER_ID=%d",list_provider_id);
  }
  q=new QSqlQuery(sql);
  while (q->next()) {
    item=new DvtListViewItem(list_programs_list);
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


void ListPrograms::UpdateItem(DvtListViewItem *item)
{
  /*
  QString sql;
  QSqlQuery *q;

  sql=QString().sprintf("select CONTACT_NAME,CONTACT_PHONE,CONTACT_EMAIL,ID \
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
  */
}
