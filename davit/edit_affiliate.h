// edit_affiliate.h
//
// Edit a Davit Affiliate.
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

#ifndef EDIT_AFFILIATE_H
#define EDIT_AFFILIATE_H

#include <vector>

#include <QLineEdit>
#include <QListView>
#include <QTextEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QButtonGroup>

#include <dvtdialog.h>
#include <dvtfeed.h>

#include "list_contacts.h"
#include "list_airings.h"
#include "list_remarks.h"
#include "showaffidavits.h"
#include "statecombobox.h"

class EditAffiliate : public DvtDialog
{
 Q_OBJECT
 public:
  EditAffiliate(DvtConfig *c,QWidget *parent=0);
  ~EditAffiliate();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(int affiliate_id);

 private slots:
  void affidavitToggledData(bool state);
  void showContactsData();
  void contactsVisibilityChangedData(bool visible);
  void showAiringsData();
  void airingsVisibilityChangedData(bool visible);
  void showRemarksData();
  void remarksVisibilityChangedData(bool visible);
  void okData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);
  void closeEvent(QCloseEvent *e);

 private:
  void SaveGeometry();
  int edit_id;
  QLineEdit *edit_station_call_edit;
  QLineEdit *edit_station_frequency_edit;
  QComboBox *edit_station_type_box;
  QLineEdit *edit_station_type_edit;
  QLineEdit *edit_license_city_edit;
  StateComboBox *edit_license_state_box;
  QLineEdit *edit_license_state_edit;
  QComboBox *edit_market_box;
  QSpinBox *edit_market_rank_spin;
  QComboBox *edit_dma_box;
  QSpinBox *edit_dma_rank_spin;
  QComboBox *edit_second_network_box;
  QLineEdit *edit_second_network_edit;
  std::vector<int> second_network_ids;
  QComboBox *edit_timezone_box;
  QLineEdit *edit_timezone_edit;
  QLineEdit *edit_station_prev_call_edit;
  QComboBox *edit_license_country_box;
  QLineEdit *edit_license_country_edit;
  QLineEdit *edit_station_power_edit;
  QLineEdit *edit_station_night_power_edit;
  QLineEdit *edit_business_name_edit;
  QLineEdit *edit_address1_edit;
  QLineEdit *edit_address2_edit;
  QLineEdit *edit_city_edit;
  StateComboBox *edit_state_box;
  QLineEdit *edit_state_edit;
  QLineEdit *edit_zipcode_edit;
  QComboBox *edit_country_box;
  QLineEdit *edit_country_edit;
  QLineEdit *edit_phone_edit;
  QLineEdit *edit_fax_edit;
  QLineEdit *edit_email_addr_edit;
  QLineEdit *edit_web_url_edit;
  QCheckBox *edit_track_affidavit_box;
  ShowAffidavits *edit_affidavits;
  ListContacts *edit_contact_list;
  ListAirings *edit_airings_list;
  ListRemarks *edit_remarks_list;
  QLineEdit *edit_web_password_edit;
  QLabel *edit_web_password_label;
  std::vector<QString> edit_program_adds;
  std::vector<QString> edit_program_deletes;
  QPushButton *edit_contacts_button;
  QPushButton *edit_airings_button;
  QPushButton *edit_remarks_button;
  QLabel *edit_station_call_label;
  QLabel *edit_station_frequency_label;
  QLabel *edit_station_type_label;
  QLabel *edit_station_power_label;
  QLabel *edit_station_power_unit_label;
  QLabel *edit_station_night_power_label;
  QLabel *edit_station_night_power_unit_label;
  QLabel *edit_license_city_label;
  QLabel *edit_license_state_label;
  QLabel *edit_market_label;
  QLabel *edit_market_rank_label;
  QLabel *edit_dma_label;
  QLabel *edit_dma_rank_label;
  QLabel *edit_second_network_label;
  QLabel *edit_timezone_label;
  QLabel *edit_station_prev_call_label;
  QLabel *edit_address_label;
  QLabel *edit_city_label;
  QLabel *edit_state_label;
  QLabel *edit_zipcode_label;
  QLabel *edit_email_addr_label;
  QLabel *edit_phone_label;
  QLabel *edit_fax_label;
  QLabel *edit_web_url_label;
  QLabel *edit_track_affidavit_label;
  QLabel *edit_business_name_call_label;
  QPushButton *edit_ok_button;
  QPushButton *edit_cancel_button;
};


#endif  // EDIT_AFFILIATE_H
