// pick_fields.cpp
//
// Generate a Davit Report
//
//   (C) Copyright 2008-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <QLabel>
#include <QMessageBox>

#include <math.h>

#include <dvtdb.h>
#include "datedialog.h"
#include "pick_fields.h"

PickFields::PickFields(QDate *start_date,QDate *end_date,
		       int *pgm_id,bool pgm_all,
		       int *affiliate_id,bool affiliate_all,
		       int *network_id,bool network_all,
		       PickFields::SortField *sort,
		       PickFields::MarketType market,
		       DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  setModal(true);

  QString sql;
  DvtSqlQuery *q;

  report_ypos=10;
  report_start_date=start_date;
  report_end_date=end_date;
  report_program_id=pgm_id;
  report_affiliate_id=affiliate_id;
  report_program_id=pgm_id;
  report_network_id=network_id;
  report_sort=sort;

  setWindowTitle("Davit");

  //
  // Dialogs
  //
  report_pickdate_dialog=
    new DateDialog(2008,QDate::currentDate().year(),config(),this);

  //
  // Program
  //
  if(pgm_id!=NULL) {
    report_program_box=new QComboBox(this);
    report_program_box->setGeometry(110,report_ypos,sizeHint().width()-120,20);
    report_program_label=new QLabel(tr("Program:"),this);
    report_program_label->setFont(labelFont());
    report_program_label->setGeometry(10,report_ypos,95,20);
    report_program_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    int count=0;
    if(pgm_all) {
      report_program_ids.push_back(0);
      report_program_box->insertItem(count++,tr("All Programs"));
    }
    QString sql=QString("select ")+
      "`ID`,"+
      "`PROGRAM_NAME` "+
      "from `PROGRAMS` order by `PROGRAM_NAME`";
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    while(q->next()) {
      report_program_box->insertItem(count++,q->value(1).toString());
      report_program_ids.push_back(q->value(0).toInt());
      if(*pgm_id==q->value(0).toInt()) {
	report_program_box->setCurrentIndex(report_program_box->count()-1);
      }
    }
    delete q;
    report_ypos+=22;
  }

  //
  // Affiliate
  //
  if(affiliate_id!=NULL) {
    report_affiliate_box=new QComboBox(this);
    report_affiliate_box->setGeometry(110,report_ypos,sizeHint().width()-120,20);
    report_affiliate_label=new QLabel(tr("Affiliate:"),this);
    report_affiliate_label->setFont(labelFont());
    report_affiliate_label->setGeometry(10,report_ypos,95,20);
    report_affiliate_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    int count=0;
    if(affiliate_all) {
      report_affiliate_ids.push_back(0);
      report_affiliate_box->insertItem(count++,tr("All Affiliates"));
    }
    QString sql=QString("select ")+
      "`ID`,"+
      "`STATION_CALL`,"+   // 00
      "`STATION_TYPE`,"+   // 01
      "`LICENSE_CITY`,"+   // 02
      "`LICENSE_STATE` "+  // 03
      "from `AFFILIATES` where "+
      "`IS_AFFILIATE`='Y' "+
      "order by `STATION_CALL`";
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    while(q->next()) {
      QString type;
      if(q->value(2).toString().toLower()=="f") {
	type=tr("FM");
      }
      if(q->value(2).toString().toLower()=="a") {
	type=tr("AM");
      }
      report_affiliate_ids.push_back(q->value(0).toInt());
      report_affiliate_box->
	insertItem(count++,q->value(1).toString()+"-"+type+" -- "+
		   q->value(3).toString()+", "+
		   q->value(4).toString());
      if(*affiliate_id==q->value(0).toInt()) {
	report_affiliate_box->setCurrentIndex(report_affiliate_box->count()-1);
      }
    }
    delete q;
    report_ypos+=30;
  }

  //
  // Network
  //
  if(network_id!=NULL) {
    report_network_box=new QComboBox(this);
    report_network_box->setGeometry(110,report_ypos,sizeHint().width()-120,20);
    report_network_label=new QLabel(tr("Network:"),this);
    report_network_label->setFont(labelFont());
    report_network_label->setGeometry(10,report_ypos,95,20);
    report_network_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    int count=0;
    if(network_all) {
      report_network_ids.push_back(0);
      report_network_box->insertItem(count++,tr("All Networks"));
    }
    QString sql=QString("select ")+
      "`ID`,"+    // 00
      "`NAME` "+  // 01
      "from `NETWORKS` order by `NAME`";
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    while(q->next()) {
      report_network_box->insertItem(count++,q->value(1).toString());
      report_network_ids.push_back(q->value(0).toInt());
      if(*network_id==q->value(0).toInt()) {
	report_network_box->setCurrentIndex(report_network_box->count()-1);
      }
    }
    delete q;
    report_ypos+=30;
  }

  //
  // Start Date
  //
  if(start_date!=NULL) {
    report_start_date_edit=new QDateEdit(this);
    report_start_date_edit->setDate(QDate::currentDate());
    report_start_date_edit->setGeometry(110,report_ypos+3,100,20);
    report_start_date_label=
      new QLabel(tr("Start Date:"),this);
    report_start_date_label->setFont(labelFont());
    report_start_date_label->setGeometry(10,report_ypos+3,95,20);
    report_start_date_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    if(start_date->isNull()) {
      *start_date=QDate::currentDate();
    }
    report_start_date_edit->setDate(*start_date);
    report_start_select_button=new QPushButton(tr("Select"),this);
    report_start_select_button->setGeometry(220,report_ypos,60,25);
    connect(report_start_select_button,SIGNAL(clicked()),this,SLOT(selectStartDateData()));
    report_ypos+=30;
  }

  //
  // End Date
  //
  if(end_date!=NULL) {
    report_end_date_edit=new QDateEdit(this);
    report_end_date_edit->setDate(QDate::currentDate());
    report_end_date_edit->setGeometry(110,report_ypos+3,100,20);
    report_end_date_label=new QLabel(tr("End Date:"),this);
    report_end_date_label->setFont(labelFont());
    report_end_date_label->setGeometry(10,report_ypos+3,95,20);
    report_end_date_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    if(end_date->isNull()) {
      *end_date=QDate::currentDate();
    }
    report_end_date_edit->setDate(*end_date);
    report_end_select_button=new QPushButton(tr("Select"),this);
    report_end_select_button->setGeometry(220,report_ypos,60,25);
    connect(report_end_select_button,SIGNAL(clicked()),this,SLOT(selectEndDateData()));
    report_ypos+=30;
  }

  //
  // Affiliate
  //
  if(sort!=NULL) {
    report_sort_box=new QComboBox(this);
    report_sort_box->setGeometry(110,report_ypos,200,20);
    report_sort_label=new QLabel(tr("Sort By:"),this);
    report_sort_label->setFont(labelFont());
    report_sort_label->setGeometry(10,report_ypos,95,20);
    report_sort_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    report_ypos+=22;
    int count=0;
    if(((*sort)&PickFields::SortAffiliate)!=0) {
      report_sort_box->insertItem(count++,tr("Affiliate"));
    }
    if(((*sort)&PickFields::SortCityState)!=0) {
      report_sort_box->insertItem(count++,tr("City/State"));
    }
    if(((*sort)&PickFields::SortMarket)!=0) {
      report_sort_box->insertItem(count++,tr("Market"));
    }
  }

  //
  // Market
  //
  report_market_label=NULL;
  report_market_box=NULL;
  report_city_label=NULL;
  report_city_edit=NULL;
  report_state_label=NULL;
  report_state_box=NULL;
  QString market_table;
  int count=0;
  switch(market) {
  case PickFields::StateMarket:
    report_city_edit=new QLineEdit(this);
    report_city_edit->setGeometry(110,report_ypos,sizeHint().width()-120,20);
    report_city_label=new QLabel(tr("City:"),this);
    report_city_label->setGeometry(10,report_ypos,95,20);
    report_city_label->setFont(labelFont());
    report_city_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    report_ypos+=22;

    report_state_box=new StateComboBox(true,this);
    report_state_box->setGeometry(110,report_ypos,sizeHint().width()-120,20);
    report_state_label=new QLabel(tr("State:"),this);
    report_state_label->setFont(labelFont());
    report_state_label->setGeometry(10,report_ypos,95,20);
    report_state_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    connect(report_state_box,SIGNAL(stateCodeChanged(const QString &)),
	    this,SLOT(stateCodeChangedData(const QString &)));
    stateCodeChangedData("aa");
    report_ypos+=22;
    break;

  case PickFields::DmaMarket:
  case PickFields::MsaMarket:
    report_market_box=new QComboBox(this);
    report_market_box->setGeometry(110,report_ypos,sizeHint().width()-120,20);
    report_market_label=new QLabel(tr("Market:"),this);
    report_market_label->setFont(labelFont());
    report_market_label->setGeometry(10,report_ypos,95,20);
    report_market_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    report_ypos+=22;
    switch(market) {
    case PickFields::DmaMarket:
      market_table="DMA_MARKETS";
      break;

    case PickFields::MsaMarket:
      market_table="MSA_MARKETS";
      break;

    case PickFields::NoMarket:
    case PickFields::StateMarket:
      break;
    }
    sql=QString("select ")+
      "`NAME` "+  // 00
      "from "+market_table+" order by `NAME`";
    q=new DvtSqlQuery(sql);
    while(q->next()) {
      if(!q->value(0).toString().isEmpty()) {
	report_market_box->insertItem(count++,q->value(0).toString());
      }
    }
    delete q;
    break;

  case PickFields::NoMarket:
    break;
  }

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMinimumSize(sizeHint());

  //
  //  Ok Button
  //
  report_ok_button=new QPushButton(this);
  report_ok_button->setFont(buttonFont());
  report_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  report_ok_button->setText(tr("OK"));
  connect(report_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  report_cancel_button=new QPushButton(this);
  report_cancel_button->setFont(buttonFont());
  report_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
  report_cancel_button->setText(tr("Cancel"));
  connect(report_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


PickFields::~PickFields()
{
}


QSize PickFields::sizeHint() const
{
  return QSize(400,report_ypos+70);
} 


QSizePolicy PickFields::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


QString PickFields::selectedMarket() const
{
  if(report_market_box==NULL) {
    return QString("");
  }
  return report_market_box->currentText();
}


QString PickFields::selectedCity() const
{
  if(report_city_edit==NULL) {
    return QString("");
  }
  return report_city_edit->text();
}


QString PickFields::selectedStateCode() const
{
  if(report_state_box==NULL) {
    return QString("");
  }
  return report_state_box->currentStateCode();
}


void PickFields::selectStartDateData()
{
  QDate date=report_start_date_edit->date();
  if(report_pickdate_dialog->exec(&date)) {
    report_start_date_edit->setDate(date);
  }
}


void PickFields::selectEndDateData()
{
  QDate date=report_end_date_edit->date();
  if(report_pickdate_dialog->exec(&date)) {
    report_end_date_edit->setDate(date);
  }
}


void PickFields::stateCodeChangedData(const QString &code)
{
  report_city_edit->setDisabled(code=="aa");
  report_city_label->setDisabled(code=="aa");
}


void PickFields::okData()
{
  if(report_program_id!=NULL) {
    *report_program_id=report_program_ids[report_program_box->currentIndex()];
  }
  if(report_affiliate_id!=NULL) {
    *report_affiliate_id=
      report_affiliate_ids[report_affiliate_box->currentIndex()];
  }
  if(report_network_id!=NULL) {
    *report_network_id=report_network_ids[report_network_box->currentIndex()];
  }
  if(report_start_date!=NULL) {
    *report_start_date=report_start_date_edit->date();
  }
  if(report_end_date!=NULL) {
    *report_end_date=report_end_date_edit->date();
  }
  if(report_sort!=NULL) {
    if(report_sort_box->currentText()==tr("Affiliate")) {
      *report_sort=PickFields::SortAffiliate;
    }
    if(report_sort_box->currentText()==tr("City/State")) {
      *report_sort=PickFields::SortCityState;
    }
    if(report_sort_box->currentText()==tr("Market")) {
      *report_sort=PickFields::SortMarket;
    }
  }
  done(true);
}


void PickFields::cancelData()
{
  done(false);
}
