// geometry.h
//
// Persistent geometry for Davit
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <qrect.h>

class Geometry
{
 public:
  enum Widget {MainWidget=0,ContactsWidget=1,AiringsWidget=2,RemarksWidget=3,
	       LastWidget=4};
  Geometry();
  bool isVisible(Widget widget) const;
  void setVisible(Widget widget,bool state);
  QRect geometry(Widget widget,const QRect &def) const;
  void setGeometry(Widget widget,const QRect &rect);
  QString openofficePath() const;
  void load();
  void save();

 private:
  QString ConfigDir();
  QString ConfigPath();
  bool geo_visible[LastWidget];
  QRect geo_rects[LastWidget];
  QString openoffice_path;
};


#endif  // GEOMETRY_H
