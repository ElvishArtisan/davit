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
#include <QValidator>

#include <dvtconf.h>
#include <dvtdb.h>

#include "add_remark.h"
#include "change_password.h"
#include "edit_affiliate.h"
#include "edit_airing.h"
#include "globals.h"

EditAffiliate::EditAffiliate(DvtConfig *c,QWidget *parent)
  : DvtDialog(c,parent)
{
  edit_affiliate_id=-1;

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());

  //
  // Dialogs
  //
  // Show Contacts
  edit_contact_list=new ListContacts(config,this);
  connect(edit_contact_list,SIGNAL(visibilityChanged(bool)),
	  this,SLOT(contactsVisibilityChangedData(bool)));

  // Show Program List
  edit_airings_list=new ListAirings(c,this);
  connect(edit_airings_list,SIGNAL(visibilityChanged(bool)),
	  this,SLOT(airingsVisibilityChangedData(bool)));

  // Show Affiliate History
  edit_remarks_list=new ListRemarks(c,this);
  connect(edit_remarks_list,SIGNAL(visibilityChanged(bool)),
	  this,SLOT(remarksVisibilityChangedData(bool)));

  //
  // Is Active Affiliate
  //
  edit_active_affiliate_check=new QCheckBox(this);
  edit_active_affiliate_label=new QLabel(tr("Affiliate Is Active"),this);
  edit_active_affiliate_label->setFont(labelFont());

  //
  // Station Call
  //
  edit_station_call_edit=new QLineEdit(this);
  edit_station_call_label=new QLabel(tr("Call")+":",this);
  edit_station_call_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_station_call_label->setFont(labelFont());

  //
  // Station Frequency
  //
  edit_station_frequency_edit=new QLineEdit(this);
  edit_station_frequency_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_station_frequency_label=new QLabel(tr("Freq")+":",this);
  edit_station_frequency_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_station_frequency_label->setFont(labelFont());

  //
  // Station Type
  //
  edit_station_type_box=new QComboBox(this);
  edit_station_type_box->insertItem(0,tr("AM"));
  edit_station_type_box->insertItem(1,tr("FM"));
  edit_station_type_box->insertItem(2,tr("Internet"));
  edit_station_type_edit=new QLineEdit(this);
  edit_station_type_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivAffiliateEdit)) {
    edit_station_type_edit->hide();
  }
  else {
    edit_station_type_box->hide();
  }
  edit_station_type_label=new QLabel("Type:",this);
  edit_station_type_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_station_type_label->setFont(labelFont());

  //
  // Daytime Power
  //
  edit_station_power_edit=new QLineEdit(this);
  QIntValidator *validator=new QIntValidator(0,100000,this);
  edit_station_power_edit->setValidator(validator);
  edit_station_power_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_station_power_label=new QLabel(tr("Power Day")+":",this);
  edit_station_power_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_station_power_label->setFont(labelFont());
  edit_station_power_unit_label=new QLabel("W",this);
  edit_station_power_unit_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Nighttime Power
  //
  edit_station_night_power_edit=new QLineEdit(this);
  edit_station_night_power_edit->setValidator(validator);
  edit_station_night_power_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_station_night_power_label=new QLabel(tr("Night")+":",this);
  edit_station_night_power_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_station_night_power_label->setFont(labelFont());
  edit_station_night_power_unit_label=new QLabel("W",this);
  edit_station_night_power_unit_label->
    setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  edit_station_night_power_unit_label->setFont(labelFont());

  //
  // City of License
  //
  edit_license_city_edit=new QLineEdit(this);
  edit_license_city_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_license_city_label=new QLabel(tr("City of License")+":",this);
  edit_license_city_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_license_city_label->setFont(labelFont());

  //
  // State of License
  //
  edit_license_state_box=new StateComboBox(false,this);
  edit_license_state_edit=new QLineEdit(this);
  edit_license_state_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivAffiliateEdit)) {
    edit_license_state_edit->hide();
  }
  else {
    edit_license_state_box->hide();
  }
  edit_license_state_label=new QLabel(tr("State of License")+":",this);
  edit_license_state_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_license_state_label->setFont(labelFont());

  //
  // MSA Market Name
  //
  edit_market_box=new QComboBox(this);
  edit_market_label=new QLabel(tr("MSA Market")+":",this);
  edit_market_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_market_label->setFont(labelFont());
  connect(edit_market_box,SIGNAL(activated(int)),
	  this,SLOT(marketNameActivatedData(int)));
  QString sql=QString("select ")+
    "`NAME`,"+  // 00
    "`RANK` "+  // 01
    "from `MSA_MARKETS` order by `NAME`";
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  edit_market_box->insertItem(0,tr("[none]"));
  while(q->next()) {
    edit_market_box->insertItem(edit_market_box->count(),
				q->value(0).toString(),q->value(1));
  }
  delete q;

  //
  // Market Rank
  //
  edit_market_rank_edit=new QLineEdit(this);
  edit_market_rank_edit->setReadOnly(true);
  edit_market_rank_label=new QLabel(tr("MSA Market Rank")+":",this);
  edit_market_rank_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_market_rank_label->setFont(labelFont());

  //
  // DMA Name
  //
  edit_dma_box=new QComboBox(this);
  edit_dma_label=new QLabel(tr("DMA Market")+":",this);
  edit_dma_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_dma_label->setFont(labelFont());
  connect(edit_dma_box,SIGNAL(activated(int)),
	  this,SLOT(dmaNameActivatedData(int)));
  sql=QString("select ")+
    "`NAME`,"+  // 00
    "`RANK` "+  // 01
    "from `DMA_MARKETS` order by `NAME`";
  q=new DvtSqlQuery(sql);
  edit_dma_box->insertItem(0,tr("[none]"));
  while(q->next()) {
    edit_dma_box->insertItem(edit_dma_box->count(),
			     q->value(0).toString(),q->value(1));
  }
  delete q;

  //
  // DMA Rank
  //
  edit_dma_rank_edit=new QLineEdit(this);
  edit_dma_rank_edit->setReadOnly(true);
  edit_dma_rank_label=new QLabel(tr("DMA Market Rank")+":",this);
  edit_dma_rank_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_dma_rank_label->setFont(labelFont());

  //
  // Second Network
  //
  edit_second_network_box=new QComboBox(this);
  edit_second_network_edit=new QLineEdit(this);
  edit_second_network_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivAffiliateEdit)) {
    edit_second_network_edit->hide();
  }
  else {
    edit_second_network_box->hide();
  }
  edit_second_network_label=new QLabel(tr("Second Network")+":",this);
  edit_second_network_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_second_network_label->setFont(labelFont());

  //
  // Time Zone
  //
  edit_timezone_box=new QComboBox(this);
  edit_timezone_box->insertItem(0,"EDT");
  edit_timezone_box->insertItem(1,"CDT");
  edit_timezone_box->insertItem(2,"MDT");
  edit_timezone_box->insertItem(3,"PDT");
  edit_timezone_box->insertItem(4,"AKDT");
  edit_timezone_box->insertItem(5,"HST");
  edit_timezone_edit=new QLineEdit(this);
  edit_timezone_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivAffiliateEdit)) {
    edit_timezone_edit->hide();
  }
  else {
    edit_timezone_box->hide();
  }
  edit_timezone_label=new QLabel(tr("Time Zone")+":",this);
  edit_timezone_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_timezone_label->setFont(labelFont());

  //
  // Previous Station Call
  //
  edit_station_prev_call_edit=new QLineEdit(this);
  edit_station_prev_call_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_station_prev_call_label=new QLabel(tr("Previous Call")+":",this);
  edit_station_prev_call_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_station_prev_call_label->setFont(labelFont());

  //
  // Business Name
  //
  edit_business_name_edit=new QLineEdit(this);
  edit_business_name_edit->setMaxLength(64);
  edit_business_name_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_business_name_call_label=new QLabel(tr("Business Name")+":",this);
  edit_business_name_call_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_business_name_call_label->setFont(labelFont());

  //
  // Address
  //
  edit_address1_edit=new QLineEdit(this);
  edit_address1_edit->setMaxLength(64);
  edit_address1_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_address2_edit=new QLineEdit(this);
  edit_address2_edit->setMaxLength(64);
  edit_address2_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_address_label=new QLabel(tr("Address")+":",this);
  edit_address_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_address_label->setFont(labelFont());

  //
  // City
  //
  edit_city_edit=new QLineEdit(this);
  edit_city_edit->setMaxLength(64);
  edit_city_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_city_label=new QLabel(tr("City")+":",this);
  edit_city_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_city_label->setFont(labelFont());

  //
  // State
  //
  edit_state_box=new StateComboBox(false,this);
  edit_state_edit=new QLineEdit(this);
  edit_state_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivAffiliateEdit)) {
    edit_state_edit->hide();
  }
  else {
    edit_state_box->hide();
  }
  edit_state_label=new QLabel(tr("State")+":",this);
  edit_state_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_state_label->setFont(labelFont());

  //
  // Zip Code
  //
  edit_zipcode_edit=new QLineEdit(this);
  edit_zipcode_edit->setMaxLength(10);
  edit_zipcode_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_zipcode_label=new QLabel(tr("Zip Code")+":",this);
  edit_zipcode_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_zipcode_label->setFont(labelFont());

  //
  // E-Mail
  //
  edit_email_addr_edit=new QLineEdit(this);
  edit_email_addr_edit->setMaxLength(64);
  edit_email_addr_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_email_addr_label=new QLabel(tr("E-Mail")+":",this);
  edit_email_addr_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_email_addr_label->setFont(labelFont());

  //
  // Phone
  //
  edit_phone_edit=new QLineEdit(this);
  edit_phone_edit->setMaxLength(20);
  edit_phone_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_phone_label=new QLabel(tr("Phone")+":",this);
  edit_phone_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_phone_label->setFont(labelFont());

  //
  // Fax
  //
  edit_fax_edit=new QLineEdit(this);
  edit_fax_edit->setMaxLength(20);
  edit_fax_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_fax_label=new QLabel(tr("Fax")+":",this);
  edit_fax_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_fax_label->setFont(labelFont());

  //
  // Web URL
  //
  edit_web_url_edit=new QLineEdit(this);
  edit_web_url_edit->setMaxLength(64);
  edit_web_url_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  edit_web_url_label=new QLabel(tr("Web URL")+":",this);
  edit_web_url_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_web_url_label->setFont(labelFont());

  //
  // Affidavit Histogram
  //
  edit_track_affidavit_box=new QCheckBox(this);
  edit_track_affidavit_label=
    new QLabel(tr("Enable Affidavit History Tracking"),this);
  edit_track_affidavit_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  edit_track_affidavit_label->setFont(labelFont());
  connect(edit_track_affidavit_box,SIGNAL(toggled(bool)),
	  this,SLOT(affidavitToggledData(bool)));

  edit_web_password_edit=new QLineEdit(this);
  edit_web_password_edit->setMaxLength(41);
  edit_web_password_label=new QLabel(tr("Web Password")+":",this);
  edit_web_password_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_web_password_label->setFont(labelFont());
  edit_affidavits=new ShowAffidavits(config,this);
  edit_affidavits->show();

  //
  // Show Program List Button
  //
  edit_airings_button=new QPushButton(tr("Show Program List"),this);
  edit_airings_button->setFont(labelFont());
  connect(edit_airings_button,SIGNAL(clicked()),this,SLOT(showAiringsData()));

  //
  // Show Contacts Button
  //
  edit_contacts_button=new QPushButton(tr("Show Contacts"),this);
  edit_contacts_button->setFont(labelFont());
  connect(edit_contacts_button,SIGNAL(clicked()),
	  this,SLOT(showContactsData()));

  //
  // Show Affiliate History Button
  //
  edit_remarks_button=new QPushButton(tr("Show Affiliate History"),this);
  edit_remarks_button->setFont(labelFont());
  connect(edit_remarks_button,SIGNAL(clicked()),this,SLOT(showRemarksData()));
  connect(edit_airings_list,SIGNAL(remarkAdded()),
	  edit_remarks_list,SLOT(refresh()));
  connect(edit_affidavits,SIGNAL(remarksUpdated()),
	  edit_remarks_list,SLOT(refresh()));

  //
  //  OK Button
  //
  edit_ok_button=new QPushButton(this);
  edit_ok_button->setDefault(true);
  edit_ok_button->setFont(buttonFont());
  edit_ok_button->setText(tr("OK"));
  connect(edit_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  edit_cancel_button=new QPushButton(this);
  edit_cancel_button->setFont(buttonFont());
  edit_cancel_button->setText(tr("Cancel"));
  connect(edit_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


EditAffiliate::~EditAffiliate()
{
}


QSize EditAffiliate::sizeHint() const
{
  return QSize(615,482+22);
} 


QSizePolicy EditAffiliate::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int EditAffiliate::exec(int affiliate_id,bool new_instance)
{
  edit_affiliate_id=affiliate_id;
  setWindowTitle("Davit - "+tr("Edit Affiliate")+
		 QString::asprintf(" [id=%d]",edit_affiliate_id));

  edit_affidavits->setAffiliateId(affiliate_id);

  edit_contact_list->setAffiliateId(affiliate_id);
  if(global_geometry->isVisible(Geometry::ContactsWidget)) {
    showContactsData();
  }

  edit_airings_list->setAffiliateId(affiliate_id);
  if(global_geometry->isVisible(Geometry::AiringsWidget)) {
    showAiringsData();
  }

  edit_remarks_list->setAffiliateId(affiliate_id);
  if(global_geometry->isVisible(Geometry::RemarksWidget)) {
    showRemarksData();
  }

  //
  // Load Data
  //
  QString sql=QString("select ")+
    "`STATION_TYPE`,"+         // 00
    "`STATION_POWER`,"+        // 01
    "`LICENSE_CITY`,"+         // 02
    "`LICENSE_STATE`,"+        // 03
    "`BUSINESS_NAME`,"+        // 04
    "`ADDRESS1`,"+             // 05
    "`ADDRESS2`,"+             // 06
    "`CITY`,"+                 // 07
    "`STATE`,"+                // 08
    "`ZIPCODE`,"+              // 09
    "`STATION_NIGHT_POWER`,"+  // 10
    "`MARKET_NAME`,"+          // 11
    "`MARKET_RANK`,"+          // 12
    "`DMA_NAME`,"+             // 13
    "`DMA_RANK`,"+             // 14
    "`SECOND_NETWORK_ID`,"+    // 15
    "`TIME_ZONE`,"+            // 16
    "`STATION_PREV_CALL`,"+    // 17
    "`STATION_FREQUENCY`,"+    // 18
    "`EMAIL_ADDR`,"+           // 19
    "`PHONE`,"+                // 20
    "`FAX`,"+                  // 21
    "`WEB_URL`,"+              // 22
    "`STATION_CALL`,"+         // 23
    "`USER_PASSWORD`,"+        // 24
    "`AFFIDAVIT_ACTIVE`,"+     // 25
    "`IS_AFFILIATE` "+         // 26
    "from `AFFILIATES` where "+
    QString::asprintf("`ID`=%d",edit_affiliate_id);
  DvtSqlQuery *q=new DvtSqlQuery(sql);
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
    edit_market_box->setCurrentIndex(0);
    edit_market_rank_edit->setText(tr("n/a"));
    for(int i=0;i<edit_market_box->count();i++) {
      if(edit_market_box->itemText(i)==q->value(11).toString()) {
	edit_market_box->setCurrentIndex(i);
	edit_market_rank_edit->
	  setText(QString::asprintf("%d",edit_market_box->itemData(i).toInt()));
      }
    }
    edit_dma_box->setCurrentIndex(0);
    edit_dma_rank_edit->setText(tr("n/a"));
    for(int i=0;i<edit_dma_box->count();i++) {
      if(edit_dma_box->itemText(i)==q->value(13).toString()) {
	edit_dma_box->setCurrentIndex(i);
	edit_dma_rank_edit->
	  setText(QString::asprintf("%d",edit_dma_box->itemData(i).toInt()));
      }
    }
    //    edit_market_rank_spin->setValue(q->value(12).toInt());
    //    edit_dma_rank_spin->setValue(q->value(14).toInt());
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
  sql=QString("select ")+
    "`ID`,"+    // 00
    "`NAME` "+  // 01
    "from `NETWORKS` order by `NAME`";
  int count=1;
  DvtSqlQuery *q1=new DvtSqlQuery(sql);
  while(q1->next()) {
    edit_second_network_box->insertItem(count++,q1->value(1).toString());
    second_network_ids.push_back(q1->value(0).toInt());
    if(q1->value(0).toInt()==q->value(15).toInt()) {
      edit_second_network_box->
	setCurrentIndex(edit_second_network_box->count()-1);
    }
  }
  delete q1;
  edit_web_url_edit->setText(q->value(22).toString());
  edit_web_password_edit->setText(q->value(24).toString());
  edit_track_affidavit_box->setChecked(q->value(25).toString()=="Y");
  affidavitToggledData(q->value(25).toString()=="Y");
  edit_affidavits->setAffiliateStatus(q->value(26).toString()=="Y");
  edit_active_affiliate_check->setChecked(q->value(26).toString()=="Y");
  delete q;
  /*
  if(!global_dvtuser->privilege(DvtUser::PrivAffiliateEdit)) {
    edit_market_rank_spin->
      setRange(edit_market_rank_spin->value(),edit_market_rank_spin->value());
    edit_dma_rank_spin->
      setRange(edit_dma_rank_spin->value(),edit_dma_rank_spin->value());
  }
  */
  if(new_instance) {
    edit_station_call_edit->selectAll();
  }
  return QDialog::exec();
}


void EditAffiliate::done(int r)
{
  edit_contact_list->hide();
  edit_airings_list->hide();
  edit_remarks_list->hide();

  QDialog::done(r);
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


void EditAffiliate::dmaNameActivatedData(int n)
{
  if(n==0) {
    edit_dma_rank_edit->setText(tr("n/a"));
  }
  else {
    edit_dma_rank_edit->
      setText(QString::asprintf("%d",edit_dma_box->itemData(n).toInt()));
  }
}


void EditAffiliate::marketNameActivatedData(int n)
{
  if(n==0) {
    edit_market_rank_edit->setText(tr("n/a"));
  }
  else {
    edit_market_rank_edit->
      setText(QString::asprintf("%d",edit_market_box->itemData(n).toInt()));
  }
}


void EditAffiliate::okData()
{
  QString priv;
  QString sql;
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

  //
  // Check for uniqueness
  //
  sql=QString("select ")+
    "`ID` "+
    "from `AFFILIATES` where "+
    "(`STATION_CALL`="+
    DvtSqlQuery::escape(edit_station_call_edit->text())+")&&"+
    "(`STATION_TYPE`="+DvtSqlQuery::escape(type)+")&&"+
    QString::asprintf("(`ID`!=%d)",edit_affiliate_id);
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  if(q->first()) {
    QMessageBox::warning(this,"Davit - "+tr("Edit Affiliate"),
		 tr("An affiliate with that call/type already exists."));
    delete q;
    return;
  }
  delete q;
  if(edit_dma_box->currentText()!="[none]") {
    dma_name=edit_dma_box->currentText();
  }
  if(edit_market_box->currentText()!="[none]") {
    msa_name=edit_market_box->currentText();
  }
  sql=QString("update `AFFILIATES` set ")+
    "`LICENSE_CITY`="+DvtSqlQuery::escape(edit_license_city_edit->text())+","+
    "`LICENSE_STATE`="+
    DvtSqlQuery::escape(edit_license_state_box->currentStateCode())+","+
    "`STATION_CALL`="+DvtSqlQuery::escape(edit_station_call_edit->text())+","+
    "`STATION_PREV_CALL`="+
    DvtSqlQuery::escape(edit_station_prev_call_edit->text())+","+
    "`STATION_TYPE`="+DvtSqlQuery::escape(type)+","+
    QString::asprintf("`STATION_POWER`=%d,",
		      edit_station_power_edit->text().toInt())+
    QString::asprintf("`STATION_NIGHT_POWER`=%d,",
		      edit_station_night_power_edit->text().toInt())+
    "`MARKET_NAME`="+DvtSqlQuery::escape(msa_name)+","+
    //    QString::asprintf("`MARKET_RANK`=%d,",edit_market_rank_spin->value())+
    "`DMA_NAME`="+DvtSqlQuery::escape(dma_name)+","+
    //    QString::asprintf("`DMA_RANK`=%d,",edit_dma_rank_spin->value())+
    "`TIME_ZONE`="+DvtSqlQuery::escape(edit_timezone_box->currentText())+","+
    "`BUSINESS_NAME`="+
    DvtSqlQuery::escape(edit_business_name_edit->text())+","+
    "`ADDRESS1`="+DvtSqlQuery::escape(edit_address1_edit->text())+","+
    "`ADDRESS2`="+DvtSqlQuery::escape(edit_address2_edit->text())+","+
    "`CITY`="+DvtSqlQuery::escape(edit_city_edit->text())+","+
    "`STATE`="+DvtSqlQuery::escape(edit_state_box->currentStateCode())+","+
    "`ZIPCODE`="+DvtSqlQuery::escape(edit_zipcode_edit->text())+","+
    QString::asprintf("`STATION_FREQUENCY`=%6.1lf,",
		      edit_station_frequency_edit->text().toDouble())+
    "`EMAIL_ADDR`="+DvtSqlQuery::escape(edit_email_addr_edit->text())+","+
    "`PHONE`="+DvtSqlQuery::escape(edit_phone_edit->text())+","+
    "`FAX`="+DvtSqlQuery::escape(edit_fax_edit->text())+","+
    "`WEB_URL`="+DvtSqlQuery::escape(edit_web_url_edit->text())+","+
    QString::asprintf("`SECOND_NETWORK_ID`=%d,",
		      second_network_ids[edit_second_network_box->currentIndex()])+
    "`USER_PASSWORD`="+DvtSqlQuery::escape(edit_web_password_edit->text())+","+
    "`AFFIDAVIT_ACTIVE`="+
    DvtSqlQuery::escape(DvtYesNo(edit_track_affidavit_box->isChecked()))+","+
    "`IS_AFFILIATE`="+
    DvtSqlQuery::escape(DvtYesNo(edit_active_affiliate_check->isChecked()))+" "+
    QString::asprintf("where `ID`=%d",edit_affiliate_id);
  DvtSqlQuery::apply(sql);

  SaveGeometry();
  done(true);
}


void EditAffiliate::cancelData()
{
  SaveGeometry();
  done(false);
}

void EditAffiliate::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  edit_active_affiliate_check->setGeometry(10,8,15,15);
  edit_active_affiliate_label->setGeometry(30,6,w-30,20);

  edit_station_call_label->setGeometry(10,32,40,20);
  edit_station_call_edit->setGeometry(55,32,50,20);

  edit_station_frequency_label->setGeometry(110,32,40,20);
  edit_station_frequency_edit->setGeometry(155,32,50,20);

  edit_station_type_label->setGeometry(210,32,40,20);
  edit_station_type_box->setGeometry(255,32,80,20);
  edit_station_type_edit->setGeometry(255,32,80,20);

  edit_station_power_label->setGeometry(345,32,70,20);
  edit_station_power_edit->setGeometry(420,32,45,20);
  edit_station_power_unit_label->setGeometry(470,32,20,20);

  edit_station_night_power_label->setGeometry(490,32,40,20);
  edit_station_night_power_edit->setGeometry(535,32,45,20);
  edit_station_night_power_unit_label->setGeometry(585,32,20,20);
  
  edit_license_city_label->setGeometry(25,54,95,20);
  edit_license_city_edit->setGeometry(125,54,160,20);

  edit_license_state_label->setGeometry(295,54,105,20);
  edit_license_state_box->setGeometry(405,54,200,20);
  edit_license_state_edit->setGeometry(405,54,200,20);

  edit_market_label->setGeometry(30,76,90,20);
  edit_market_box->setGeometry(125,76,260,20);

  edit_market_rank_label->setGeometry(400,76,130,20);
  edit_market_rank_edit->setGeometry(535,76,40,20);

  edit_dma_label->setGeometry(30,98,90,20);
  edit_dma_box->setGeometry(125,98,260,20);

  edit_dma_rank_label->setGeometry(400,98,130,20);
  edit_dma_rank_edit->setGeometry(535,98,40,20);

  edit_second_network_label->setGeometry(10,120,110,20);
  edit_second_network_box->setGeometry(125,120,160,20);
  edit_second_network_edit->setGeometry(125,120,160,20);

  edit_timezone_label->setGeometry(290,120,80,20);
  edit_timezone_box->setGeometry(375,120,60,20);
  edit_timezone_edit->setGeometry(375,120,60,20);

  edit_station_prev_call_label->setGeometry(450,120,90,20);
  edit_station_prev_call_edit->setGeometry(545,120,50,20);

  edit_business_name_call_label->setGeometry(10,142,110,20);
  edit_business_name_edit->setGeometry(125,142,w-135,20);
  edit_address1_edit->setGeometry(125,164,w-135,20);
  edit_address2_edit->setGeometry(125,186,w-135,20);
  edit_address_label->setGeometry(10,164,110,20);

  edit_city_label->setGeometry(10,208,110,20);
  edit_city_edit->setGeometry(125,208,190,20);

  edit_state_label->setGeometry(315,208,50,20);
  edit_state_box->setGeometry(370,208,200,20);
  edit_state_edit->setGeometry(370,208,200,20);

  edit_zipcode_label->setGeometry(10,230,110,20);
  edit_zipcode_edit->setGeometry(125,230,100,20);

  edit_email_addr_label->setGeometry(230,230,50,20);
  edit_email_addr_edit->setGeometry(285,230,140,20);

  edit_phone_label->setGeometry(10,252,110,20);
  edit_phone_edit->setGeometry(125,252,100,20);

  edit_fax_label->setGeometry(230,252,50,20);
  edit_fax_edit->setGeometry(285,252,100,20);

  edit_web_url_label->setGeometry(10,274,110,20);
  edit_web_url_edit->setGeometry(125,274,200,20);

  edit_track_affidavit_label->setGeometry(40,306,w-20,20);
  edit_track_affidavit_box->setGeometry(20,309,15,15);
  edit_affidavits->setGeometry(10,304+22,w-20,55);

  edit_web_password_edit->setGeometry(w/2+107,305,70,20);
  edit_web_password_label->setGeometry(w/2,306,102,20);

  edit_contacts_button->setGeometry(60,399,160,30);
  edit_contact_list->
    setGeometry(global_geometry->geometry(Geometry::ContactsWidget,
					  edit_contact_list->geometry()));
  edit_airings_button->setGeometry(230,399,160,30);
  edit_airings_list->
    setGeometry(global_geometry->geometry(Geometry::AiringsWidget,
					  edit_airings_list->geometry()));
  edit_remarks_button->setGeometry(400,399,160,30);
  edit_remarks_list->
    setGeometry(global_geometry->geometry(Geometry::RemarksWidget,
					  edit_remarks_list->geometry()));

  edit_ok_button->setGeometry(w-180,h-60,80,50);
  edit_cancel_button->setGeometry(w-90,h-60,80,50);
}


void EditAffiliate::closeEvent(QCloseEvent *e)
{
  cancelData();
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
