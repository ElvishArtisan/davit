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
#include <QValidator>

#include <dvtconf.h>
#include <dvtdb.h>
//#include <dvtlistviewitem.h>
#include <dvtfeed.h>

#include "edit_feed.h"
#include "edit_program.h"
#include "globals.h"
#include "migrate_affiliates.h"

EditProgram::EditProgram(DvtConfig *c,QWidget *parent)
  : DvtDialog(c,parent)
{
  setModal(true);

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());

  //
  // Dialogs
  //
  edit_editfeed_dialog=new EditFeed(c,this);
  
  //
  // Program Section
  //
  edit_program_group=new QGroupBox(tr("Program"),this);
  edit_program_group->setFont(sectionLabelFont());

  //
  // Program Name
  //
  edit_program_name_edit=new QLineEdit(edit_program_group);
  edit_program_name_edit->setReadOnly(true);
  edit_program_name_edit->setFont(defaultFont());
  edit_program_name_label=new QLabel(tr("Name")+":",edit_program_group);
  edit_program_name_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_program_name_label->setFont(labelFont());

  //
  // Program Length
  //
  edit_program_length_edit=new QTimeEdit(edit_program_group);
  edit_program_length_edit->setDisplayFormat("mm:ss");
  edit_program_length_edit->setFont(defaultFont());
  edit_program_length_lineedit=new QLineEdit(edit_program_group);
  edit_program_length_lineedit->setReadOnly(true);
  edit_program_length_edit->setFont(defaultFont());
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_program_length_lineedit->hide();
  }
  else {
    edit_program_length_edit->hide();
  }
  edit_program_length_label=new QLabel(tr("Length")+":",edit_program_group);
  edit_program_length_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_program_length_label->setFont(labelFont());

  //
  // Contact Section
  //
  edit_contact_group=new QGroupBox(tr("Contact Info"),this);
  edit_contact_group->setFont(sectionLabelFont());

  //
  // Contact Name
  //
  edit_contact_name_edit=new QLineEdit(edit_contact_group);
  edit_contact_name_edit->setFont(defaultFont());
  edit_contact_name_edit->setMaxLength(64);
  edit_contact_name_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  edit_contact_name_label=new QLabel(tr("Name")+":",edit_contact_group);
  edit_contact_name_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_contact_name_label->setFont(labelFont());

  //
  // Contact Phone
  //
  edit_contact_phone_edit=new QLineEdit(edit_contact_group);
  edit_contact_phone_edit->setFont(defaultFont());
  edit_contact_phone_edit->setMaxLength(20);
  edit_contact_phone_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  edit_contact_phone_label=new QLabel(tr("Phone")+":",edit_contact_group);
  edit_contact_phone_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_contact_phone_label->setFont(labelFont());

  //
  // Contact Fax
  //
  edit_contact_fax_edit=new QLineEdit(edit_contact_group);
  edit_contact_fax_edit->setFont(defaultFont());
  edit_contact_fax_edit->setMaxLength(20);
  edit_contact_fax_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  edit_contact_fax_label=new QLabel(tr("Fax")+":",edit_contact_group);
  edit_contact_fax_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_contact_fax_label->setFont(labelFont());

  //
  // Contact Email
  //
  edit_contact_email_edit=new QLineEdit(edit_contact_group);
  edit_contact_email_edit->setFont(defaultFont());
  edit_contact_email_edit->setMaxLength(64);
  edit_contact_email_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  edit_contact_email_label=
    new QLabel(tr("E-Mail Address")+":",edit_contact_group);
  edit_contact_email_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_contact_email_label->setFont(labelFont());

  //
  // Feeds Section
  //
  edit_feeds_group=new QGroupBox(tr("Feeds"),this);
  edit_feeds_group->setFont(sectionLabelFont());

  //
  // Feeds List
  //
  edit_feeds_view=new DvtTableView(edit_feeds_group);
  edit_feeds_view->setFont(defaultFont());
  edit_feeds_model=new FeedListModel(this);
  edit_feeds_model->setFont(defaultFont());
  edit_feeds_model->setPalette(palette());
  edit_feeds_view->setModel(edit_feeds_model);
  connect(edit_feeds_view,SIGNAL(doubleClicked(const QModelIndex &)),
	  this,SLOT(doubleClickedData(const QModelIndex &)));
  connect(edit_feeds_view->selectionModel(),
     SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)),
     this,
     SLOT(selectionChangedData(const QItemSelection &,const QItemSelection &)));
  /*
  edit_feeds_view=new QListView(edit_feeds_group);
  edit_feeds_view->setAllColumnsShowFocus(true);
  edit_feeds_view->setItemMargin(5);
  connect(edit_feeds_view,
	  SIGNAL(doubleClicked(QListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(QListViewItem *,const QPoint &,int)));
  edit_feeds_view->addColumn(tr("Start Time"));
  edit_feeds_view->setColumnAlignment(0,Qt::AlignCenter);
  edit_feeds_view->addColumn(tr("Su"));
  edit_feeds_view->setColumnAlignment(1,Qt::AlignCenter);
  edit_feeds_view->addColumn(tr("Mn"));
  edit_feeds_view->setColumnAlignment(2,Qt::AlignCenter);
  edit_feeds_view->addColumn(tr("Tu"));
  edit_feeds_view->setColumnAlignment(3,Qt::AlignCenter);
  edit_feeds_view->addColumn(tr("We"));
  edit_feeds_view->setColumnAlignment(4,Qt::AlignCenter);
  edit_feeds_view->addColumn(tr("Th"));
  edit_feeds_view->setColumnAlignment(5,Qt::AlignCenter);
  edit_feeds_view->addColumn(tr("Fr"));
  edit_feeds_view->setColumnAlignment(6,Qt::AlignCenter);
  edit_feeds_view->addColumn(tr("Sa"));
  edit_feeds_view->setColumnAlignment(7,Qt::AlignCenter);
  */

  edit_add_button=new QPushButton(edit_feeds_group);
  edit_add_button->setFont(buttonFont());
  edit_add_button->setText(tr("Add"));
  edit_add_button->setEnabled(global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  connect(edit_add_button,SIGNAL(clicked()),this,SLOT(addFeedData()));

  edit_edit_button=new QPushButton(edit_feeds_group);
  edit_edit_button->setFont(buttonFont());
  edit_edit_button->setText(tr("Edit"));
  edit_edit_button->setEnabled(global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  connect(edit_edit_button,SIGNAL(clicked()),this,SLOT(editFeedData()));

  edit_delete_button=new QPushButton(edit_feeds_group);
  edit_delete_button->setFont(buttonFont());
  edit_delete_button->setText(tr("Delete"));
  edit_delete_button->setEnabled(global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  connect(edit_delete_button,SIGNAL(clicked()),this,SLOT(deleteFeedData()));

  //
  // Primary ISDN Section
  //
  edit_primary_group=new QGroupBox(tr("Primary Codec Parameters"),this);
  edit_primary_group->setFont(sectionLabelFont());

  //
  // Primary ISDN Number
  //
  edit_primary_number_edit=new QLineEdit(edit_primary_group);
  edit_primary_number_edit->setFont(defaultFont());
  edit_primary_number_edit->setMaxLength(20);
  edit_primary_number_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  connect(edit_primary_number_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(primaryIsdnChangedData(const QString &)));
  edit_primary_number_label=new QLabel(tr("Primary ISDN #")+":",edit_primary_group);
  edit_primary_number_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_primary_number_label->setFont(labelFont());

  //
  // Primary Transmit Algorithm
  //
  edit_primary_tx_algo_box=new AlgoComboBox(edit_primary_group);
  edit_primary_tx_algo_box->setFont(defaultFont());
  edit_primary_tx_algo_edit=new QLineEdit(edit_primary_group);
  edit_primary_tx_algo_edit->setFont(defaultFont());
  edit_primary_tx_algo_edit->setGeometry(175,332,110,20);
  edit_primary_tx_algo_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_primary_tx_algo_edit->hide();
  }
  else {
    edit_primary_tx_algo_box->hide();
  }
  edit_primary_tx_algo_label=new QLabel("Transmit Format:",edit_primary_group);
  edit_primary_tx_algo_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_primary_tx_algo_label->setFont(labelFont());

  //
  // Primary Receive Transmit Algorithm
  //
  edit_primary_rx_algo_box=new AlgoComboBox(edit_primary_group);
  edit_primary_rx_algo_box->setFont(defaultFont());
  edit_primary_rx_algo_edit=new QLineEdit(edit_primary_group);
  edit_primary_rx_algo_edit->setFont(defaultFont());
  edit_primary_rx_algo_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_primary_rx_algo_edit->hide();
  }
  else {
    edit_primary_rx_algo_box->hide();
  }
  edit_primary_rx_algo_label=new QLabel(tr("Receive Format")+":",edit_primary_group);
  edit_primary_rx_algo_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_primary_rx_algo_label->setFont(labelFont());

  //
  // Primary Bitrate
  //
  edit_primary_bitrate_box=new QComboBox(edit_primary_group);
  edit_primary_bitrate_box->setFont(defaultFont());
  edit_primary_bitrate_box->insertItem(0,"56000");
  edit_primary_bitrate_box->insertItem(1,"64000");
  edit_primary_bitrate_edit=new QLineEdit(edit_primary_group);
  edit_primary_bitrate_edit->setFont(defaultFont());
  edit_primary_bitrate_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_primary_bitrate_edit->hide();
  }
  else {
    edit_primary_bitrate_box->hide();
  }
  edit_primary_bitrate_label=new QLabel(tr("Bit Rate")+":",edit_primary_group);
  edit_primary_bitrate_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_primary_bitrate_label->setFont(labelFont());

  //
  // Primary Samplerate
  //
  edit_primary_samplerate_box=new QComboBox(edit_primary_group);
  edit_primary_samplerate_box->setFont(defaultFont());
  edit_primary_samplerate_box->insertItem(0,"32000");
  edit_primary_samplerate_box->insertItem(1,"44100");
  edit_primary_samplerate_box->insertItem(2,"48000");
  edit_primary_samplerate_edit=new QLineEdit(edit_primary_group);
  edit_primary_samplerate_edit->setFont(defaultFont());
  edit_primary_samplerate_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_primary_samplerate_edit->hide();
  }
  else {
    edit_primary_samplerate_box->hide();
  }
  edit_primary_samplerate_label=new QLabel(tr("Sample Rate")+":",edit_primary_group);
  edit_primary_samplerate_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_primary_samplerate_label->setFont(labelFont());

  //
  // Primary Channels
  //
  edit_primary_channels_box=new QComboBox(edit_primary_group);
  edit_primary_channels_box->setFont(defaultFont());
  edit_primary_channels_box->insertItem(0,"1");
  edit_primary_channels_box->insertItem(1,"2");
  edit_primary_channels_edit=new QLineEdit(edit_primary_group);
  edit_primary_channels_edit->setFont(defaultFont());
  edit_primary_channels_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_primary_channels_edit->hide();
  }
  else {
    edit_primary_channels_box->hide();
  }
  edit_primary_channels_label=new QLabel(tr("Channels")+":",edit_primary_group);
  edit_primary_channels_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_primary_channels_label->setFont(labelFont());

  //
  // Secondary ISDN Section
  //
  edit_secondary_group=new QGroupBox(tr("Secondary Codec Parameters"),this);
  edit_secondary_group->setFont(sectionLabelFont());

  //
  // Secondary ISDN Number
  //
  edit_secondary_number_edit=new QLineEdit(edit_secondary_group);
  edit_secondary_number_edit->setFont(defaultFont());
  edit_secondary_number_edit->setMaxLength(70);
  edit_secondary_number_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  connect(edit_secondary_number_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(secondaryIsdnChangedData(const QString &)));
  edit_secondary_number_label=new QLabel(tr("Secondary ISDN #")+":",edit_secondary_group);
  edit_secondary_number_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_secondary_number_label->setFont(labelFont());

  //
  // Secondary Transmit Algorithm
  //
  edit_secondary_tx_algo_box=new AlgoComboBox(edit_secondary_group);
  edit_secondary_tx_algo_box->setFont(defaultFont());
  edit_secondary_tx_algo_edit=new QLineEdit(edit_secondary_group);
  edit_secondary_tx_algo_edit->setFont(defaultFont());
  edit_secondary_tx_algo_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_secondary_tx_algo_edit->hide();
  }
  else {
    edit_secondary_tx_algo_box->hide();
  }
  edit_secondary_tx_algo_label=new QLabel(tr("Transmit Format")+":",edit_secondary_group);
  edit_secondary_tx_algo_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_secondary_tx_algo_label->setFont(labelFont());

  //
  // Secondary Receive Transmit Algorithm
  //
  edit_secondary_rx_algo_box=new AlgoComboBox(edit_secondary_group);
  edit_secondary_rx_algo_box->setFont(defaultFont());
  edit_secondary_rx_algo_edit=new QLineEdit(edit_secondary_group);
  edit_secondary_rx_algo_edit->setFont(defaultFont());
  edit_secondary_rx_algo_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_secondary_rx_algo_edit->hide();
  }
  else {
    edit_secondary_rx_algo_box->hide();
  }
  edit_secondary_rx_algo_label=new QLabel(tr("Receive Format")+":",edit_secondary_group);
  edit_secondary_rx_algo_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_secondary_rx_algo_label->setFont(labelFont());

  //
  // Secondary Bitrate
  //
  edit_secondary_bitrate_box=new QComboBox(edit_secondary_group);
  edit_secondary_bitrate_box->setFont(defaultFont());
  edit_secondary_bitrate_box->insertItem(0,"56000");
  edit_secondary_bitrate_box->insertItem(1,"64000");
  edit_secondary_bitrate_edit=new QLineEdit(edit_secondary_group);
  edit_secondary_bitrate_edit->setFont(defaultFont());
  edit_secondary_bitrate_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_secondary_bitrate_edit->hide();
  }
  else {
    edit_secondary_bitrate_box->hide();
  }
  edit_secondary_bitrate_label=new QLabel(tr("Bit Rate")+":",edit_secondary_group);
  edit_secondary_bitrate_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_secondary_bitrate_label->setFont(labelFont());

  //
  // Secondary Samplerate
  //
  edit_secondary_samplerate_box=new QComboBox(edit_secondary_group);
  edit_secondary_samplerate_box->setFont(defaultFont());
  edit_secondary_samplerate_box->insertItem(0,"32000");
  edit_secondary_samplerate_box->insertItem(1,"44100");
  edit_secondary_samplerate_box->insertItem(2,"48000");
  edit_secondary_samplerate_edit=new QLineEdit(edit_secondary_group);
  edit_secondary_samplerate_edit->setFont(defaultFont());
  edit_secondary_samplerate_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_secondary_samplerate_edit->hide();
  }
  else {
    edit_secondary_samplerate_box->hide();
  }
  edit_secondary_samplerate_label=new QLabel(tr("Sample Rate")+":",edit_secondary_group);
  edit_secondary_samplerate_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_secondary_samplerate_label->setFont(labelFont());

  //
  // Secondary Channels
  //
  edit_secondary_channels_box=new QComboBox(edit_secondary_group);
  edit_secondary_channels_box->setFont(defaultFont());
  edit_secondary_channels_box->insertItem(0,"1");
  edit_secondary_channels_box->insertItem(1,"2");
  edit_secondary_channels_edit=new QLineEdit(edit_secondary_group);
  edit_secondary_channels_edit->setFont(defaultFont());
  edit_secondary_channels_edit->setReadOnly(true);
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    edit_secondary_channels_edit->hide();
  }
  else {
    edit_secondary_channels_box->hide();
  }
  edit_secondary_channels_label=new QLabel(tr("Channels")+":",edit_secondary_group);
  edit_secondary_channels_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_secondary_channels_label->setFont(labelFont());

  //
  // Remarks
  //
  edit_remarks_edit=new QTextEdit(this);
  edit_remarks_edit->
    setReadOnly(!global_dvtuser->privilege(DvtUser::PrivProgramEdit));
  edit_remarks_label=new QLabel(tr("Remarks"),this);
  edit_remarks_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  edit_remarks_label->setFont(sectionLabelFont());

  //
  //  Migrate Affiliates Button
  //
  edit_migrate_button=new QPushButton(this);
  edit_migrate_button->setFont(buttonFont());
  edit_migrate_button->setText(tr("Migrate")+"\n"+tr("Affiliates"));
  edit_migrate_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateSchedule));
  connect(edit_migrate_button,SIGNAL(clicked()),this,SLOT(migrateData()));

  //
  //  OK Button
  //
  edit_ok_button=new QPushButton(this);
  edit_ok_button->setDefault(true);
  edit_ok_button->setFont(buttonFont());
  edit_ok_button->setText("OK");
  connect(edit_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  edit_cancel_button=new QPushButton(this);
  edit_cancel_button->setFont(buttonFont());
  edit_cancel_button->setText("Cancel");
  connect(edit_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


EditProgram::~EditProgram()
{
}


QSize EditProgram::sizeHint() const
{
  return QSize(606,670+50);
} 


QSizePolicy EditProgram::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int EditProgram::exec(int program_id,bool new_entry)
{
  edit_program_id=program_id;

  setWindowTitle("Davit - "+
		 tr("Edit Program")+QString::asprintf(" [ID: %d]",program_id));

  //
  // Load Data
  //
  edit_feeds_model->setProgramId(program_id);
  edit_feeds_view->resizeColumnsToContents();

  QString sql=QString("select ")+
    "`ID`,"+                         // 00
    "`CONTACT_NAME`,"+               // 01
    "`CONTACT_PHONE`,"+              // 02
    "`CONTACT_FAX`,"+                // 03
    "`CONTACT_EMAIL`,"+              // 04
    "`PRIMARY_ISDN_NUMBER`,"+        // 05
    "`PRIMARY_ISDN_TX_ALGO`,"+       // 06
    "`PRIMARY_ISDN_RX_ALGO`,"+       // 07
    "`PRIMARY_ISDN_BITRATE`,"+       // 08
    "`PRIMARY_ISDN_SAMPLERATE`,"+    // 09
    "`PRIMARY_ISDN_CHANNELS`,"+      // 10
    "`SECONDARY_ISDN_NUMBER`,"+      // 11
    "`SECONDARY_ISDN_TX_ALGO`,"+     // 12
    "`SECONDARY_ISDN_RX_ALGO`,"+     // 13
    "`SECONDARY_ISDN_BITRATE`,"+     // 14
    "`SECONDARY_ISDN_SAMPLERATE`,"+  // 15
    "`SECONDARY_ISDN_CHANNELS`,"+    // 16
    "`REMARKS`,"+                    // 17
    "`AIR_LENGTH`,"+                 // 18
    "`PROGRAM_NAME` "+               // 19
    "from `PROGRAMS` where "+
    QString::asprintf("`ID`=%d",program_id);
  QSqlQuery *q=new QSqlQuery(sql);
  if(q->first()) {
    edit_program_id=q->value(0).toInt();
    edit_program_name_edit->setText(q->value(19).toString());
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

  return QDialog::exec();
}


void EditProgram::migrateData()
{
  MigrateAffiliates *d=
    new MigrateAffiliates(edit_program_id,edit_program_name_edit->text(),this);
  d->exec();
  delete d;
}


void EditProgram::addFeedData()
{
  QString sql=QString("insert into `FEEDS` set ")+
    QString::asprintf("`PROGRAM_ID`=%d",edit_program_id);
  int feed_id=DvtSqlQuery::run(sql).toInt();
  if(edit_editfeed_dialog->exec(feed_id,true)) {
    QModelIndex index=edit_feeds_model->addFeed(feed_id);
    if(index.isValid()) {
      edit_feeds_view->
	selectionModel()->select(index,QItemSelectionModel::ClearAndSelect|
				 QItemSelectionModel::Rows);
    }
  }
  else {
    sql=QString("delete from `FEEDS` where ")+
      QString::asprintf("`ID`=%d ",feed_id);
    DvtSqlQuery::apply(sql);
  }
}


void EditProgram::editFeedData()
{
  QModelIndexList rows=edit_feeds_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  int feed_id=edit_feeds_model->feedId(rows.first());
  if(edit_editfeed_dialog->exec(feed_id)) {
    edit_feeds_model->refresh(rows.first());
  }
}


void EditProgram::deleteFeedData()
{
  QModelIndexList rows=edit_feeds_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  //
  // Get Program Name and Start time
  //
  QString sql=QString("select ")+
    "`FEEDS`.`AIR_TIME`,"+         // 00
    "`PROGRAMS`.`PROGRAM_NAME` "+  // 01
    "from `FEEDS` left join `PROGRAMS` "+
    "on `FEEDS`.`PROGRAM_ID`=`PROGRAMS`.`ID` where "+
    QString::asprintf("`FEEDS`.`ID`=%d ",
		      edit_feeds_model->feedId(rows.first()));
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  if(!q->first()) {
    delete q;
    return;
  }
  if(QMessageBox::question(this,
			   "Davit",tr("Are you sure you want to delete the")+
			   " "+q->value(0).toTime().toString("hh:mm:ss")+" "+
			   tr("feed of")+" "+" \""+q->value(1).toString()+"\"?",
		 QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
    delete q;
    return;
  }
  delete q;

  //
  // Delete Feed Record
  //
  sql=QString("delete from `FEEDS` where ")+
    QString::asprintf("`ID`=%d",edit_feeds_model->feedId(rows.first()));
  DvtSqlQuery::apply(sql);
  edit_feeds_model->removeFeed(rows.first());

  /*
  DvtListViewItem *item=(DvtListViewItem *)edit_feeds_view->selectedItem();
  if(item==NULL) {
    return;
  }
  delete item;
  */
}


void EditProgram::doubleClickedData(const QModelIndex &index)
{
  if(global_dvtuser->privilege(DvtUser::PrivProgramEdit)) {
    editFeedData();
  }
}


void EditProgram::selectionChangedData(const QItemSelection &before,
				       const QItemSelection &after)
{
}


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
  QString sql;

  sql=QString("update `PROGRAMS` set ")+
    "`CONTACT_NAME`="+DvtSqlQuery::escape(edit_contact_name_edit->text())+","+
    "`CONTACT_PHONE`="+DvtSqlQuery::escape(edit_contact_phone_edit->text())+","+
    "`CONTACT_FAX`="+DvtSqlQuery::escape(edit_contact_fax_edit->text())+","+
    "`CONTACT_EMAIL`="+DvtSqlQuery::escape(edit_contact_email_edit->text())+","+
    "`PRIMARY_ISDN_NUMBER`="+
    DvtSqlQuery::escape(edit_primary_number_edit->text())+","+
    "`PRIMARY_ISDN_TX_ALGO`="+
    DvtSqlQuery::escape(edit_primary_tx_algo_box->currentAlgoCode())+","+
    "`PRIMARY_ISDN_RX_ALGO`="+
    DvtSqlQuery::escape(edit_primary_rx_algo_box->currentAlgoCode())+","+
    QString::asprintf("`PRIMARY_ISDN_BITRATE`=%d,",
		      edit_primary_bitrate_box->currentText().toInt())+
    QString::asprintf("`PRIMARY_ISDN_SAMPLERATE`=%d,",
		      edit_primary_samplerate_box->currentText().toInt())+
    QString::asprintf("`PRIMARY_ISDN_CHANNELS`=%d,",
		      edit_primary_channels_box->currentText().toInt())+
    "`SECONDARY_ISDN_NUMBER`="+
    DvtSqlQuery::escape(edit_secondary_number_edit->text())+","+
    "`SECONDARY_ISDN_TX_ALGO`="+
    DvtSqlQuery::escape(edit_secondary_tx_algo_box->currentAlgoCode())+","+
    "`SECONDARY_ISDN_RX_ALGO`="+
    DvtSqlQuery::escape(edit_secondary_rx_algo_box->currentAlgoCode())+","+
    QString::asprintf("`SECONDARY_ISDN_BITRATE`=%d,",
		      edit_secondary_bitrate_box->currentText().toInt())+
    QString::asprintf("`SECONDARY_ISDN_SAMPLERATE`=%d,",
		      edit_secondary_samplerate_box->currentText().toInt())+
    QString::asprintf("`SECONDARY_ISDN_CHANNELS`=%d,",
		      edit_secondary_channels_box->currentText().toInt())+
    "`REMARKS`="+DvtSqlQuery::escape(edit_remarks_edit->toPlainText())+","+
    QString::asprintf("`AIR_LENGTH`=%d where ",
		      QTime(0,0,0).msecsTo(edit_program_length_edit->time()))+
    QString::asprintf("`ID`=%d",edit_program_id);
  DvtSqlQuery::apply(sql);

  /*
  sql=QString("delete from `FEEDS` where ")+
    QString::asprintf("`PROGRAM_ID`=%d",edit_program_id);
  DvtSqlQuery::apply(sql);
  DvtListViewItem *item=(DvtListViewItem *)edit_feeds_view->firstChild();
  while(item!=NULL) {
    sql=QString::asprintf("insert into FEEDS set \
                           PROGRAM_ID=%d,\
                           AIR_TIME=\"%s\",\
                           AIR_SUN=\"%s\",\
                           AIR_MON=\"%s\",\
                           AIR_TUE=\"%s\",\
                           AIR_WED=\"%s\",\
                           AIR_THU=\"%s\",\
                           AIR_FRI=\"%s\",\
                           AIR_SAT=\"%s\"",
			  edit_program_id,
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
  done(true);
}


void EditProgram::cancelData()
{
  done(false);
}

/*
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
*/

void EditProgram::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  edit_program_group->setGeometry(10,3,w-20,70);
  int grp_w=edit_program_group->width();
  edit_program_name_label->setGeometry(10,24,100,20);
  edit_program_name_edit->setGeometry(115,24,grp_w-125,20);
  edit_program_length_label->setGeometry(10,46,100,20);
  edit_program_length_edit->setGeometry(115,46,60,20);
  edit_program_length_lineedit->setGeometry(115,46,60,20);

  //
  // Contact Section
  //
  edit_contact_group->setGeometry(10,33+50,w-20,96);
  grp_w=edit_contact_group->width();
  edit_contact_name_label->setGeometry(10,24,100,20);
  edit_contact_name_edit->setGeometry(115,24,grp_w-125,20);
  edit_contact_phone_label->setGeometry(10,46,100,20);
  edit_contact_phone_edit->setGeometry(115,46,150,20);
  edit_contact_fax_label->setGeometry(270,46,30,20);
  edit_contact_fax_edit->setGeometry(305,46,150,20);
  edit_contact_email_label->setGeometry(10,68,100,20);
  edit_contact_email_edit->setGeometry(115,68,grp_w-125,20);

  //
  // Feeds Section
  //
  edit_feeds_group->setGeometry(10,140+50,w-20,125+10);
  grp_w=edit_feeds_group->width();
  edit_feeds_view->setGeometry(10,25,grp_w-80,100);
  edit_add_button->setGeometry(grp_w-60,25,50,30);
  edit_edit_button->setGeometry(grp_w-60,60,50,30);
  edit_delete_button->setGeometry(grp_w-60,95,50,30);

  //
  // Primary Section
  //
  edit_primary_group->setGeometry(10,280+50,285,170);
  edit_primary_number_label->setGeometry(10,30,150,20);
  edit_primary_number_edit->setGeometry(165,30,110,20);
  edit_primary_tx_algo_label->setGeometry(10,52,150,20);
  edit_primary_tx_algo_box->setGeometry(165,52,110,20);
  edit_primary_rx_algo_label->setGeometry(10,74,150,20);
  edit_primary_rx_algo_box->setGeometry(165,74,110,20);
  edit_primary_rx_algo_edit->setGeometry(165,74,110,20);
  edit_primary_bitrate_label->setGeometry(10,96,150,20);
  edit_primary_bitrate_box->setGeometry(165,96,70,20);
  edit_primary_bitrate_edit->setGeometry(165,96,70,20);
  edit_primary_samplerate_label->setGeometry(10,118,150,20);
  edit_primary_samplerate_box->setGeometry(165,118,70,20);
  edit_primary_samplerate_edit->setGeometry(165,118,70,20);
  edit_primary_channels_label->setGeometry(10,140,150,20);
  edit_primary_channels_box->setGeometry(165,140,40,20);
  edit_primary_channels_edit->setGeometry(165,140,40,20);

  //
  // Secondary Section
  //
  edit_secondary_group->setGeometry(310,280+50,285,170);
  edit_secondary_number_label->setGeometry(10,30,150,20);
  edit_secondary_number_edit->setGeometry(165,30,110,20);
  edit_secondary_tx_algo_label->setGeometry(10,52,150,20);
  edit_secondary_tx_algo_box->setGeometry(165,52,110,20);
  edit_secondary_rx_algo_label->setGeometry(10,74,150,20);
  edit_secondary_rx_algo_box->setGeometry(165,74,110,20);
  edit_secondary_rx_algo_edit->setGeometry(165,74,110,20);
  edit_secondary_bitrate_label->setGeometry(10,96,150,20);
  edit_secondary_bitrate_box->setGeometry(165,96,70,20);
  edit_secondary_bitrate_edit->setGeometry(165,96,70,20);
  edit_secondary_samplerate_label->setGeometry(10,118,150,20);
  edit_secondary_samplerate_box->setGeometry(165,118,70,20);
  edit_secondary_samplerate_edit->setGeometry(165,118,70,20);
  edit_secondary_channels_label->setGeometry(10,140,150,20);
  edit_secondary_channels_box->setGeometry(165,140,40,20);
  edit_secondary_channels_edit->setGeometry(165,140,40,20);

  //
  // Remarks Section
  //
  edit_remarks_label->setGeometry(20,463+50,100,20);
  edit_remarks_edit->setGeometry(10,485+50,w-20,100);
  edit_migrate_button->setGeometry(10,h-60,80,50);
  edit_ok_button->setGeometry(w-180,h-60,80,50);
  edit_cancel_button->setGeometry(w-90,h-60,80,50);
}


void EditProgram::RefreshList()
{
  /*
  QString sql;
  QSqlQuery *q;

  edit_feeds_view->clear();
  DvtListViewItem *item=NULL;
  sql=QString::asprintf("select AIR_TIME,\
                         AIR_SUN,AIR_MON,AIR_TUE,AIR_WED,AIR_THU,AIR_FRI,\
                         AIR_SAT,ID from FEEDS \
                         where PROGRAM_ID=%d",
			edit_program_id);
  q=new QSqlQuery(sql);
  while(q->next()) {
    item=new DvtListViewItem(edit_feeds_view);
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


/*
void EditProgram::RefreshItem(DvtListViewItem *item,DvtFeed *feed)
{
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
}
*/
