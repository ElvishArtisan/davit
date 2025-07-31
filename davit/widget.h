// widget.h
//
// Base class for Davit widgets
//
//   (C) Copyright 2019-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <dvtconfig.h>

#include "fontengine.h"

class Widget : public QWidget,public FontEngine
{
  Q_OBJECT;
 public:
  Widget(DvtConfig *c,QWidget *parent=0);
};


#endif  // WIDGET_H
