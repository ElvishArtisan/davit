// migrate_affiliates.cpp
//
// Migrate a Davit Affiliates.
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

#include <QMessageBox>
#include <QValidator>

#include <globals.h>
#include <dvtconf.h>

#include "migrate_affiliates.h"

MigrateAffiliates::MigrateAffiliates(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  setModal(true);
  edit_program_id=-1;
  QString sql;
  DvtSqlQuery *q;

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());

  //
  // Source Program
  //
  edit_program_box=new QComboBox(this);
  edit_program_label=new QLabel(tr("Migrate Affiliates From:"),this);
  edit_program_label->setFont(labelFont());
  edit_program_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  connect(edit_program_box,SIGNAL(currentTextChanged(const QString &)),
  	  this,SLOT(programActivatedData(const QString &)));

  //
  // Affiliate Selector
  //
  edit_affiliates_sel=new ListSelector(config(),this);
  connect(edit_affiliates_sel,SIGNAL(destItemsQuantityChanged(int)),
	  this,SLOT(destItemsQuantityChangedData(int)));
  
  //
  //  OK Button
  //
  list_ok_button=new QPushButton(this);
  list_ok_button->setDefault(true);
  list_ok_button->setFont(buttonFont());
  list_ok_button->setText("&OK");
  connect(list_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel List_Ok_Button
  //
  list_cancel_button=new QPushButton(this);
  list_cancel_button->setFont(buttonFont());
  list_cancel_button->setText("&Cancel");
  connect(list_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Data
  //
  sql=QString("select ")+
    "`ID`,"+            // 00
    "`PROGRAM_NAME` "+  // 01
    "from `PROGRAMS` where "+
    QString::asprintf("`ID`!=%d	",edit_program_id)+
    "order by `PROGRAM_NAME`";
  int count=0;
  edit_program_box->insertItem(count++,tr("[select source program]"),-1);
  q=new DvtSqlQuery(sql);
  while(q->next()) {
    edit_program_box->
      insertItem(count++,q->value(1).toString(),q->value(0).toInt());
  }
  delete q;
}


MigrateAffiliates::~MigrateAffiliates()
{
}


QSize MigrateAffiliates::sizeHint() const
{
  return QSize(800,600);
} 


QSizePolicy MigrateAffiliates::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int MigrateAffiliates::exec(int pgm_id,const QString &pgm_name)
{
  setWindowTitle("Davit - "+tr("Migrate Affiliates")+
		 QString::asprintf(" [program_id: %d]",pgm_id));

  edit_program_id=pgm_id;
  edit_program_name=pgm_name;

  edit_affiliates_sel->destSetLabel(pgm_name);
  list_ok_button->setDisabled(true);

  return QDialog::exec();
}


void MigrateAffiliates::programActivatedData(const QString &str)
{
  QString sql;
  DvtSqlQuery *q;
  int pgm_id=0;

  edit_affiliates_sel->clear();
  edit_affiliates_sel->sourceSetLabel(str);
  if((pgm_id=GetProgramId(str))>0) {
    sql=QString("select ")+
      "`AFFILIATES`.`ID`,"+             // 00
      "`AFFILIATES`.`STATION_CALL`,"+   // 01
      "`AFFILIATES`.`STATION_TYPE`,"+   // 02
      "`AFFILIATES`.`LICENSE_CITY`,"+   // 03
      "`AFFILIATES`.`LICENSE_STATE`,"+  // 04
      "`AIRINGS`.`ID`,"+                // 05
      "`AIRINGS`.`AIR_TIME`,"+          // 06
      "`AIRINGS`.`AIR_MON`,"+           // 07
      "`AIRINGS`.`AIR_TUE`,"+           // 08
      "`AIRINGS`.`AIR_WED`,"+           // 09
      "`AIRINGS`.`AIR_THU`,"+           // 00
      "`AIRINGS`.`AIR_FRI`,"+           // 11
      "`AIRINGS`.`AIR_SAT`,"+           // 12
      "`AIRINGS`.`AIR_SUN` "+           // 13
      "from `AFFILIATES` left join `AIRINGS` "+
      "on `AFFILIATES`.`ID`=`AIRINGS`.`AFFILIATE_ID` where "+
      QString::asprintf("(`AIRINGS`.`PROGRAM_ID`=%d) ",pgm_id)+
      "order by `AFFILIATES`.`STATION_CALL`,`AFFILIATES`.`STATION_TYPE`";
    q=new DvtSqlQuery(sql);
    while(q->next()) {
      QString station=q->value(1).toString();
      if(q->value(2).toString().toLower()=="a") {
	station+="-AM";
      }
      if(q->value(2).toString().toLower()=="f") {
	station+="-FM";
      }
      station+=" ";
      station+=q->value(3).toString();
      station+=", ";
      station+=q->value(4).toString().toUpper();
      station+=" - ";
      station+=GetDowSummary(q)+" @ ";
      station+=q->value(6).toTime().toString("hh:mm:ss");
      edit_affiliates_sel->sourceInsertItem(q->value(5).toInt(),station);
    }
    delete q;
  }
}


void MigrateAffiliates::destItemsQuantityChangedData(int quan)
{
  list_ok_button->setDisabled(quan==0);
}


void MigrateAffiliates::okData()
{
  QString sql;
  int pgm_id=0;
  int affiliate_id=0;
  QString dt=QDateTime(QDate::currentDate(),QTime::currentTime()).
    toString("yyyy-MM-dd hh:mm:ss");

  if((pgm_id=edit_program_box->
      itemData(edit_program_box->currentIndex()).toInt())<=0) {
    return;
  }
  for(unsigned i=0;i<edit_affiliates_sel->destCount();i++) {
    affiliate_id=GetAffiliateId(edit_affiliates_sel->destText(i));
    sql=QString("insert into `AFFILIATE_REMARKS` set ")+
      "`REMARK_DATETIME`="+DvtSqlQuery::escape(dt)+","+
      QString::asprintf("`EVENT_TYPE`=%d,",Dvt::RemarkProgramDelete)+
      QString::asprintf("`AFFILIATE_ID`=%d,",affiliate_id)+
      QString::asprintf("`PROGRAM_ID`=%d,",pgm_id)+
      "`USER_NAME`="+DvtSqlQuery::escape(global_dvtuser->name())+","+
      "`REMARK`="+DvtSqlQuery::escape("Deleted an airing of "+
				      edit_program_box->currentText()+".");
    DvtSqlQuery::apply(sql);
    sql=QString("insert into `AFFILIATE_REMARKS` set ")+
      "`REMARK_DATETIME`="+DvtSqlQuery::escape(dt)+","+
      QString::asprintf("`EVENT_TYPE`=%d,",Dvt::RemarkProgramAdd)+
      QString::asprintf("`AFFILIATE_ID`=%d,",affiliate_id)+
      QString::asprintf("`PROGRAM_ID`=%d,",edit_program_id)+
      "`USER_NAME`="+DvtSqlQuery::escape(global_dvtuser->name())+","+
      "`REMARK`="+DvtSqlQuery::escape("Added an airing of "+
				      edit_program_name+".");
    DvtSqlQuery::apply(sql);
    sql=QString("update `AIRINGS` set ")+
      QString::asprintf("`PROGRAM_ID`=%d ",edit_program_id)+
      "where "+
      QString::asprintf("`ID`=%d",edit_affiliates_sel->destAiringId(i));
    DvtSqlQuery::apply(sql);
  }
  done(true);
}


void MigrateAffiliates::cancelData()
{
  done(false);
}


void MigrateAffiliates::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();
  
  edit_program_box->setGeometry(200,10,w-210,20);
  edit_program_label->setGeometry(10,10,185,20);
  edit_affiliates_sel->setGeometry(10,40,w-20,h-110);

  list_ok_button->setGeometry(w-180,h-60,80,50);
  list_cancel_button->setGeometry(w-90,h-60,80,50);
}


