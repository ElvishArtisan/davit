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

#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QSqlDatabase>
#include <QPixmap>

#include "add_affiliate.h"
#include "affiliatelistmodel.h"
#include "dialog.h"
#include "edit_affiliate.h"
#include "generate_affadavit.h"
#include "tableview.h"
#include "tablewidgetitem.h"

class ListAffiliates : public Dialog
{
 Q_OBJECT
 public:
  ListAffiliates(DvtConfig *c,QWidget *parent);
  ~ListAffiliates();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void showAffiliatesChangedData(int state);
  void missingAffiliateChangedData(int state);
  void monthActivatedData(int index);
  void yearActivatedData(int index);
  void filterTextChangedData(const QString &str);
  void addData();
  void editData();
  void deleteData();
  void affadavitData();
  void affidavitReminderData();
  void doubleClickedData(const QModelIndex &index);
  void selectionChangedData(const QItemSelection &before,
			    const QItemSelection &after);
  void rowCountChangedData(int matches);
  void closeData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  QDate MissingAffiliatesDate() const;
  QLabel *list_show_affiliates_label;
  QCheckBox *list_show_affiliates_check;
  QLabel *list_missing_affiliates_label;
  QCheckBox *list_missing_affiliates_check;
  QComboBox *list_month_box;
  QComboBox *list_year_box;
  QLabel *list_callfilter_label;
  QLineEdit *list_callfilter_edit;
  TableView *list_affiliates_view;
  AffiliateListModel *list_affiliates_model;
  QPushButton *list_add_button;
  QPushButton *list_edit_button;
  QPushButton *list_delete_button;
  QPushButton *list_affadavit_button;
  QPushButton *list_affidavit_reminder_button;
  QPushButton *list_close_button;
  AddAffiliate *list_addaffiliate_dialog;
  EditAffiliate *list_editaffiliate_dialog;
  GenerateAffadavit *list_generateaffadavit_dialog;
  QPixmap *list_greenball_map;
  QPixmap *list_redball_map;
  QPixmap *list_whiteball_map;
  QLabel *list_showing_label;
  QLineEdit *list_showing_edit;
  QLabel *list_greenball_labels[2];
  QLabel *list_redball_labels[2];
  QLabel *list_blueball_labels[2];
  QLabel *list_whiteball_labels[2];
};


#endif  // LIST_AFFILIATES_H
