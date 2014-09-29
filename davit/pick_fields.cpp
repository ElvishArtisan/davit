// pick_fields.cpp
//
// Generate a Davit Report
//
//   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: pick_fields.cpp,v 1.7 2011/02/21 23:41:18 pcvs Exp $
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

#include <qlabel.h>
#include <qsqldatabase.h>
#include <qmessagebox.h>

#include <math.h>

#include <dvtdatedialog.h>

#include <pick_fields.h>


PickFields::PickFields(QDate *start_date,QDate *end_date,
		       int *pgm_id,bool pgm_all,
		       int *affiliate_id,bool affiliate_all,
		       int *network_id,bool network_all,
		       PickFields::SortField *sort,
		       PickFields::MarketType market,
		       QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  QString sql;
  QSqlQuery *q;

  report_ypos=10;
  report_start_date=start_date;
  report_end_date=end_date;
  report_program_id=pgm_id;
  report_affiliate_id=affiliate_id;
  report_program_id=pgm_id;
  report_network_id=network_id;
  report_sort=sort;

  setCaption("Davit");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // Program
  //
  if(pgm_id!=NULL) {
    report_program_box=new QComboBox(this,"report_program_box");
    report_program_box->setGeometry(110,report_ypos,sizeHint().width()-120,20);
    report_program_box->setFont(font);
    report_program_label=
      new QLabel(report_program_box,tr("Program:"),this,"report_program_label");
    report_program_label->setGeometry(10,report_ypos,95,20);
    report_program_label->setFont(label_font);
    report_program_label->setAlignment(AlignVCenter|AlignRight);
    if(pgm_all) {
      report_program_ids.push_back(0);
      report_program_box->insertItem(tr("All Programs"));
    }
    QString sql="select ID,PROGRAM_NAME from PROGRAMS order by PROGRAM_NAME";
    QSqlQuery *q=new QSqlQuery(sql);
    while(q->next()) {
      report_program_box->insertItem(q->value(1).toString());
      report_program_ids.push_back(q->value(0).toInt());
      if(*pgm_id==q->value(0).toInt()) {
	report_program_box->setCurrentItem(report_program_box->count()-1);
      }
    }
    delete q;
    report_ypos+=22;
  }

  //
  // Affiliate
  //
  if(affiliate_id!=NULL) {
    report_affiliate_box=new QComboBox(this,"report_affiliate_box");
    report_affiliate_box->setGeometry(110,report_ypos,sizeHint().width()-120,20);
    report_affiliate_box->setFont(font);
    report_affiliate_label=new QLabel(report_affiliate_box,tr("Affiliate:"),
				      this,"report_affiliate_label");
    report_affiliate_label->setGeometry(10,report_ypos,95,20);
    report_affiliate_label->setFont(label_font);
    report_affiliate_label->setAlignment(AlignVCenter|AlignRight);
    if(affiliate_all) {
      report_affiliate_ids.push_back(0);
      report_affiliate_box->insertItem(tr("All Affiliates"));
    }
    QString sql="select ID,STATION_CALL,STATION_TYPE,LICENSE_CITY,\
                 LICENSE_STATE from AFFILIATES order by STATION_CALL";
    QSqlQuery *q=new QSqlQuery(sql);
    while(q->next()) {
      QString type;
      if(q->value(2).toString().lower()=="f") {
	type=tr("FM");
      }
      if(q->value(2).toString().lower()=="a") {
	type=tr("AM");
      }
      report_affiliate_ids.push_back(q->value(0).toInt());
      report_affiliate_box->insertItem(q->value(1).toString()+"-"+type+" -- "+
				  q->value(3).toString()+", "+
				  q->value(4).toString());
      if(*affiliate_id==q->value(0).toInt()) {
	report_affiliate_box->setCurrentItem(report_affiliate_box->count()-1);
      }
    }
    delete q;
    report_ypos+=30;
  }

  //
  // Network
  //
  if(network_id!=NULL) {
    report_network_box=new QComboBox(this,"report_network_box");
    report_network_box->setGeometry(110,report_ypos,sizeHint().width()-120,20);
    report_network_box->setFont(font);
    report_network_label=
      new QLabel(report_network_box,tr("Network:"),this,"report_network_label");
    report_network_label->setGeometry(10,report_ypos,95,20);
    report_network_label->setFont(label_font);
    report_network_label->setAlignment(AlignVCenter|AlignRight);
    if(network_all) {
      report_network_ids.push_back(0);
      report_network_box->insertItem(tr("All Networks"));
    }
    QString sql="select ID,NAME from NETWORKS order by NAME";
    QSqlQuery *q=new QSqlQuery(sql);
    while(q->next()) {
      report_network_box->insertItem(q->value(1).toString());
      report_network_ids.push_back(q->value(0).toInt());
      if(*network_id==q->value(0).toInt()) {
	report_network_box->setCurrentItem(report_network_box->count()-1);
      }
    }
    delete q;
    report_ypos+=30;
  }

  //
  // Start Date
  //
  if(start_date!=NULL) {
    report_start_date_edit=new QDateEdit(this,"report_start_date_edit");
    report_start_date_edit->setGeometry(110,report_ypos+3,100,20);
    report_start_date_edit->setDate(QDate::currentDate());
    report_start_date_label=new QLabel(report_start_date_edit,
				       tr("Start Date:"),
				       this,"report_start_date_label");
    report_start_date_label->setGeometry(10,report_ypos+3,95,20);
    report_start_date_label->setFont(label_font);
    report_start_date_label->setAlignment(AlignVCenter|AlignRight);
    if(start_date->isNull()) {
      *start_date=QDate::currentDate();
    }
    report_start_date_edit->setDate(*start_date);
    QPushButton *button=new QPushButton(tr("Select"),this);
    button->setFont(font);
    button->setGeometry(220,report_ypos,60,25);
    connect(button,SIGNAL(clicked()),this,SLOT(selectStartDateData()));
    report_ypos+=30;
  }

  //
  // End Date
  //
  if(end_date!=NULL) {
    report_end_date_edit=new QDateEdit(this,"report_end_date_edit");
    report_end_date_edit->setGeometry(110,report_ypos+3,100,20);
    report_end_date_edit->setDate(QDate::currentDate());
    report_end_date_label=new QLabel(report_end_date_edit,
				       tr("End Date:"),
				       this,"report_end_date_label");
    report_end_date_label->setGeometry(10,report_ypos+3,95,20);
    report_end_date_label->setFont(label_font);
    report_end_date_label->setAlignment(AlignVCenter|AlignRight);
    if(end_date->isNull()) {
      *end_date=QDate::currentDate();
    }
    report_end_date_edit->setDate(*end_date);
    QPushButton *button=new QPushButton(tr("Select"),this);
    button->setFont(font);
    button->setGeometry(220,report_ypos,60,25);
    connect(button,SIGNAL(clicked()),this,SLOT(selectEndDateData()));
    report_ypos+=30;
  }

  //
  // Affiliate
  //
  if(sort!=NULL) {
    report_sort_box=new QComboBox(this,"report_sort_box");
    report_sort_box->setGeometry(110,report_ypos,200,20);
    report_sort_box->setFont(font);
    report_sort_label=new QLabel(report_sort_box,tr("Sort By:"),
				      this,"report_sort_label");
    report_sort_label->setGeometry(10,report_ypos,95,20);
    report_sort_label->setFont(label_font);
    report_sort_label->setAlignment(AlignVCenter|AlignRight);
    report_ypos+=22;
    if(((*sort)&PickFields::SortAffiliate)!=0) {
      report_sort_box->insertItem(tr("Affiliate"));
    }
    if(((*sort)&PickFields::SortCityState)!=0) {
      report_sort_box->insertItem(tr("City/State"));
    }
    if(((*sort)&PickFields::SortMarket)!=0) {
      report_sort_box->insertItem(tr("Market"));
    }
  }

  //
  // Market
  //
  report_market_box=NULL;
  if(market!=PickFields::NoMarket) {
    report_market_box=new QComboBox(this,"report_market_box");
    report_market_box->setGeometry(110,report_ypos,sizeHint().width()-120,20);
    report_market_box->setFont(font);
    report_market_label=new QLabel(report_market_box,tr("Market:"),
				      this,"report_market_label");
    report_market_label->setGeometry(10,report_ypos,95,20);
    report_market_label->setFont(label_font);
    report_market_label->setAlignment(AlignVCenter|AlignRight);
    report_ypos+=22;
    QString market_table;
    switch(market) {
    case PickFields::DmaMarket:
      market_table="DMA_MARKETS";
      break;

    case PickFields::MsaMarket:
      market_table="MSA_MARKETS";
      break;

    case PickFields::NoMarket:
      QMessageBox::warning(this,tr("Davit - Internal Error"),
			   tr("Invalid Market Type"));
      break;
    }
    sql=QString().sprintf("select NAME from %s order by NAME",
			  (const char *)market_table);
    q=new QSqlQuery(sql);
    while(q->next()) {
      report_market_box->insertItem(q->value(0).toString());
    }
    delete q;
  }

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  //
  //  Ok Button
  //
  report_ok_button=new QPushButton(this,"report_ok_button");
  report_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  report_ok_button->setFont(label_font);
  report_ok_button->setText("&Ok");
  connect(report_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  report_cancel_button=new QPushButton(this,"report_cancel_button");
  report_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
  report_cancel_button->setFont(label_font);
  report_cancel_button->setText("&Cancel");
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


void PickFields::selectStartDateData()
{
  QDate date=report_start_date_edit->date();
  DvtDateDialog *d=new DvtDateDialog(2008,QDate::currentDate().year(),this);
  if(d->exec(&date)==0) {
    report_start_date_edit->setDate(date);
  }
  delete d;
}


void PickFields::selectEndDateData()
{
  QDate date=report_end_date_edit->date();
  DvtDateDialog *d=new DvtDateDialog(2008,QDate::currentDate().year(),this);
  if(d->exec(&date)==0) {
    report_end_date_edit->setDate(date);
  }
  delete d;
}


void PickFields::okData()
{
  if(report_program_id!=NULL) {
    *report_program_id=report_program_ids[report_program_box->currentItem()];
  }
  if(report_affiliate_id!=NULL) {
    *report_affiliate_id=
      report_affiliate_ids[report_affiliate_box->currentItem()];
  }
  if(report_network_id!=NULL) {
    *report_network_id=report_network_ids[report_network_box->currentItem()];
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
  done(0);
}


void PickFields::cancelData()
{
  done(-1);
}
