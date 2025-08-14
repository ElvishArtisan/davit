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

#include <dvtconf.h>
#include <dvtdb.h>

#include "edit_system.h"
#include "globals.h"

EditSystem::EditSystem(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  setModal(true);
  setWindowTitle("Davit - "+tr("Edit System E-Mail Settings"));

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());

  /*
  //
  // SMTP Settings
  //
  edit_smtp_group=new QGroupBox(tr("SMTP Server Settings"),this);
  edit_smtp_group->setFont(labelFont());
  edit_smtp_hostname_edit=new QLineEdit(edit_smtp_group);
  edit_smtp_hostname_edit->setFont(defaultFont());
  edit_smtp_hostname_edit->setMaxLength(255);
  edit_smtp_hostname_label=new QLabel(tr("Hostname")+":",edit_smtp_group);
  edit_smtp_hostname_label->setFont(labelFont());
  edit_smtp_hostname_label->setAlignment(Qt::AlignLeft);
  
  edit_smtp_port_spin=new QSpinBox(edit_smtp_group);
  edit_smtp_port_spin->setFont(defaultFont());
  edit_smtp_port_spin->setRange(0,0xFFFF);
  edit_smtp_port_label=new QLabel("Port:",edit_smtp_group);
  edit_smtp_port_label->setFont(labelFont());
  edit_smtp_port_label->setAlignment(Qt::AlignLeft);
  */
  //
  // Originating Email Address
  //
  edit_origin_email_edit=new QLineEdit(this);
  edit_origin_email_edit->setMaxLength(64);
  edit_origin_email_label=new QLabel(tr("Originating E-Mail Address"),this);
  edit_origin_email_label->setFont(labelFont());
  edit_origin_email_label->setAlignment(Qt::AlignLeft);

  //
  // Archive Email Address
  //
  edit_alert_email_edit=new QLineEdit(this);
  edit_alert_email_edit->setMaxLength(64);
  edit_alert_email_label=new QLabel(tr("Archive E-Mail Address"),this);
  edit_alert_email_label->setFont(labelFont());
  edit_alert_email_label->setAlignment(Qt::AlignLeft);

  //
  // Subject Line
  //
  edit_affidavit_subject_edit=new QLineEdit(this);
  edit_affidavit_subject_edit->setMaxLength(64);
  edit_affidavit_subject_label=
    new QLabel(tr("Affidavit E-Mail Subject Line"),this);
  edit_affidavit_subject_label->setFont(labelFont());
  edit_affidavit_subject_label->setAlignment(Qt::AlignLeft);

  //
  // Affidavit E-Mail Template
  //
  edit_affidavit_template_edit=new QTextEdit(this);
  edit_affidavit_template_label=
    new QLabel(tr("Affidavit E-Mail Template"),this);
  edit_affidavit_template_label->setFont(labelFont());
  edit_affidavit_template_label->setAlignment(Qt::AlignLeft);

  //
  // Wildcard Key
  //
  edit_key_label=new QLabel("%S - "+tr("Station Call Letters")+"\n%U - "+
			    tr("Web Login Username")+"\n%P - "+
			    tr("Web Login Password"),this);

  //
  // Test Button
  //
  edit_test_button=new QPushButton(tr("Send Test E-Mail"),this);
  edit_test_button->setFont(buttonFont());
  connect(edit_test_button,SIGNAL(clicked()),this,SLOT(testData()));

  //
  // Ok Button
  //
  edit_ok_button=new QPushButton(tr("OK"),this);
  edit_ok_button->setFont(buttonFont());
  connect(edit_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  edit_cancel_button=new QPushButton(tr("Cancel"),this);
  edit_cancel_button->setFont(buttonFont());
  connect(edit_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


EditSystem::~EditSystem()
{
}


QSize EditSystem::sizeHint() const
{
  return QSize(500,487-55);
}


QSizePolicy EditSystem::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int EditSystem::exec()
{
  QString sql=QString("select ")+
    "`ORIGIN_EMAIL`,"+              // 00
    "`ALERT_EMAIL`,"+               // 01
    "`AFFIDAVIT_EMAIL_SUBJECT`,"+   // 02
    "`AFFIDAVIT_EMAIL_TEMPLATE`,"+  // 03
    "`SMTP_HOSTNAME`,"+             // 04
    "`SMTP_PORT` "+                 // 05
    "from `SYSTEM`";
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  if(q->first()) {
    edit_origin_email_edit->setText(q->value(0).toString());
    edit_alert_email_edit->setText(q->value(1).toString());
    edit_affidavit_subject_edit->setText(q->value(2).toString());
    edit_affidavit_template_edit->setText(q->value(3).toString());
    //    edit_smtp_hostname_edit->setText(q->value(4).toString());
    //    edit_smtp_port_spin->setValue(q->value(5).toInt());
  }
  delete q;  

  return QDialog::exec();
}


void EditSystem::testData()
{
  mail_dialog->exec(QStringList(),QStringList(),QStringList(),
		    edit_origin_email_edit->text(),
		    edit_origin_email_edit->text(),"Test message",
		    "This is a test!");
}


void EditSystem::okData()
{
  QString sql;

  if(!DvtIsEmailAddress(edit_origin_email_edit->text())) {
    QMessageBox::warning(this,"Davit - Edit System",
			 tr("The e-mail address is invalid!"));
    return;
  }

  sql=QString("update `SYSTEM` set ")+
    "`ORIGIN_EMAIL`="+DvtSqlQuery::escape(edit_origin_email_edit->text())+","+
    "`ALERT_EMAIL`="+DvtSqlQuery::escape(edit_alert_email_edit->text())+","+
    "`AFFIDAVIT_EMAIL_SUBJECT`="+
    DvtSqlQuery::escape(edit_affidavit_subject_edit->text())+","+
    "`AFFIDAVIT_EMAIL_TEMPLATE`="+
    DvtSqlQuery::escape(edit_affidavit_template_edit->toPlainText())+" ";
    //    "`SMTP_HOSTNAME`="+DvtSqlQuery::escape(edit_smtp_hostname_edit->text())+","+
    //    QString::asprintf("`SMTP_PORT`=%d ",edit_smtp_port_spin->value());
  DvtSqlQuery::apply(sql);

  done(true);
}


void EditSystem::cancelData()
{
  done(false);
}


void EditSystem::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();
  /*
  edit_smtp_group->setGeometry(15,5,w-30,46);
  edit_smtp_hostname_label->setGeometry(10,27,65,20);
  edit_smtp_hostname_edit->setGeometry(80,22,w-250,20);
  edit_smtp_port_label->setGeometry(w-135,27,30,20);
  edit_smtp_port_spin->setGeometry(w-100,22,60,20);
  */
  edit_origin_email_label->setGeometry(15,5,w-25,18);
  edit_origin_email_edit->setGeometry(10,20,w-20,20);
  edit_alert_email_label->setGeometry(15,52,w-25,18);
  edit_alert_email_edit->setGeometry(10,67,w-20,20);
  edit_affidavit_subject_label->setGeometry(15,99,w-25,18);
  edit_affidavit_subject_edit->setGeometry(10,114,w-20,20);
  edit_affidavit_template_label->setGeometry(15,145,w-25,18);
  edit_affidavit_template_edit->setGeometry(10,160,w-20,h-227);

  edit_key_label->setGeometry(25,h-60,w-145,60);
  edit_test_button->setGeometry(w-310,h-40,150,30);
  edit_ok_button->setGeometry(w-140,h-40,60,30);
  edit_cancel_button->setGeometry(w-70,h-40,60,30);
}


void EditSystem::closeEvent(QCloseEvent *e)
{
  cancelData();
}
