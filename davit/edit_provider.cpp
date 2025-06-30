// edit_provider.cpp
//
// Edit a Davit Provider.
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
#include <QPainter>
#include <QValidator>

#include <dvtdb.h>

#include "edit_provider.h"
#include "globals.h"
#include "list_programs.h"

EditProvider::EditProvider(DvtConfig *c,ListPrograms *listprograms_dialog,
			   QWidget *parent)
  : DvtDialog(c,parent)
{
  setModal(true);
  edit_provider_id=-1;
  edit_listprograms_dialog=listprograms_dialog;
  
  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());

  //
  // Business Name
  //
  edit_business_name_edit=new QLineEdit(this);
  edit_business_name_edit->setMaxLength(64);
  edit_business_name_label=new QLabel(tr("Business Name")+":",this);
  edit_business_name_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_business_name_label->setFont(labelFont());

  //
  // Address
  //
  edit_address1_edit=new QLineEdit(this);
  edit_address1_edit->setMaxLength(64);
  edit_address1_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  edit_address2_edit=new QLineEdit(this);
  edit_address2_edit->setMaxLength(64);
  edit_address2_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  edit_address_label=new QLabel(tr("Address")+":",this);
  edit_address_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_address_label->setFont(labelFont());

  //
  // City
  //
  edit_city_edit=new QLineEdit(this);
  edit_city_edit->setMaxLength(64);
  edit_city_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  edit_city_label=new QLabel(tr("City")+":",this);
  edit_city_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_city_label->setFont(labelFont());

  //
  // State
  //
  edit_state_box=new StateComboBox(false,this);
  edit_state_edit=new QLineEdit(this);
  edit_state_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProviderEdit)) {
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
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  edit_zipcode_label=new QLabel(tr("Zip Code")+":",this);
  edit_zipcode_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_zipcode_label->setFont(labelFont());

  //
  // Contact Section Label
  //
  edit_contact_group=new QGroupBox(tr("Contact"),this);
  edit_contact_group->setFont(sectionLabelFont());

  //
  // Contact Name
  //
  edit_contact_name_edit=new QLineEdit(edit_contact_group);
  edit_contact_name_edit->setFont(defaultFont());
  edit_contact_name_edit->setMaxLength(64);
  edit_contact_name_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  edit_contact_name_label=new QLabel(tr("Name")+":",edit_contact_group);
  edit_contact_name_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_contact_name_label->setFont(labelFont());

  //
  // Contact Phone
  //
  edit_contact_phone_edit=new QLineEdit(edit_contact_group);
  edit_contact_phone_edit->setFont(defaultFont());
  edit_contact_phone_edit->setMaxLength(20);
  edit_contact_phone_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  edit_contact_phone_label=new QLabel(tr("Phone")+":",edit_contact_group);
  edit_contact_phone_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_contact_phone_label->setFont(labelFont());

  //
  // Contact Fax
  //
  edit_contact_fax_edit=new QLineEdit(edit_contact_group);
  edit_contact_fax_edit->setFont(defaultFont());
  edit_contact_fax_edit->setMaxLength(20);
  edit_contact_fax_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  edit_contact_fax_label=new QLabel(tr("Fax")+":",edit_contact_group);
  edit_contact_fax_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_contact_fax_label->setFont(labelFont());

  //
  // Contact Email
  //
  edit_contact_email_edit=new QLineEdit(edit_contact_group);
  edit_contact_email_edit->setFont(defaultFont());
  edit_contact_email_edit->setMaxLength(64);
  edit_contact_email_label=
    new QLabel(tr("E-Mail Address")+":",edit_contact_group);
  edit_contact_email_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_contact_email_label->setFont(labelFont());

  //
  //  Programs Button
  //
  edit_programs_button=new QPushButton(this);
  edit_contact_email_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  edit_programs_button->setFont(buttonFont());
  edit_programs_button->setText("Programs");
  edit_programs_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivProgramView));
  connect(edit_programs_button,SIGNAL(clicked()),this,SLOT(programsData()));

  //
  //  OK Button
  //
  edit_ok_button=new QPushButton(this);
  edit_ok_button->setDefault(true);
  edit_ok_button->setFont(buttonFont());
  edit_ok_button->setText("OK");
  connect(edit_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  edit_cancel_button=new QPushButton(this);
  edit_cancel_button->setFont(buttonFont());
  edit_cancel_button->setText("Cancel");
  connect(edit_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


EditProvider::~EditProvider()
{
}


QSize EditProvider::sizeHint() const
{
  return QSize(615,302);
} 


QSizePolicy EditProvider::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int EditProvider::exec(int provider_id,bool new_entry)
{
  edit_provider_id=provider_id;

  setWindowTitle("Davit - "+tr("Edit Provider")+
		 QString::asprintf(" [ID: %d]",provider_id));

  QString sql=QString("select ")+
    "`ID`,"+             // 00
    "`BUSINESS_NAME`,"+  // 01
    "`ADDRESS1`,"+       // 02
    "`ADDRESS2`,"+       // 03
    "`CITY`,"+           // 04
    "`STATE`,"+          // 05
    "`ZIPCODE`,"+        // 06
    "`CONTACT_NAME`,"+   // 07
    "`CONTACT_PHONE`,"+  // 08
    "`CONTACT_FAX`,"+    // 09
    "`CONTACT_EMAIL` "+  // 10
    "from `PROVIDERS` where "+
    QString::asprintf("`ID`=%d ",provider_id);
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  if(q->first()) {
    edit_business_name_edit->setText(q->value(1).toString());
    edit_address1_edit->setText(q->value(2).toString());
    edit_address2_edit->setText(q->value(3).toString());
    edit_city_edit->setText(q->value(4).toString());
    edit_state_box->setCurrentStateCode(q->value(5).toString());
    edit_state_edit->setText(q->value(5).toString());
    edit_zipcode_edit->setText(q->value(6).toString());
    edit_contact_name_edit->setText(q->value(7).toString());
    edit_contact_phone_edit->setText(q->value(8).toString());
    edit_contact_fax_edit->setText(q->value(9).toString());
    edit_contact_email_edit->setText(q->value(10).toString());
  }
  delete q;
  if(new_entry) {
    edit_business_name_edit->selectAll();
    edit_business_name_edit->setFocus();
  }
  
  return QDialog::exec();
}


void EditProvider::programsData()
{
  edit_listprograms_dialog->exec(edit_provider_id);
}


void EditProvider::okData()
{
  QString sql=QString::asprintf("update `PROVIDERS` set ")+
    "`BUSINESS_NAME`="+DvtSqlQuery::escape(edit_business_name_edit->text())+","+
    "`ADDRESS1`="+DvtSqlQuery::escape(edit_address1_edit->text())+","+
    "`ADDRESS2`="+DvtSqlQuery::escape(edit_address2_edit->text())+","+
    "`CITY`="+DvtSqlQuery::escape(edit_city_edit->text())+","+
    "`STATE`="+DvtSqlQuery::escape(edit_state_box->currentStateCode())+","+
    "`ZIPCODE`="+DvtSqlQuery::escape(edit_zipcode_edit->text())+","+
    "`CONTACT_NAME`="+DvtSqlQuery::escape(edit_contact_name_edit->text())+","+
    "`CONTACT_PHONE`="+DvtSqlQuery::escape(edit_contact_phone_edit->text())+","+
    "`CONTACT_FAX`="+DvtSqlQuery::escape(edit_contact_fax_edit->text())+","+
    "`CONTACT_EMAIL`="+DvtSqlQuery::escape(edit_contact_email_edit->text())+" "+
    QString::asprintf("where `ID`=%d",edit_provider_id);
  DvtSqlQuery::apply(sql);

  done(true);
}


void EditProvider::cancelData()
{
  done(false);
}


void EditProvider::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  edit_business_name_edit->setGeometry(125,10,w-135,20);
  edit_business_name_label->setGeometry(10,10,110,20);
  edit_address1_edit->setGeometry(125,32,w-135,20);
  edit_address2_edit->setGeometry(125,54,w-135,20);
  edit_address_label->setGeometry(10,32,110,20);
  edit_city_edit->setGeometry(125,76,190,20);
  edit_city_label->setGeometry(10,76,110,20);
  edit_state_box->setGeometry(370,76,200,20);
  edit_state_edit->setGeometry(370,76,200,20);
  edit_state_label->setGeometry(315,76,50,20);
  edit_zipcode_edit->setGeometry(125,98,100,20);
  edit_zipcode_label->setGeometry(10,98,110,20);

  // Contact Section
  edit_contact_group->setGeometry(10,138,w-20,93);
  edit_contact_name_edit->setGeometry(115,25,w-275,20);
  edit_contact_name_label->setGeometry(10,25,100,20);
  edit_contact_phone_edit->setGeometry(115,47,150,20);
  edit_contact_phone_label->setGeometry(10,47,100,20);
  edit_contact_fax_edit->setGeometry(305,47,150,20);
  edit_contact_fax_label->setGeometry(270,47,30,20);
  edit_contact_email_edit->setGeometry(115,69,w-275,20);
  edit_contact_email_label->setGeometry(10,69,100,20);

  edit_programs_button->setGeometry(10,h-60,80,50);
  edit_ok_button->setGeometry(w-180,h-60,80,50);
  edit_cancel_button->setGeometry(w-90,h-60,80,50);
}
