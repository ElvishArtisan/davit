// edit_affiliate.cpp
//
// Edit a Davit Affiliate.
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
#include <QPainter>
#include <QRadioButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QValidator>

#include <dvtconf.h>

#include "add_remark.h"
#include "change_password.h"
#include "edit_affiliate.h"
#include "edit_airing.h"
#include "globals.h"

EditAffiliate::EditAffiliate(int id,QWidget *parent)
  : QDialog(parent)
{
  setModal(true);
  edit_id=id;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setWindowTitle(QString::asprintf("Davit - Edit Affiliate [id=%d]",id));

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont big_font=QFont("Helvetica",16,QFont::Bold);
  big_font.setPixelSize(16);

  //
  // Station Call
  //
  edit_station_call_edit=new QLineEdit(this);
  edit_station_call_edit->setGeometry(55,10,50,20);
  edit_station_call_edit->setFont(font);
  QLabel *label=new QLabel("Call:",this);
  label->setGeometry(10,10,40,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Station Frequency
  //
  edit_station_frequency_edit=new QLineEdit(this);
  edit_station_frequency_edit->setGeometry(160,10,50,20);
  edit_station_frequency_edit->setFont(font);
  edit_station_frequency_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  label=new QLabel("Freq:",this);
  label->setGeometry(115,10,40,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Station Type
  //
  edit_station_type_box=new QComboBox(this);
  edit_station_type_box->setGeometry(260,10,80,20);
  edit_station_type_box->setFont(font);
  edit_station_type_box->insertItem(0,tr("AM"));
  edit_station_type_box->insertItem(1,tr("FM"));
  edit_station_type_box->insertItem(2,tr("Internet"));
  edit_station_type_edit=new QLineEdit(this);
  edit_station_type_edit->setGeometry(260,10,80,20);
  edit_station_type_edit->setFont(font);
  edit_station_type_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivAffiliateEdit)) {
    edit_station_type_edit->hide();
  }
  else {
    edit_station_type_box->hide();
  }
  label=new QLabel("Type:",this);
  label->setGeometry(215,10,40,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Daytime Power
  //
  edit_station_power_edit=new QLineEdit(this);
  edit_station_power_edit->setGeometry(415,10,45,20);
  edit_station_power_edit->setFont(font);
  QIntValidator *validator=new QIntValidator(0,100000,this);
  edit_station_power_edit->setValidator(validator);
  edit_station_power_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  label=new QLabel("Power Day:",this);
  label->setGeometry(340,10,70,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  label=new QLabel("W",this);
  label->setGeometry(465,10,20,20);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  label->setFont(font);

  //
  // Nighttime Power
  //
  edit_station_night_power_edit=new QLineEdit(this);
  edit_station_night_power_edit->setGeometry(535,10,45,20);
  edit_station_night_power_edit->setFont(font);
  edit_station_night_power_edit->setValidator(validator);
  edit_station_night_power_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  label=new QLabel("Night:",this);
  label->setGeometry(490,10,40,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  label=new QLabel("W",this);
  label->setGeometry(585,10,20,20);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  label->setFont(font);

  //
  // City of License
  //
  edit_license_city_edit=new QLineEdit(this);
  edit_license_city_edit->setGeometry(125,32,160,20);
  edit_license_city_edit->setFont(font);
  edit_license_city_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  label=new QLabel("City of License:",this);
  label->setGeometry(30,32,90,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // State of License
  //
  edit_license_state_box=new StateComboBox(false,this);
  edit_license_state_box->setGeometry(405,32,200,20);
  edit_license_state_box->setFont(font);
  edit_license_state_edit=new QLineEdit(this);
  edit_license_state_edit->setGeometry(405,32,200,20);
  edit_license_state_edit->setFont(font);
  edit_license_state_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivAffiliateEdit)) {
    edit_license_state_edit->hide();
  }
  else {
    edit_license_state_box->hide();
  }
  label=new QLabel("State of License:",this);
  label->setGeometry(300,32,100,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // MSA Market Name
  //
  edit_market_box=new QComboBox(this);
  edit_market_box->setGeometry(125,54,260,20);
  edit_market_box->setFont(font);
  label=new QLabel("MSA Market:",this);
  label->setGeometry(30,54,90,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Market Rank
  //
  edit_market_rank_spin=new QSpinBox(this);
  edit_market_rank_spin->setGeometry(535,54,60,20);
  edit_market_rank_spin->setFont(font);
  edit_market_rank_spin->setRange(0,999);
  edit_market_rank_spin->setSpecialValueText(tr("None"));
  label=new QLabel("MSA Market Rank:",this);
  label->setGeometry(400,54,130,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  edit_market_rank_spin->hide();
  label->hide();

  //
  // DMA Name
  //
  edit_dma_box=new QComboBox(this);
  edit_dma_box->setGeometry(125,76,260,20);
  edit_dma_box->setFont(font);
  label=new QLabel("DMA Market:",this);
  label->setGeometry(30,76,90,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // DMA Rank
  //
  edit_dma_rank_spin=new QSpinBox(this);
  edit_dma_rank_spin->setGeometry(535,76,60,20);
  edit_dma_rank_spin->setFont(font);
  edit_dma_rank_spin->setRange(0,999);
  edit_dma_rank_spin->setSpecialValueText(tr("None"));
  label=new QLabel("DMA Market Rank:",this);
  label->setGeometry(400,76,130,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  edit_dma_rank_spin->hide();
  label->hide();

  //
  // Second Network
  //
  edit_second_network_box=new QComboBox(this);
  edit_second_network_box->setGeometry(125,98,160,20);
  edit_second_network_box->setFont(font);
  edit_second_network_edit=new QLineEdit(this);
  edit_second_network_edit->setGeometry(125,98,160,20);
  edit_second_network_edit->setFont(font);
  edit_second_network_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivAffiliateEdit)) {
    edit_second_network_edit->hide();
  }
  else {
    edit_second_network_box->hide();
  }
  label=new QLabel("Second Network:",this);
  label->setGeometry(10,98,110,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Time Zone
  //
  edit_timezone_box=new QComboBox(this);
  edit_timezone_box->setGeometry(375,98,60,20);
  edit_timezone_box->setFont(font);
  edit_timezone_box->insertItem(0,"EDT");
  edit_timezone_box->insertItem(1,"CDT");
  edit_timezone_box->insertItem(2,"MDT");
  edit_timezone_box->insertItem(3,"PDT");
  edit_timezone_box->insertItem(4,"AKDT");
  edit_timezone_box->insertItem(5,"HST");
  edit_timezone_edit=new QLineEdit(this);
  edit_timezone_edit->setGeometry(375,98,60,20);
  edit_timezone_edit->setFont(font);
  edit_timezone_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivAffiliateEdit)) {
    edit_timezone_edit->hide();
  }
  else {
    edit_timezone_box->hide();
  }
  label=new QLabel("Time Zone:",this);
  label->setGeometry(290,98,80,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Previous Station Call
  //
  edit_station_prev_call_edit=new QLineEdit(this);
  edit_station_prev_call_edit->setGeometry(545,98,50,20);
  edit_station_prev_call_edit->setFont(font);
  edit_station_prev_call_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  label=new QLabel("Previous Call:",this);
  label->setGeometry(450,98,90,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Business Name
  //
  edit_business_name_edit=new QLineEdit(this);
  edit_business_name_edit->setGeometry(125,120,sizeHint().width()-135,20);
  edit_business_name_edit->setFont(font);
  edit_business_name_edit->setMaxLength(64);
  edit_business_name_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  label=new QLabel("Business Name:",this);
  label->setGeometry(10,120,110,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Address
  //
  edit_address1_edit=new QLineEdit(this);
  edit_address1_edit->setGeometry(125,142,sizeHint().width()-135,20);
  edit_address1_edit->setFont(font);
  edit_address1_edit->setMaxLength(64);
  edit_address1_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_address2_edit=new QLineEdit(this);
  edit_address2_edit->setGeometry(125,164,sizeHint().width()-135,20);
  edit_address2_edit->setFont(font);
  edit_address2_edit->setMaxLength(64);
  edit_address2_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  label=new QLabel("Address:",this);
  label->setGeometry(10,142,110,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // City
  //
  edit_city_edit=new QLineEdit(this);
  edit_city_edit->setGeometry(125,186,190,20);
  edit_city_edit->setFont(font);
  edit_city_edit->setMaxLength(64);
  edit_city_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  label=new QLabel("City:",this);
  label->setGeometry(10,186,110,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // State
  //
  edit_state_box=new StateComboBox(false,this);
  edit_state_box->setGeometry(370,186,200,20);
  edit_state_box->setFont(font);
  edit_state_edit=new QLineEdit(this);
  edit_state_edit->setGeometry(370,186,200,20);
  edit_state_edit->setFont(font);
  edit_state_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivAffiliateEdit)) {
    edit_state_edit->hide();
  }
  else {
    edit_state_box->hide();
  }
  label=new QLabel("State:",this);
  label->setGeometry(315,186,50,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Zip Code
  //
  edit_zipcode_edit=new QLineEdit(this);
  edit_zipcode_edit->setGeometry(125,208,100,20);
  edit_zipcode_edit->setFont(font);
  edit_zipcode_edit->setMaxLength(10);
  edit_zipcode_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  label=new QLabel("Zip Code:",this);
  label->setGeometry(10,1208,110,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // E-Mail
  //
  edit_email_addr_edit=new QLineEdit(this);
  edit_email_addr_edit->setGeometry(285,208,140,20);
  edit_email_addr_edit->setFont(font);
  edit_email_addr_edit->setMaxLength(64);
  edit_email_addr_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  label=new QLabel("E-Mail:",this);
  label->setGeometry(230,208,50,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Phone
  //
  edit_phone_edit=new QLineEdit(this);
  edit_phone_edit->setGeometry(125,230,100,20);
  edit_phone_edit->setFont(font);
  edit_phone_edit->setMaxLength(20);
  edit_phone_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  label=new QLabel("Phone:",this);
  label->setGeometry(10,230,110,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Fax
  //
  edit_fax_edit=new QLineEdit(this);
  edit_fax_edit->setGeometry(285,230,100,20);
  edit_fax_edit->setFont(font);
  edit_fax_edit->setMaxLength(20);
  edit_fax_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  label=new QLabel("Fax:",this);
  label->setGeometry(230,230,50,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Web URL
  //
  edit_web_url_edit=new QLineEdit(this);
  edit_web_url_edit->setGeometry(125,252,200,20);
  edit_web_url_edit->setFont(font);
  edit_web_url_edit->setMaxLength(64);
  edit_web_url_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  label=new QLabel("Web Address:",this);
  label->setGeometry(10,252,110,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Affidavit Histogram
  //
  edit_track_affidavit_box=new QCheckBox(this);
  edit_track_affidavit_box->setGeometry(20,287,15,15);
  label=new QLabel("Enable Affidavit History Tracking",this);
  label->setGeometry(40,284,sizeHint().width()-20,20);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  label->setFont(label_font);
  connect(edit_track_affidavit_box,SIGNAL(toggled(bool)),
	  this,SLOT(affidavitToggledData(bool)));

  edit_web_password_edit=new QLineEdit(this);
  edit_web_password_edit->setGeometry(sizeHint().width()/2+107,283,70,20);
  edit_web_password_edit->setMaxLength(41);
  edit_web_password_label=new QLabel("Web Password:",this);
  edit_web_password_label->setGeometry(sizeHint().width()/2,284,102,20);
  edit_web_password_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_web_password_label->setFont(label_font);
  edit_affidavits=new ShowAffidavits(id,this);
  edit_affidavits->setGeometry(10,304,sizeHint().width()-20,55);

  //
  // Contacts Dialog
  //
  edit_contact_list=new ListContacts(id,this);
  connect(edit_contact_list,SIGNAL(visibilityChanged(bool)),
	  this,SLOT(contactsVisibilityChangedData(bool)));
  edit_contacts_button=new QPushButton(tr("Show Contacts"),this);
  edit_contacts_button->setGeometry(60,377,160,30);
  edit_contacts_button->setFont(label_font);
  connect(edit_contacts_button,SIGNAL(clicked()),
	  this,SLOT(showContactsData()));
  edit_contact_list->
    setGeometry(global_geometry->geometry(Geometry::ContactsWidget,
					  edit_contact_list->geometry()));
  if(global_geometry->isVisible(Geometry::ContactsWidget)) {
    showContactsData();
  }

  //
  // Airings List
  //
  edit_airings_list=new ListAirings(id,this);
  connect(edit_airings_list,SIGNAL(visibilityChanged(bool)),
	  this,SLOT(airingsVisibilityChangedData(bool)));
  edit_airings_button=new QPushButton(tr("Show Program List"),this);
  edit_airings_button->setGeometry(230,377,160,30);
  edit_airings_button->setFont(label_font);
  connect(edit_airings_button,SIGNAL(clicked()),this,SLOT(showAiringsData()));
  edit_airings_list->
    setGeometry(global_geometry->geometry(Geometry::AiringsWidget,
					  edit_airings_list->geometry()));
  if(global_geometry->isVisible(Geometry::AiringsWidget)) {
    showAiringsData();
  }

  //
  // Remarks
  //
  edit_remarks_list=new ListRemarks(id,this);
  connect(edit_remarks_list,SIGNAL(visibilityChanged(bool)),
	  this,SLOT(remarksVisibilityChangedData(bool)));
  edit_remarks_button=new QPushButton(tr("Show Affiliate History"),this);
  edit_remarks_button->setGeometry(400,377,160,30);
  edit_remarks_button->setFont(label_font);
  connect(edit_remarks_button,SIGNAL(clicked()),this,SLOT(showRemarksData()));
  connect(edit_airings_list,SIGNAL(remarkAdded()),
	  edit_remarks_list,SLOT(refresh()));
  connect(edit_affidavits,SIGNAL(remarksUpdated()),
	  edit_remarks_list,SLOT(refresh()));
  edit_remarks_list->
    setGeometry(global_geometry->geometry(Geometry::RemarksWidget,
					  edit_remarks_list->geometry()));
  if(global_geometry->isVisible(Geometry::RemarksWidget)) {
    showRemarksData();
  }


  //
  //  OK Button
  //
  QPushButton *button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(label_font);
  button->setText("&OK");
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText("&Cancel");
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Data
  //
  QString sql=QString::asprintf("select STATION_TYPE,\
                                 STATION_POWER,LICENSE_CITY,LICENSE_STATE,\
                                 BUSINESS_NAME,ADDRESS1,ADDRESS2,CITY,STATE,\
                                 ZIPCODE,STATION_NIGHT_POWER,MARKET_NAME,\
                                 MARKET_RANK,DMA_NAME,\
                                 DMA_RANK,SECOND_NETWORK_ID,\
                                 TIME_ZONE,STATION_PREV_CALL,\
                                 STATION_FREQUENCY,EMAIL_ADDR,\
                                 PHONE,FAX,WEB_URL,STATION_CALL,USER_PASSWORD,\
                                 AFFIDAVIT_ACTIVE,IS_AFFILIATE from AFFILIATES\
                                 where ID=%d",edit_id);
  QSqlQuery *q=new QSqlQuery(sql);
  if(q->first()) {
    edit_station_call_edit->setText(q->value(23).toString());
    edit_license_city_edit->setText(q->value(2).toString());
    edit_license_state_box->setCurrentStateCode(q->value(3).toString());
    if(q->value(0).toString().toLower()=="a") {
      edit_station_type_box->setCurrentIndex(0);
      edit_station_type_edit->setText(tr("AM"));
      edit_station_power_edit->setText(q->value(1).toString());
      edit_station_night_power_edit->setText(q->value(10).toString());
    }
    else {
      if(q->value(0).toString().toLower()=="f") {
	edit_station_type_box->setCurrentIndex(1);
	edit_station_type_edit->setText(tr("FM"));
	edit_station_power_edit->setText(q->value(1).toString());
	edit_station_night_power_edit->setText(q->value(1).toString());
      }
      else {
	edit_station_type_box->setCurrentIndex(2);
	edit_station_type_edit->setText(tr("Internet"));
      }
    }
    edit_business_name_edit->setText(q->value(4).toString());
    edit_address1_edit->setText(q->value(5).toString());
    edit_address2_edit->setText(q->value(6).toString());
    edit_city_edit->setText(q->value(7).toString());
    edit_state_box->setCurrentStateCode(q->value(8).toString());
    edit_state_edit->setText(q->value(8).toString());
    edit_zipcode_edit->setText(q->value(9).toString());

    sql="select NAME from MSA_MARKETS order by NAME";
    QSqlQuery *q1=new QSqlQuery(sql);
    edit_market_box->insertItem(0,"[none]");
    int count=1;
    while(q1->next()) {
      edit_market_box->insertItem(count++,q1->value(0).toString());
      if(q1->value(0).toString()==q->value(11).toString()) {
	edit_market_box->setCurrentIndex(edit_market_box->count()-1);
      }
    }
    delete q1;

    sql="select NAME from DMA_MARKETS order by NAME";
    q1=new QSqlQuery(sql);
    edit_dma_box->insertItem(0,"[none]");
    count=1;
    while(q1->next()) {
      edit_dma_box->insertItem(count++,q1->value(0).toString());
      if(q1->value(0).toString()==q->value(13).toString()) {
	edit_dma_box->setCurrentIndex(edit_dma_box->count()-1);
      }
    }
    delete q1;

    //    edit_market_box->setText(q->value(11).toString());

    edit_market_rank_spin->setValue(q->value(12).toInt());
    edit_dma_rank_spin->setValue(q->value(14).toInt());
    for(int i=0;i<edit_timezone_box->count();i++) {
      if(edit_timezone_box->itemText(i)==q->value(16).toString().toUpper()) {
	edit_timezone_box->setCurrentIndex(i);
	edit_timezone_edit->setText(edit_timezone_box->currentText());
      }
    }
    edit_station_prev_call_edit->setText(q->value(17).toString());
    if(q->value(18).toDouble()<520) {
      edit_station_frequency_edit->
	setText(QString::asprintf("%5.1lf",q->value(18).toDouble()).
		trimmed());
    }
    else {
      edit_station_frequency_edit->
	setText(QString::asprintf("%4.0lf",q->value(18).toDouble()).
		trimmed());
    }
  }
  edit_email_addr_edit->setText(q->value(19).toString());
  edit_phone_edit->setText(q->value(20).toString());
  edit_fax_edit->setText(q->value(21).toString());
  edit_second_network_box->insertItem(0,"[none]");
  second_network_ids.push_back(0);
  sql="select ID,NAME from NETWORKS order by NAME";
  int count=1;
  QSqlQuery *q1=new QSqlQuery(sql);
  while(q1->next()) {
    edit_second_network_box->insertItem(count++,q1->value(1).toString());
    second_network_ids.push_back(q1->value(0).toInt());
    if(q1->value(0).toInt()==q->value(15).toInt()) {
      edit_second_network_box->
	setCurrentIndex(edit_second_network_box->count()-1);
    }
  }
  delete q1;
  edit_web_password_edit->setText(q->value(24).toString());
  edit_track_affidavit_box->setChecked(q->value(25).toString()=="Y");
  affidavitToggledData(q->value(25).toString()=="Y");
  edit_affidavits->setAffiliateStatus(q->value(26).toString()=="Y");
  delete q;

  if(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit)) {
    edit_market_rank_spin->
      setRange(edit_market_rank_spin->value(),edit_market_rank_spin->value());
    edit_dma_rank_spin->
      setRange(edit_dma_rank_spin->value(),edit_dma_rank_spin->value());
  }
}


EditAffiliate::~EditAffiliate()
{
}


QSize EditAffiliate::sizeHint() const
{
  return QSize(615,482);
} 


QSizePolicy EditAffiliate::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditAffiliate::affidavitToggledData(bool state)
{
  edit_affidavits->setEnabled(state);
  edit_web_password_edit->setEnabled(state);
  edit_web_password_label->setEnabled(state);
}


void EditAffiliate::showContactsData()
{
  if(edit_contact_list->isVisible()) {
    edit_contact_list->hide();
  }
  else {
    edit_contact_list->show();
  }
}


void EditAffiliate::contactsVisibilityChangedData(bool visible)
{
  if(visible) {
    edit_contacts_button->setText(tr("Hide Contacts"));
  }
  else {
    edit_contacts_button->setText(tr("Show Contacts"));
  }
}


void EditAffiliate::showAiringsData()
{
  if(edit_airings_list->isVisible()) {
    edit_airings_list->hide();
  }
  else {
    edit_airings_list->show();
  }
}


void EditAffiliate::airingsVisibilityChangedData(bool visible)
{
  if(visible) {
    edit_airings_button->setText(tr("Hide Program List"));
  }
  else {
    edit_airings_button->setText(tr("Show Program List"));
  }
}


void EditAffiliate::showRemarksData()
{
  if(edit_remarks_list->isVisible()) {
    edit_remarks_list->hide();
  }
  else {
    edit_remarks_list->show();
  }
}


void EditAffiliate::remarksVisibilityChangedData(bool visible)
{
  if(visible) {
    edit_remarks_button->setText(tr("Hide Affiliate History"));
  }
  else {
    edit_remarks_button->setText(tr("Show Affiliate History"));
  }
}


void EditAffiliate::okData()
{
  QString priv;
  QString sql;
  QSqlQuery *q;
  QString type;
  QString dma_name="";
  QString msa_name="";

  switch(edit_station_type_box->currentIndex()) {
      case 0:  // AM
	type="A";
	break;

      case 1:  // FM
	type="F";
	break;

      default:  // Internet/Other
	type="I";
	break;
  }
  if(edit_dma_box->currentText()!="[none]") {
    dma_name=edit_dma_box->currentText();
  }
  if(edit_market_box->currentText()!="[none]") {
    msa_name=edit_market_box->currentText();
  }
  sql=QString::asprintf("update AFFILIATES set LICENSE_CITY=\"%s\",\
             LICENSE_STATE=\"%s\",\
             STATION_CALL=\"%s\",\
             STATION_PREV_CALL=\"%s\",\
             STATION_TYPE=\"%s\",\
             STATION_POWER=%d,\
             STATION_NIGHT_POWER=%d,\
             MARKET_NAME=\"%s\",\
             MARKET_RANK=%d,\
             DMA_NAME=\"%s\",\
             DMA_RANK=%d,\
             TIME_ZONE=\"%s\",\
             BUSINESS_NAME=\"%s\",\
             ADDRESS1=\"%s\",\
             ADDRESS2=\"%s\",\
             CITY=\"%s\",\
             STATE=\"%s\",\
             ZIPCODE=\"%s\",\
             STATION_FREQUENCY=%6.1lf,\
             EMAIL_ADDR=\"%s\",\
             PHONE=\"%s\",\
             FAX=\"%s\",\
             WEB_URL=\"%s\",\
             SECOND_NETWORK_ID=%d,\
             USER_PASSWORD=\"%s\",\
             AFFIDAVIT_ACTIVE=\"%s\" \
             where ID=%d",
	    DvtEscapeString(edit_license_city_edit->text()).
	    toUtf8().constData(),
	    DvtEscapeString(edit_license_state_box->
			    currentStateCode()).toUtf8().constData(),
	    DvtEscapeString(edit_station_call_edit->text()).
	    toUtf8().constData(),
	    DvtEscapeString(edit_station_prev_call_edit->text()).
	    toUtf8().constData(),
	    type.toUtf8().constData(),
	    edit_station_power_edit->text().toInt(),
	    edit_station_night_power_edit->text().toInt(),
	    DvtEscapeString(msa_name).toUtf8().constData(),
	    edit_market_rank_spin->value(),
	    DvtEscapeString(dma_name).toUtf8().constData(),
	    edit_dma_rank_spin->value(),
	    DvtEscapeString(edit_timezone_box->currentText()).
	    toUtf8().constData(),
	    DvtEscapeString(edit_business_name_edit->text()).
	    toUtf8().constData(),
	    DvtEscapeString(edit_address1_edit->text()).toUtf8().constData(),
	    DvtEscapeString(edit_address2_edit->text()).toUtf8().constData(),
	    DvtEscapeString(edit_city_edit->text()).toUtf8().constData(),
	    DvtEscapeString(edit_state_box->currentStateCode()).
	    toUtf8().constData(),
	    DvtEscapeString(edit_zipcode_edit->text()).toUtf8().constData(),
	    edit_station_frequency_edit->text().toDouble(),
	    DvtEscapeString(edit_email_addr_edit->text()).toUtf8().constData(),
	    DvtEscapeString(edit_phone_edit->text()).toUtf8().constData(),
	    DvtEscapeString(edit_fax_edit->text()).toUtf8().constData(),
	    DvtEscapeString(edit_web_url_edit->text()).toUtf8().constData(),
	    second_network_ids[edit_second_network_box->currentIndex()],
	    DvtEscapeString(edit_web_password_edit->text()).
	    toUtf8().constData(),
	    DvtYesNo(edit_track_affidavit_box->isChecked()).
	    toUtf8().constData(),
	    edit_id);
  q=new QSqlQuery(sql);
  delete q;

  SaveGeometry();
  done(0);
}


void EditAffiliate::cancelData()
{
  SaveGeometry();
  done(-1);
}


void EditAffiliate::SaveGeometry()
{
#ifdef WIN32
  global_geometry->
    setGeometry(Geometry::ContactsWidget,edit_contact_list->geometry());
#else
  global_geometry->
    setGeometry(Geometry::ContactsWidget,edit_contact_list->frameGeometry());
#endif  // WIN32
  global_geometry->
    setVisible(Geometry::ContactsWidget,
	       edit_contact_list->isVisible());

#ifdef WIN32
  global_geometry->
    setGeometry(Geometry::AiringsWidget,edit_airings_list->geometry());
#else
  global_geometry->
    setGeometry(Geometry::AiringsWidget,edit_airings_list->frameGeometry());
#endif  // WIN32
  global_geometry->
    setVisible(Geometry::AiringsWidget,
	       edit_airings_list->isVisible());

#ifdef WIN32
  global_geometry->
    setGeometry(Geometry::RemarksWidget,edit_remarks_list->geometry());
#else
  global_geometry->
    setGeometry(Geometry::RemarksWidget,edit_remarks_list->frameGeometry());
#endif  // WIN32
  global_geometry->
    setVisible(Geometry::RemarksWidget,
	       edit_remarks_list->isVisible());
}
