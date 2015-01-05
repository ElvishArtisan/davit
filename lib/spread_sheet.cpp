// spread_sheet.cpp
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

#include "spread_sheet.h"

SpreadSheet::SpreadSheet()
  : Spread()
{
}


SpreadSheet::~SpreadSheet()
{
  for(unsigned i=0;i<sheet_tabs.size();i++) {
    delete sheet_tabs[i];
  }
}


unsigned SpreadSheet::tabs()
{
  return sheet_tabs.size();
}


SpreadTab *SpreadSheet::tab(int tabnum) const
{
  int index=GetTabIndex(tabnum);
  if(index<0) {
    return NULL;
  }
  return sheet_tabs[index];
}


SpreadTab *SpreadSheet::addTab(int tabnum)
{
  SpreadTab *t=tab(tabnum);

  if(t==NULL) {
    sheet_tabs.push_back(new SpreadTab(tabnum));
    t=sheet_tabs.back();
  }

  return t;
}


void SpreadSheet::deleteTab(int tabnum)
{
}


QString SpreadSheet::write(Spread::FileFormat fmt)
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


QString SpreadSheet::WriteSlkFormat()
{
  QString ret="";

  for(unsigned i=0;i<sheet_tabs.size();i++) {
    ret+=sheet_tabs[i]->write(Spread::SlkFormat);
  }

  return ret;
}


QString SpreadSheet::WriteExcelXmlFormat()
{
  QString xml="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  xml+="<?mso-application progid=\"Excel.Sheet\"?>\n";
  xml+=QString("<Workbook ")+
    "xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\" "+
    "xmlns:c=\"urn:schemas-microsoft-com:office:component:spreadsheet\" "+
    "xmlns:html=\"http://www.w3.org/TR/REC-html40\" "+
    "xmlns:o=\"urn:schemas-microsoft-com:office:office\" "+
    "xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\" "+
    "xmlns:x2=\"http://schemas.microsoft.com/office/excel/2003/xml\" "+
    "xmlns:x=\"urn:schemas-microsoft-com:office:excel\" "+
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n";
  xml+=QString("<OfficeDocumentSettings ")+
    "xmlns=\"urn:schemas-microsoft-com:office:office\">"+
    "<Colors><Color><Index>3</Index><RGB>#c0c0c0</RGB></Color>"+
    "<Color><Index>4</Index><RGB>#ff0000</RGB></Color>"+
    "</Colors></OfficeDocumentSettings>\n";
  xml+=QString("<ExcelWorkbook ")+
    "xmlns=\"urn:schemas-microsoft-com:office:excel\">"+
    "<WindowHeight>9000</WindowHeight>"+
    "<WindowWidth>13860</WindowWidth>"+
    "<WindowTopX>240</WindowTopX>"+
    "<WindowTopY>75</WindowTopY>"+
    "<ProtectStructure>False</ProtectStructure>"+
    "<ProtectWindows>False</ProtectWindows>"+
    "</ExcelWorkbook>\n";
  xml+=QString("<Styles>")+
    "<Style ss:ID=\"Default\" ss:Name=\"Default\"/>"+
    "<Style ss:ID=\"Result\" ss:Name=\"Result\">"+
    "<Font ss:Bold=\"1\" ss:Italic=\"1\" ss:Underline=\"Single\"/>"+
    "</Style><Style ss:ID=\"Result2\" ss:Name=\"Result2\">"+
    "<Font ss:Bold=\"1\" ss:Italic=\"1\" ss:Underline=\"Single\"/>"+
    "<NumberFormat ss:Format=\"Currency\"/></Style>"+
    "<Style ss:ID=\"Heading\" ss:Name=\"Heading\">"+
    "<Font ss:Bold=\"1\" ss:Italic=\"1\" ss:Size=\"16\"/></Style>"+
    "<Style ss:ID=\"Heading1\" ss:Name=\"Heading1\">"+
    "<Font ss:Bold=\"1\" ss:Italic=\"1\" ss:Size=\"16\"/></Style>"+
    "<Style ss:ID=\"co1\"/><Style ss:ID=\"ta1\"/></Styles>\n";

  for(unsigned i=0;i<sheet_tabs.size();i++) {
    xml+=sheet_tabs[i]->write(Spread::ExcelXmlFormat);
  }

  xml+="</Workbook>\n";

  return xml;
}


int SpreadSheet::GetTabIndex(int tabnum) const
{
  for(unsigned i=0;i<sheet_tabs.size();i++) {
    if(sheet_tabs[i]->tab()==tabnum) {
      return i;
    }
  }

  return -1;
}
