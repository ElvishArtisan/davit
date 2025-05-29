// dvttableview.h
//
// QTableView widget with consistent defaults
//
//   (C) Copyright 2021-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef DVTTABLEVIEW_H
#define DVTTABLEVIEW_H

#include <QTableView>

class DvtTableView : public QTableView
{
  Q_OBJECT
 public:
  DvtTableView(QWidget *parent=0);
};


#endif  // DVTTABLEVIEW_H
