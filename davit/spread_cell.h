// spread_cell.h
//
// Abstract a spreadhsheet cell.
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

#ifndef SPREAD_CELL_H
#define SPREAD_CELL_H

#include <QFontMetrics>
#include <QString>

#include <spread.h>

class SpreadCell : public Spread
{
 public:
  SpreadCell();
  SpreadCell(int col,int row);
  virtual ~SpreadCell();
  int column() const;
  void setColumn(int col);
  int row() const;
  void setRow(int row);
  QString text() const;
  void setText(const QString &str,QFontMetrics *auto_fm=NULL);
  double width() const;
  void setWidth(double w);
  double height() const;
  void setHeight(double h);
  QString write(Spread::FileFormat fmt);

 private:
  QString WriteSlkFormat();
  QString WriteExcelXmlFormat();
  int cell_column;
  int cell_row;
  QString cell_text;
  double cell_width;
  double cell_height;
};


#endif   // SPREAD_CELL_H
