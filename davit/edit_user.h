// edit_user.h
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

#ifndef EDIT_USER_H
#define EDIT_USER_H

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

#include <dvtuser.h>

#include "change_password.h"
#include "dialog.h"

class EditUser : public Dialog
{
 Q_OBJECT
 public:
  EditUser(DvtConfig *c,QWidget *parent=0);
  ~EditUser();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(const QString &username);

 private slots:
  void changePasswordData();
  void adminToggledData(bool state);
  void viewAffiliateToggledData(bool state);
  void viewProviderToggledData(bool state);
  void viewProgramToggledData(bool state);
  void okData();
  void cancelData();

 protected:
  void closeEvent(QCloseEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *edit_loginname_label;
  QLineEdit *edit_loginname_edit;
  QLabel *edit_fullname_label;
  QLineEdit *edit_fullname_edit;
  QLabel *edit_description_label;
  QLineEdit *edit_description_edit;
  QLabel *edit_email_label;
  QLineEdit *edit_email_edit;
  QLabel *edit_phone_label;
  QLineEdit *edit_phone_edit;
  QString edit_loginname;
  QString edit_password;
  QPushButton *edit_password_button;

  QGroupBox *edit_admin_group;
  QCheckBox *edit_admin_config_check;
  QLabel *edit_admin_config_label;

  QGroupBox *edit_affiliate_group;
  QCheckBox *edit_affiliate_view_check;
  QLabel *edit_affiliate_view_label;
  QCheckBox *edit_affiliate_edit_check;
  QLabel *edit_affiliate_edit_label;
  QCheckBox *edit_affiliate_schedule_check;
  QLabel *edit_affiliate_schedule_label;
  QCheckBox *edit_affiliate_remark_check;
  QLabel *edit_affiliate_remark_label;

  QGroupBox *edit_provider_group;
  QCheckBox *edit_provider_view_check;
  QLabel *edit_provider_view_label;
  QCheckBox *edit_provider_edit_check;
  QLabel *edit_provider_edit_label;

  QGroupBox *edit_program_group;
  QCheckBox *edit_program_view_check;
  QLabel *edit_program_view_label;
  QCheckBox *edit_program_edit_check;
  QLabel *edit_program_edit_label;

  QGroupBox *edit_report_group;
  QCheckBox *edit_report_view_check;
  QLabel *edit_report_view_label;

  QPushButton *edit_ok_button;
  QPushButton *edit_cancel_button;

  ChangePassword *edit_changepassword_dialog;
};


#endif  // EDIT_USER_H
