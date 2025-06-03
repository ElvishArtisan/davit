// contactlistmodel.cpp
//
// Data model for Davit contacts
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

#include "contactlistmodel.h"

#include "../icons/user.xpm"

ContactListModel::ContactListModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  //
  // Column Attributes
  //
  unsigned left=Qt::AlignLeft|Qt::AlignVCenter;
  unsigned center=Qt::AlignCenter;
  //  unsigned right=Qt::AlignRight|Qt::AlignVCenter;

  d_headers.push_back(tr("Name"));
  d_alignments.push_back(left);

  d_headers.push_back(tr("Title"));
  d_alignments.push_back(left);

  d_headers.push_back(tr("Phone Number"));
  d_alignments.push_back(left);

  d_headers.push_back(tr("FAX Number"));
  d_alignments.push_back(left);

  d_headers.push_back(tr("E-Mail Address"));
  d_alignments.push_back(left);

  d_headers.push_back(tr("Affadavit"));
  d_alignments.push_back(center);

  d_headers.push_back(tr("Gen Mgr"));
  d_alignments.push_back(center);

  d_headers.push_back(tr("Locked"));
  d_alignments.push_back(center);
}


ContactListModel::~ContactListModel()
{
}


QPalette ContactListModel::palette()
{
  return d_palette;
}


void ContactListModel::setPalette(const QPalette &pal)
{
  d_palette=pal;
}


void ContactListModel::setFont(const QFont &font)
{
  d_font=font;
}


void ContactListModel::setAffiliateId(int id)
{
  d_affiliate_id=id;
  refresh();
}


int ContactListModel::columnCount(const QModelIndex &parent) const
{
  return d_headers.size();
}


int ContactListModel::rowCount(const QModelIndex &parent) const
{
  return d_texts.size();
}


QVariant ContactListModel::headerData(int section,Qt::Orientation orient,
				    int role) const
{
  if((orient==Qt::Horizontal)&&(role==Qt::DisplayRole)) {
    return d_headers.at(section);
  }
  return QVariant();
}


QVariant ContactListModel::data(const QModelIndex &index,int role) const
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


int ContactListModel::contactId(const QModelIndex &row) const
{
  return d_ids.at(row.row());
}


QString ContactListModel::contactName(const QModelIndex &row) const
{
  return d_texts.at(row.row()).at(0).toString();
}


QModelIndex ContactListModel::addContact(int contact_id)
{
  //
  // Find the insertion offset
  //
  int offset=d_ids.size();

  beginInsertRows(QModelIndex(),offset,offset);
  d_ids.insert(offset,contact_id);
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


void ContactListModel::removeContact(const QModelIndex &row)
{
  beginRemoveRows(QModelIndex(),row.row(),row.row());

  d_ids.removeAt(row.row());
  d_texts.removeAt(row.row());
  d_icons.removeAt(row.row());

  endRemoveRows();
}


void ContactListModel::removeContact(int contact_id)
{
  for(int i=0;i<d_ids.size();i++) {
    if(d_ids.at(i)==contact_id) {
      removeContact(createIndex(i,0));
      return;
    }
  }
}


void ContactListModel::refresh(const QModelIndex &row)
{
  if(row.row()<d_texts.size()) {
    QString sql=sqlFields()+
      "where "+
      QString::asprintf("`CONTACTS`.`ID`=%d ",d_ids.at(row.row()));
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    if(q->first()) {
      updateRow(row.row(),q);
      emit dataChanged(createIndex(row.row(),0),
		       createIndex(row.row(),columnCount()));
    }
    delete q;
  }
}


void ContactListModel::refresh(int contact_id)
{
  for(int i=0;i<d_ids.size();i++) {
    if(d_ids.at(i)==contact_id) {
      updateRowLine(i);
      return;
    }
  }
}


void ContactListModel::refresh()
{
  updateModel();
}


void ContactListModel::updateModel()
{
  QList<QVariant> texts; 
  QList<QVariant> icons;

  DvtSqlQuery *q=NULL;
  QString sql=sqlFields()+
    QString::asprintf("where `AFFILIATE_ID`=%d ",d_affiliate_id)+
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


void ContactListModel::updateRowLine(int line)
{
  if(line<d_texts.size()) {
    QString sql=sqlFields()+
      " where "+
      QString::asprintf("`ID`=%d ",d_ids.at(line));
    printf("UPDATEROWLINE line: %d SQL: %s\n",line,sql.toUtf8().constData());
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    if(q->first()) {
      updateRow(line,q);
    }
    delete q;
  }
}


void ContactListModel::updateRow(int row,DvtSqlQuery *q)
{
  QList<QVariant> texts;

  // Contact Name
  texts.push_back(q->value(1));
  d_icons[row]=QPixmap(user_xpm);

  // Title
  texts.push_back(q->value(2));

  // Phone
  texts.push_back(DvtFormatPhoneNumber(q->value(3).toString()));

  // FAX
  texts.push_back(DvtFormatPhoneNumber(q->value(4).toString()));

  // E-Mail
  texts.push_back(q->value(5));

  // Affadavit
  texts.push_back(q->value(6));

  // Program Director
  texts.push_back(q->value(7));

  // General Manager
  texts.push_back(q->value(8));

  // Locked
  texts.push_back(q->value(9));

  d_texts[row]=texts;
}


QString ContactListModel::sqlFields() const
{
  QString sql=QString("select ")+
    "`CONTACTS`.`ID`,"+                // 00
    "`CONTACTS`.`NAME`,"+              // 01
    "`CONTACTS`.`TITLE`,"+             // 02
    "`CONTACTS`.`PHONE`,"+             // 03
    "`CONTACTS`.`FAX`,"+               // 04
    "`CONTACTS`.`EMAIL`,"+             // 05
    "`CONTACTS`.`AFFIDAVIT`,"+         // 06
    "`CONTACTS`.`PROGRAM_DIRECTOR`,"+  // 07
    "`CONTACTS`.`GENERAL_MANAGER`,"+   // 08
    "`CONTACTS`.`LOCKED` "+            // 09
    "from `CONTACTS` ";

    return sql;
}
