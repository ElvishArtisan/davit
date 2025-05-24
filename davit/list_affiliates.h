// list_affiliates.h
//
// List Davit Affiliates.
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

#ifndef LIST_AFFILIATES_H
#define LIST_AFFILIATES_H

#include <QDialog>
#include <QCheckBox>
//#include <QListview>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QSqlDatabase>
#include <QProgressDialog>
#include <QPixmap>

#include <dvtlistviewitem.h>

class ListAffiliates : public QDialog
{
 Q_OBJECT
 public:
  ListAffiliates(QWidget *parent=0);
  ~ListAffiliates();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void filterStateChangedData(int state);
  void missingStateChangedData(int state);
  void monthActivatedData(int index);
  void yearActivatedData(int index);
  void filterTextChangedData(const QString &str);
  void addData();
  void editData();
  void deleteData();
  void affadavitData();
  void affidavitReminderData();
  //  void doubleClickedData(QListViewItem *item,const QPoint &pt,int c);
  void closeData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  void SendAffidavitReminder();
  void DeleteAffiliate(int id);
  void RefreshList();
  void UpdateItem(DvtListViewItem *item);
  QString FilterSql(const QString &filter,bool affils_only,
		    const QDate &affidavit_date);
  //QString FilterSql(const QString &filter,bool affils_only,QSqlQuery *q);
  QLabel *list_filter_label;
  QCheckBox *list_filter_box;
  QLabel *list_missing_label;
  QCheckBox *list_missing_box;
  QComboBox *list_month_box;
  QComboBox *list_year_box;
  QLabel *list_callfilter_label;
  QLineEdit *list_callfilter_edit;
  //  QListView *list_affiliates_list;
  QPushButton *list_add_button;
  QPushButton *list_edit_button;
  QPushButton *list_delete_button;
  QPushButton *list_affadavit_button;
  QPushButton *list_affidavit_reminder_button;
  QPushButton *list_close_button;
  QProgressDialog *list_email_progress;
  QPixmap *list_greenball_map;
  QPixmap *list_redball_map;
  QPixmap *list_whiteball_map;
  QLabel *list_showing_label;
  QLineEdit *list_showing_edit;
};


#endif  // LIST_AFFILIATES_H
