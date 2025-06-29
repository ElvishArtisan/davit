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

#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QValidator>

#include "edit_provider.h"
#include "globals.h"
#include "list_programs.h"

EditProvider::EditProvider(const QString &bname,QWidget *parent)
  : QDialog(parent)
{
  setModal(true);

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setWindowTitle("Davit - Edit Provider");

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
  // Business Name
  //
  edit_business_name_edit=new QLineEdit(this);
  edit_business_name_edit->setGeometry(125,10,sizeHint().width()-135,20);
  edit_business_name_edit->setFont(font);
  edit_business_name_edit->setMaxLength(64);
  edit_business_name_edit->setReadOnly(true);
  QLabel *label=new QLabel("Business Name:",this);
  label->setGeometry(10,10,110,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Address
  //
  edit_address1_edit=new QLineEdit(this);
  edit_address1_edit->setGeometry(125,32,sizeHint().width()-135,20);
  edit_address1_edit->setFont(font);
  edit_address1_edit->setMaxLength(64);
  edit_address1_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  edit_address2_edit=new QLineEdit(this);
  edit_address2_edit->setGeometry(125,54,sizeHint().width()-135,20);
  edit_address2_edit->setFont(font);
  edit_address2_edit->setMaxLength(64);
  edit_address2_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  label=new QLabel("Address:",this);
  label->setGeometry(10,32,110,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // City
  //
  edit_city_edit=new QLineEdit(this);
  edit_city_edit->setGeometry(125,76,190,20);
  edit_city_edit->setFont(font);
  edit_city_edit->setMaxLength(64);
  edit_city_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  label=new QLabel("City:",this);
  label->setGeometry(10,76,110,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // State
  //
  edit_state_box=new StateComboBox(false,this);
  edit_state_box->setGeometry(370,76,200,20);
  edit_state_box->setFont(font);
  edit_state_edit=new QLineEdit(this);
  edit_state_edit->setGeometry(370,76,200,20);
  edit_state_edit->setFont(font);
  edit_state_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProviderEdit)) {
    edit_state_edit->hide();
  }
  else {
    edit_state_box->hide();
  }
  label=new QLabel("State:",this);
  label->setGeometry(315,76,50,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Zip Code
  //
  edit_zipcode_edit=new QLineEdit(this);
  edit_zipcode_edit->setGeometry(125,98,100,20);
  edit_zipcode_edit->setFont(font);
  edit_zipcode_edit->setMaxLength(10);
  edit_zipcode_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  label=new QLabel("Zip Code:",this);
  label->setGeometry(10,98,110,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Contact Section Label
  //
  label=new QLabel("Contact",this);
  label->setGeometry(50,130,70,20);
  label->setAlignment(Qt::AlignCenter);
  label->setFont(big_font);

  //
  // Contact Name
  //
  edit_contact_name_edit=new QLineEdit(this);
  edit_contact_name_edit->setGeometry(125,152,sizeHint().width()-275,20);
  edit_contact_name_edit->setFont(font);
  edit_contact_name_edit->setMaxLength(64);
  edit_contact_name_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  label=new QLabel("Name:",this);
  label->setGeometry(20,152,100,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Contact Phone
  //
  edit_contact_phone_edit=new QLineEdit(this);
  edit_contact_phone_edit->setGeometry(125,174,150,20);
  edit_contact_phone_edit->setFont(font);
  edit_contact_phone_edit->setMaxLength(20);
  edit_contact_phone_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  label=new QLabel("Phone:",this);
  label->setGeometry(20,174,100,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Contact Fax
  //
  edit_contact_fax_edit=new QLineEdit(this);
  edit_contact_fax_edit->setGeometry(315,174,150,20);
  edit_contact_fax_edit->setFont(font);
  edit_contact_fax_edit->setMaxLength(20);
  edit_contact_fax_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  label=new QLabel("Fax:",this);
  label->setGeometry(280,174,30,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Contact Email
  //
  edit_contact_email_edit=new QLineEdit(this);
  edit_contact_email_edit->setGeometry(125,196,sizeHint().width()-275,20);
  edit_contact_email_edit->setFont(font);
  edit_contact_email_edit->setMaxLength(64);
  edit_contact_email_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProviderEdit));
  label=new QLabel("E-Mail Address:",this);
  label->setGeometry(20,196,100,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  //  Programs Button
  //
  QPushButton *button=new QPushButton(this);
  button->setGeometry(10,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText("&Programs");
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivProgramView));
  connect(button,SIGNAL(clicked()),this,SLOT(programsData()));

  //
  //  OK Button
  //
  button=new QPushButton(this);
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
  QString sql=QString::asprintf("select ID,ADDRESS1,ADDRESS2,CITY,\
                                 STATE,ZIPCODE,CONTACT_NAME,\
                                 CONTACT_PHONE,CONTACT_FAX,CONTACT_EMAIL \
                                 from PROVIDERS where BUSINESS_NAME=\"%s\"",
				bname.toUtf8().constData());
  QSqlQuery *q=new QSqlQuery(sql);
  if(q->first()) {
    edit_id=q->value(0).toInt();
    edit_business_name_edit->setText(bname);
    edit_address1_edit->setText(q->value(1).toString());
    edit_address2_edit->setText(q->value(2).toString());
    edit_city_edit->setText(q->value(3).toString());
    edit_state_box->setCurrentStateCode(q->value(4).toString());
    edit_state_edit->setText(q->value(4).toString());
    edit_zipcode_edit->setText(q->value(5).toString());
    edit_contact_name_edit->setText(q->value(6).toString());
    edit_contact_phone_edit->setText(q->value(7).toString());
    edit_contact_fax_edit->setText(q->value(8).toString());
    edit_contact_email_edit->setText(q->value(9).toString());
  }
  delete q;
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


void EditProvider::programsData()
{
  /*
  ListPrograms *list=new ListPrograms(edit_id,this);
  list->exec();
  delete list;
  */
}


void EditProvider::okData()
{
  QString sql;

  sql=QString::asprintf("update PROVIDERS set \
                         ADDRESS1=\"%s\",\
                         ADDRESS2=\"%s\",\
                         CITY=\"%s\",\
                         STATE=\"%s\",\
                         ZIPCODE=\"%s\",\
                         CONTACT_NAME=\"%s\",\
                         CONTACT_PHONE=\"%s\",\
                         CONTACT_FAX=\"%s\",\
                         CONTACT_EMAIL=\"%s\" \
                         where ID=%d",
			edit_address1_edit->text().toUtf8().constData(),
			edit_address2_edit->text().toUtf8().constData(),
			edit_city_edit->text().toUtf8().constData(),
			edit_state_box->currentStateCode().toUtf8().constData(),
			edit_zipcode_edit->text().toUtf8().constData(),
			edit_contact_name_edit->text().toUtf8().constData(),
			edit_contact_phone_edit->text().toUtf8().constData(),
			edit_contact_fax_edit->text().toUtf8().constData(),
			edit_contact_email_edit->text().toUtf8().constData(),
			edit_id);
  QSqlQuery *q=new QSqlQuery(sql);
  delete q;
  done(0);
}


void EditProvider::cancelData()
{
  done(-1);
}


void EditProvider::paintEvent(QPaintEvent *e)
{
  QPainter *p=new QPainter(this);
  p->setPen(Qt::black);
  p->drawRect(10,138,sizeHint().width()-20,86);
  p->end();
  delete p;
}
