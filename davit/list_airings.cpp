// list_airings.cpp
//
// List Davit Airings.
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
#include <QSqlDatabase>

#include <dvtconf.h>

#include "edit_airing.h"
#include "globals.h"
#include "list_airings.h"

ListAirings::ListAirings(DvtConfig *c,QWidget *parent)
  : DvtDialog(c,parent)
{
  setModal(false);

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  //
  // Edit Contact Dialog
  //
  list_editairing_dialog=new EditAiring(config,this);

  //
  // Airings List
  //
  list_airings_view=new DvtTableView(this);
  list_airings_model=new AiringListModel(this);
  list_airings_model->setFont(defaultFont());
  list_airings_model->setPalette(palette());
  list_airings_view->setModel(list_airings_model);
  connect(list_airings_view,SIGNAL(doubleClicked(const QModelIndex &)),
	  this,SLOT(doubleClickedData(const QModelIndex &)));
  connect(list_airings_view->selectionModel(),
     SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)),
     this,
     SLOT(selectionChangedData(const QItemSelection &,const QItemSelection &)));

  list_add_button=new QPushButton(this);
  list_add_button->setFont(label_font);
  list_add_button->setText(tr("Add"));
  list_add_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateSchedule));
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  list_edit_button=new QPushButton(this);
  list_edit_button->setFont(label_font);
  list_edit_button->setText(tr("Edit"));
  list_edit_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateSchedule));
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));
  list_edit_button->setDisabled(true);

  list_delete_button=new QPushButton(this);
  list_delete_button->setFont(label_font);
  list_delete_button->setText(tr("Delete"));
  list_delete_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateSchedule));
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));
  list_delete_button->setDisabled(true);
}


ListAirings::~ListAirings()
{
}


QSize ListAirings::sizeHint() const
{
  return QSize(500,300);
}


void ListAirings::show()
{
  QDialog::show();
  emit visibilityChanged(true);
}


void ListAirings::hide()
{
  QDialog::hide();
  emit visibilityChanged(false);
}


QSizePolicy ListAirings::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListAirings::setAffiliateId(int id)
{
  list_affiliate_id=id;
  setWindowTitle("Davit - "+tr("Program List for")+" "+
		 DvtStationCallString(id));
  list_airings_model->setAffiliateId(id);
  list_airings_view->resizeColumnsToContents();
}


void ListAirings::addData()
{
  QString sql=QString("insert into `AIRINGS` set ")+
    QString::asprintf("`AFFILIATE_ID`=%d,",list_affiliate_id)+
    "`PROGRAM_ID`=0 ";
  int airing_id=DvtSqlQuery::run(sql).toInt();
  if(list_editairing_dialog->exec(airing_id,true)) {
    QModelIndex index=list_airings_model->addAiring(airing_id);
    if(index.isValid()) {
      list_airings_view->
	selectionModel()->select(index,QItemSelectionModel::ClearAndSelect|
				 QItemSelectionModel::Rows);
    }
  }
  else {
    sql=QString("delete from `AIRINGS` where ")+
      QString::asprintf("`ID`=%d ",airing_id);
    DvtSqlQuery::apply(sql);
  }
}


void ListAirings::editData()
{
  QModelIndexList rows=list_airings_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  if(list_editairing_dialog->exec(list_airings_model->airingId(rows.first()))) {
    list_airings_model->refresh(rows.first());
  }
}


void ListAirings::deleteData()
{
  QModelIndexList rows=list_airings_view->selectionModel()->selectedRows();
  int program_id=-1;

  if(rows.size()!=1) {
    return;
  }
  if(QMessageBox::question(this,
	     "Davit",tr("Are you sure you want to delete the airing for")+
			   " \""+
			   list_airings_model->programName(rows.first())+
			   "\"?",
		 QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
    return;
  }

  //
  // Get Program ID
  //
  QString sql=QString("select ")+
    "`PROGRAM_ID` "+  // 00
    "from `AIRINGS` where "+
    QString::asprintf("`ID`=%d ",list_airings_model->airingId(rows.first()));
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  if(q->first()) {
    program_id=q->value(0).toInt();
  }
  delete q;
  
  //
  // Add Remark
  //
  sql=QString("insert into `AFFILIATE_REMARKS` set ")+
    QString::asprintf("`AFFILIATE_ID`=%d,",list_affiliate_id)+
    QString::asprintf("`PROGRAM_ID`=%d,",program_id)+
    QString::asprintf("`EVENT_TYPE`=%d,",Dvt::RemarkProgramDelete)+
    "`REMARK_DATETIME`=now(),"+
    "`USER_NAME`="+DvtSqlQuery::escape(global_dvtuser->name())+","+
    "`REMARK`="+DvtSqlQuery::escape(tr("Deleted an airing of")+" "+
			   list_airings_model->programName(rows.first())+".");
  DvtSqlQuery::apply(sql);

  //
  // Delete Airing Record
  //
  sql=QString("delete from `AIRINGS` where ")+
    QString::asprintf("`ID`=%d",list_airings_model->airingId(rows.first()));
  DvtSqlQuery::apply(sql);
  list_airings_model->removeAiring(rows.first());

  emit remarkAdded();
}


void ListAirings::doubleClickedData(const QModelIndex &index)
{
  editData();
}


void ListAirings::selectionChangedData(const QItemSelection &now,
				       const QItemSelection &prev)
{
  QModelIndexList rows=list_airings_view->selectionModel()->selectedRows();

  list_edit_button->setEnabled(rows.size()==1);
  list_delete_button->setEnabled(rows.size()==1);
}


void ListAirings::resizeEvent(QResizeEvent *e)
{
  list_airings_view->setGeometry(0,0,size().width(),size().height()-40);
  list_add_button->setGeometry(10,size().height()-35,50,30);
  list_edit_button->setGeometry(70,size().height()-35,50,30);
  list_delete_button->setGeometry(130,size().height()-35,50,30);
}


void ListAirings::closeEvent(QCloseEvent *)
{
  hide();
}
