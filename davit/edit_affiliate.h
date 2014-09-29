// edit_affiliate.h
//
// Edit a Davit Affiliate.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: edit_affiliate.h,v 1.13 2011/03/14 21:07:36 pcvs Exp $
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

#include <qdialog.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qtextedit.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>

#include <dvtfeed.h>
#include <dvtlistviewitem.h>

#include <statecombobox.h>
#include <list_contacts.h>
#include <list_airings.h>
#include <list_remarks.h>
#include <showaffidavits.h>

class EditAffiliate : public QDialog
{
 Q_OBJECT
 public:
  EditAffiliate(int id,QWidget *parent=0,const char *name=0);
  ~EditAffiliate();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

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
};


#endif  // EDIT_AFFILIATE_H
