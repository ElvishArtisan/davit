// edit_program.cpp
//
// Edit a Davit Program.
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

#include <math.h>

#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QValidator>

#include <dvtconf.h>
#include <dvtlistviewitem.h>
#include <dvtfeed.h>

#include "edit_feed.h"
#include "edit_program.h"
#include "globals.h"
#include "migrate_affiliates.h"

EditProgram::EditProgram(const QString &pname,QWidget *parent)
  : QDialog(parent)
{
  setModal(true);

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setWindowTitle("Davit - Edit Program");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont big_font=QFont("Helvetica",16,QFont::Bold);
  big_font.setPixelSize(16);

  //
  // Program Name
  //
  edit_program_name_edit=new QLineEdit(this);
  edit_program_name_edit->setGeometry(125,10,360,20);
  edit_program_name_edit->setFont(font);
  edit_program_name_edit->setReadOnly(true);
  QLabel *label=new QLabel("Program Name:",this);
  label->setGeometry(20,10,100,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Program Length
  //
  edit_program_length_edit=new QTimeEdit(this);
  edit_program_length_edit->setGeometry(550,10,sizeHint().width()-560,20);
  edit_program_length_edit->setDisplayFormat("mm:ss");
  edit_program_length_lineedit=new QLineEdit(this);
  edit_program_length_lineedit->setGeometry(550,10,sizeHint().width()-560,20);
  edit_program_length_lineedit->setFont(font);
  edit_program_length_lineedit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_program_length_lineedit->hide();
  }
  else {
    edit_program_length_edit->hide();
  }
  label=new QLabel("Length:",this);
  label->setGeometry(495,10,50,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Contact Section Label
  //
  label=new QLabel("Contact",this);
  label->setGeometry(50,35,70,20);
  label->setAlignment(Qt::AlignCenter);
  label->setFont(big_font);

  //
  // Contact Name
  //
  edit_contact_name_edit=new QLineEdit(this);
  edit_contact_name_edit->setGeometry(125,57,sizeHint().width()-275,20);
  edit_contact_name_edit->setFont(font);
  edit_contact_name_edit->setMaxLength(64);
  edit_contact_name_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  label=new QLabel("Name:",this);
  label->setGeometry(20,57,100,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Contact Phone
  //
  edit_contact_phone_edit=new QLineEdit(this);
  edit_contact_phone_edit->setGeometry(125,79,150,20);
  edit_contact_phone_edit->setFont(font);
  edit_contact_phone_edit->setMaxLength(20);
  edit_contact_phone_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  label=new QLabel("Phone:",this);
  label->setGeometry(20,79,100,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Contact Fax
  //
  edit_contact_fax_edit=new QLineEdit(this);
  edit_contact_fax_edit->setGeometry(315,79,150,20);
  edit_contact_fax_edit->setFont(font);
  edit_contact_fax_edit->setMaxLength(20);
  edit_contact_fax_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  label=new QLabel("Fax:",this);
  label->setGeometry(280,79,30,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Contact Email
  //
  edit_contact_email_edit=new QLineEdit(this);
  edit_contact_email_edit->setGeometry(125,101,sizeHint().width()-275,20);
  edit_contact_email_edit->setFont(font);
  edit_contact_email_edit->setMaxLength(64);
  edit_contact_email_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  label=new QLabel("E-Mail Address:",this);
  label->setGeometry(20,101,100,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Feeds Section Label
  //
  label=new QLabel("Feeds",this);
  label->setGeometry(100,140,50,20);
  label->setAlignment(Qt::AlignCenter);
  label->setFont(big_font);

  //
  // Feeds List
  //
  edit_feeds_list=new QListView(this);
  edit_feeds_list->setGeometry(70,165,sizeHint().width()-200,100);
  /*
  edit_feeds_list->setAllColumnsShowFocus(true);
  edit_feeds_list->setItemMargin(5);
  connect(edit_feeds_list,
	  SIGNAL(doubleClicked(QListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(QListViewItem *,const QPoint &,int)));
  edit_feeds_list->addColumn(tr("Start Time"));
  edit_feeds_list->setColumnAlignment(0,Qt::AlignCenter);
  edit_feeds_list->addColumn(tr("Su"));
  edit_feeds_list->setColumnAlignment(1,Qt::AlignCenter);
  edit_feeds_list->addColumn(tr("Mn"));
  edit_feeds_list->setColumnAlignment(2,Qt::AlignCenter);
  edit_feeds_list->addColumn(tr("Tu"));
  edit_feeds_list->setColumnAlignment(3,Qt::AlignCenter);
  edit_feeds_list->addColumn(tr("We"));
  edit_feeds_list->setColumnAlignment(4,Qt::AlignCenter);
  edit_feeds_list->addColumn(tr("Th"));
  edit_feeds_list->setColumnAlignment(5,Qt::AlignCenter);
  edit_feeds_list->addColumn(tr("Fr"));
  edit_feeds_list->setColumnAlignment(6,Qt::AlignCenter);
  edit_feeds_list->addColumn(tr("Sa"));
  edit_feeds_list->setColumnAlignment(7,Qt::AlignCenter);
  */

  QPushButton *button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-120,165,50,30);
  button->setFont(label_font);
  button->setText(tr("Add"));
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  connect(button,SIGNAL(clicked()),this,SLOT(addFeedData()));

  button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-120,200,50,30);
  button->setFont(label_font);
  button->setText(tr("Edit"));
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  connect(button,SIGNAL(clicked()),this,SLOT(editFeedData()));

  button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-120,235,50,30);
  button->setFont(label_font);
  button->setText(tr("Delete"));
  button->setEnabled(global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  connect(button,SIGNAL(clicked()),this,SLOT(deleteFeedData()));

  //
  // Primary ISDN Section Label
  //
  label=new QLabel("Primary ISDN Parameters",this);
  label->setGeometry(75,285,200,20);
  label->setAlignment(Qt::AlignCenter);
  label->setFont(big_font);

  //
  // Primary ISDN Number
  //
  edit_primary_number_edit=new QLineEdit(this);
  edit_primary_number_edit->setGeometry(175,310,110,20);
  edit_primary_number_edit->setFont(font);
  edit_primary_number_edit->setMaxLength(20);
  edit_primary_number_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  connect(edit_primary_number_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(primaryIsdnChangedData(const QString &)));
  label=new QLabel("Primary ISDN #:",this);
  label->setGeometry(70,310,100,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Primary Transmit Algorithm
  //
  edit_primary_tx_algo_box=new AlgoComboBox(this);
  edit_primary_tx_algo_box->setGeometry(175,332,110,20);
  edit_primary_tx_algo_edit=new QLineEdit(this);
  edit_primary_tx_algo_edit->setGeometry(175,332,110,20);
  edit_primary_tx_algo_edit->setFont(font);
  edit_primary_tx_algo_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_primary_tx_algo_edit->hide();
  }
  else {
    edit_primary_tx_algo_box->hide();
  }
  edit_primary_tx_algo_label=
    new QLabel("Transmit Format:",this);
  edit_primary_tx_algo_label->setGeometry(70,332,100,20);
  edit_primary_tx_algo_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_primary_tx_algo_label->setFont(label_font);

  //
  // Primary Receive Transmit Algorithm
  //
  edit_primary_rx_algo_box=new AlgoComboBox(this);
  edit_primary_rx_algo_box->setGeometry(175,354,110,20);
  edit_primary_rx_algo_edit=new QLineEdit(this);
  edit_primary_rx_algo_edit->setGeometry(175,354,110,20);
  edit_primary_rx_algo_edit->setFont(font);
  edit_primary_rx_algo_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_primary_rx_algo_edit->hide();
  }
  else {
    edit_primary_rx_algo_box->hide();
  }
  edit_primary_rx_algo_label=
    new QLabel("Receive Format:",this);
  edit_primary_rx_algo_label->setGeometry(70,354,100,20);
  edit_primary_rx_algo_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_primary_rx_algo_label->setFont(label_font);

  //
  // Primary Bitrate
  //
  edit_primary_bitrate_box=new QComboBox(this);
  edit_primary_bitrate_box->setGeometry(175,376,60,20);
  edit_primary_bitrate_box->insertItem(0,"56000");
  edit_primary_bitrate_box->insertItem(1,"64000");
  edit_primary_bitrate_edit=new QLineEdit(this);
  edit_primary_bitrate_edit->setGeometry(175,376,60,20);
  edit_primary_bitrate_edit->setFont(font);
  edit_primary_bitrate_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_primary_bitrate_edit->hide();
  }
  else {
    edit_primary_bitrate_box->hide();
  }
  edit_primary_bitrate_label=
    new QLabel("Bit Rate:",this);
  edit_primary_bitrate_label->setGeometry(70,376,100,20);
  edit_primary_bitrate_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_primary_bitrate_label->setFont(label_font);

  //
  // Primary Samplerate
  //
  edit_primary_samplerate_box=new QComboBox(this);
  edit_primary_samplerate_box->setGeometry(175,398,60,20);
  edit_primary_samplerate_box->insertItem(0,"32000");
  edit_primary_samplerate_box->insertItem(1,"44100");
  edit_primary_samplerate_box->insertItem(2,"48000");
  edit_primary_samplerate_edit=new QLineEdit(this);
  edit_primary_samplerate_edit->setGeometry(175,398,60,20);
  edit_primary_samplerate_edit->setFont(font);
  edit_primary_samplerate_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_primary_samplerate_edit->hide();
  }
  else {
    edit_primary_samplerate_box->hide();
  }
  edit_primary_samplerate_label=
    new QLabel("Sample Rate:",this);
  edit_primary_samplerate_label->setGeometry(70,398,100,20);
  edit_primary_samplerate_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_primary_samplerate_label->setFont(label_font);

  //
  // Primary Channels
  //
  edit_primary_channels_box=new QComboBox(this);
  edit_primary_channels_box->setGeometry(175,420,40,20);
  edit_primary_channels_box->insertItem(0,"1");
  edit_primary_channels_box->insertItem(1,"2");
  edit_primary_channels_edit=new QLineEdit(this);
  edit_primary_channels_edit->setGeometry(175,420,40,20);
  edit_primary_channels_edit->setFont(font);
  edit_primary_channels_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_primary_channels_edit->hide();
  }
  else {
    edit_primary_channels_box->hide();
  }
  edit_primary_channels_label=
    new QLabel("Channels:",this);
  edit_primary_channels_label->setGeometry(70,420,100,20);
  edit_primary_channels_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_primary_channels_label->setFont(label_font);

  //
  // Secondary ISDN Section Label
  //
  label=new QLabel("Secondary ISDN Parameters",this);
  label->setGeometry(315,285,220,20);
  label->setAlignment(Qt::AlignCenter);
  label->setFont(big_font);

  //
  // Secondary ISDN Number
  //
  edit_secondary_number_edit=new QLineEdit(this);
  edit_secondary_number_edit->setGeometry(425,310,110,20);
  edit_secondary_number_edit->setFont(font);
  edit_secondary_number_edit->setMaxLength(70);
  edit_secondary_number_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  connect(edit_secondary_number_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(secondaryIsdnChangedData(const QString &)));
  label=new QLabel("Secondary ISDN #:",this);
  label->setGeometry(315,310,105,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Secondary Transmit Algorithm
  //
  edit_secondary_tx_algo_box=new AlgoComboBox(this);
  edit_secondary_tx_algo_box->setGeometry(425,332,110,20);
  edit_secondary_tx_algo_edit=new QLineEdit(this);
  edit_secondary_tx_algo_edit->setGeometry(425,332,110,20);
  edit_secondary_tx_algo_edit->setFont(font);
  edit_secondary_tx_algo_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_secondary_tx_algo_edit->hide();
  }
  else {
    edit_secondary_tx_algo_box->hide();
  }
  edit_secondary_tx_algo_label=
    new QLabel("Transmit Format:",this);
  edit_secondary_tx_algo_label->setGeometry(320,332,100,20);
  edit_secondary_tx_algo_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_secondary_tx_algo_label->setFont(label_font);

  //
  // Secondary Receive Transmit Algorithm
  //
  edit_secondary_rx_algo_box=new AlgoComboBox(this);
  edit_secondary_rx_algo_box->setGeometry(425,354,110,20);
  edit_secondary_rx_algo_edit=new QLineEdit(this);
  edit_secondary_rx_algo_edit->setGeometry(425,354,110,20);
  edit_secondary_rx_algo_edit->setFont(font);
  edit_secondary_rx_algo_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_secondary_rx_algo_edit->hide();
  }
  else {
    edit_secondary_rx_algo_box->hide();
  }
  edit_secondary_rx_algo_label=
    new QLabel("Receive Format:",this);
  edit_secondary_rx_algo_label->setGeometry(320,354,100,20);
  edit_secondary_rx_algo_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_secondary_rx_algo_label->setFont(label_font);

  //
  // Secondary Bitrate
  //
  edit_secondary_bitrate_box=new QComboBox(this);
  edit_secondary_bitrate_box->setGeometry(425,376,60,20);
  edit_secondary_bitrate_box->insertItem(0,"56000");
  edit_secondary_bitrate_box->insertItem(1,"64000");
  edit_secondary_bitrate_edit=new QLineEdit(this);
  edit_secondary_bitrate_edit->setGeometry(425,376,60,20);
  edit_secondary_bitrate_edit->setFont(font);
  edit_secondary_bitrate_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_secondary_bitrate_edit->hide();
  }
  else {
    edit_secondary_bitrate_box->hide();
  }
  edit_secondary_bitrate_label=
    new QLabel("Bit Rate:",this);
  edit_secondary_bitrate_label->setGeometry(320,376,100,20);
  edit_secondary_bitrate_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_secondary_bitrate_label->setFont(label_font);

  //
  // Secondary Samplerate
  //
  edit_secondary_samplerate_box=new QComboBox(this);
  edit_secondary_samplerate_box->setGeometry(425,398,60,20);
  edit_secondary_samplerate_box->insertItem(0,"32000");
  edit_secondary_samplerate_box->insertItem(1,"44100");
  edit_secondary_samplerate_box->insertItem(2,"48000");
  edit_secondary_samplerate_edit=new QLineEdit(this);
  edit_secondary_samplerate_edit->setGeometry(425,398,60,20);
  edit_secondary_samplerate_edit->setFont(font);
  edit_secondary_samplerate_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_secondary_samplerate_edit->hide();
  }
  else {
    edit_secondary_samplerate_box->hide();
  }
  edit_secondary_samplerate_label=new QLabel("Sample Rate:",this);
  edit_secondary_samplerate_label->setGeometry(320,398,100,20);
  edit_secondary_samplerate_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_secondary_samplerate_label->setFont(label_font);

  //
  // Secondary Channels
  //
  edit_secondary_channels_box=new QComboBox(this);
  edit_secondary_channels_box->setGeometry(425,420,40,20);
  edit_secondary_channels_box->insertItem(0,"1");
  edit_secondary_channels_box->insertItem(1,"2");
  edit_secondary_channels_edit=new QLineEdit(this);
  edit_secondary_channels_edit->setGeometry(425,420,40,20);
  edit_secondary_channels_edit->setFont(font);
  edit_secondary_channels_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_secondary_channels_edit->hide();
  }
  else {
    edit_secondary_channels_box->hide();
  }
  edit_secondary_channels_label=new QLabel("Channels:",this);
  edit_secondary_channels_label->setGeometry(320,420,100,20);
  edit_secondary_channels_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_secondary_channels_label->setFont(label_font);

  //
  // Remarks
  //
  edit_remarks_edit=new QTextEdit(this);
  edit_remarks_edit->setGeometry(10,485,sizeHint().width()-20,100);
  edit_remarks_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  label=new QLabel("Remarks",this);
  label->setGeometry(20,463,100,20);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  label->setFont(big_font);

  //
  //  Migrate Affiliates Button
  //
  button=new QPushButton(this);
  button->setGeometry(10,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText("&Migrate\nAffiliates");
  button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateSchedule));
  connect(button,SIGNAL(clicked()),this,SLOT(migrateData()));

  //
  //  OK Button
  //
  button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(label_font);
  button->setText("&OK");
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText("&Cancel");
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Data
  //
  QString sql=QString().sprintf("select ID,CONTACT_NAME,\
                                 CONTACT_PHONE,CONTACT_FAX,CONTACT_EMAIL,\
                                 PRIMARY_ISDN_NUMBER,PRIMARY_ISDN_TX_ALGO,\
                                 PRIMARY_ISDN_RX_ALGO,PRIMARY_ISDN_BITRATE,\
                                 PRIMARY_ISDN_SAMPLERATE,\
                                 PRIMARY_ISDN_CHANNELS,\
                                 SECONDARY_ISDN_NUMBER,SECONDARY_ISDN_TX_ALGO,\
                                 SECONDARY_ISDN_RX_ALGO,\
                                 SECONDARY_ISDN_BITRATE,\
                                 SECONDARY_ISDN_SAMPLERATE,\
                                 SECONDARY_ISDN_CHANNELS,REMARKS,AIR_LENGTH \
                                 from PROGRAMS where PROGRAM_NAME=\"%s\"",
				pname.toUtf8().constData());
  QSqlQuery *q=new QSqlQuery(sql);
  if(q->first()) {
    edit_id=q->value(0).toInt();
    edit_program_name_edit->setText(pname);
    edit_program_length_edit->setTime(QTime().addMSecs(q->value(18).toInt()));
    edit_program_length_lineedit->
      setText(QTime().addMSecs(q->value(18).toInt()).toString("mm:ss"));
    edit_contact_name_edit->setText(q->value(1).toString());
    edit_contact_phone_edit->setText(q->value(2).toString());
    edit_contact_fax_edit->setText(q->value(3).toString());
    edit_contact_email_edit->setText(q->value(4).toString());
    edit_primary_number_edit->setText(q->value(5).toString());
    edit_primary_tx_algo_box->setCurrentAlgoCode(q->value(6).toString());
    edit_primary_tx_algo_edit->
      setText(edit_primary_tx_algo_box->currentText());
    edit_primary_rx_algo_box->setCurrentAlgoCode(q->value(7).toString());
    edit_primary_rx_algo_edit->
      setText(edit_primary_rx_algo_box->currentText());
    for(int i=0;i<edit_primary_bitrate_box->count();i++) {
      if(edit_primary_bitrate_box->itemText(i).toInt()==q->value(8).toInt()) {
	edit_primary_bitrate_box->setCurrentIndex(i);
	edit_primary_bitrate_edit->
	  setText(edit_primary_bitrate_box->currentText());
      }
    }
    for(int i=0;i<edit_primary_samplerate_box->count();i++) {
      if(edit_primary_samplerate_box->itemText(i).toInt()==q->value(9).toInt()) {
	edit_primary_samplerate_box->setCurrentIndex(i);
	edit_primary_samplerate_edit->
	  setText(edit_primary_samplerate_box->currentText());
      }
    }
    edit_primary_channels_box->setCurrentIndex(q->value(10).toInt()-1);
    edit_primary_channels_edit->
      setText(edit_primary_channels_box->currentText());
    primaryIsdnChangedData(edit_primary_number_edit->text());
    edit_secondary_number_edit->setText(q->value(11).toString());
    edit_secondary_tx_algo_box->setCurrentAlgoCode(q->value(12).toString());
    edit_secondary_tx_algo_edit->
      setText(edit_secondary_tx_algo_box->currentText());
    edit_secondary_rx_algo_box->setCurrentAlgoCode(q->value(13).toString());
    edit_secondary_rx_algo_edit->
      setText(edit_secondary_rx_algo_box->currentText());
    for(int i=0;i<edit_secondary_bitrate_box->count();i++) {
      if(edit_secondary_bitrate_box->itemText(i).toInt()==q->value(14).toInt()) {
	edit_secondary_bitrate_box->setCurrentIndex(i);
	edit_secondary_bitrate_edit->
	  setText(edit_secondary_bitrate_box->currentText());
      }
    }
    for(int i=0;i<edit_secondary_samplerate_box->count();i++) {
      if(edit_secondary_samplerate_box->itemText(i).toInt()==q->value(15).toInt()) {
	edit_secondary_samplerate_box->setCurrentIndex(i);
	edit_secondary_samplerate_edit->
	  setText(edit_secondary_samplerate_box->currentText());
      }
    }
    edit_secondary_channels_box->setCurrentIndex(q->value(16).toInt()-1);
	edit_secondary_channels_edit->
	  setText(edit_secondary_channels_box->currentText());
    secondaryIsdnChangedData(edit_secondary_number_edit->text());
  }
  edit_remarks_edit->setText(q->value(17).toString());
  delete q;

  RefreshList();
}


