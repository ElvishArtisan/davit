// list_airings.cpp
//
// List Davit Airings.
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: list_airings.cpp,v 1.2 2011/03/14 21:07:36 pcvs Exp $
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

#include <qsqldatabase.h>
#include <qmessagebox.h>

#include <dvtconf.h>

#include <globals.h>
#include <edit_airing.h>
#include <list_airings.h>

ListAirings::ListAirings(int id,QWidget *parent,const char *name)
  : QDialog(parent,name,false)
{
  list_id=id;
  setCaption(tr("Davit - Program List for ")+DvtStationCallString(id));

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
  // Airings List
  //
  list_airings_list=new QListView(this,"list_airings_list");
  list_airings_list->setGeometry(10,489,sizeHint().width()-80,100);
  list_airings_list->setAllColumnsShowFocus(true);
  list_airings_list->setItemMargin(5);
  connect(list_airings_list,
	  SIGNAL(doubleClicked(QListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(QListViewItem *,const QPoint &,int)));
  list_airings_list->addColumn(tr("Program"));
  list_airings_list->setColumnAlignment(0,AlignLeft);
  list_airings_list->addColumn(tr("Start Time"));
  list_airings_list->setColumnAlignment(1,AlignCenter);
  list_airings_list->addColumn(tr("End Time"));
  list_airings_list->setColumnAlignment(2,AlignCenter);
  list_airings_list->addColumn(tr("Su"));
  list_airings_list->setColumnAlignment(3,AlignCenter);
  list_airings_list->addColumn(tr("Mn"));
  list_airings_list->setColumnAlignment(4,AlignCenter);
  list_airings_list->addColumn(tr("Tu"));
  list_airings_list->setColumnAlignment(5,AlignCenter);
  list_airings_list->addColumn(tr("We"));
  list_airings_list->setColumnAlignment(6,AlignCenter);
  list_airings_list->addColumn(tr("Th"));
  list_airings_list->setColumnAlignment(7,AlignCenter);
  list_airings_list->addColumn(tr("Fr"));
  list_airings_list->setColumnAlignment(8,AlignCenter);
  list_airings_list->addColumn(tr("Sa"));
  list_airings_list->setColumnAlignment(9,AlignCenter);

  list_add_button=new QPushButton(this,"list_addfeed_button");
  list_add_button->setGeometry(sizeHint().width()-60,489,50,30);
  list_add_button->setFont(label_font);
  list_add_button->setText(tr("Add"));
  list_add_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateSchedule));
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  list_edit_button=new QPushButton(this,"list_editfeed_button");
  list_edit_button->setGeometry(sizeHint().width()-60,524,50,30);
  list_edit_button->setFont(label_font);
  list_edit_button->setText(tr("Edit"));
  list_edit_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateSchedule));
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  list_delete_button=new QPushButton(this,"list_deletefeed_button");
  list_delete_button->setGeometry(sizeHint().width()-60,559,50,30);
  list_delete_button->setFont(label_font);
  list_delete_button->setText(tr("Delete"));
  list_delete_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateSchedule));
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  RefreshList();
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


void ListAirings::addData()
{
  QString sql;
  QSqlQuery *q;
  DvtFeed feed;
  int program_id=-1;
  EditAiring *edit_airing=new EditAiring(&feed,this);
  if(edit_airing->exec()<0) {
    delete edit_airing;
    return;
  }
  program_id=feed.addToAffiliate(list_id);
  DvtListViewItem *item=new DvtListViewItem(list_airings_list);
  RefreshItem(item,&feed);
  list_airings_list->ensureItemVisible(item);

  //
  // Add Remark
  //
  sql=QString().sprintf("insert into AFFILIATE_REMARKS set \
                         AFFILIATE_ID=%d,\
                         PROGRAM_ID=%d,\
                         EVENT_TYPE=%d,\
                         REMARK_DATETIME=now(),\
                         USER_NAME=\"%s\",\
                         REMARK=\"Added an airing of %s.\"",
			list_id,
			program_id,
			Dvt::RemarkProgramAdd,
			(const char *)
			DvtEscapeString(global_dvtuser->name()),
			(const char *)
			DvtEscapeString(feed.name()));
  q=new QSqlQuery(sql);
  delete q;
  delete edit_airing;
  emit remarkAdded();
}


void ListAirings::editData()
{
  DvtListViewItem *item=(DvtListViewItem *)list_airings_list->selectedItem();
  if(item==NULL) {
    return;
  }
  DvtFeed feed;
  feed.setName(item->text(0));
  feed.setStartTime(item->text(1));
  feed.setLength(item->text(2));
  feed.setDowActive(7,!item->text(3).isEmpty());
  feed.setDowActive(1,!item->text(4).isEmpty());
  feed.setDowActive(2,!item->text(5).isEmpty());
  feed.setDowActive(3,!item->text(6).isEmpty());
  feed.setDowActive(4,!item->text(7).isEmpty());
  feed.setDowActive(5,!item->text(8).isEmpty());
  feed.setDowActive(6,!item->text(9).isEmpty());
  EditAiring *edit_airing=new EditAiring(&feed,this);
  if(edit_airing->exec()<0) {
    delete edit_airing;
    return;
  }
  feed.modifyAffiliate(item->id());
  RefreshItem(item,&feed);
  delete edit_airing;
}


