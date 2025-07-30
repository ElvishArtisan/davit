// statecombobox.h
//
// QComboBox for choosing a US state.
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

#ifndef STATECOMBOBOX_H
#define STATECOMBOBOX_H

#include <QComboBox>

class StateComboBox : public QComboBox
{
 Q_OBJECT
 public:
  StateComboBox(bool incl_all,QWidget *parent);
  QString currentStateCode() const;

 signals:
  void stateCodeChanged(const QString &code);

 public slots:
  void setCurrentStateCode(const QString &state_code);

 private slots:
  void activatedData(const QString &str);
};


#endif  // STATECOMBOBOX_H
