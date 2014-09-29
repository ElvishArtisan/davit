// statecombobox.h
//
// QComboBox for choosing a US state.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: statecombobox.h,v 1.1 2007/03/16 19:10:54 fredg Exp $
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

#include <qcombobox.h>


class StateComboBox : public QComboBox
{
 Q_OBJECT
 public:
  StateComboBox(QWidget *parent=0,const char *name=0);
  QString currentStateCode() const;

 public slots:
  void setCurrentStateCode(const QString &state_code);
};


#endif  // STATECOMBOBOX_H
