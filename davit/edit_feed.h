// edit_feed.h
//
// Edit a Davit Feed
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

#ifndef EDIT_FEED_H
#define EDIT_FEED_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QPushButton>

#include <dvtfeed.h>

class EditFeed : public QDialog
{
 Q_OBJECT
 public:
  EditFeed(DvtFeed *slot,QWidget *parent=0);
  ~EditFeed();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void okData();
  void cancelData();

 private:
  QLabel *edit_starttime_label;
  QTimeEdit *edit_starttime_edit;
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
  DvtFeed *edit_feed;
};


#endif  // EDIT_FEED_H
