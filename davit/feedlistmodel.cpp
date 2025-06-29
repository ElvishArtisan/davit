// feedlistmodel.cpp
//
// Data model for Davit feeds
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

#include "feedlistmodel.h"

#include "../icons/user.xpm"

FeedListModel::FeedListModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  //
  // Column Attributes
  //
  unsigned left=Qt::AlignLeft|Qt::AlignVCenter;
  unsigned center=Qt::AlignCenter;
  //  unsigned right=Qt::AlignRight|Qt::AlignVCenter;

  d_headers.push_back(tr("Start Time"));    // 00
  d_alignments.push_back(left);

  d_headers.push_back(tr("End Time"));      // 01
  d_alignments.push_back(left);

  d_headers.push_back(tr("Su"));            // 02
  d_alignments.push_back(center);

  d_headers.push_back(tr("Mn"));            // 03
  d_alignments.push_back(center);

  d_headers.push_back(tr("Tu"));            // 04
  d_alignments.push_back(center);

  d_headers.push_back(tr("We"));            // 05
  d_alignments.push_back(center);

  d_headers.push_back(tr("Th"));            // 06
  d_alignments.push_back(center);

  d_headers.push_back(tr("Fr"));            // 07
  d_alignments.push_back(center);

  d_headers.push_back(tr("Sa"));            // 08
  d_alignments.push_back(center);
}


FeedListModel::~FeedListModel()
{
}


QPalette FeedListModel::palette()
{
  return d_palette;
}


void FeedListModel::setPalette(const QPalette &pal)
{
  d_palette=pal;
}


void FeedListModel::setFont(const QFont &font)
{
  d_font=font;
}


void FeedListModel::setProgramId(int id)
{
  d_program_id=id;
  refresh();
}


int FeedListModel::columnCount(const QModelIndex &parent) const
{
  return d_headers.size();
}


int FeedListModel::rowCount(const QModelIndex &parent) const
{
  return d_texts.size();
}


QVariant FeedListModel::headerData(int section,Qt::Orientation orient,
				    int role) const
{
  if((orient==Qt::Horizontal)&&(role==Qt::DisplayRole)) {
    return d_headers.at(section);
  }
  return QVariant();
}


QVariant FeedListModel::data(const QModelIndex &index,int role) const
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

    case Qt::SizeHintRole:
      if((col>=2)&&(col<9)) {
	return QSize(30,10);
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


int FeedListModel::feedId(const QModelIndex &row) const
{
  return d_ids.at(row.row());
}


QString FeedListModel::programName(const QModelIndex &row) const
{
  return d_texts.at(row.row()).at(0).toString();
}


QModelIndex FeedListModel::addFeed(int feed_id)
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
  d_ids.insert(offset,feed_id);
  d_texts.insert(offset,list);
  d_icons.insert(offset,QVariant());
  updateRowLine(offset);
  endInsertRows();

  return createIndex(offset,0);
}


void FeedListModel::removeFeed(const QModelIndex &row)
{
  beginRemoveRows(QModelIndex(),row.row(),row.row());

  d_ids.removeAt(row.row());
  d_texts.removeAt(row.row());
  d_icons.removeAt(row.row());

  endRemoveRows();
}


void FeedListModel::removeFeed(int feed_id)
{
  for(int i=0;i<d_ids.size();i++) {
    if(d_ids.at(i)==feed_id) {
      removeFeed(createIndex(i,0));
      return;
    }
  }
}


void FeedListModel::refresh(const QModelIndex &row)
{
  if(row.row()<d_texts.size()) {
    QString sql=sqlFields()+"where "+
      QString::asprintf("`FEEDS`.`ID`=%d ",d_ids.at(row.row()));
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    if(q->first()) {
      updateRow(row.row(),q);
      emit dataChanged(createIndex(row.row(),0),
		       createIndex(row.row(),columnCount()));
    }
    delete q;
  }
}


void FeedListModel::refresh(int feed_id)
{
  for(int i=0;i<d_ids.size();i++) {
    if(d_ids.at(i)==feed_id) {
      updateRowLine(i);
      return;
    }
  }
}


void FeedListModel::refresh()
{
  updateModel();
}


void FeedListModel::save() const
{
}


void FeedListModel::updateModel()
{
  QList<QVariant> texts; 
  QList<QVariant> icons;

  DvtSqlQuery *q=NULL;
  QString sql=sqlFields()+
    QString::asprintf("where `FEEDS`.`PROGRAM_ID`=%d ",d_program_id)+
    "order by `FEEDS`.`AIR_TIME` ";
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


void FeedListModel::updateRowLine(int line)
{
  if(line<d_texts.size()) {
    QString sql=sqlFields()+
      "where "+
      QString::asprintf("`FEEDS`.`ID`=%d ",d_ids.at(line));
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    if(q->first()) {
      updateRow(line,q);
    }
    delete q;
  }
}


void FeedListModel::updateRow(int row,DvtSqlQuery *q)
{
  QList<QVariant> texts;

  d_icons[row]=QPixmap(user_xpm);

  // Start Time
  texts.push_back(q->value(1).toTime().toString("hh:mm:ss"));

  // End Time
  texts.push_back(q->value(1).toTime().addSecs(q->value(2).toInt()).
		  toString("hh:mm:ss"));

  // Sunday
  if(q->value(3).toString()=="Y") {
    texts.push_back("Su");
  }
  else {
    texts.push_back("");
  }

  // Monday
  if(q->value(4).toString()=="Y") {
    texts.push_back("Mo");
  }
  else {
    texts.push_back("");
  }

  // Tuesday
  if(q->value(5).toString()=="Y") {
    texts.push_back("Tu");
  }
  else {
    texts.push_back("");
  }

  // Wednesday
  if(q->value(6).toString()=="Y") {
    texts.push_back("We");
  }
  else {
    texts.push_back("");
  }

  // Thursday
  if(q->value(7).toString()=="Y") {
    texts.push_back("Th");
  }
  else {
    texts.push_back("");
  }

  // Friday
  if(q->value(8).toString()=="Y") {
    texts.push_back("Fr");
  }
  else {
    texts.push_back("");
  }

  // Saturday
  if(q->value(9).toString()=="Y") {
    texts.push_back("Sa");
  }
  else {
    texts.push_back("");
  }

  d_texts[row]=texts;
}


QString FeedListModel::sqlFields() const
{
  QString sql=QString("select ")+
    "`FEEDS`.`ID`,"+               // 00
    "`FEEDS`.`AIR_TIME`,"+         // 01
    "`PROGRAMS`.`AIR_LENGTH`,"+    // 02
    "`FEEDS`.`AIR_SUN`,"+          // 03
    "`FEEDS`.`AIR_MON`,"+          // 04
    "`FEEDS`.`AIR_TUE`,"+          // 05
    "`FEEDS`.`AIR_WED`,"+          // 06
    "`FEEDS`.`AIR_THU`,"+          // 07
    "`FEEDS`.`AIR_FRI`,"+          // 08
    "`FEEDS`.`AIR_SAT` "+          // 09
    "from `PROGRAMS` left join `FEEDS` "+
    "on `PROGRAMS`.`ID`=`FEEDS`.`PROGRAM_ID` ";
    return sql;
}