EditProgram::~EditProgram()
{
}


QSize EditProgram::sizeHint() const
{
  return QSize(615,670);
} 


QSizePolicy EditProgram::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditProgram::migrateData()
{
  MigrateAffiliates *d=
    new MigrateAffiliates(edit_id,edit_program_name_edit->text(),this);
  d->exec();
  delete d;
}


void EditProgram::addFeedData()
{
  DvtFeed feed;
  EditFeed *edit_feed=new EditFeed(&feed,this);
  if(edit_feed->exec()<0) {
    delete edit_feed;
    return;
  }
  DvtListViewItem *item=new DvtListViewItem(edit_feeds_list);
  RefreshItem(item,&feed);
  //  edit_feeds_list->ensureItemVisible(item);
  delete edit_feed;
}


void EditProgram::editFeedData()
{
  /*
  DvtListViewItem *item=(DvtListViewItem *)edit_feeds_list->selectedItem();
  if(item==NULL) {
    return;
  }
  DvtFeed feed;
  feed.setStartTime(item->text(0));
  feed.setDowActive(7,!item->text(1).isEmpty());
  feed.setDowActive(1,!item->text(2).isEmpty());
  feed.setDowActive(2,!item->text(3).isEmpty());
  feed.setDowActive(3,!item->text(4).isEmpty());
  feed.setDowActive(4,!item->text(5).isEmpty());
  feed.setDowActive(5,!item->text(6).isEmpty());
  feed.setDowActive(6,!item->text(7).isEmpty());
  EditFeed *edit_feed=new EditFeed(&feed,this);
  if(edit_feed->exec()<0) {
    delete edit_feed;
    return;
  }
  RefreshItem(item,&feed);
  delete edit_feed;
  */
}


