// networklistmodel.cpp
//
// Data model for Davit networks
//
//   (C) Copyright 2025 Fred Gleason <fredg@paravelsystems.com>
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

#include "networklistmodel.h"

#include "../icons/network.xpm"

NetworkListModel::NetworkListModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  //
  // Column Attributes
  //
  unsigned left=Qt::AlignLeft|Qt::AlignVCenter;
  //  unsigned center=Qt::AlignCenter;
  //  unsigned right=Qt::AlignRight|Qt::AlignVCenter;

  d_headers.push_back(tr("Name"));            // 00
  d_alignments.push_back(left);
}


NetworkListModel::~NetworkListModel()
{
}


QPalette NetworkListModel::palette()
{
  return d_palette;
}


void NetworkListModel::setPalette(const QPalette &pal)
{
  d_palette=pal;
}


void NetworkListModel::setFont(const QFont &font)
{
  d_font=font;
}


void NetworkListModel::setAffiliateId(int id)
{
  d_affiliate_id=id;
  refresh();
}


int NetworkListModel::columnCount(const QModelIndex &parent) const
{
  return d_headers.size();
}


int NetworkListModel::rowCount(const QModelIndex &parent) const
{
  return d_texts.size();
}


QVariant NetworkListModel::headerData(int section,Qt::Orientation orient,
				    int role) const
{
  if((orient==Qt::Horizontal)&&(role==Qt::DisplayRole)) {
    return d_headers.at(section);
  }
  return QVariant();
}


QVariant NetworkListModel::data(const QModelIndex &index,int role) const
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


int NetworkListModel::networkId(const QModelIndex &row) const
{
  return d_ids.at(row.row());
}


QString NetworkListModel::networkName(const QModelIndex &row) const
{
  return d_texts.at(row.row()).at(0).toString();
}


QModelIndex NetworkListModel::addNetwork(int network_id)
{
  //
  // Find the insertion offset
  //
  int offset=d_ids.size();

  beginInsertRows(QModelIndex(),offset,offset);
  d_ids.insert(offset,network_id);
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


void NetworkListModel::removeNetwork(const QModelIndex &row)
{
  beginRemoveRows(QModelIndex(),row.row(),row.row());

  d_ids.removeAt(row.row());
  d_texts.removeAt(row.row());
  d_icons.removeAt(row.row());

  endRemoveRows();
}


void NetworkListModel::removeNetwork(int network_id)
{
  for(int i=0;i<d_ids.size();i++) {
    if(d_ids.at(i)==network_id) {
      removeNetwork(createIndex(i,0));
      return;
    }
  }
}


void NetworkListModel::refresh(const QModelIndex &row)
{
  if(row.row()<d_texts.size()) {
    QString sql=sqlFields()+
      "where "+
      QString::asprintf("`NETWORKS`.`ID`=%d ",d_ids.at(row.row()));
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    if(q->first()) {
      updateRow(row.row(),q);
      emit dataChanged(createIndex(row.row(),0),
		       createIndex(row.row(),columnCount()));
    }
    delete q;
  }
}


void NetworkListModel::refresh(int network_id)
{
  for(int i=0;i<d_ids.size();i++) {
    if(d_ids.at(i)==network_id) {
      updateRowLine(i);
      return;
    }
  }
}


void NetworkListModel::refresh()
{
  updateModel();
}


void NetworkListModel::updateModel()
{
  QList<QVariant> texts; 
  QList<QVariant> icons;

  DvtSqlQuery *q=NULL;
  QString sql=sqlFields()+
    "order by `NAME` ";
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


void NetworkListModel::updateRowLine(int line)
{
  if(line<d_texts.size()) {
    QString sql=sqlFields()+
      " where "+
      QString::asprintf("`ID`=%d ",d_ids.at(line));
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    if(q->first()) {
      updateRow(line,q);
    }
    delete q;
  }
}


void NetworkListModel::updateRow(int row,DvtSqlQuery *q)
{
  QList<QVariant> texts;

  // Network Name
  texts.push_back(q->value(1));
  d_icons[row]=QPixmap(network_xpm);

  d_texts[row]=texts;
}


QString NetworkListModel::sqlFields() const
{
  QString sql=QString("select ")+
    "`NETWORKS`.`ID`,"+                // 00
    "`NETWORKS`.`NAME` "+              // 01
    "from `NETWORKS` ";

    return sql;
}
