// pick_daypart.h
//
// Pick a daypart for a Davit report.
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

#ifndef PICK_DAYPART_H
#define PICK_DAYPART_H

#include <vector>

#include <QDialog>
#include <QDateTimeEdit>
#include <QLabel>

#include <daypicker.h>

class PickDaypart : public QDialog
{
 Q_OBJECT
 public:
  PickDaypart(QTime *start_time,QTime *end_time,bool dows[7],QWidget *parent=0);
  ~PickDaypart();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void okData();
  void cancelData();

 private:
  QLabel *report_start_label;
  QTimeEdit *report_start_edit;
  QLabel *report_end_label;
  QTimeEdit *report_end_edit;
  DayPicker *report_dow_picker;
  QTime *report_start_time;
  QTime *report_end_time;
  bool *report_dows;
};


#endif  // PICK_DAYPART_H