void EditProgram::deleteFeedData()
{
  /*
  DvtListViewItem *item=(DvtListViewItem *)edit_feeds_list->selectedItem();
  if(item==NULL) {
    return;
  }
  delete item;
  */
}

/*
void EditProgram::doubleClickedData(QListViewItem *item,const QPoint &pt,
				    int col)
{
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    editFeedData();
  }
}
*/

void EditProgram::primaryIsdnChangedData(const QString &str)
{
  edit_primary_tx_algo_label->setDisabled(str.isEmpty());
  edit_primary_tx_algo_box->setDisabled(str.isEmpty());
  edit_primary_tx_algo_edit->setDisabled(str.isEmpty());
  edit_primary_rx_algo_label->setDisabled(str.isEmpty());
  edit_primary_rx_algo_box->setDisabled(str.isEmpty());
  edit_primary_rx_algo_edit->setDisabled(str.isEmpty());
  edit_primary_bitrate_label->setDisabled(str.isEmpty());
  edit_primary_bitrate_box->setDisabled(str.isEmpty());
  edit_primary_bitrate_edit->setDisabled(str.isEmpty());
  edit_primary_samplerate_label->setDisabled(str.isEmpty());
  edit_primary_samplerate_box->setDisabled(str.isEmpty());
  edit_primary_samplerate_edit->setDisabled(str.isEmpty());
  edit_primary_channels_label->setDisabled(str.isEmpty());
  edit_primary_channels_box->setDisabled(str.isEmpty());
  edit_primary_channels_edit->setDisabled(str.isEmpty());
}


