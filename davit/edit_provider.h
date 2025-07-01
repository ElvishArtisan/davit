// edit_provider.h
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

#ifndef EDIT_PROVIDER_H
#define EDIT_PROVIDER_H

#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <dvtdialog.h>
#include <statecombobox.h>

#include <list_programs.h>

class EditProvider : public DvtDialog
{
 Q_OBJECT
 public:
  EditProvider(DvtConfig *c,QWidget *parent=0);
  ~EditProvider();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(int provider_id,bool new_entry=false);

 private slots:
  void programsData();
  void okData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *edit_business_name_label;
  QLineEdit *edit_business_name_edit;
  QLabel *edit_address_label;
  QLineEdit *edit_address1_edit;
  QLineEdit *edit_address2_edit;
  QLabel *edit_city_label;
  QLineEdit *edit_city_edit;
  QLabel *edit_state_label;
  StateComboBox *edit_state_box;
  QLineEdit *edit_state_edit;
  QLabel *edit_zipcode_label;
  QLineEdit *edit_zipcode_edit;
  QGroupBox *edit_contact_group;
  QLabel *edit_contact_name_label;
  QLineEdit *edit_contact_name_edit;
  QLabel *edit_contact_phone_label;
  QLineEdit *edit_contact_phone_edit;
  QLabel *edit_contact_fax_label;
  QLineEdit *edit_contact_fax_edit;
  QLabel *edit_contact_email_label;
  QLineEdit *edit_contact_email_edit;
  QPushButton *edit_programs_button;
  QPushButton *edit_ok_button;
  QPushButton *edit_cancel_button;
  int edit_provider_id;
  ListPrograms *edit_listprograms_dialog;
};


#endif  // EDIT_PROVIDER_H
