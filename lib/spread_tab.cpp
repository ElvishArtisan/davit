// spread_tab.cpp
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

#include <qobject.h>

#include "spread_tab.h"

SpreadTab::SpreadTab()
  : Spread()
{
  tab_tab=-1;
  tab_name="[tab]";
}


SpreadTab::SpreadTab(int tab)
  : Spread()
{
  tab_tab=tab;
  tab_name=QObject::tr("Sheet")+QString().sprintf("%d",tab);
}


SpreadTab::~SpreadTab()
{
  for(unsigned i=0;i<tab_cells.size();i++) {
    delete tab_cells[i];
  }
}


int SpreadTab::tab() const
{
  return tab_tab;
}


void SpreadTab::setTab(int tab)
{
  tab_tab=tab;
}


QString SpreadTab::name() const
{
  return tab_name;
}


void SpreadTab::setName(const QString &str)
{
  tab_name=str;
}


int SpreadTab::maximumRow() const
{
  int rows=0;

  for(unsigned i=0;i<tab_cells.size();i++) {
    if(tab_cells[i]->row()>rows) {
      rows=tab_cells[i]->row()+1;
    }
  }
  return rows;
}


int SpreadTab::maximumColumn() const
{
  int cols=0;

  for(unsigned i=0;i<tab_cells.size();i++) {
    if(tab_cells[i]->column()>cols) {
      cols=tab_cells[i]->column();
    }
  }
  return cols;
}


double SpreadTab::columnWidth(int colnum) const
{
  double width=0;

  for(unsigned i=0;i<tab_cells.size();i++) {
    if(tab_cells[i]->column()==colnum) {
      if(tab_cells[i]->width()>width) {
	width=tab_cells[i]->width();
      }
    }
  }
  return width;
}


double SpreadTab::rowHeight(int rownum) const
{
  double height=0.0;

  for(unsigned i=0;i<tab_cells.size();i++) {
    if(tab_cells[i]->row()==rownum) {
      if(tab_cells[i]->height()>height) {
	height=tab_cells[i]->height();
      }
    }
  }
  return height;
}


SpreadCell *SpreadTab::cell(int col,int row) const
{
  int index=GetCellIndex(col,row);
  if(index<0) {
    return NULL;
  }
  return tab_cells[index];
}


SpreadCell *SpreadTab::addCell(int col,int row)
{
  SpreadCell *c=NULL;

  if((c=cell(col,row))==NULL) {
    tab_cells.push_back(new SpreadCell(col,row));
    c=tab_cells.back();
  }

  return c;
}


void SpreadTab::deleteCell(int col,int row)
{
  int index=GetCellIndex(col,row);
  if(index<0) {
    return;
  }
  tab_cells.erase(tab_cells.begin()+index);
}


QString SpreadTab::write(Spread::FileFormat fmt)
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


QString SpreadTab::WriteSlkFormat()
{
  QString ret="";

  ret+="ID;PSCALC3\n";
  for(unsigned i=0;i<tab_cells.size();i++) {
    ret+=tab_cells[i]->write(Spread::SlkFormat);
  }
  ret+="E\n";

  return ret;
}


QString SpreadTab::WriteExcelXmlFormat()
{
  QString xml;
  SpreadCell *c=NULL;

  xml=QString("<ss:Worksheet ss:Name=\""+tab_name+"\">")+
    "<Table ss:StyleID=\"ta1\">";

  int cols=maximumColumn();
  int rows=maximumRow();
  for(int i=0;i<cols;i++) {
    xml+="<Column ss:Width=\""+
      QString().sprintf("%10.4lf",0.8*columnWidth(i+1)).stripWhiteSpace()+
      "\"/>\n";
  }

  for(int i=0;i<rows;i++) {
    xml+="<Row ss:Height=\""+
      QString().sprintf("%10.4lf",0.8*rowHeight(i+1)).stripWhiteSpace()+"\">\n";
    for(int j=0;j<cols;j++) {
      if((c=cell(j+1,i+1))!=NULL) {
	xml+="<Cell>\n";
	xml+="<Data ss:Type=\"String\">"+escapeXml(c->text())+"</Data>\n";
	xml+="</Cell>\n";
      }
    }
    xml+="</Row>\n";
  }
  xml+=QString("</Table><x:WorksheetOptions/></ss:Worksheet>\n");

  return xml;
}


int SpreadTab::GetCellIndex(int col,int row) const
{
  for(unsigned i=0;i<tab_cells.size();i++) {
    if((tab_cells[i]->column()==col)&&(tab_cells[i]->row()==row)) {
      return i;
    }
  }
  return -1;
}

