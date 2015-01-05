// spread_tab.h
//
// Abstract a spreadsheet tab.
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

#ifndef SPREAD_TAB_H
#define SPREAD_TAB_H

#include <vector>

#include <spread_cell.h>

class SpreadTab : public Spread
{
 public:
  SpreadTab();
  SpreadTab(int tab);
  ~SpreadTab();
  int tab() const;
  void setTab(int tab);
  QString name() const;
  void setName(const QString &str);
  int maximumRow() const;
  int maximumColumn() const;
  double columnWidth(int colnum) const;
  double rowHeight(int rownum) const;
  SpreadCell *cell(int col,int row) const;
  SpreadCell *addCell(int col,int row);
  void deleteCell(int col,int row);
  QString write(Spread::FileFormat fmt);

 private:
  QString WriteSlkFormat();
  QString WriteExcelXmlFormat();
  int GetCellIndex(int col,int row) const;
  int tab_tab;
  QString tab_name;
  std::vector<SpreadCell *> tab_cells;
};


#endif   // SPREAD_TAB_H
