// programlistmodel.cpp
//
// Data model for Davit programs
//
//   (C) Copyright 2021-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <dvtconf.h>

#include "programlistmodel.h"

#include "../icons/program.xpm"

ProgramListModel::ProgramListModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  //
  // Column Attributes
  //
  unsigned left=Qt::AlignLeft|Qt::AlignVCenter;
  //  unsigned center=Qt::AlignCenter;
  //  unsigned right=Qt::AlignRight|Qt::AlignVCenter;

  d_headers.push_back(tr("Program Name"));    // 00
  d_alignments.push_back(left);

  d_headers.push_back(tr("Provider"));        // 01
  d_alignments.push_back(left);

  d_headers.push_back(tr("Contact"));         // 02
  d_alignments.push_back(left);

  d_headers.push_back(tr("Phone"));           // 03
  d_alignments.push_back(left);

  d_headers.push_back(tr("E-Mail Address"));  // 04
  d_alignments.push_back(left);
}


ProgramListModel::~ProgramListModel()
{
}


QPalette ProgramListModel::palette()
{
  return d_palette;
}


void ProgramListModel::setPalette(const QPalette &pal)
{
  d_palette=pal;
}


void ProgramListModel::setFont(const QFont &font)
{
  d_font=font;
}


int ProgramListModel::columnCount(const QModelIndex &parent) const
{
  return d_headers.size();
}


int ProgramListModel::rowCount(const QModelIndex &parent) const
{
  return d_texts.size();
}


QVariant ProgramListModel::headerData(int section,Qt::Orientation orient,
				    int role) const
{
  if((orient==Qt::Horizontal)&&(role==Qt::DisplayRole)) {
    return d_headers.at(section);
  }
  return QVariant();
}


QVariant ProgramListModel::data(const QModelIndex &index,int role) const
{
  QVariant ret;
  QString str;
  int col=index.column();
  int row=index.row();

  if(row<d_texts.size()) {
    switch((Qt::ItemDataRole)role) {
    case Qt::DisplayRole:
      ret=d_texts.at(row).at(col);
      break;

    case Qt::DecorationRole:
      if(col==0) {
	ret=d_icons.at(row);
      }
      break;

    case Qt::TextAlignmentRole:
      ret=d_alignments.at(col);
      break;

    case Qt::FontRole:
      ret=d_font;
      break;

    case Qt::ForegroundRole:
      // Nothing to do!
      break;

    case Qt::BackgroundRole:
      // Nothing to do!
      break;

    default:
      break;
    }
  }

  return ret;
}


bool ProgramListModel::showActiveOnly() const
{
  return d_show_active_only;
}


void ProgramListModel::setShowActiveOnly(bool state)
{
  if(state!=d_show_active_only) {
    d_show_active_only=state;
    refresh();
  }
}


int ProgramListModel::programId(const QModelIndex &row) const
{
  return d_ids.at(row.row());
}


QString ProgramListModel::programName(const QModelIndex &row) const
{
  return d_texts.at(row.row()).at(0).toString();
}


QModelIndex ProgramListModel::addProgram(int program_id)
{
  //
  // Find the insertion offset
  //
  int offset=d_ids.size();

  beginInsertRows(QModelIndex(),offset,offset);
  d_ids.insert(offset,program_id);
  QList<QVariant> list;
  for(int i=0;i<columnCount();i++) {
    list.push_back(QVariant());
  }
  d_texts.insert(offset,list);
  d_icons.insert(offset,QVariant());
  updateRowLine(offset);
  endInsertRows();

  return createIndex(offset,0);
}


void ProgramListModel::removeProgram(const QModelIndex &row)
{
  beginRemoveRows(QModelIndex(),row.row(),row.row());

  d_ids.removeAt(row.row());
  d_texts.removeAt(row.row());
  d_icons.removeAt(row.row());

  endRemoveRows();
}


void ProgramListModel::removeProgram(int program_id)
{
  for(int i=0;i<d_ids.size();i++) {
    if(d_ids.at(i)==program_id) {
      removeProgram(createIndex(i,0));
      return;
    }
  }
}