void ListAirings::deleteData()
{
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
  int program_id=-1;
  DvtListViewItem *item=(DvtListViewItem *)list_airings_list->selectedItem();
  if(item==NULL) {
    return;
  }
  if(QMessageBox::question(this,tr("Davit - Delete Airing?"),
		 tr("Are you sure you want to deleting this airing of \"")+
			   item->text(0)+"\"?",
			   QMessageBox::Yes,QMessageBox::No)!=
     QMessageBox::Yes) {
    return;
  }
  sql=QString().
    sprintf("select PROGRAM_ID from AIRINGS where ID=%d",item->id());
  q=new QSqlQuery(sql);
  if(q->first()) {
    program_id=q->value(0).toInt();
  }
  delete q;

  sql=QString().sprintf("delete from AIRINGS where ID=%d",item->id());
  q=new QSqlQuery(sql);
  delete q;

  sql=QString().
    sprintf("select ID from AIRINGS where AFFILIATE_ID=%d",list_id);
  q=new QSqlQuery(sql);
  if(!q->first()) {
    sql=QString().sprintf("update AFFILIATES set IS_AFFILIATE=\"N\",\
                           AFFIDAVIT_ACTIVE=\"N\" where ID=%d",list_id);
    q1=new QSqlQuery(sql);
    delete q1;
  }
  delete q;

  //
  // Add Remark
  //
  sql=QString().sprintf("insert into AFFILIATE_REMARKS set \
                         AFFILIATE_ID=%d,\
                         PROGRAM_ID=%d,\
                         EVENT_TYPE=%d,\
                         REMARK_DATETIME=now(),\
                         USER_NAME=\"%s\",\
                         REMARK=\"Deleted an airing of %s.\"",
			list_id,
			program_id,
			Dvt::RemarkProgramDelete,
			(const char *)
			DvtEscapeString(global_dvtuser->name()),
			(const char *)
			DvtEscapeString(item->text(0)));
  q=new QSqlQuery(sql);
  delete q;
  delete item;
  emit remarkAdded();
}


void ListAirings::doubleClickedData(QListViewItem *item,const QPoint &pt,int c)
{
  editData();
}


void ListAirings::resizeEvent(QResizeEvent *e)
{
  list_airings_list->setGeometry(0,0,size().width(),size().height()-40);
  list_add_button->setGeometry(10,size().height()-35,50,30);
  list_edit_button->setGeometry(70,size().height()-35,50,30);
  list_delete_button->setGeometry(130,size().height()-35,50,30);
}


void ListAirings::RefreshList()
{
  QString sql;
  QSqlQuery *q;

  list_airings_list->clear();
  DvtListViewItem *item=NULL;
  sql=QString().sprintf("select PROGRAMS.PROGRAM_NAME,\
                         AIRINGS.AIR_TIME,\
                         AIRINGS.AIR_LENGTH,\
                         AIRINGS.AIR_SUN,\
                         AIRINGS.AIR_MON,\
                         AIRINGS.AIR_TUE,\
                         AIRINGS.AIR_WED,\
                         AIRINGS.AIR_THU,\
                         AIRINGS.AIR_FRI,\
                         AIRINGS.AIR_SAT,\
                         AIRINGS.ID from PROGRAMS left join AIRINGS \
                         on PROGRAMS.ID=AIRINGS.PROGRAM_ID \
                         where AIRINGS.AFFILIATE_ID=%d",
			list_id);
  q=new QSqlQuery(sql);
  while(q->next()) {
    item=new DvtListViewItem(list_airings_list);
    item->setId(q->value(10).toInt());
    item->setText(0,q->value(0).toString());
    item->setText(1,q->value(1).toTime().toString("hh:mm:ss"));
    QTime t=q->value(1).toTime().addSecs(q->value(2).toInt());
    if(t==QTime(23,59,59)) {
      t=QTime(0,0,0);
    }
    item->setText(2,t.toString("hh:mm:ss"));
    if(q->value(3).toString()=="Y") {
      item->setText(3,tr("Su"));
    }
    if(q->value(4).toString()=="Y") {
      item->setText(4,tr("Mo"));
    }
    if(q->value(5).toString()=="Y") {
      item->setText(5,tr("Tu"));
    }
    if(q->value(6).toString()=="Y") {
      item->setText(6,tr("We"));
    }
    if(q->value(7).toString()=="Y") {
      item->setText(7,tr("Th"));
    }
    if(q->value(8).toString()=="Y") {
      item->setText(8,tr("Fr"));
    }
    if(q->value(9).toString()=="Y") {
      item->setText(9,tr("Sa"));
    }
  }
  delete q;
}


void ListAirings::RefreshItem(DvtListViewItem *item,DvtFeed *airing)
{
  item->setText(0,airing->name());
  item->setText(1,airing->startTime().toString("hh:mm:ss"));
  item->setText(2,airing->startTime().
		addSecs(airing->length()).toString("hh:mm:ss"));
  if(airing->dowActive(7)) {
    item->setText(3,tr("Su"));
  }
  else {
    item->setText(3,"");
  }
  if(airing->dowActive(1)) {
    item->setText(4,tr("Mo"));
  }
  else {
    item->setText(4,"");
  }
  if(airing->dowActive(2)) {
    item->setText(5,tr("Tu"));
  }
  else {
    item->setText(5,"");
  }
  if(airing->dowActive(3)) {
    item->setText(6,tr("We"));
  }
  else {
    item->setText(6,"");
  }
  if(airing->dowActive(4)) {
    item->setText(7,tr("Th"));
  }
  else {
    item->setText(7,"");
  }
  if(airing->dowActive(5)) {
    item->setText(8,tr("Fr"));
  }
  else {
    item->setText(8,"");
  }
  if(airing->dowActive(6)) {
    item->setText(9,tr("Sa"));
  }
  else {
    item->setText(9,"");
  }
}
