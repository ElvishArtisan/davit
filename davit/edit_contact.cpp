// edit_contact.cpp
//
// Edit a Davit Contact.
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
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

#include <dvtconf.h>
#include <dvtdb.h>

#include "edit_contact.h"

EditContact::EditContact(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  edit_contact_id=-1;

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());

  setWindowTitle("Davit - "+tr("Edit Contact"));

  //
  // Name
  //
  edit_name_edit=new QLineEdit(this);
  edit_name_label=new QLabel("Name:",this);
  edit_name_label->setFont(labelFont());
  edit_name_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // Title
  //
  edit_title_edit=new QLineEdit(this);
  edit_title_label=new QLabel("Title:",this);
  edit_title_label->setFont(labelFont());
  edit_title_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // Phone
  //
  edit_phone_edit=new QLineEdit(this);
  edit_phone_label=new QLabel("Phone:",this);
  edit_phone_label->setFont(labelFont());
  edit_phone_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // Fax
  //
  edit_fax_edit=new QLineEdit(this);
  edit_fax_label=new QLabel("Fax:",this);
  edit_fax_label->setFont(labelFont());
  edit_fax_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // Email
  //
  edit_email_edit=new QLineEdit(this);
  connect(edit_email_edit,SIGNAL(textChanged(const QString &)),
    this,SLOT(emailChangedData(const QString &)));
  edit_email_label=new QLabel("Email:",this);
  edit_email_label->setFont(labelFont());
  edit_email_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // Affidavit Checkbox
  //
  edit_affidavit_box=new QCheckBox(this);
  edit_affidavit_label=new QLabel("Receives Affidavit E-mail",this);
  edit_affidavit_label->setFont(labelFont());
  edit_affidavit_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Program Director Checkbox
  //
  edit_program_director_box=new QCheckBox(this);
  edit_program_director_label=
    new QLabel("Receives Program Director E-mail",this);
  edit_program_director_label->setFont(labelFont());
  edit_program_director_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // General Manager Checkbox
  //
  edit_general_manager_box=new QCheckBox(this);
  edit_general_manager_label=new QLabel("Receives General Manager E-mail",this);
  edit_general_manager_label->setFont(labelFont());
  edit_general_manager_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Locked Checkbox
  //
  edit_locked_box=new QCheckBox(this);
  edit_locked_label=
    new QLabel("Lock This Contact Against External Updates",this);
  edit_locked_label->setFont(labelFont());
  edit_locked_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  //  OK Button
  //
  edit_ok_button=new QPushButton(this);
  edit_ok_button->setDefault(true);
  edit_ok_button->setFont(buttonFont());
  edit_ok_button->setText("&OK");
  connect(edit_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  edit_cancel_button=new QPushButton(this);
  edit_cancel_button->setFont(buttonFont());
  edit_cancel_button->setText("&Cancel");
  connect(edit_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


EditContact::~EditContact()
{
}


QSize EditContact::sizeHint() const
{
  return QSize(450,282);
} 


QSizePolicy EditContact::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int EditContact::exec(int contact_id,bool new_contact)
{
  edit_contact_id=contact_id;

  QString sql=QString("select ")+
    "`CONTACTS`.`AFFILIATE_ID`,"+      // 00
    "`CONTACTS`.`NAME`,"+              // 01
    "`CONTACTS`.`TITLE`,"+             // 02
    "`CONTACTS`.`PHONE`,"+             // 03
    "`CONTACTS`.`FAX`,"+               // 04
    "`CONTACTS`.`EMAIL`,"+             // 05
    "`CONTACTS`.`AFFIDAVIT`,"+         // 06
    "`CONTACTS`.`PROGRAM_DIRECTOR`,"+  // 07
    "`CONTACTS`.`GENERAL_MANAGER`,"+   // 08
    "`CONTACTS`.`LOCKED` "+            // 09
    "from `CONTACTS` where "+
    QString::asprintf("`CONTACTS`.`ID`=%d ",contact_id);
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  if(q->first()) {
    edit_name_edit->setText(q->value(1).toString());
    edit_title_edit->setText(q->value(2).toString());
    edit_phone_edit->setText(DvtFormatPhoneNumber(q->value(3).toString()));
    edit_fax_edit->setText(DvtFormatPhoneNumber(q->value(4).toString()));
    edit_email_edit->setText(q->value(5).toString());
    edit_affidavit_box->setChecked(q->value(6).toString()=="Y");
    edit_program_director_box->setChecked(q->value(7).toString()=="Y");
    edit_general_manager_box->setChecked(q->value(8).toString()=="Y");
    edit_locked_box->setChecked(q->value(9).toString()=="Y");
  }
  delete q;
  emailChangedData(edit_email_edit->text());
  if(new_contact) {
    edit_name_edit->selectAll();
  }

  return QDialog::exec();
}


void EditContact::emailChangedData(const QString &str)
{
  edit_affidavit_label->setEnabled(DvtIsEmailAddress(str));
  edit_affidavit_box->setEnabled(DvtIsEmailAddress(str));
  edit_program_director_label->setEnabled(DvtIsEmailAddress(str));
  edit_program_director_box->setEnabled(DvtIsEmailAddress(str));
}


void EditContact::okData()
{
  bool ok=false;
  QString phone=DvtNormalizePhoneNumber(edit_phone_edit->text(),&ok);
  if((!edit_phone_edit->text().isEmpty())&&(!ok)) {
    QMessageBox::warning(this,"Davit - Edit Contact",
			 tr("The phone number is invalid!"));
    return;
  }
  QString fax=DvtNormalizePhoneNumber(edit_fax_edit->text(),&ok);
  if((!edit_fax_edit->text().isEmpty())&&(!ok)) {
    QMessageBox::warning(this,"Davit - Edit Contact",
			 tr("The FAX number is invalid!"));
    return;
  }
  if((!edit_email_edit->text().isEmpty())&&
     (!DvtIsEmailAddress(edit_email_edit->text()))) {
    QMessageBox::warning(this,"Davit - Edit Contact",
			 tr("The E-Mail address is invalid!"));
    return;
  }

  QString sql=QString("update `CONTACTS` set ")+
    "`CONTACTS`.`NAME`="+DvtSqlQuery::escape(edit_name_edit->text())+","+
    "`CONTACTS`.`TITLE`="+DvtSqlQuery::escape(edit_title_edit->text())+","+
    "`CONTACTS`.`PHONE`="+DvtSqlQuery::escape(phone)+","+
    "`CONTACTS`.`FAX`="+DvtSqlQuery::escape(fax)+","+
    "`CONTACTS`.`EMAIL`="+DvtSqlQuery::escape(edit_email_edit->text())+","+
    "`CONTACTS`.`AFFIDAVIT`="+
    DvtSqlQuery::escape(edit_affidavit_box->isChecked())+","+
    "`CONTACTS`.`PROGRAM_DIRECTOR`="+
    DvtSqlQuery::escape(edit_program_director_box->isChecked())+","+
    "`CONTACTS`.`GENERAL_MANAGER`="+
    DvtSqlQuery::escape(edit_general_manager_box->isChecked())+","+
    "`CONTACTS`.`LOCKED`="+
    DvtSqlQuery::escape(edit_locked_box->isChecked())+" where "+
    QString::asprintf("`CONTACTS`.`ID`=%d ",edit_contact_id);
  DvtSqlQuery::apply(sql);

  done(true);
}


void EditContact::cancelData()
{
  done(false);
}


void EditContact::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  edit_name_edit->setGeometry(100,10,w-110,20);
  edit_name_label->setGeometry(10,10,85,20);
  edit_title_edit->setGeometry(100,32,w-110,20);
  edit_title_label->setGeometry(10,32,85,20);
  edit_phone_edit->setGeometry(100,54,w-110,20);
  edit_phone_label->setGeometry(10,54,85,20);
  edit_fax_edit->setGeometry(100,76,w-110,20);
  edit_fax_label->setGeometry(10,76,85,20);
  edit_email_edit->setGeometry(100,98,w-110,20);
  edit_email_label->setGeometry(10,98,85,20);
  edit_affidavit_box->setGeometry(105,118,20,20);
  edit_affidavit_label->setGeometry(122,118,w-130,20);
  edit_program_director_box->setGeometry(105,140,20,20);
  edit_program_director_label->setGeometry(122,140,w-130,20);
  edit_general_manager_box->setGeometry(105,162,20,20);
  edit_general_manager_label->setGeometry(122,162,w-130,20);
  edit_locked_box->setGeometry(105,192,20,20);
  edit_locked_label->setGeometry(122,191,w-130,20);
  edit_ok_button->setGeometry(w-180,h-60,80,50);
  edit_cancel_button->setGeometry(w-90,h-60,80,50);
}


void EditContact::closeEvent(QResizeEvent *e)
{
  cancelData();
}
