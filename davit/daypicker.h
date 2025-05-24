// daypicker.h
//
// A Widget for selecting days of the week.
//
//   (C) Copyright 2010-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef DAYPICKER_H
#define DAYPICKER_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>

class DayPicker : public QWidget
{
 Q_OBJECT
 public:
  DayPicker(QWidget *parent=0);
  ~DayPicker();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  bool state(int dow) const;
  void setState(int dow,bool state);

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  QCheckBox *dow_box[7];
  QLabel *dow_label[7];
};


#endif  // DAYPICKER_H
