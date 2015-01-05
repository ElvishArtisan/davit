// spread.cpp
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

#include "spread.h"

Spread::Spread()
{
  obj_font=QFont(SPREAD_DEFAULT_FONT_FACE,SPREAD_DEFAULT_FONT_SIZE,
		 SPREAD_DEFAULT_FONT_WEIGHT);
  obj_font_metrics=new QFontMetrics(obj_font);
}


Spread::~Spread()
{
  delete obj_font_metrics;
}


QFont Spread::defaultFont() const
{
  return obj_font;
}


void Spread::setDefaultFont(const QFont &font)
{
  obj_font=font;
  delete obj_font_metrics;
  obj_font_metrics=new QFontMetrics(obj_font);
}


QString Spread::fileExtension(FileFormat fmt)
{
  QString ret="";

  switch(fmt) {
  case Spread::SlkFormat:
    ret="slk";
    break;

  case Spread::ExcelXmlFormat:
    ret="xml";
    break;
  }

  return ret;
}


QFontMetrics *Spread::fontMetrics() const
{
  return obj_font_metrics;
}


QString Spread::escapeXml(const QString &xml)
{
  /*
   * Escape a string in accordance with XML-1.0
   */
  QString ret=xml;
  ret.replace("&","&amp;");
  ret.replace("<","&lt;");
  ret.replace(">","&gt;");
  ret.replace("'","&apos;");
  ret.replace("\"","&quot;");
  return ret;
}


QString Spread::unEscapeXml(const QString &xml)
{
  /*
   * Unescape a string in accordance with XML-1.0
   */
  QString ret=xml;
  ret.replace("&amp;","&");
  ret.replace("&lt;","<");
  ret.replace("&gt;",">");
  ret.replace("&apos;","'");
  ret.replace("&quot;","\"");
  return ret;
}
