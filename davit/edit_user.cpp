// edit_show.cpp
//
// Edit a Davit User.
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

#include <QButtonGroup>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <change_password.h>
#include <dvtconf.h>

#include "edit_user.h"
#include "globals.h"

EditUser::EditUser(QString lname,QWidget *parent)
  : QDialog(parent)
{
  setModal(true);
  edit_loginname=lname;
  edit_password_changed=false;
  DvtUser *user=new DvtUser(lname);

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setWindowTitle("Davit - Edit User");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont section_font=QFont("Helvetica",10,QFont::Bold);
  section_font.setPixelSize(10);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  font.setPixelSize(10);

  //
  // User Name
  //
  edit_loginname_edit=new QLineEdit(this);
  edit_loginname_edit->setGeometry(120,10,80,20);
  edit_loginname_edit->setFont(font);
  edit_loginname_edit->setReadOnly(true);
  edit_loginname_edit->setText(user->name());
  QLabel *label=new QLabel("User Name:",this);
  label->setGeometry(10,10,105,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Full Name
  //
  edit_fullname_edit=new QLineEdit(this);
  edit_fullname_edit->setGeometry(120,32,sizeHint().width()-130,20);
  edit_fullname_edit->setFont(font);
  edit_fullname_edit->setMaxLength(64);
  edit_fullname_edit->setText(user->fullName());
  label=new QLabel("Full Name:",this);
  label->setGeometry(10,32,105,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Description
  //
  edit_description_edit=new QLineEdit(this);
  edit_description_edit->setGeometry(120,54,sizeHint().width()-130,20);
  edit_description_edit->setFont(font);
  edit_description_edit->setMaxLength(64);
  edit_description_edit->setText(user->description());
  label=new QLabel("Description:",this);
  label->setGeometry(10,54,105,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Email
  //
  edit_email_edit=new QLineEdit(this);
  edit_email_edit->setGeometry(120,76,sizeHint().width()-130,20);
  edit_email_edit->setFont(font);
  edit_email_edit->setMaxLength(64);
  edit_email_edit->setText(user->email());
  label=new QLabel("E-Mail Address:",this);
  label->setGeometry(10,76,105,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Phone Number
  //
  edit_phone_edit=new QLineEdit(this);
  edit_phone_edit->setGeometry(120,98,120,20);
  edit_phone_edit->setFont(font);
  edit_phone_edit->setMaxLength(14);
  edit_phone_edit->setText(DvtFormatPhoneNumber(user->phoneNumber()));
  label=new QLabel("Phone Number:",this);
  label->setGeometry(10,98,105,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Change Password Button
  //
  QPushButton *button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-100,98,90,44);
  button->setFont(label_font);
  button->setText("Change\n&Password");
  connect(button,SIGNAL(clicked()),this,SLOT(changePasswordData()));

  //
  // Administrative Rights
  //
  /*
  QButtonGroup *group=new QButtonGroup(tr("Administrative Rights"),this);
  group->setGeometry(10,147,sizeHint().width()-20,45);
  group->setFont(label_font);

  edit_admin_box=new QCheckBox(group,"edit_admin_box");
  edit_admin_box->setGeometry(10,21,15,15);
  edit_admin_box->setChecked(user->privilege(DvtUser::PrivAdmin));
  connect(edit_admin_box,SIGNAL(toggled(bool)),
	  this,SLOT(adminToggledData(bool)));
  label=new QLabel(edit_admin_box,tr("Administer S&ystem"),group);
  label->setGeometry(30,21,150,19);
  label->setFont(small_font);
  label->setAlignment(Qt::AlignLeft|Qt::ShowPrefix);
  */
  //
  // Affiliate Rights
  //
  /*
  group=new QButtonGroup(tr("Affiliate Rights"),this);
  group->setGeometry(10,200,sizeHint().width()-20,65);
  group->setFont(label_font);

  edit_affiliate_view_box=new QCheckBox(group,"edit_affiliate_view_box");
  edit_affiliate_view_box->setGeometry(10,21,15,15);
  edit_affiliate_view_box->
    setChecked(user->privilege(DvtUser::PrivAffiliateView));
  connect(edit_affiliate_view_box,SIGNAL(toggled(bool)),
	  this,SLOT(viewAffiliateToggledData(bool)));
  edit_affiliate_view_label=
    new QLabel(edit_affiliate_view_box,tr("View Affiliate Data"),group);
  edit_affiliate_view_label->setGeometry(30,21,130,19);
  edit_affiliate_view_label->setFont(small_font);
  edit_affiliate_view_label->setAlignment(Qt::AlignLeft|Qt::ShowPrefix);

  edit_affiliate_edit_box=new QCheckBox(group,"edit_affiliate_edit_box");
  edit_affiliate_edit_box->setGeometry(sizeHint().width()/2+10,21,15,15);
  edit_affiliate_edit_box->
    setChecked(user->privilege(DvtUser::PrivAffiliateEdit));
  edit_affiliate_edit_label=
    new QLabel(edit_affiliate_edit_box,tr("Edit Affiliate Data"),group);
  edit_affiliate_edit_label->setGeometry(sizeHint().width()/2+30,21,100,19);
  edit_affiliate_edit_label->setFont(small_font);
  edit_affiliate_edit_label->setAlignment(Qt::AlignLeft|Qt::ShowPrefix);

  edit_affiliate_schedule_box=
    new QCheckBox(group,"edit_affiliate_schedule_box");
  edit_affiliate_schedule_box->setGeometry(10,42,15,15);
  edit_affiliate_schedule_box->
    setChecked(user->privilege(DvtUser::PrivAffiliateSchedule));
  edit_affiliate_schedule_label=
    new QLabel(edit_affiliate_schedule_box,tr("Schedule Programs"),group);
  edit_affiliate_schedule_label->setGeometry(30,42,130,19);
  edit_affiliate_schedule_label->setFont(small_font);
  edit_affiliate_schedule_label->setAlignment(Qt::AlignLeft|Qt::ShowPrefix);

  edit_affiliate_remark_box=new QCheckBox(group,"edit_affiliate_remark_box");
  edit_affiliate_remark_box->setGeometry(sizeHint().width()/2+10,42,15,15);
  edit_affiliate_remark_box->
    setChecked(user->privilege(DvtUser::PrivAffiliateRemark));
  edit_affiliate_remark_label=
    new QLabel(edit_affiliate_remark_box,tr("Add Remarks"),group);
  edit_affiliate_remark_label->setGeometry(sizeHint().width()/2+30,42,100,19);
  edit_affiliate_remark_label->setFont(small_font);
  edit_affiliate_remark_label->setAlignment(Qt::AlignLeft|Qt::ShowPrefix);
  //
  // Provider Rights
  //
  group=new QButtonGroup(tr("Provider Rights"),this);
  group->setGeometry(10,277,sizeHint().width()-20,45);
  group->setFont(label_font);

  edit_provider_view_box=new QCheckBox(group,"edit_provider_view_box");
  edit_provider_view_box->setGeometry(10,21,15,15);
  edit_provider_view_box->
    setChecked(user->privilege(DvtUser::PrivProviderView));
  connect(edit_provider_view_box,SIGNAL(toggled(bool)),
	  this,SLOT(viewProviderToggledData(bool)));
  edit_provider_view_label=
    new QLabel(edit_provider_view_box,tr("View Provider Data"),group);
  edit_provider_view_label->setGeometry(30,21,130,19);
  edit_provider_view_label->setFont(small_font);
  edit_provider_view_label->setAlignment(Qt::AlignLeft|Qt::ShowPrefix);

  edit_provider_edit_box=new QCheckBox(group,"edit_provider_edit_box");
  edit_provider_edit_box->setGeometry(sizeHint().width()/2+10,21,15,15);
  edit_provider_edit_box->
    setChecked(user->privilege(DvtUser::PrivProviderEdit));
  edit_provider_edit_label=
    new QLabel(edit_provider_edit_box,tr("Edit Provider Data"),group);
  edit_provider_edit_label->setGeometry(sizeHint().width()/2+30,21,100,19);
  edit_provider_edit_label->setFont(small_font);
  edit_provider_edit_label->setAlignment(Qt::AlignLeft|Qt::ShowPrefix);

  //
  // Program Rights
  //
  group=new QButtonGroup(tr("Program Rights"),this);
  group->setGeometry(10,332,sizeHint().width()-20,45);
  group->setFont(label_font);

  edit_program_view_box=new QCheckBox(group,"edit_program_view_box");
  edit_program_view_box->setGeometry(10,21,15,15);
  edit_program_view_box->
    setChecked(user->privilege(DvtUser::PrivProgramView));
  connect(edit_program_view_box,SIGNAL(toggled(bool)),
	  this,SLOT(viewProgramToggledData(bool)));
  edit_program_view_label=
    new QLabel(edit_program_view_box,tr("View Program Data"),group);
  edit_program_view_label->setGeometry(30,21,130,19);
  edit_program_view_label->setFont(small_font);
  edit_program_view_label->setAlignment(Qt::AlignLeft|Qt::ShowPrefix);

  edit_program_edit_box=new QCheckBox(group,"edit_program_edit_box");
  edit_program_edit_box->setGeometry(sizeHint().width()/2+10,21,15,15);
  edit_program_edit_box->
    setChecked(user->privilege(DvtUser::PrivProgramEdit));
  edit_program_edit_label=
    new QLabel(edit_program_edit_box,tr("Edit Program Data"),group);
  edit_program_edit_label->setGeometry(sizeHint().width()/2+30,21,100,19);
  edit_program_edit_label->setFont(small_font);
  edit_program_edit_label->setAlignment(Qt::AlignLeft|Qt::ShowPrefix);

  //
  // Report Rights
  //
  group=new QButtonGroup(tr("Report Rights"),this);
  group->setGeometry(10,387,sizeHint().width()-20,45);
  group->setFont(label_font);

  edit_report_view_box=new QCheckBox(group,"edit_report_view_box");
  edit_report_view_box->setGeometry(10,21,15,15);
  edit_report_view_box->
    setChecked(user->privilege(DvtUser::PrivReportView));
  edit_report_view_label=
    new QLabel(edit_report_view_box,tr("View Reports"),group);
  edit_report_view_label->setGeometry(30,21,130,19);
  edit_report_view_label->setFont(small_font);
  edit_report_view_label->setAlignment(Qt::AlignLeft|Qt::ShowPrefix);
  */

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
}


EditUser::~EditUser()
{
}


QSize EditUser::sizeHint() const
{
  return QSize(350,512);
} 


QSizePolicy EditUser::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditUser::changePasswordData()
{
  ChangePassword *change=new ChangePassword(&edit_password,this);
  if(change->exec()==0) {
    edit_password_changed=true;
  }
  delete change;
}


void EditUser::adminToggledData(bool state)
{
  if((!state)&&(global_dvtuser->name()==edit_loginname)) {
    QMessageBox::information(this,"Current User",
	"You cannot revoke administrative\nprivileges on the current user!");
    edit_admin_box->setChecked(true);
  }
}


void EditUser::viewAffiliateToggledData(bool state)
{
  edit_affiliate_edit_box->setEnabled(state);
  edit_affiliate_edit_label->setEnabled(state);
  edit_affiliate_schedule_box->setEnabled(state);
  edit_affiliate_schedule_label->setEnabled(state);
  edit_affiliate_remark_box->setEnabled(state);
  edit_affiliate_remark_label->setEnabled(state);
}


void EditUser::viewProviderToggledData(bool state)
{
  edit_provider_edit_box->setEnabled(state);
  edit_provider_edit_label->setEnabled(state);
}


void EditUser::viewProgramToggledData(bool state)
{
  edit_program_edit_box->setEnabled(state);
  edit_program_edit_label->setEnabled(state);
}


void EditUser::okData()
{
  bool ok=false;
  DvtUser *user=new DvtUser(edit_loginname_edit->text());
  QString pnum=DvtNormalizePhoneNumber(edit_phone_edit->text(),&ok);
  if((!pnum.isEmpty())&&(!ok)) {
    QMessageBox::warning(this,"Davit - Edit User",
			 tr("The phone number is invalid!"));
    return;
  }
  user->setFullName(edit_fullname_edit->text());
  user->setDescription(edit_description_edit->text());
  user->setEmail(edit_email_edit->text());
  user->setPhoneNumber(pnum);
  user->setPrivilege(DvtUser::PrivAdmin,edit_admin_box->isChecked());
  user->setPrivilege(DvtUser::PrivAffiliateView,
		     edit_affiliate_view_box->isChecked());
  user->setPrivilege(DvtUser::PrivAffiliateEdit,
		     edit_affiliate_edit_box->isChecked());
  user->setPrivilege(DvtUser::PrivAffiliateSchedule,
		     edit_affiliate_schedule_box->isChecked());
  user->setPrivilege(DvtUser::PrivAffiliateRemark,
		     edit_affiliate_remark_box->isChecked());
  user->setPrivilege(DvtUser::PrivProviderView,
		     edit_provider_view_box->isChecked());
  user->setPrivilege(DvtUser::PrivProviderEdit,
		     edit_provider_edit_box->isChecked());
  user->setPrivilege(DvtUser::PrivProgramView,
		     edit_program_view_box->isChecked());
  user->setPrivilege(DvtUser::PrivProgramEdit,
		     edit_program_edit_box->isChecked());
  user->setPrivilege(DvtUser::PrivReportView,
		     edit_report_view_box->isChecked());
  user->save();
  delete user;

  //
  // Update Password
  //
  if(edit_password_changed) {
    QString sql;
    sql=QString().sprintf("update USERS set USER_PASSWORD=password(\"%s\")\
                           where USER_NAME=\"%s\"",
			  edit_password.toUtf8().constData(),
			  edit_loginname.toUtf8().constData());    
    QSqlQuery *q=new QSqlQuery(sql);
    delete q;
  }

  done(0);
}


void EditUser::cancelData()
{
  done(-1);
}
