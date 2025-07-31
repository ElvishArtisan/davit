// edit_user.cpp
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

#include <dvtconf.h>
#include <dvtdb.h>

#include "edit_user.h"
#include "globals.h"

EditUser::EditUser(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMaximumHeight(sizeHint().height());

  setWindowTitle("Davit - "+tr("Edit User"));

  edit_changepassword_dialog=new ChangePassword(c,this);

  //
  // User Name
  //
  edit_loginname_edit=new QLineEdit(this);
  edit_loginname_edit->setFont(defaultFont());
  edit_loginname_edit->setReadOnly(true);
  edit_loginname_label=new QLabel(tr("User Name")+":",this);
  edit_loginname_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_loginname_label->setFont(labelFont());

  //
  // Full Name
  //
  edit_fullname_edit=new QLineEdit(this);
  edit_fullname_edit->setFont(defaultFont());
  edit_fullname_edit->setMaxLength(64);
  edit_fullname_label=new QLabel(tr("Full Name")+":",this);
  edit_fullname_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_fullname_label->setFont(labelFont());

  //
  // Description
  //
  edit_description_edit=new QLineEdit(this);
  edit_description_edit->setFont(defaultFont());
  edit_description_edit->setMaxLength(64);
  edit_description_label=new QLabel(tr("Description")+":",this);
  edit_description_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_description_label->setFont(labelFont());

  //
  // Email
  //
  edit_email_edit=new QLineEdit(this);
  edit_email_edit->setFont(defaultFont());
  edit_email_edit->setMaxLength(64);
  edit_email_label=new QLabel(tr("E-Mail Address")+":",this);
  edit_email_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_email_label->setFont(labelFont());

  //
  // Phone Number
  //
  edit_phone_edit=new QLineEdit(this);
  edit_phone_edit->setFont(defaultFont());
  edit_phone_edit->setMaxLength(14);
  edit_phone_label=new QLabel(tr("Phone Number")+":",this);
  edit_phone_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_phone_label->setFont(labelFont());

  //
  // Change Password Button
  //
  edit_password_button=new QPushButton(this);
  edit_password_button->setFont(labelFont());
  edit_password_button->setText(tr("Change")+"\n"+tr("Password"));
  connect(edit_password_button,SIGNAL(clicked()),this,SLOT(changePasswordData()));

  //
  // Administrative Rights
  //
  edit_admin_group=new QGroupBox(tr("Administrative Rights"),this);
  edit_admin_group->setFont(labelFont());

  edit_admin_config_check=new QCheckBox(edit_admin_group);
  connect(edit_admin_config_check,SIGNAL(toggled(bool)),
	  this,SLOT(adminToggledData(bool)));
  edit_admin_config_label=new QLabel(tr("Administer System"),edit_admin_group);
  edit_admin_config_label->setFont(subLabelFont());
  edit_admin_config_label->setAlignment(Qt::AlignLeft);

  //
  // Affiliate Rights
  //
  edit_affiliate_group=new QGroupBox(tr("Affiliate Rights"),this);
  edit_affiliate_group->setFont(labelFont());

  edit_affiliate_view_check=new QCheckBox(edit_affiliate_group);
  connect(edit_affiliate_view_check,SIGNAL(toggled(bool)),
	  this,SLOT(viewAffiliateToggledData(bool)));
  edit_affiliate_view_label=
    new QLabel(tr("View Affiliate Data"),edit_affiliate_group);
  edit_affiliate_view_label->setFont(subLabelFont());
  edit_affiliate_view_label->setAlignment(Qt::AlignLeft);

  edit_affiliate_edit_check=new QCheckBox(edit_affiliate_group);
  edit_affiliate_edit_label=
    new QLabel(tr("Edit Affiliate Data"),edit_affiliate_group);
  edit_affiliate_edit_label->setFont(subLabelFont());
  edit_affiliate_edit_label->setAlignment(Qt::AlignLeft);

  edit_affiliate_schedule_check=new QCheckBox(edit_affiliate_group);
  edit_affiliate_schedule_label=
    new QLabel(tr("Schedule Programs"),edit_affiliate_group);
  edit_affiliate_schedule_label->setFont(subLabelFont());
  edit_affiliate_schedule_label->setAlignment(Qt::AlignLeft);

  edit_affiliate_remark_check=new QCheckBox(edit_affiliate_group);
  edit_affiliate_remark_label=
    new QLabel(tr("Add Remarks"),edit_affiliate_group);
  edit_affiliate_remark_label->setFont(subLabelFont());
  edit_affiliate_remark_label->setAlignment(Qt::AlignLeft);

  //
  // Provider Rights
  //
  edit_provider_group=new QGroupBox(tr("Provider Rights"),this);
  edit_provider_group->setFont(labelFont());

  edit_provider_view_check=new QCheckBox(edit_provider_group);
  connect(edit_provider_view_check,SIGNAL(toggled(bool)),
	  this,SLOT(viewProviderToggledData(bool)));
  edit_provider_view_label=
    new QLabel(tr("View Provider Data"),edit_provider_group);
  edit_provider_view_label->setFont(subLabelFont());
  edit_provider_view_label->setAlignment(Qt::AlignLeft);

  edit_provider_edit_check=new QCheckBox(edit_provider_group);
  edit_provider_edit_label=
    new QLabel(tr("Edit Provider Data"),edit_provider_group);
  edit_provider_edit_label->setFont(subLabelFont());
  edit_provider_edit_label->setAlignment(Qt::AlignLeft);

  //
  // Program Rights
  //
  edit_program_group=new QGroupBox(tr("Program Rights"),this);
  edit_program_group->setFont(labelFont());

  edit_program_view_check=new QCheckBox(edit_program_group);
  connect(edit_program_view_check,SIGNAL(toggled(bool)),
	  this,SLOT(viewProgramToggledData(bool)));
  edit_program_view_label=
    new QLabel(tr("View Program Data"),edit_program_group);
  edit_program_view_label->setFont(subLabelFont());
  edit_program_view_label->setAlignment(Qt::AlignLeft);

  edit_program_edit_check=new QCheckBox(edit_program_group);
  edit_program_edit_label=
    new QLabel(tr("Edit Program Data"),edit_program_group);
  edit_program_edit_label->setFont(subLabelFont());
  edit_program_edit_label->setAlignment(Qt::AlignLeft);

  //
  // Report Rights
  //
  edit_report_group=new QGroupBox(tr("Report Rights"),this);
  edit_report_group->setFont(labelFont());

  edit_report_view_check=new QCheckBox(edit_report_group);
  edit_report_view_label=
    new QLabel(tr("View Report Data"),edit_report_group);
  edit_report_view_label->setFont(subLabelFont());
  edit_report_view_label->setAlignment(Qt::AlignLeft);

  //
  //  OK Button
  //
  edit_ok_button=new QPushButton(this);
  edit_ok_button->setDefault(true);
  edit_ok_button->setFont(labelFont());
  edit_ok_button->setText("OK");
  connect(edit_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  edit_cancel_button=new QPushButton(this);
  edit_cancel_button->setFont(labelFont());
  edit_cancel_button->setText("Cancel");
  connect(edit_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
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
  return QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
}


int EditUser::exec(const QString &username)
{
  edit_loginname=username;

  DvtUser *user=new DvtUser(username);
  edit_loginname_edit->setText(user->name());
  edit_fullname_edit->setText(user->fullName());
  edit_description_edit->setText(user->description());
  edit_email_edit->setText(user->email());
  edit_phone_edit->setText(DvtFormatPhoneNumber(user->phoneNumber()));

  edit_admin_config_check->setChecked(user->privilege(DvtUser::PrivAdmin));
  edit_affiliate_view_check->
    setChecked(user->privilege(DvtUser::PrivAffiliateView));
  edit_affiliate_edit_check->
    setChecked(user->privilege(DvtUser::PrivAffiliateEdit));
  edit_affiliate_schedule_check->
    setChecked(user->privilege(DvtUser::PrivAffiliateSchedule));
  edit_affiliate_remark_check->
    setChecked(user->privilege(DvtUser::PrivAffiliateRemark));

  edit_provider_view_check->
    setChecked(user->privilege(DvtUser::PrivProviderView));
  edit_provider_edit_check->
    setChecked(user->privilege(DvtUser::PrivProviderEdit));

  edit_program_view_check->
    setChecked(user->privilege(DvtUser::PrivProgramView));
  edit_program_edit_check->
    setChecked(user->privilege(DvtUser::PrivProgramEdit));

  edit_report_view_check->
    setChecked(user->privilege(DvtUser::PrivReportView));

  delete user;
  return QDialog::exec();
}


void EditUser::changePasswordData()
{
  if(edit_changepassword_dialog->exec(&edit_password)) {
    QString sql=QString("update `USERS` set ")+
      "`USER_PASSWORD`=password("+DvtSqlQuery::escape(edit_password)+") "+
      "where "+
      "`USER_NAME`="+DvtSqlQuery::escape(edit_loginname_edit->text());
    DvtSqlQuery::apply(sql);
  }
}


void EditUser::adminToggledData(bool state)
{
  if((!state)&&(global_dvtuser->name()==edit_loginname)) {
    QMessageBox::information(this,"Current User",
       tr("You cannot revoke administrative\nprivileges on the current user!"));
    edit_admin_config_check->setChecked(true);
  }
}


void EditUser::viewAffiliateToggledData(bool state)
{
  edit_affiliate_edit_check->setEnabled(state);
  edit_affiliate_edit_label->setEnabled(state);
  edit_affiliate_schedule_check->setEnabled(state);
  edit_affiliate_schedule_label->setEnabled(state);
  edit_affiliate_remark_check->setEnabled(state);
  edit_affiliate_remark_label->setEnabled(state);
}


void EditUser::viewProviderToggledData(bool state)
{
  edit_provider_edit_check->setEnabled(state);
  edit_provider_edit_label->setEnabled(state);
}


void EditUser::viewProgramToggledData(bool state)
{
  edit_program_edit_check->setEnabled(state);
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
  user->setPrivilege(DvtUser::PrivAdmin,edit_admin_config_check->isChecked());
  user->setPrivilege(DvtUser::PrivAffiliateView,
		     edit_affiliate_view_check->isChecked());
  user->setPrivilege(DvtUser::PrivAffiliateEdit,
		     edit_affiliate_edit_check->isChecked());
  user->setPrivilege(DvtUser::PrivAffiliateSchedule,
		     edit_affiliate_schedule_check->isChecked());
  user->setPrivilege(DvtUser::PrivAffiliateRemark,
		     edit_affiliate_remark_check->isChecked());
  user->setPrivilege(DvtUser::PrivProviderView,
		     edit_provider_view_check->isChecked());
  user->setPrivilege(DvtUser::PrivProviderEdit,
		     edit_provider_edit_check->isChecked());
  user->setPrivilege(DvtUser::PrivProgramView,
		     edit_program_view_check->isChecked());
  user->setPrivilege(DvtUser::PrivProgramEdit,
		     edit_program_edit_check->isChecked());
  user->setPrivilege(DvtUser::PrivReportView,
		     edit_report_view_check->isChecked());
  user->save();
  delete user;

  done(true);
}


void EditUser::cancelData()
{
  done(false);
}


void EditUser::closeEvent(QCloseEvent *e)
{
  cancelData();
}


void EditUser::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();
  
  edit_loginname_edit->setGeometry(120,10,80,20);
  edit_loginname_label->setGeometry(10,10,105,20);

  edit_fullname_edit->setGeometry(120,32,w-130,20);
  edit_fullname_label->setGeometry(10,32,105,20);

  edit_description_edit->setGeometry(120,54,w-130,20);
  edit_description_label->setGeometry(10,54,105,20);

  edit_email_edit->setGeometry(120,76,w-130,20);
  edit_email_label->setGeometry(10,76,105,20);

  edit_phone_edit->setGeometry(120,98,120,20);
  edit_phone_label->setGeometry(10,98,105,20);

  edit_password_button->setGeometry(w-100,98,90,44);

  edit_admin_group->setGeometry(10,151,w-20,45);
  edit_admin_config_check->setGeometry(10,21+4,15,15);
  edit_admin_config_label->setGeometry(30,21+5,150,19);

  edit_affiliate_group->setGeometry(10,205,w-20,65);
  edit_affiliate_view_check->setGeometry(10,21+4,15,15);
  edit_affiliate_view_label->setGeometry(30,21+5,150,19);
  edit_affiliate_edit_check->setGeometry(w/2+10,21+4,15,15);
  edit_affiliate_edit_label->setGeometry(w/2+30,21+5,100,19);
  edit_affiliate_schedule_check->setGeometry(10,42+4,15,15);
  edit_affiliate_schedule_label->setGeometry(30,42+5,130,19);
  edit_affiliate_remark_check->setGeometry(w/2+10,42+4,15,15);
  edit_affiliate_remark_label->setGeometry(w/2+30,42+5,100,19);

  edit_provider_group->setGeometry(10,277,w-20,45);
  edit_provider_view_check->setGeometry(10,21+4,15,15);
  edit_provider_view_label->setGeometry(30,21+5,130,19);
  edit_provider_edit_check->setGeometry(w/2+10,21+4,15,15);
  edit_provider_edit_label->setGeometry(w/2+30,21+5,100,19);

  edit_program_group->setGeometry(10,332,w-20,45);
  edit_program_view_check->setGeometry(10,21+4,15,15);
  edit_program_view_label->setGeometry(30,21+5,130,19);
  edit_program_edit_check->setGeometry(w/2+10,21+4,15,15);
  edit_program_edit_label->setGeometry(w/2+30,21+5,100,19);

  edit_report_group->setGeometry(10,387,w-20,45);
  edit_report_view_check->setGeometry(10,21+4,15,15);
  edit_report_view_label->setGeometry(30,21+5,130,19);

  edit_ok_button->setGeometry(w-180,h-60,80,50);
  edit_cancel_button->setGeometry(w-90,h-60,80,50);
}
