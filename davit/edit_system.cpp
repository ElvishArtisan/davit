// edit_system.cpp
//
// Edit Davit system-wide settings.
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: edit_system.cpp,v 1.3 2011/03/22 15:12:53 pcvs Exp $
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

#include <qsqldatabase.h>
#include <qmessagebox.h>
#include <qpainter.h>

#include <dvtconf.h>

#include <edit_system.h>

EditSystem::EditSystem(QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  setCaption(tr("Davit -- Edit System Settings"));

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
  edit_smtp_label->setAlignment(AlignCenter);
  edit_smtp_hostname_edit=new QLineEdit(this);
  edit_smtp_hostname_edit->setMaxLength(255);
  edit_smtp_hostname_label=
    new QLabel(edit_smtp_hostname_edit,"Hostname:",this);
  edit_smtp_hostname_label->setFont(label_font);
  edit_smtp_hostname_label->setAlignment(AlignLeft);
  
  edit_smtp_port_spin=new QSpinBox(this);
  edit_smtp_port_spin->setRange(0,0xFFFF);
  edit_smtp_port_label=
    new QLabel(edit_smtp_port_spin,"Port:",this);
  edit_smtp_port_label->setFont(label_font);
  edit_smtp_port_label->setAlignment(AlignLeft);

  //
  // Originating Email Address
  //
  edit_origin_email_edit=new QLineEdit(this);
  edit_origin_email_edit->setMaxLength(64);
  edit_origin_email_label=
    new QLabel(edit_origin_email_edit,"Originating E-Mail Address:",this);
  edit_origin_email_label->setFont(label_font);
  edit_origin_email_label->setAlignment(AlignLeft);

  //
  // Alert Email Address
  //
  edit_alert_email_edit=new QLineEdit(this);
  edit_alert_email_edit->setMaxLength(64);
  edit_alert_email_label=
    new QLabel(edit_alert_email_edit,"Alert E-Mail Address:",this);
  edit_alert_email_label->setFont(label_font);
  edit_alert_email_label->setAlignment(AlignLeft);

  //
  // Subject Line
  //
  edit_affidavit_subject_edit=new QLineEdit(this);
  edit_affidavit_subject_edit->setMaxLength(64);
  edit_affidavit_subject_label=
    new QLabel(edit_affidavit_subject_edit,"Affidavit E-Mail Subject Line:",
	       this);
  edit_affidavit_subject_label->setFont(label_font);
  edit_affidavit_subject_label->setAlignment(AlignLeft);

  //
  // Affidavit E-Mail Template
  //
  edit_affidavit_template_edit=new QTextEdit(this);
  edit_affidavit_template_label=
    new QLabel(edit_affidavit_template_edit,"Affidavit E-Mail Template",this);
  edit_affidavit_template_label->setFont(label_font);
  edit_affidavit_template_label->setAlignment(AlignLeft);

  //
  // Wildcard Key
  //
  edit_key_label=new QLabel("%S - Station Call Letters\n%U - Web Login Username\n%P - Web Login Password",this);

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


void EditSystem::okData()
{
  QString sql;
  QSqlQuery *q;

  if(!DvtIsEmailAddress(edit_origin_email_edit->text())) {
    QMessageBox::warning(this,"Davit - Edit System",
			 tr("The e-mail address is invalid!"));
    return;
  }

  sql=QString().
    sprintf("update SYSTEM set ORIGIN_EMAIL=\"%s\",\
             ALERT_EMAIL=\"%s\",\
             AFFIDAVIT_EMAIL_SUBJECT=\"%s\",\
             AFFIDAVIT_EMAIL_TEMPLATE=\"%s\",\
             SMTP_HOSTNAME=\"%s\",\
             SMTP_PORT=%d",
	    (const char *)DvtEscapeString(edit_origin_email_edit->text()),
	    (const char *)DvtEscapeString(edit_alert_email_edit->text()),
	    (const char *)DvtEscapeString(edit_affidavit_subject_edit->text()),
	    (const char *)DvtEscapeString(edit_affidavit_template_edit->text()),
	    (const char *)DvtEscapeString(edit_smtp_hostname_edit->text()),
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
  QPainter *p=new QPainter(this);
  p->setPen(Qt::black);
  p->setBrush(Qt::black);
  p->moveTo(10,15);
  p->lineTo(size().width()-10,15);
  p->lineTo(size().width()-10,49);
  p->lineTo(10,49);
  p->lineTo(10,15);

  delete p;
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
  edit_ok_button->setGeometry(size().width()-120,size().height()-35,50,30);
  edit_cancel_button->setGeometry(size().width()-60,size().height()-35,50,30);
}
