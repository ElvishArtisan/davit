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

#include <QDialog>
#include <QLineEdit>

#include <statecombobox.h>

class EditProvider : public QDialog
{
 Q_OBJECT
 public:
  EditProvider(const QString &bname,QWidget *parent=0);
  ~EditProvider();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void programsData();
  void okData();
  void cancelData();

 protected:
  void paintEvent(QPaintEvent *e);

 private:
  QLineEdit *edit_business_name_edit;
  QLineEdit *edit_address1_edit;
  QLineEdit *edit_address2_edit;
  QLineEdit *edit_city_edit;
  StateComboBox *edit_state_box;
  QLineEdit *edit_state_edit;
  QLineEdit *edit_zipcode_edit;
  QLineEdit *edit_contact_name_edit;
  QLineEdit *edit_contact_phone_edit;
  QLineEdit *edit_contact_fax_edit;
  QLineEdit *edit_contact_email_edit;
  int edit_id;
};


#endif  // EDIT_PROVIDER_H
