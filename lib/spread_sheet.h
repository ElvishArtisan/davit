// spread_sheet.h
//
// Abstract a spreadsheet.
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

#ifndef SPREAD_SHEET_H
#define SPREAD_SHEET_H

#include <spread_tab.h>

class SpreadSheet : public SpreadObject
{
 public:
  SpreadSheet();
  ~SpreadSheet();
  unsigned tabs();
  SpreadTab *tab(int tabnum) const;
  SpreadTab *addTab(int tabnum);
  void deleteTab(int tabnum);
  QString write(FileFormat fmt);

 private:
  QString WriteSlkFormat();
  QString WriteExcelXmlFormat();
  int GetTabIndex(int tabnum) const;
  std::vector<SpreadTab *> sheet_tabs;
};


#endif   // SPREAD_SHEET_H
