// spread_object.h
//
// Abstract base class for a spreadsheet object.
//
//   (C) Copyright 2014 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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

#ifndef SPREAD_OBJECT_H
#define SPREAD_OBJECT_H

#include <qstring.h>

class SpreadObject
{
 public:
  enum FileFormat {SlkFormat=0,ExcelXmlFormat=1};
  SpreadObject();
  ~SpreadObject();
  virtual QString write(FileFormat fmt)=0;

 protected:
  QString escapeXml(const QString &xml);
  QString unEscapeXml(const QString &xml);
};


#endif   // SPREAD_OBJECT_H
