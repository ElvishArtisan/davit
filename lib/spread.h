// spread.h
//
// Abstract base class for a spreadsheet object.
//
//   (C) Copyright 2014-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef SPREAD_H
#define SPREAD_H

#include <QFont>
#include <QFontMetrics>
#include <QString>

#define SPREAD_DEFAULT_FONT_FACE "arial"
#define SPREAD_DEFAULT_FONT_SIZE 10
#define SPREAD_DEFAULT_FONT_WEIGHT QFont::Normal

class Spread
{
 public:
  enum FileFormat {SlkFormat=0,ExcelXmlFormat=1};
  Spread();
  ~Spread();
  QFont defaultFont() const;
  void setDefaultFont(const QFont &font);
  virtual QString write(FileFormat fmt)=0;
  static QString fileExtension(FileFormat fmt);

 protected:
  QFontMetrics *fontMetrics() const;
  QString escapeXml(const QString &xml);
  QString unEscapeXml(const QString &xml);

 private:
  QFont obj_font;
  QFontMetrics *obj_font_metrics;
};


#endif   // SPREAD_H
