// pick_fields.h
//
// Pick fields for a Davit report.
//
//   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: pick_fields.h,v 1.7 2011/02/21 23:41:18 pcvs Exp $
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

#include <qdialog.h>
#include <qcombobox.h>
#include <qdatetimeedit.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlabel.h>

#include <statecombobox.h>

class PickFields : public QDialog
{
 Q_OBJECT
 public:
  enum SortField {SortAffiliate=0x0001,SortCityState=0x0002,SortMarket=0x0004};
  enum MarketType {NoMarket=0,DmaMarket=1,MsaMarket=2};
  PickFields(QDate *start_date,QDate *end_date,
	     int *pgm_id,bool pgm_all,
	     int *affiliate_id,bool affiliate_all,
	     int *network_id,bool network_all,
	     PickFields::SortField *sort,
      	     PickFields::MarketType market,
	     QWidget *parent=0,const char *name=0);
  ~PickFields();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  QString selectedMarket() const;
  QString selectedCity() const;
  QString selectedStateCode() const;

 private slots:
  void selectStartDateData();
  void selectEndDateData();
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
  QLabel *report_end_date_label;
  QDateEdit *report_end_date_edit;
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
};


#endif  // PICK_FIELDS_H
