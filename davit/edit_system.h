// edit_system.h
//
// Edit Davit system-wide settings.
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: edit_system.h,v 1.3 2011/03/22 15:12:53 pcvs Exp $
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

#include <qdialog.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include <dvtuser.h>

class EditSystem : public QDialog
{
 Q_OBJECT
 public:
  EditSystem(QWidget *parent=0,const char *name=0);
  ~EditSystem();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void testData();
  void okData();
  void cancelData();

 protected:
  void paintEvent(QPaintEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *edit_smtp_label;
  QLabel *edit_smtp_hostname_label;
  QLineEdit *edit_smtp_hostname_edit;
  QLabel *edit_smtp_port_label;
  QSpinBox *edit_smtp_port_spin;
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