void EditProgram::secondaryIsdnChangedData(const QString &str)
{
  edit_secondary_tx_algo_label->setDisabled(str.isEmpty());
  edit_secondary_tx_algo_box->setDisabled(str.isEmpty());
  edit_secondary_tx_algo_edit->setDisabled(str.isEmpty());
  edit_secondary_rx_algo_label->setDisabled(str.isEmpty());
  edit_secondary_rx_algo_box->setDisabled(str.isEmpty());
  edit_secondary_rx_algo_edit->setDisabled(str.isEmpty());
  edit_secondary_bitrate_label->setDisabled(str.isEmpty());
  edit_secondary_bitrate_box->setDisabled(str.isEmpty());
  edit_secondary_bitrate_edit->setDisabled(str.isEmpty());
  edit_secondary_samplerate_label->setDisabled(str.isEmpty());
  edit_secondary_samplerate_box->setDisabled(str.isEmpty());
  edit_secondary_samplerate_edit->setDisabled(str.isEmpty());
  edit_secondary_channels_label->setDisabled(str.isEmpty());
  edit_secondary_channels_box->setDisabled(str.isEmpty());
  edit_secondary_channels_edit->setDisabled(str.isEmpty());
}


void EditProgram::okData()
{
  /*
  QString sql;
  QSqlQuery *q;

  sql=QString().sprintf("update PROGRAMS set \
                         CONTACT_NAME=\"%s\",\
                         CONTACT_PHONE=\"%s\",\
                         CONTACT_FAX=\"%s\",\
                         CONTACT_EMAIL=\"%s\",\
                         PRIMARY_ISDN_NUMBER=\"%s\",\
                         PRIMARY_ISDN_TX_ALGO=\"%s\",\
                         PRIMARY_ISDN_RX_ALGO=\"%s\",\
                         PRIMARY_ISDN_BITRATE=%d,\
                         PRIMARY_ISDN_SAMPLERATE=%d,\
                         PRIMARY_ISDN_CHANNELS=%d,\
                         SECONDARY_ISDN_NUMBER=\"%s\",\
                         SECONDARY_ISDN_TX_ALGO=\"%s\",\
                         SECONDARY_ISDN_RX_ALGO=\"%s\",\
                         SECONDARY_ISDN_BITRATE=%d,\
                         SECONDARY_ISDN_SAMPLERATE=%d,\
                         SECONDARY_ISDN_CHANNELS=%d,\
                         REMARKS=\"%s\",\
                         AIR_LENGTH=%d \
                         where ID=%d",
			(const char *)edit_contact_name_edit->text(),
			(const char *)edit_contact_phone_edit->text(),
			(const char *)edit_contact_fax_edit->text(),
			(const char *)edit_contact_email_edit->text(),

			(const char *)edit_primary_number_edit->text(),
			(const char *)edit_primary_tx_algo_box->
			currentAlgoCode(),
			(const char *)edit_primary_rx_algo_box->
			currentAlgoCode(),
			edit_primary_bitrate_box->currentText().toInt(),
			edit_primary_samplerate_box->currentText().toInt(),
			edit_primary_channels_box->currentText().toInt(),
			(const char *)edit_secondary_number_edit->text(),
			(const char *)edit_secondary_tx_algo_box->
			currentAlgoCode(),
			(const char *)edit_secondary_rx_algo_box->
			currentAlgoCode(),
			edit_secondary_bitrate_box->currentText().toInt(),
			edit_secondary_samplerate_box->currentText().toInt(),
			edit_secondary_channels_box->currentText().toInt(),
			(const char *)edit_remarks_edit->text(),
			QTime().msecsTo(edit_program_length_edit->time()),
			edit_id);
  q=new QSqlQuery(sql);
  delete q;

  sql=QString().sprintf("delete from FEEDS where PROGRAM_ID=%d",edit_id);
  q=new QSqlQuery(sql);
  delete q;
  DvtListViewItem *item=(DvtListViewItem *)edit_feeds_list->firstChild();
  while(item!=NULL) {
    sql=QString().sprintf("insert into FEEDS set \
                           PROGRAM_ID=%d,\
                           AIR_TIME=\"%s\",\
                           AIR_SUN=\"%s\",\
                           AIR_MON=\"%s\",\
                           AIR_TUE=\"%s\",\
                           AIR_WED=\"%s\",\
                           AIR_THU=\"%s\",\
                           AIR_FRI=\"%s\",\
                           AIR_SAT=\"%s\"",
			  edit_id,
			  (const char *)item->text(0),
			  (const char *)DvtYesNo(!item->text(1).isEmpty()),
			  (const char *)DvtYesNo(!item->text(2).isEmpty()),
			  (const char *)DvtYesNo(!item->text(3).isEmpty()),
			  (const char *)DvtYesNo(!item->text(4).isEmpty()),
			  (const char *)DvtYesNo(!item->text(5).isEmpty()),
			  (const char *)DvtYesNo(!item->text(6).isEmpty()),
			  (const char *)DvtYesNo(!item->text(7).isEmpty()));
    q=new QSqlQuery(sql);
    delete q;
    item=(DvtListViewItem *)item->nextSibling();
  }
  */
  done(0);
}


