// edit_contact.h
//
// Edit a Davit Contact
//
//   (C) Copyright 2010-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef EDIT_CONTACT_H
#define EDIT_CONTACT_H

#include <QCheckBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>

#include "contact.h"

class EditContact : public QDialog
{
 Q_OBJECT
 public:
  EditContact(QWidget *parent=0);
  ~EditContact();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(Contact *contact);

 private slots:
  void emailChangedData(const QString &str);
  void okData();
  void cancelData();

 private:
  QLineEdit *edit_name_edit;
  QLineEdit *edit_title_edit;
  QLineEdit *edit_phone_edit;
  QLineEdit *edit_fax_edit;
  QLineEdit *edit_email_edit;
  QCheckBox *edit_affidavit_box;
  QLabel *edit_affidavit_label;
  QCheckBox *edit_program_director_box;
  QLabel *edit_program_director_label;
  QCheckBox *edit_general_manager_box;
  QLabel *edit_general_manager_label;
  QCheckBox *edit_locked_box;
  QLabel *edit_locked_label;
  Contact *edit_contact;
};


#endif  // EDIT_CONTACT_H