QString MigrateAffiliates::GetDowSummary(DvtSqlQuery *q) const
{
  QString ret="";
  int weekday_count=0;
  QLocale locale;

  for(int i=7;i<12;i++) {
    if(q->value(i).toString()=="Y") {
      ret+=locale.dayName(i,QLocale::ShortFormat)+", ";
      weekday_count++;
    }
  }
  if(weekday_count==5) {
    ret=tr("Weekdays")+", ";
  }
  if(q->value(12).toString()=="Y") {
    ret+=locale.dayName(6,QLocale::ShortFormat)+", ";
  }
  if(q->value(13).toString()=="Y") {
    ret+=locale.dayName(7,QLocale::ShortFormat)+", ";
  }

  return ret.left(ret.length()-2);
}


int MigrateAffiliates::GetProgramId(const QString &str) const
{
  QString sql;
  DvtSqlQuery *q;
  int id=0;
  sql=QString("select ")+
    "`ID` "+  // 00
    "from `PROGRAMS` where "+
    "`PROGRAM_NAME`="+DvtSqlQuery::escape(str);
  q=new DvtSqlQuery(sql);
  if(q->first()) {
    id=q->value(0).toInt();
  }
  delete q;
  return id;
}


int MigrateAffiliates::GetAffiliateId(const QString &str) const
{
  QString sql;
  DvtSqlQuery *q;
  int id=0;
  QString name=str.left(str.indexOf(" "));
  int n=name.indexOf("-");
  if(n<0) {
    sql=QString("select ")+
      "`ID` "+  // 00
      "from `AFFILIATES` where "+
      "`STATION_CALL`="+DvtSqlQuery::escape(name);
    q=new DvtSqlQuery(sql);
    if(q->first()) {
      id=q->value(0).toInt();
    }
    delete q;
    return id;
  }
  sql=QString("select ")+
    "`ID` "+  // 00
    "from `AFFILIATES` where "+
    "(`STATION_CALL`="+DvtSqlQuery::escape(name.left(n))+"&&"+
    "(`STATION_TYPE`="+DvtSqlQuery::escape(name.right(2).left(1).toLower());
  q=new DvtSqlQuery(sql);
  if(q->first()) {
    id=q->value(0).toInt();
  }
  delete q;
  return id;
}
