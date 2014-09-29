// maildialog.h
//
// Compose an e-mail message.
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: maildialog.h,v 1.3 2011/02/02 18:06:32 pcvs Exp $
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

#include <qdialog.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include <contact.h>

class MailDialog : public QDialog
{
 Q_OBJECT
 public:
  MailDialog(QWidget *parent=0,const char *name=0);
  ~MailDialog();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(const QStringList &to_addrs,const QStringList &cc_addrs,
	   const QStringList &bcc_addrs,const QString &from_addr,
	   const QString &reply_addr,const QString &subj,const QString &msg);

 private slots:
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
