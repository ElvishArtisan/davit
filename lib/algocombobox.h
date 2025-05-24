// algocombobox.h
//
// QComboBox for choosing an audio encoding algorithm.
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

#ifndef ALGOCOMBOBOX_H
#define ALGOCOMBOBOX_H

#include <QComboBox>

class AlgoComboBox : public QComboBox
{
 Q_OBJECT
 public:
  AlgoComboBox(QWidget *parent=0);
  QString currentAlgoCode() const;

 public slots:
  void setCurrentAlgoCode(const QString &algo_code);
};


#endif  // ALGOCOMBOBOX_H
