// airinglistmodel.cpp
//
// Data model for Davit airings
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

#include "airinglistmodel.h"

#include "../icons/user.xpm"

AiringListModel::AiringListModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  //
  // Column Attributes
  //
  unsigned left=Qt::AlignLeft|Qt::AlignVCenter;
  unsigned center=Qt::AlignCenter;
  //  unsigned right=Qt::AlignRight|Qt::AlignVCenter;

  d_headers.push_back(tr("Program Name"));  // 00
  d_alignments.push_back(left);

  d_headers.push_back(tr("Start Time"));    // 01
  d_alignments.push_back(left);

  d_headers.push_back(tr("End Time"));      // 02
  d_alignments.push_back(left);

  d_headers.push_back(tr("Su"));            // 03
  d_alignments.push_back(center);

  d_headers.push_back(tr("Mn"));            // 04
  d_alignments.push_back(center);

  d_headers.push_back(tr("Tu"));            // 05
  d_alignments.push_back(center);

  d_headers.push_back(tr("We"));            // 06
  d_alignments.push_back(center);

  d_headers.push_back(tr("Th"));            // 07
  d_alignments.push_back(center);

  d_headers.push_back(tr("Fr"));            // 08
  d_alignments.push_back(center);

  d_headers.push_back(tr("Sa"));            // 09
  d_alignments.push_back(center);
}


AiringListModel::~AiringListModel()
{
}


QPalette AiringListModel::palette()
{
  return d_palette;
}


void AiringListModel::setPalette(const QPalette &pal)
{
  d_palette=pal;
}


void AiringListModel::setFont(const QFont &font)
{
  d_font=font;
}


void AiringListModel::setAffiliateId(int id)
{
  d_affiliate_id=id;
  refresh();
}


int AiringListModel::columnCount(const QModelIndex &parent) const
{
  return d_headers.size();
}


int AiringListModel::rowCount(const QModelIndex &parent) const
{
  return d_texts.size();
}


QVariant AiringListModel::headerData(int section,Qt::Orientation orient,
				    int role) const
{
  if((orient==Qt::Horizontal)&&(role==Qt::DisplayRole)) {
    return d_headers.at(section);
  }
  return QVariant();
}


QVariant AiringListModel::data(const QModelIndex &index,int role) const
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


int AiringListModel::airingId(const QModelIndex &row) const
{
  return d_ids.at(row.row());
}


QModelIndex AiringListModel::addAiring(int airing_id)
{
  //
  // Find the insertion offset
  //
  int offset=d_ids.size();

  beginInsertRows(QModelIndex(),offset,offset);
  QList<QVariant> list;
  for(int i=0;i<columnCount();i++) {
    list.push_back(QVariant());
  }
  d_ids.insert(offset,airing_id);
  d_texts.insert(offset,list);
  d_icons.insert(offset,QVariant());
  updateRowLine(offset);
  endInsertRows();

  return createIndex(offset,0);
}


void AiringListModel::removeAiring(const QModelIndex &row)
{
  beginRemoveRows(QModelIndex(),row.row(),row.row());

  d_ids.removeAt(row.row());
  d_texts.removeAt(row.row());
  d_icons.removeAt(row.row());

  endRemoveRows();
}


void AiringListModel::removeAiring(int airing_id)
{
  for(int i=0;i<d_ids.size();i++) {
    if(d_ids.at(i)==airing_id) {
      removeAiring(createIndex(i,0));
      return;
    }
  }
}


void AiringListModel::refresh(const QModelIndex &row)
{
  if(row.row()<d_texts.size()) {
    QString sql=sqlFields()+
      "from 'AIRINGS` where "+
      QString::asprintf("`AIRINGS`.`ID`=%d ",d_ids.at(row.row()));
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    if(q->first()) {
      updateRow(row.row(),q);
      emit dataChanged(createIndex(row.row(),0),
		       createIndex(row.row(),columnCount()));
    }
    delete q;
  }
}


void AiringListModel::refresh(int airing_id)
{
  for(int i=0;i<d_ids.size();i++) {
    if(d_ids.at(i)==airing_id) {
      updateRowLine(i);
      return;
    }
  }
}


void AiringListModel::refresh()
{
  updateModel();
}


void AiringListModel::updateModel()
{
  QList<QVariant> texts; 
  QList<QVariant> icons;

  DvtSqlQuery *q=NULL;
  QString sql=sqlFields()+
    QString::asprintf("where `AIRINGS`.`AFFILIATE_ID`=%d ",d_affiliate_id)+
    "order by `PROGRAMS`.`PROGRAM_NAME` ";
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


void AiringListModel::updateRowLine(int line)
{
  if(line<d_texts.size()) {
    QString sql=sqlFields()+
      "from `AIRINGS` where "+
      QString::asprintf("`ID`=%d ",d_ids.at(line));
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    if(q->first()) {
      updateRow(line,q);
    }
    delete q;
  }
}


void AiringListModel::updateRow(int row,DvtSqlQuery *q)
{
  QList<QVariant> texts;

  // Program Name
  texts.push_back(q->value(1));
  d_icons[row]=QPixmap(user_xpm);

  // Start Time
  texts.push_back(q->value(2).toTime().toString("hh:mm:ss"));

  // End Time
  texts.push_back(q->value(2).toTime().addSecs(q->value(3).toInt()).
		  toString("hh:mm:ss"));

  // Sunday
  if(q->value(4).toString()=="Y") {
    texts.push_back("Su");
  }
  else {
    texts.push_back("");
  }

  // Monday
  if(q->value(5).toString()=="Y") {
    texts.push_back("Mo");
  }
  else {
    texts.push_back("");
  }

  // Tuesday
  if(q->value(6).toString()=="Y") {
    texts.push_back("Tu");
  }
  else {
    texts.push_back("");
  }

  // Wednesday
  if(q->value(7).toString()=="Y") {
    texts.push_back("We");
  }
  else {
    texts.push_back("");
  }

  // Thursday
  if(q->value(8).toString()=="Y") {
    texts.push_back("Th");
  }
  else {
    texts.push_back("");
  }

  // Friday
  if(q->value(9).toString()=="Y") {
    texts.push_back("Fr");
  }
  else {
    texts.push_back("");
  }

  // Saturday
  if(q->value(10).toString()=="Y") {
    texts.push_back("Sa");
  }
  else {
    texts.push_back("");
  }

  d_texts[row]=texts;
}


QString AiringListModel::sqlFields() const
{
  QString sql=QString("select ")+
    "`AIRINGS`.`ID`,"+             // 00
    "`PROGRAMS`.`PROGRAM_NAME`,"+  // 01
    "`AIRINGS`.`AIR_TIME`,"+       // 02
    "`AIRINGS`.`AIR_LENGTH`,"+     // 03
    "`AIRINGS`.`AIR_SUN`,"+        // 04
    "`AIRINGS`.`AIR_MON`,"+        // 05
    "`AIRINGS`.`AIR_TUE`,"+        // 06
    "`AIRINGS`.`AIR_WED`,"+        // 07
    "`AIRINGS`.`AIR_THU`,"+        // 08
    "`AIRINGS`.`AIR_FRI`,"+        // 09
    "`AIRINGS`.`AIR_SAT` "+        // 10
    "from `PROGRAMS` left join `AIRINGS` "+
    "on `PROGRAMS`.`ID`=`AIRINGS`.`PROGRAM_ID` ";
  /*
  where "+
    QString::asprintf("`AIRINGS`.`AFFILIATE_ID`=%d",)+
			list_id);
  */
    return sql;
}
