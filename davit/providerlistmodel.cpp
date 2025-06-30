// providerlistmodel.cpp
//
// Data model for Davit providers
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

#include "providerlistmodel.h"

#include "../icons/user.xpm"

ProviderListModel::ProviderListModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  //
  // Column Attributes
  //
  unsigned left=Qt::AlignLeft|Qt::AlignVCenter;
  //  unsigned center=Qt::AlignCenter;
  //  unsigned right=Qt::AlignRight|Qt::AlignVCenter;

  d_headers.push_back(tr("Business Name"));            // 00
  d_alignments.push_back(left);

  d_headers.push_back(tr("Contact"));           // 01
  d_alignments.push_back(left);

  d_headers.push_back(tr("Phone Number"));    // 02
  d_alignments.push_back(left);

  d_headers.push_back(tr("E-Mail Address"));  // 04
  d_alignments.push_back(left);
}


ProviderListModel::~ProviderListModel()
{
}


QPalette ProviderListModel::palette()
{
  return d_palette;
}


void ProviderListModel::setPalette(const QPalette &pal)
{
  d_palette=pal;
}


void ProviderListModel::setFont(const QFont &font)
{
  d_font=font;
}


int ProviderListModel::columnCount(const QModelIndex &parent) const
{
  return d_headers.size();
}


int ProviderListModel::rowCount(const QModelIndex &parent) const
{
  return d_texts.size();
}


QVariant ProviderListModel::headerData(int section,Qt::Orientation orient,
				    int role) const
{
  if((orient==Qt::Horizontal)&&(role==Qt::DisplayRole)) {
    return d_headers.at(section);
  }
  return QVariant();
}


QVariant ProviderListModel::data(const QModelIndex &index,int role) const
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


int ProviderListModel::providerId(const QModelIndex &row) const
{
  return d_ids.at(row.row());
}


QString ProviderListModel::providerName(const QModelIndex &row) const
{
  return d_texts.at(row.row()).at(0).toString();
}


QModelIndex ProviderListModel::addProvider(int provider_id)
{
  //
  // Find the insertion offset
  //
  int offset=d_ids.size();

  beginInsertRows(QModelIndex(),offset,offset);
  d_ids.insert(offset,provider_id);
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


void ProviderListModel::removeProvider(const QModelIndex &row)
{
  beginRemoveRows(QModelIndex(),row.row(),row.row());

  d_ids.removeAt(row.row());
  d_texts.removeAt(row.row());
  d_icons.removeAt(row.row());

  endRemoveRows();
}


void ProviderListModel::removeProvider(int provider_id)
{
  for(int i=0;i<d_ids.size();i++) {
    if(d_ids.at(i)==provider_id) {
      removeProvider(createIndex(i,0));
      return;
    }
  }
}


void ProviderListModel::refresh(const QModelIndex &row)
{
  if(row.row()<d_texts.size()) {
    QString sql=sqlFields()+
      "where "+
      QString::asprintf("`PROVIDERS`.`ID`=%d ",d_ids.at(row.row()));
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    if(q->first()) {
      updateRow(row.row(),q);
      emit dataChanged(createIndex(row.row(),0),
		       createIndex(row.row(),columnCount()));
    }
    delete q;
  }
}


void ProviderListModel::refresh(int provider_id)
{
  for(int i=0;i<d_ids.size();i++) {
    if(d_ids.at(i)==provider_id) {
      updateRowLine(i);
      return;
    }
  }
}


void ProviderListModel::refresh()
{
  updateModel();
}


void ProviderListModel::updateModel()
{
  QList<QVariant> texts; 
  QList<QVariant> icons;

  DvtSqlQuery *q=NULL;
  QString sql=sqlFields()+
    "order by `PROVIDERS`.`BUSINESS_NAME` ";
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


void ProviderListModel::updateRowLine(int line)
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


void ProviderListModel::updateRow(int row,DvtSqlQuery *q)
{
  QList<QVariant> texts;

  d_icons[row]=QPixmap(user_xpm);

  // Business Name
  texts.push_back(q->value(1));

  // Contact Name
  texts.push_back(q->value(2));

  // Phone
  texts.push_back(DvtFormatPhoneNumber(q->value(3).toString()));

  // E-Mail
  texts.push_back(q->value(4));

  d_texts[row]=texts;
}


QString ProviderListModel::sqlFields() const
{
  QString sql=QString("select ")+
    "`PROVIDERS`.`ID`,"+             // 00
    "`PROVIDERS`.`BUSINESS_NAME`,"+  // 01
    "`PROVIDERS`.`CONTACT_NAME`,"+   // 02
    "`PROVIDERS`.`CONTACT_PHONE`,"+  // 03
    "`PROVIDERS`.`CONTACT_EMAIL` "+  // 04
    "from `PROVIDERS` ";

    return sql;
}
