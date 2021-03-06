// maildialog.cpp
//
// Compose an e-mail message.
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: maildialog.cpp,v 1.7 2013/02/26 19:06:17 pcvs Exp $
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

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include <qmessagebox.h>

#include <dvtconf.h>
#include <dvtmail.h>

#include <globals.h>
#include <maildialog.h>

MailDialog::MailDialog(QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  setCaption(tr("Davit - Compose Mail"));
  
  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  //
  // Fonts
  //
  QFont label_font=QFont("helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  edit_to_edit=new QTextEdit(this);
  edit_to_label=new QLabel(edit_to_edit,tr("To:"),this);
  edit_to_label->setFont(label_font);
  edit_to_label->setAlignment(AlignRight|AlignVCenter);

  edit_cc_edit=new QTextEdit(this);
  edit_cc_label=new QLabel(edit_cc_edit,tr("Cc:"),this);
  edit_cc_label->setFont(label_font);
  edit_cc_label->setAlignment(AlignRight|AlignVCenter);

  edit_bcc_edit=new QTextEdit(this);
  edit_bcc_label=new QLabel(edit_bcc_edit,tr("Bcc:"),this);
  edit_bcc_label->setFont(label_font);
  edit_bcc_label->setAlignment(AlignRight|AlignVCenter);

  edit_subject_edit=new QLineEdit(this);
  edit_subject_label=new QLabel(edit_subject_edit,tr("Subject:"),this);
  edit_subject_label->setFont(label_font);
  edit_subject_label->setAlignment(AlignRight|AlignVCenter);

  edit_body_edit=new QTextEdit(this);

  edit_send_button=new QPushButton(tr("Send"),this);
  edit_send_button->setFont(label_font);
  connect(edit_send_button,SIGNAL(clicked()),this,SLOT(sendData()));

  edit_cancel_button=new QPushButton(tr("Cancel"),this);
  edit_cancel_button->setFont(label_font);
  connect(edit_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


MailDialog::~MailDialog()
{
}


QSize MailDialog::sizeHint() const
{
  return QSize(400,600);
}


QSizePolicy MailDialog::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int MailDialog::exec(const QStringList &to_addrs,const QStringList &cc_addrs,
		     const QStringList &bcc_addrs,const QString &from_addr,
		     const QString &reply_addr,const QString &subj,
		     const QString &msg)
{
  edit_to_edit->setText("");
  edit_cc_edit->setText("");
  edit_bcc_edit->setText("");
  for(unsigned i=0;i<to_addrs.size();i++) {
    if(!to_addrs[i].isEmpty()) {
      edit_to_edit->setText(edit_to_edit->text()+to_addrs[i]+"\n");
    }
  }
  edit_to_edit->
    setText(edit_to_edit->text().left(edit_to_edit->text().length()-1));
  for(unsigned i=0;i<cc_addrs.size();i++) {
    if(!cc_addrs[i].isEmpty()) {
      edit_cc_edit->setText(edit_cc_edit->text()+cc_addrs[i]+"\n");
    }
  }
  edit_cc_edit->
    setText(edit_cc_edit->text().left(edit_cc_edit->text().length()-1));
  edit_bcc_edit->setText(reply_addr+"\n");
  for(unsigned i=0;i<bcc_addrs.size();i++) {
    if(!bcc_addrs[i].isEmpty()) {
      edit_bcc_edit->setText(edit_bcc_edit->text()+bcc_addrs[i]+"\n");
    }
  }
  edit_bcc_edit->
    setText(edit_bcc_edit->text().left(edit_bcc_edit->text().length()-1));
  edit_subject_edit->setText(subj);
  edit_body_edit->setText(msg);
  edit_from_address=from_addr;
  edit_reply_address=reply_addr;

  return QDialog::exec();
}


void MailDialog::sendData()
{
  QStringList to_addrs;
  QStringList cc_addrs;
  QStringList bcc_addrs;

  //
  // Validate Addresses
  //
  if(!DvtIsFullEmailAddress(edit_from_address)) {
    QMessageBox::warning(this,"Davit - Compose Mail",
			 tr("Invalid address in From: field!"));
    return;
  }
  if(!DvtIsFullEmailAddress(edit_reply_address)) {
    QMessageBox::warning(this,"Davit - Compose Mail",
			 tr("Invalid address in Reply-To: field!"));
    return;
  }
  if(!DvtNormalizeAddresses(edit_to_edit->text(),&to_addrs)) {
    QMessageBox::warning(this,"Davit - Compose Mail",
			 tr("Invalid address in To: field!"));
    return;
  }
  if(!DvtNormalizeAddresses(edit_cc_edit->text(),&cc_addrs)) {
    QMessageBox::warning(this,"Davit - Compose Mail",
			 tr("Invalid address in Cc: field!"));
    return;
  }
  if(!DvtNormalizeAddresses(edit_bcc_edit->text(),&bcc_addrs)) {
    QMessageBox::warning(this,"Davit - Compose Mail",
			 tr("Invalid address in Bcc: field!"));
    return;
  }

  if(!DvtSendMail(to_addrs,cc_addrs,bcc_addrs,edit_from_address,
		  edit_reply_address,edit_subject_edit->text(),
		  edit_body_edit->text())) {
    QMessageBox::warning(this,"Davit - Compose Mail",
		   tr("Mailer system returned an error, message not sent!"));
    return;
  }

  done(0);
}


void MailDialog::cancelData()
{
  done(-1);
}


void MailDialog::resizeEvent(QResizeEvent *e)
{
  edit_to_label->setGeometry(10,10,30,20);
  edit_to_edit->setGeometry(45,10,size().width()-55,50);

  edit_cc_label->setGeometry(10,65,30,20);
  edit_cc_edit->setGeometry(45,65,size().width()-55,50);

  edit_bcc_label->setGeometry(10,120,30,20);
  edit_bcc_edit->setGeometry(45,120,size().width()-55,50);

  edit_subject_label->setGeometry(10,180,50,20);
  edit_subject_edit->setGeometry(65,180,size().width()-75,20);

  edit_body_edit->setGeometry(10,210,size().width()-20,size().height()-270);

  edit_send_button->setGeometry(size().width()-130,size().height()-35,60,25);
  edit_cancel_button->setGeometry(size().width()-65,size().height()-35,60,25);
}