void ProgramListModel::refresh(const QModelIndex &row)
{
  if(row.row()<d_texts.size()) {
    QString sql=sqlFields()+
      "where "+
      QString::asprintf("`PROGRAMS`.`ID`=%d ",d_ids.at(row.row()));
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    if(q->first()) {
      updateRow(row.row(),q);
      emit dataChanged(createIndex(row.row(),0),
		       createIndex(row.row(),columnCount()));
    }
    delete q;
  }
}


void ProgramListModel::refresh(int provider_id)
{
  updateModel(provider_id);
}


void ProgramListModel::updateModel(int provider_id)
{
  QList<QVariant> texts; 
  QList<QVariant> icons;

  DvtSqlQuery *q=NULL;
  QString sql=sqlFields();
  if(provider_id>0) {
    sql+=QString::asprintf("where `PROGRAMS`.`PROVIDER_ID`=%d ",provider_id);
  }
  sql+="order by `PROGRAMS`.`PROGRAM_NAME` ";
  beginResetModel();
  d_ids.clear();
  d_texts.clear();
  d_icons.clear();
  q=new DvtSqlQuery(sql);
  while(q->next()) {
    d_ids.push_back(q->value(0).toInt());
    d_texts.push_back(texts);
    d_icons.push_back(icons);
    updateRow(d_texts.size()-1,q);
  }
  delete q;
  endResetModel();
}


void ProgramListModel::updateRowLine(int line)
{
  if(line<d_texts.size()) {
    QString sql=sqlFields()+
      " where "+
      QString::asprintf("`PROGRAMS`.`ID`=%d ",d_ids.at(line));
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    if(q->first()) {
      updateRow(line,q);
    }
    delete q;
  }
}


void ProgramListModel::updateRow(int row,DvtSqlQuery *q)
{
  QList<QVariant> texts;

  // Program Name
  texts.push_back(q->value(2));
  d_icons[row]=QPixmap(program_xpm);

  // Provider Name
  texts.push_back(q->value(3));

  // Contact Name
  texts.push_back(q->value(4));

  // Contact Phone
  texts.push_back(DvtFormatPhoneNumber(q->value(5).toString()));

  // Contact E-Mail
  texts.push_back(q->value(6));

  d_texts[row]=texts;
}


QString ProgramListModel::sqlFields() const
{
  QString sql=QString("select ")+
    "`PROGRAMS`.`ID`,"+              // 00
    "`PROGRAMS`.`PROVIDER_ID`,"+     // 01
    "`PROGRAMS`.`PROGRAM_NAME`,"+    // 02
    "`PROVIDERS`.`BUSINESS_NAME`,"+  // 03
    "`PROGRAMS`.`CONTACT_NAME`,"+    // 04
    "`PROGRAMS`.`CONTACT_PHONE`,"+   // 05
    "`PROGRAMS`.`CONTACT_EMAIL` "+   // 06
    "from `PROGRAMS` left join `PROVIDERS` "+
    "on `PROGRAMS`.`PROVIDER_ID`=`PROVIDERS`.`ID` ";
  /*
  QString sql=QString("select ")+
    "`PROGRAMS`.`ID`,"+              // 00
    "`PROGRAMS`.`PROVIDER_ID`,"+     // 01
    "`PROGRAMS`.`PROGRAM_NAME`,"+    // 02
    "`PROVIDERS`.`BUSINESS_NAME`,"+  // 03
    "`PROGRAMS`.`CONTACT_NAME`,"+    // 04
    "`PROGRAMS`.`CONTACT_PHONE`,"+   // 05
    "`PROGRAMS`.`CONTACT_EMAIL` "+   // 06
    "from `PROGRAMS` left join `PROVIDERS` "+
    "on `PROGRAMS`.`PROVIDER_ID`=`PROVIDERS`.`ID` left join `AIRINGS` "+
    "on `PROGRAMS`.`ID`=`AIRINGS`.`PROGRAM_ID` ";
  */

    return sql;
}
