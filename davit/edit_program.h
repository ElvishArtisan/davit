// edit_program.h
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

#ifndef EDIT_PROGRAM_H
#define EDIT_PROGRAM_H

#include <QDateTimeEdit>
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QTextEdit>

#include <dvtconfig.h>
#include <dvtdialog.h>
#include <dvttableview.h>

#include "algocombobox.h"
#include "edit_feed.h"
#include "feedlistmodel.h"

class EditProgram : public DvtDialog
{
 Q_OBJECT
 public:
  EditProgram(DvtConfig *c,QWidget *parent=0);
  ~EditProgram();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(int program_id,bool new_entry=false);

 private slots:
  void migrateData();
  void addFeedData();
  void editFeedData();
  void deleteFeedData();
  void doubleClickedData(const QModelIndex &index);
  void selectionChangedData(const QItemSelection &,const QItemSelection &);
  void primaryIsdnChangedData(const QString &str);
  void secondaryIsdnChangedData(const QString &str);
  void okData();
  void cancelData();

 protected:
  //  void paintEvent(QPaintEvent *e);
  void resizeEvent(QResizeEvent *e);

 private:
  void RefreshList();
  //  void RefreshItem(DvtListViewItem *item,DvtFeed *feed);
  QGroupBox *edit_program_group;
  QLabel *edit_program_name_label;
  QLineEdit *edit_program_name_edit;
  QLabel *edit_program_length_label;
  QTimeEdit *edit_program_length_edit;
  QLineEdit *edit_program_length_lineedit;
  QGroupBox *edit_contact_group;
  QLabel *edit_contact_name_label;
  QLineEdit *edit_contact_name_edit;
  QLabel *edit_contact_phone_label;
  QLineEdit *edit_contact_phone_edit;
  QLabel *edit_contact_fax_label;
  QLineEdit *edit_contact_fax_edit;
  QLabel *edit_contact_email_label;
  QLineEdit *edit_contact_email_edit;
  QGroupBox *edit_feeds_group;
  QLabel *edit_feeds_label;
  DvtTableView *edit_feeds_view;
  FeedListModel *edit_feeds_model;
  QGroupBox *edit_primary_group;
  QLabel *edit_primary_number_label;
  QLineEdit *edit_primary_number_edit;
  QLabel *edit_primary_tx_algo_label;
  AlgoComboBox *edit_primary_tx_algo_box;
  QLineEdit *edit_primary_tx_algo_edit;
  QLabel *edit_primary_rx_algo_label;
  AlgoComboBox *edit_primary_rx_algo_box;
  QLineEdit *edit_primary_rx_algo_edit;
  QLabel *edit_primary_bitrate_label;
  QComboBox *edit_primary_bitrate_box;
  QLineEdit *edit_primary_bitrate_edit;
  QLabel *edit_primary_samplerate_label;
  QComboBox *edit_primary_samplerate_box;
  QLineEdit *edit_primary_samplerate_edit;
  QLabel *edit_primary_channels_label;
  QComboBox *edit_primary_channels_box;
  QLineEdit *edit_primary_channels_edit;
  QGroupBox *edit_secondary_group;
  QLabel *edit_secondary_number_label;
  QLineEdit *edit_secondary_number_edit;
  QLabel *edit_secondary_tx_algo_label;
  AlgoComboBox *edit_secondary_tx_algo_box;
  QLineEdit *edit_secondary_tx_algo_edit;
  QLabel *edit_secondary_rx_algo_label;
  AlgoComboBox *edit_secondary_rx_algo_box;
  QLineEdit *edit_secondary_rx_algo_edit;
  QLabel *edit_secondary_bitrate_label;
  QComboBox *edit_secondary_bitrate_box;
  QLineEdit *edit_secondary_bitrate_edit;
  QLabel *edit_secondary_samplerate_label;
  QComboBox *edit_secondary_samplerate_box;
  QLineEdit *edit_secondary_samplerate_edit;
  QLabel *edit_secondary_channels_label;
  QComboBox *edit_secondary_channels_box;
  QLineEdit *edit_secondary_channels_edit;
  QLabel *edit_remarks_label;
  QTextEdit *edit_remarks_edit;
  QPushButton *edit_add_button;
  QPushButton *edit_edit_button;
  QPushButton *edit_delete_button;
  QPushButton *edit_migrate_button;
  QPushButton *edit_ok_button;
  QPushButton *edit_cancel_button;
  int edit_program_id;
  EditFeed *edit_editfeed_dialog;
};


#endif  // EDIT_PROGRAM_H
