// edit_system.cpp
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

#include <QMessageBox>
#include <QPainter>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <dvtconf.h>

#include "edit_system.h"
#include "globals.h"

EditSystem::EditSystem(QWidget *parent)
  : QDialog(parent)
{
  setModal(true);
  setWindowTitle(tr("Davit -- Edit System Settings"));

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  //
  // Generate Fonts
  //
  QFont label_font("helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  //
  // SMTP Settings
  //
  edit_smtp_label=new QLabel(tr("SMTP Server Settings"),this);
  edit_smtp_label->setFont(label_font);
  edit_smtp_label->setAlignment(Qt::AlignCenter);
  edit_smtp_hostname_edit=new QLineEdit(this);
  edit_smtp_hostname_edit->setMaxLength(255);
  edit_smtp_hostname_label=new QLabel("Hostname:",this);
  edit_smtp_hostname_label->setFont(label_font);
  edit_smtp_hostname_label->setAlignment(Qt::AlignLeft);
  
  edit_smtp_port_spin=new QSpinBox(this);
  edit_smtp_port_spin->setRange(0,0xFFFF);
  edit_smtp_port_label=new QLabel("Port:",this);
  edit_smtp_port_label->setFont(label_font);
  edit_smtp_port_label->setAlignment(Qt::AlignLeft);

  //
  // Originating Email Address
  //
  edit_origin_email_edit=new QLineEdit(this);
  edit_origin_email_edit->setMaxLength(64);
  edit_origin_email_label=new QLabel("Originating E-Mail Address:",this);
  edit_origin_email_label->setFont(label_font);
  edit_origin_email_label->setAlignment(Qt::AlignLeft);

  //
  // Alert Email Address
  //
  edit_alert_email_edit=new QLineEdit(this);
  edit_alert_email_edit->setMaxLength(64);
  edit_alert_email_label=new QLabel("Alert E-Mail Address:",this);
  edit_alert_email_label->setFont(label_font);
  edit_alert_email_label->setAlignment(Qt::AlignLeft);

  //
  // Subject Line
  //
  edit_affidavit_subject_edit=new QLineEdit(this);
  edit_affidavit_subject_edit->setMaxLength(64);
  edit_affidavit_subject_label=
    new QLabel("Affidavit E-Mail Subject Line:",this);
  edit_affidavit_subject_label->setFont(label_font);
  edit_affidavit_subject_label->setAlignment(Qt::AlignLeft);

  //
  // Affidavit E-Mail Template
  //
  edit_affidavit_template_edit=new QTextEdit(this);
  edit_affidavit_template_label=new QLabel("Affidavit E-Mail Template",this);
  edit_affidavit_template_label->setFont(label_font);
  edit_affidavit_template_label->setAlignment(Qt::AlignLeft);

  //
  // Wildcard Key
  //
  edit_key_label=new QLabel("%S - Station Call Letters\n%U - Web Login Username\n%P - Web Login Password",this);

  //
  // Test Button
  //
  edit_test_button=new QPushButton(tr("Send Test"),this);
  edit_test_button->setFont(label_font);
  connect(edit_test_button,SIGNAL(clicked()),this,SLOT(testData()));

  //
  // Ok Button
  //
  edit_ok_button=new QPushButton(tr("OK"),this);
  edit_ok_button->setFont(label_font);
  connect(edit_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  edit_cancel_button=new QPushButton(tr("Cancel"),this);
  edit_cancel_button->setFont(label_font);
  connect(edit_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));

  QString sql="select ORIGIN_EMAIL,ALERT_EMAIL,AFFIDAVIT_EMAIL_SUBJECT,\
               AFFIDAVIT_EMAIL_TEMPLATE,SMTP_HOSTNAME,SMTP_PORT from SYSTEM";
  QSqlQuery *q=new QSqlQuery(sql);
  if(q->first()) {
    edit_origin_email_edit->setText(q->value(0).toString());
    edit_alert_email_edit->setText(q->value(1).toString());
    edit_affidavit_subject_edit->setText(q->value(2).toString());
    edit_affidavit_template_edit->setText(q->value(3).toString());
    edit_smtp_hostname_edit->setText(q->value(4).toString());
    edit_smtp_port_spin->setValue(q->value(5).toInt());
  }
  delete q;  
}


EditSystem::~EditSystem()
{
}


QSize EditSystem::sizeHint() const
{
  return QSize(500,487);
}


QSizePolicy EditSystem::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditSystem::testData()
{
  /*
  mail_dialog->exec("","","",edit_origin_email_edit->text(),
		    edit_origin_email_edit->text(),"Test message",
		    "This is a test!");
  */
}


void EditSystem::okData()
{
  QString sql;
  QSqlQuery *q;

  if(!DvtIsEmailAddress(edit_origin_email_edit->text())) {
    QMessageBox::warning(this,"Davit - Edit System",
			 tr("The e-mail address is invalid!"));
    return;
  }

  sql=QString::asprintf("update SYSTEM set ORIGIN_EMAIL=\"%s\",\
             ALERT_EMAIL=\"%s\",\
             AFFIDAVIT_EMAIL_SUBJECT=\"%s\",\
             AFFIDAVIT_EMAIL_TEMPLATE=\"%s\",\
             SMTP_HOSTNAME=\"%s\",\
             SMTP_PORT=%d",
	    DvtEscapeString(edit_origin_email_edit->text()).
	    toUtf8().constData(),
	    DvtEscapeString(edit_alert_email_edit->text()).toUtf8().constData(),
	    DvtEscapeString(edit_affidavit_subject_edit->text()).
	    toUtf8().constData(),
	    DvtEscapeString(edit_affidavit_template_edit->toPlainText()).
	    toUtf8().constData(),
	    DvtEscapeString(edit_smtp_hostname_edit->text()).
	    toUtf8().constData(),
	    edit_smtp_port_spin->value());
  q=new QSqlQuery(sql);
  delete q;

  done(0);
}


void EditSystem::cancelData()
{
  done(-1);
}


void EditSystem::paintEvent(QPaintEvent *e)
{
  /*
  QPainter *p=new QPainter(this);
  p->setPen(Qt::black);
  p->setBrush(Qt::black);
  p->moveTo(10,15);
  p->lineTo(size().width()-10,15);
  p->lineTo(size().width()-10,49);
  p->lineTo(10,49);
  p->lineTo(10,15);

  delete p;
  */
}


void EditSystem::resizeEvent(QResizeEvent *e)
{
  edit_smtp_label->setGeometry(15,5,130,20);
  edit_smtp_hostname_label->setGeometry(20,25,65,20);
  edit_smtp_hostname_edit->setGeometry(90,25,size().width()-230,20);
  edit_smtp_port_label->setGeometry(size().width()-115,25,30,20);
  edit_smtp_port_spin->setGeometry(size().width()-80,25,60,20);

  edit_origin_email_label->setGeometry(15,50,size().width()-25,18);
  edit_origin_email_edit->setGeometry(10,70,size().width()-20,20);
  edit_alert_email_label->setGeometry(15,97,size().width()-25,18);
  edit_alert_email_edit->setGeometry(10,117,size().width()-20,20);
  edit_affidavit_subject_label->setGeometry(15,144,size().width()-25,18);
  edit_affidavit_subject_edit->setGeometry(10,162,size().width()-20,20);
  edit_affidavit_template_label->setGeometry(15,192,size().width()-25,18);
  edit_affidavit_template_edit->
    setGeometry(10,210,size().width()-20,size().height()-272);
  edit_key_label->setGeometry(25,size().height()-60,size().width()-145,60);
  edit_test_button->setGeometry(size().width()-250,size().height()-35,80,30);
  edit_ok_button->setGeometry(size().width()-120,size().height()-35,50,30);
  edit_cancel_button->setGeometry(size().width()-60,size().height()-35,50,30);
}
