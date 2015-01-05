// spread_cell.cpp
//
// Abstract a spreadhsheet cell.
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

#include <stdio.h>

#include "spread_cell.h"

SpreadCell::SpreadCell()
  : Spread()
{
  cell_column=-1;
  cell_row=-1;
  cell_text="";
  cell_width=58.8144;
  cell_height=12.1032;
}


SpreadCell::SpreadCell(int col,int row)
{
  cell_column=col;
  cell_row=row;
  cell_width=58.8144;
  cell_height=12.1032;
}


SpreadCell::~SpreadCell()
{
}


int SpreadCell::column() const
{
  return cell_column;
}


void SpreadCell::setColumn(int col)
{
  cell_column=col;
}


int SpreadCell::row() const
{
  return cell_row;
}


void SpreadCell::setRow(int row)
{
  cell_row=row;
}


QString SpreadCell::text() const
{
  return cell_text;
}


void SpreadCell::setText(const QString &str,QFontMetrics *auto_fm)
{
  cell_text=str;

  if(auto_fm==NULL) {
    auto_fm=fontMetrics();
  }
  setWidth(auto_fm->width(cell_text)+10);
  setHeight(auto_fm->height());
}


double SpreadCell::width() const
{
  return cell_width;
}


void SpreadCell::setWidth(double w)
{
  cell_width=w;
}


double SpreadCell::height() const
{
  return cell_height;
}


void SpreadCell::setHeight(double h)
{
  cell_height=h;
}


QString SpreadCell::write(Spread::FileFormat fmt)
{
  switch(fmt) {
  case Spread::SlkFormat:
    return WriteSlkFormat();
    break;

  case Spread::ExcelXmlFormat:
    return WriteExcelXmlFormat();
    break;
  }
  return QString("");
}


QString SpreadCell::WriteSlkFormat()
{
  QString ret="";

  if((cell_column>0)&&(cell_row>0)) {
    ret+=QString().sprintf("C;X%d;Y%d;",cell_column,cell_row);
    ret+="K\""+cell_text+"\"\n";
  }
  else {
    fprintf(stderr,"SpreadCell::write() -- attempting to write non-anchored cell, text: %s\n",(const char *)cell_text.utf8());
  }

  return ret;
}


QString SpreadCell::WriteExcelXmlFormat()
{
  QString xml="";

  xml+="<Cell>\n";
  xml+=QString("Data ss:Type=\"String\">")+cell_text+"</Data>\n";
  xml+="</Cell>\n";

  return xml;
}
