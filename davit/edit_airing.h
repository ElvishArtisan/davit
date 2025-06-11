// edit_airing.h
//
// Edit a Davit Airing
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
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

#ifndef EDIT_AIRING_H
#define EDIT_AIRING_H

#include <QCheckBox>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <dvtdialog.h>

class EditAiring : public DvtDialog
{
 Q_OBJECT
 public:
  EditAiring(DvtConfig *c,QWidget *parent=0);
  ~EditAiring();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(int airing_id,bool new_entry=false);

 private slots:
  void programIndexChangedData(int index);
  void okData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);
  void closeEvent(QCloseEvent *e);

 private:
  QLabel *edit_program_label;
  QComboBox *edit_program_box;
  QLabel *edit_starttime_label;
  QTimeEdit *edit_starttime_edit;
  QLabel *edit_endtime_label;
  QTimeEdit *edit_endtime_edit;
  QLabel *edit_sun_label;
  QCheckBox *edit_sun_button;
  QLabel *edit_mon_label;
  QCheckBox *edit_mon_button;
  QLabel *edit_tue_label;
  QCheckBox *edit_tue_button;
  QLabel *edit_wed_label;
  QCheckBox *edit_wed_button;
  QLabel *edit_thu_label;
  QCheckBox *edit_thu_button;
  QLabel *edit_fri_label;
  QCheckBox *edit_fri_button;
  QLabel *edit_sat_label;
  QCheckBox *edit_sat_button;
  QPushButton *edit_ok_button;
  QPushButton *edit_cancel_button;
  int edit_airing_id;
};


#endif  // EDIT_AIRING_H
