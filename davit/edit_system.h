// edit_system.h
//
// Edit Davit system-wide settings.
//
//   (C) Copyright 2010-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef EDIT_SYSTEM_H
#define EDIT_SYSTEM_H

#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>

#include <dvtuser.h>

#include "dialog.h"

class EditSystem : public Dialog
{
 Q_OBJECT
 public:
  EditSystem(DvtConfig *c,QWidget *parent=0);
  ~EditSystem();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec();

 private slots:
  void testData();
  void okData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);
  void closeEvent(QCloseEvent *e);

 private:
  //  QGroupBox *edit_smtp_group;
  //  QLabel *edit_smtp_hostname_label;
  //  QLineEdit *edit_smtp_hostname_edit;
  //  QLabel *edit_smtp_port_label;
  //  QSpinBox *edit_smtp_port_spin;
  QLabel *edit_origin_email_label;
  QLineEdit *edit_origin_email_edit;
  QLabel *edit_alert_email_label;
  QLineEdit *edit_alert_email_edit;
  QLabel *edit_affidavit_subject_label;
  QLineEdit *edit_affidavit_subject_edit;
  QLabel *edit_affidavit_template_label;
  QTextEdit *edit_affidavit_template_edit;
  QPushButton *edit_test_button;
  QPushButton *edit_ok_button;
  QPushButton *edit_cancel_button;
  QLabel *edit_key_label;
};


#endif  // EDIT_SYSTEM_H
