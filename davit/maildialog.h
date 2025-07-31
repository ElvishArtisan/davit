// maildialog.h
//
// Compose an e-mail message.
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

#ifndef MAILDIALOG_H
#define MAILDIALOG_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

#include "contact.h"
#include "dialog.h"

class MailDialog : public Dialog
{
 Q_OBJECT
 public:
  MailDialog(DvtConfig *c,QWidget *parent=0);
  ~MailDialog();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(const QStringList &to_addrs,const QStringList &cc_addrs,
	   const QStringList &bcc_addrs,const QString &from_addr,
	   const QString &reply_addr,const QString &subj,const QString &msg);

 private slots:
  void addressChangedData();
  void sendData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *edit_to_label;
  QTextEdit *edit_to_edit;
  QLabel *edit_cc_label;
  QTextEdit *edit_cc_edit;
  QLabel *edit_bcc_label;
  QTextEdit *edit_bcc_edit;
  QLabel *edit_subject_label;
  QLineEdit *edit_subject_edit;
  QTextEdit *edit_body_edit;
  QPushButton *edit_send_button;
  QPushButton *edit_cancel_button;
  QString edit_from_address;
  QString edit_reply_address;
};


#endif  // MAILDIALOG_H
