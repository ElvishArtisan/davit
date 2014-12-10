// affidavit_picker.h
//
// Pick fields for an Affidavit Report
//
//   (C) Copyright 2014 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef AFFIDAVIT_PCIKER_H
#define AFFIDAVIT_PICKER_H

#include <vector>

#include <qdialog.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qlabel.h>

#include <dvt.h>

class AffidavitPicker : public QDialog
{
 Q_OBJECT
 public:
  AffidavitPicker(Dvt::AffidavitStationFilter *stations,
		  Dvt::AffidavitSortType *sort,
		  int *pgm_id,QWidget *parent=0);
  ~AffidavitPicker();
  QSize sizeHint() const;

 private slots:
  void stationsSelectedData(int num);
  void okData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *report_stations_label;
  QComboBox *report_stations_box;
  QLabel *report_program_label;
  QComboBox *report_program_box;
  std::vector<int> report_program_ids;
  QLabel *report_sort_label;
  QComboBox *report_sort_box;
  QPushButton *report_ok_button;
  QPushButton *report_cancel_button;
  Dvt::AffidavitStationFilter *report_stations;
  Dvt::AffidavitSortType *report_sort_type;
  int *report_program_id;
};


#endif  // AFFIDAVIT_PICKER_H