void EditProgram::cancelData()
{
  done(-1);
}


void EditProgram::paintEvent(QPaintEvent *e)
{
  QPainter *p=new QPainter(this);
  p->setPen(Qt::black);
  p->drawRect(10,43,sizeHint().width()-20,86);
  p->drawRect(60,150,sizeHint().width()-120,125);
  p->drawRect(60,295,235,155);
  p->drawRect(310,295,235,155);
  p->end();
  delete p;
}


void EditProgram::RefreshList()
{
  /*
  QString sql;
  QSqlQuery *q;

  edit_feeds_list->clear();
  DvtListViewItem *item=NULL;
  sql=QString().sprintf("select AIR_TIME,\
                         AIR_SUN,AIR_MON,AIR_TUE,AIR_WED,AIR_THU,AIR_FRI,\
                         AIR_SAT,ID from FEEDS \
                         where PROGRAM_ID=%d",
			edit_id);
  q=new QSqlQuery(sql);
  while(q->next()) {
    item=new DvtListViewItem(edit_feeds_list);
    item->setId(q->value(8).toInt());
    item->setText(0,q->value(0).toTime().toString("hh:mm:ss"));
    if(q->value(1).toString()=="Y") {
      item->setText(1,tr("Su"));
    }
    if(q->value(2).toString()=="Y") {
      item->setText(2,tr("Mo"));
    }
    if(q->value(3).toString()=="Y") {
      item->setText(3,tr("Tu"));
    }
    if(q->value(4).toString()=="Y") {
      item->setText(4,tr("We"));
    }
    if(q->value(5).toString()=="Y") {
      item->setText(5,tr("Th"));
    }
    if(q->value(6).toString()=="Y") {
      item->setText(6,tr("Fr"));
    }
    if(q->value(7).toString()=="Y") {
      item->setText(7,tr("Sa"));
    }
  }
  delete q;
  */
}


void EditProgram::RefreshItem(DvtListViewItem *item,DvtFeed *feed)
{
  /*
  item->setText(0,feed->startTime().toString("hh:mm:ss"));
  if(feed->dowActive(7)) {
    item->setText(1,tr("Su"));
  }
  else {
    item->setText(1,"");
  }
  if(feed->dowActive(1)) {
    item->setText(2,tr("Mo"));
  }
  else {
    item->setText(2,"");
  }
  if(feed->dowActive(2)) {
    item->setText(3,tr("Tu"));
  }
  else {
    item->setText(3,"");
  }
  if(feed->dowActive(3)) {
    item->setText(4,tr("We"));
  }
  else {
    item->setText(4,"");
  }
  if(feed->dowActive(4)) {
    item->setText(5,tr("Th"));
  }
  else {
    item->setText(5,"");
  }
  if(feed->dowActive(5)) {
    item->setText(6,tr("Fr"));
  }
  else {
    item->setText(6,"");
  }
  if(feed->dowActive(6)) {
    item->setText(7,tr("Sa"));
  }
  else {
    item->setText(7,"");
  }
  */
}
