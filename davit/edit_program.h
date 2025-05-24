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

#include <QDialog>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QListView>
#include <QLabel>
#include <QTextEdit>

#include <algocombobox.h>
#include <dvtlistviewitem.h>
#include <dvtfeed.h>

class EditProgram : public QDialog
{
 Q_OBJECT
 public:
  EditProgram(const QString &pname,QWidget *parent=0);
  ~EditProgram();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void migrateData();
  void addFeedData();
  void editFeedData();
  void deleteFeedData();
  //  void doubleClickedData(QListViewItem *item,const QPoint &pt,int col);
  void primaryIsdnChangedData(const QString &str);
  void secondaryIsdnChangedData(const QString &str);
  void okData();
  void cancelData();

 protected:
  void paintEvent(QPaintEvent *e);

 private:
  void RefreshList();
  void RefreshItem(DvtListViewItem *item,DvtFeed *feed);
  QLineEdit *edit_program_name_edit;
  QTimeEdit *edit_program_length_edit;
  QLineEdit *edit_program_length_lineedit;
  QLineEdit *edit_contact_name_edit;
  QLineEdit *edit_contact_phone_edit;
  QLineEdit *edit_contact_fax_edit;
  QLineEdit *edit_contact_email_edit;
  QListView *edit_feeds_list;
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
  QTextEdit *edit_remarks_edit;
  int edit_id;
};


#endif  // EDIT_PROGRAM_H
