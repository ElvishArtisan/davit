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
#include <QLabel>
#include <QLineEdit>

#include <dvtuser.h>

class EditUser : public QDialog
{
 Q_OBJECT
 public:
  EditUser(QString lname,QWidget *parent=0);
  ~EditUser();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void changePasswordData();
  void adminToggledData(bool state);
  void viewAffiliateToggledData(bool state);
  void viewProviderToggledData(bool state);
  void viewProgramToggledData(bool state);
  void okData();
  void cancelData();

 private:
  QLineEdit *edit_loginname_edit;
  QLineEdit *edit_fullname_edit;
  QLineEdit *edit_description_edit;
  QLineEdit *edit_email_edit;
  QLineEdit *edit_phone_edit;
//  QComboBox *edit_admin_box;
  QString edit_loginname;
  QString edit_password;
  bool edit_password_changed;
  QCheckBox *edit_admin_box;
  QLabel *edit_admin_label;
  QCheckBox *edit_affiliate_view_box;
  QLabel *edit_affiliate_view_label;
  QCheckBox *edit_affiliate_edit_box;
  QLabel *edit_affiliate_edit_label;
  QCheckBox *edit_affiliate_schedule_box;
  QLabel *edit_affiliate_schedule_label;
  QCheckBox *edit_affiliate_remark_box;
  QLabel *edit_affiliate_remark_label;
  QCheckBox *edit_provider_view_box;
  QLabel *edit_provider_view_label;
  QCheckBox *edit_provider_edit_box;
  QLabel *edit_provider_edit_label;
  QCheckBox *edit_program_view_box;
  QLabel *edit_program_view_label;
  QCheckBox *edit_program_edit_box;
  QLabel *edit_program_edit_label;
  QCheckBox *edit_report_view_box;
  QLabel *edit_report_view_label;
};


#endif  // EDIT_USER_H
