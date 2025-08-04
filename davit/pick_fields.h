// pick_fields.h
//
// Pick fields for a Davit report.
//
//   (C) Copyright 2008-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef PICK_FIELDS_H
#define PICK_FIELDS_H

#include <vector>

#include <QComboBox>
#include <QDateTimeEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "datedialog.h"
#include "dialog.h"
#include "statecombobox.h"

class PickFields : public Dialog
{
 Q_OBJECT
 public:
  enum SortField {SortAffiliate=0x0001,SortCityState=0x0002,SortMarket=0x0004};
  enum MarketType {NoMarket=0,DmaMarket=1,MsaMarket=2,StateMarket=3};
  PickFields(QDate *start_date,QDate *end_date,
	     int *pgm_id,bool pgm_all,
	     int *affiliate_id,bool affiliate_all,
	     int *network_id,bool network_all,
	     PickFields::SortField *sort,
      	     PickFields::MarketType market,
	     DvtConfig *c,QWidget *parent=0);
  ~PickFields();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  QString selectedMarket() const;
  QString selectedCity() const;
  QString selectedStateCode() const;

 private slots:
  void selectStartDateData();
  void selectEndDateData();
  void stateCodeChangedData(const QString &code);
  void okData();
  void cancelData();

 private:
  QLabel *report_program_label;
  QComboBox *report_program_box;
  QLabel *report_market_label;
  QComboBox *report_market_box;
  QLabel *report_city_label;
  QLineEdit *report_city_edit;
  QLabel *report_state_label;
  StateComboBox *report_state_box;
  QLabel *report_affiliate_label;
  QComboBox *report_affiliate_box;
  QLabel *report_network_label;
  QComboBox *report_network_box;
  QLabel *report_start_date_label;
  QDateEdit *report_start_date_edit;
  QPushButton *report_start_select_button;
  QLabel *report_end_date_label;
  QDateEdit *report_end_date_edit;
  QPushButton *report_end_select_button;
  QLabel *report_sort_label;
  QComboBox *report_sort_box;
  QPushButton *report_ok_button;
  QPushButton *report_cancel_button;
  int *report_program_id;
  std::vector<int> report_program_ids;
  QDate *report_start_date;
  QDate *report_end_date;
  int *report_affiliate_id;
  int *report_network_id;
  PickFields::SortField *report_sort;
  std::vector<int> report_affiliate_ids;
  std::vector<int> report_network_ids;
  int report_ypos;
  DateDialog *report_pickdate_dialog;
};


#endif  // PICK_FIELDS_H
