// userlistmodel.cpp
//
// Data model for Davit users
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

#include "userlistmodel.h"

#include "../icons/admin.xpm"
#include "../icons/user.xpm"

UserListModel::UserListModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  //
  // Column Attributes
  //
  unsigned left=Qt::AlignLeft|Qt::AlignVCenter;
  unsigned center=Qt::AlignCenter;
  //  unsigned right=Qt::AlignRight|Qt::AlignVCenter;

  d_headers.push_back(tr("User Name"));
  d_alignments.push_back(left);

  d_headers.push_back(tr("Full Name"));
  d_alignments.push_back(left);

  d_headers.push_back(tr("Description"));
  d_alignments.push_back(left);

  d_headers.push_back(tr("Phone Number"));
  d_alignments.push_back(left);

  d_headers.push_back(tr("E-Mail Address"));
  d_alignments.push_back(center);
}


UserListModel::~UserListModel()
{
}


QPalette UserListModel::palette()
{
  return d_palette;
}


void UserListModel::setPalette(const QPalette &pal)
{
  d_palette=pal;
}


void UserListModel::setFont(const QFont &font)
{
  d_font=font;
}


int UserListModel::columnCount(const QModelIndex &parent) const
{
  return d_headers.size();
}


int UserListModel::rowCount(const QModelIndex &parent) const
{
  return d_texts.size();
}


QVariant UserListModel::headerData(int section,Qt::Orientation orient,
				    int role) const
{
  if((orient==Qt::Horizontal)&&(role==Qt::DisplayRole)) {
    return d_headers.at(section);
  }
  return QVariant();
}


QVariant UserListModel::data(const QModelIndex &index,int role) const
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


QString UserListModel::userName(const QModelIndex &row) const
{
  return d_texts.at(row.row()).at(0).toString();
}


QModelIndex UserListModel::addUser(const QString &username)
{
  //
  // Find the insertion offset
  //
  int offset=d_texts.size();
  for(int i=0;i<d_texts.size();i++) {
    if(username<d_texts.at(i).at(0).toString()) {
      offset=i;
      break;
    }
  }

  beginInsertRows(QModelIndex(),offset,offset);
  QList<QVariant> list;
  for(int i=0;i<columnCount();i++) {
    list.push_back(QVariant());
  }
  list[0]=username;
  d_texts.insert(offset,list);
  d_icons.insert(offset,QVariant());
  updateRowLine(offset);
  endInsertRows();

  return createIndex(offset,0);
}


void UserListModel::removeUser(const QModelIndex &row)
{
  beginRemoveRows(QModelIndex(),row.row(),row.row());

  d_texts.removeAt(row.row());
  d_icons.removeAt(row.row());

  endRemoveRows();
}


void UserListModel::removeUser(const QString &username)
{
  for(int i=0;i<d_texts.size();i++) {
    if(d_texts.at(i).at(0)==username) {
      removeUser(createIndex(i,0));
      return;
    }
  }
}


void UserListModel::refresh(const QModelIndex &row)
{
  if(row.row()<d_texts.size()) {
    QString sql=sqlFields()+
      "where `LOGIN_NAME`="+
      DvtSqlQuery::escape(d_texts.at(row.row()).at(0).toString());
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    if(q->first()) {
      updateRow(row.row(),q);
      emit dataChanged(createIndex(row.row(),0),
		       createIndex(row.row(),columnCount()));
    }
    delete q;
  }
}


void UserListModel::refresh(const QString &username)
{
  for(int i=0;i<d_texts.size();i++) {
    if(d_texts.at(i).at(0)==username) {
      updateRowLine(i);
      return;
    }
  }
}


void UserListModel::refresh()
{
  updateModel();
}


void UserListModel::updateModel()
{
  QList<QVariant> texts; 
  QList<QVariant> icons;

  DvtSqlQuery *q=NULL;
  QString sql=sqlFields();
  sql+="order by `USER_NAME` ";
  beginResetModel();
  d_texts.clear();
  d_icons.clear();
  q=new DvtSqlQuery(sql);
  while(q->next()) {
    d_texts.push_back(texts);
    d_icons.push_back(icons);
    updateRow(d_texts.size()-1,q);
  }
  delete q;
  endResetModel();
}


void UserListModel::updateRowLine(int line)
{
  if(line<d_texts.size()) {
    QString sql=sqlFields()+
      "where `USER_NAME`="+DvtSqlQuery::escape(d_texts.at(line).at(0).toString());
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    if(q->first()) {
      updateRow(line,q);
    }
    delete q;
  }
}


void UserListModel::updateRow(int row,DvtSqlQuery *q)
{
  QList<QVariant> texts;

  // User Name
  texts.push_back(q->value(0));
  if(q->value(1).toString()=="Y") {
    d_icons[row]=QPixmap(admin_xpm);
  }
  else {
    d_icons[row]=QPixmap(user_xpm);
  }

  // Full Name
  texts.push_back(q->value(2));

  // Description
  texts.push_back(q->value(3));

  // E-Mail Address
  texts.push_back(q->value(4));

  // Phone Number
  texts.push_back(q->value(5));

  d_texts[row]=texts;
}


QString UserListModel::sqlFields() const
{
  QString sql=QString("select ")+
    "`USERS`.`USER_NAME`,"+         // 00
    "`USERS`.`ADMIN_PRIV`,"+        // 01
    "`USERS`.`FULL_NAME`,"+         // 03
    "`USERS`.`DESCRIPTION`,"+       // 04
    "`USERS`.`EMAIL`,"+             // 05
    "`USERS`.`PHONE_NUMBER` "+      // 06
    "from `USERS` ";

    return sql;
}
