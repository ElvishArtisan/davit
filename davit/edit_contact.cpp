// edit_contact.cpp
//
// Edit a Davit Contact.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
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

#include <qpushbutton.h>
#include <qlabel.h>
#include <qsqldatabase.h>
#include <qpainter.h>
#include <qmessagebox.h>

#include <math.h>

#include <dvtconf.h>

#include <edit_contact.h>

EditContact::EditContact(QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  edit_contact=NULL;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Davit - Edit Contact");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont day_font=QFont("Helvetica",10,QFont::Normal);
  day_font.setPixelSize(10);

  //
  // Name
  //
  edit_name_edit=new QLineEdit(this);
  edit_name_edit->setGeometry(100,10,sizeHint().width()-110,20);
  QLabel *label=new QLabel(edit_name_edit,"Name:",this);
  label->setGeometry(10,10,85,20);
  label->setFont(label_font);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  // Title
  //
  edit_title_edit=new QLineEdit(this);
  edit_title_edit->setGeometry(100,32,sizeHint().width()-110,20);
  label=new QLabel(edit_title_edit,"Title:",this);
  label->setGeometry(10,32,85,20);
  label->setFont(label_font);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  // Phone
  //
  edit_phone_edit=new QLineEdit(this);
  edit_phone_edit->setGeometry(100,54,sizeHint().width()-110,20);
  label=new QLabel(edit_phone_edit,"Phone:",this);
  label->setGeometry(10,54,85,20);
  label->setFont(label_font);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  // Fax
  //
  edit_fax_edit=new QLineEdit(this);
  edit_fax_edit->setGeometry(100,76,sizeHint().width()-110,20);
  label=new QLabel(edit_fax_edit,"Fax:",this);
  label->setGeometry(10,76,85,20);
  label->setFont(label_font);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  // Email
  //
  edit_email_edit=new QLineEdit(this);
  edit_email_edit->setGeometry(100,98,sizeHint().width()-110,20);
  connect(edit_email_edit,SIGNAL(textChanged(const QString &)),
    this,SLOT(emailChangedData(const QString &)));
  label=new QLabel(edit_email_edit,"Email:",this);
  label->setGeometry(10,98,85,20);
  label->setFont(label_font);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  // Affidavit Checkbox
  //
  edit_affidavit_box=new QCheckBox(this,"edit_affidavit_button");
  edit_affidavit_box->setGeometry(105,118,20,20);
  edit_affidavit_label=
    new QLabel(edit_affidavit_box,"Receives Affidavit E-mail",this);
  edit_affidavit_label->setGeometry(122,117,sizeHint().width()-130,20);
  edit_affidavit_label->setFont(label_font);
  edit_affidavit_label->setAlignment(AlignLeft|AlignVCenter);

  //
  // Program Director Checkbox
  //
  edit_program_director_box=new QCheckBox(this,"edit_program_director_button");
  edit_program_director_box->setGeometry(105,140,20,20);
  edit_program_director_label=
    new QLabel(edit_program_director_box,"Receives Program Director E-mail",
	       this);
  edit_program_director_label->setGeometry(122,137,sizeHint().width()-130,20);
  edit_program_director_label->setFont(label_font);
  edit_program_director_label->setAlignment(AlignLeft|AlignVCenter);

  //
  // General Manager Checkbox
  //
  edit_general_manager_box=new QCheckBox(this,"edit_general_manager_button");
  edit_general_manager_box->setGeometry(105,162,20,20);
  edit_general_manager_label=
    new QLabel(edit_general_manager_box,"Receives General Manager E-mail",
	       this);
  edit_general_manager_label->setGeometry(122,161,sizeHint().width()-130,20);
  edit_general_manager_label->setFont(label_font);
  edit_general_manager_label->setAlignment(AlignLeft|AlignVCenter);

  //
  // Locked Checkbox
  //
  edit_locked_box=new QCheckBox(this,"edit_locked_button");
  edit_locked_box->setGeometry(105,192,20,20);
  edit_locked_label=
    new QLabel(edit_locked_box,"Lock This Contact Against External Updates",
	       this);
  edit_locked_label->setGeometry(122,191,sizeHint().width()-130,20);
  edit_locked_label->setFont(label_font);
  edit_locked_label->setAlignment(AlignLeft|AlignVCenter);

  //
  //  OK Button
  //
  QPushButton *button=new QPushButton(this,"ok_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(label_font);
  button->setText("&OK");
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this,"cancel_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText("&Cancel");
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));
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


int EditContact::exec(Contact *contact)
{
  edit_contact=contact;
  edit_name_edit->setText(contact->name());
  edit_title_edit->setText(contact->title());
  edit_phone_edit->setText(DvtFormatPhoneNumber(contact->phone()));
  edit_fax_edit->setText(DvtFormatPhoneNumber(contact->fax()));
  edit_email_edit->setText(contact->email());
  edit_affidavit_box->setChecked(contact->affidavit());
  edit_program_director_box->setChecked(contact->programDirector());
  edit_general_manager_box->setChecked(contact->generalManager());
  edit_locked_box->setChecked(contact->locked());
  emailChangedData(contact->email());
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
  
  edit_contact->setName(edit_name_edit->text());
  edit_contact->setTitle(edit_title_edit->text());
  edit_contact->setPhone(phone);
  edit_contact->setFax(fax);
  edit_contact->setEmail(edit_email_edit->text());
  edit_contact->setAffidavit(edit_affidavit_box->isChecked());
  edit_contact->setProgramDirector(edit_program_director_box->isChecked());
  edit_contact->setGeneralManager(edit_general_manager_box->isChecked());
  edit_contact->setLocked(edit_locked_box->isChecked());
  done(0);
}


void EditContact::cancelData()
{
  done(-1);
}
