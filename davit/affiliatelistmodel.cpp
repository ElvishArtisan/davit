// affiliatelistmodel.cpp
//
// Data model for Davit affiliates
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

#include "affiliatelistmodel.h"

#include "../icons/greenball.xpm"
#include "../icons/redball.xpm"
#include "../icons/whiteball.xpm"

AffiliateListModel::AffiliateListModel(QObject *parent)
  : QAbstractTableModel(parent)
{
  d_affiliates_only=false;
  d_call_filter="";

  //
  // Column Attributes
  //
  unsigned left=Qt::AlignLeft|Qt::AlignVCenter;
  //  unsigned center=Qt::AlignCenter;
  //  unsigned right=Qt::AlignRight|Qt::AlignVCenter;

  d_headers.push_back(tr("Call"));
  d_alignments.push_back(left);

  d_headers.push_back(tr("City of License"));
  d_alignments.push_back(left);

  d_headers.push_back(tr("Type"));
  d_alignments.push_back(left);

  d_headers.push_back(tr("DMA"));
  d_alignments.push_back(left);

  d_headers.push_back(tr("Business Name"));
  d_alignments.push_back(left);
}


AffiliateListModel::~AffiliateListModel()
{
}


QPalette AffiliateListModel::palette()
{
  return d_palette;
}


void AffiliateListModel::setPalette(const QPalette &pal)
{
  d_palette=pal;
}


void AffiliateListModel::setFont(const QFont &font)
{
  d_font=font;
}


int AffiliateListModel::columnCount(const QModelIndex &parent) const
{
  return d_headers.size();
}


int AffiliateListModel::rowCount(const QModelIndex &parent) const
{
  return d_texts.size();
}


QVariant AffiliateListModel::headerData(int section,Qt::Orientation orient,
				    int role) const
{
  if((orient==Qt::Horizontal)&&(role==Qt::DisplayRole)) {
    return d_headers.at(section);
  }
  return QVariant();
}


QVariant AffiliateListModel::data(const QModelIndex &index,int role) const
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


int AffiliateListModel::affiliateId(const QModelIndex &row) const
{
  return d_ids.at(row.row());
}


QModelIndex AffiliateListModel::addAffiliate(int affiliate_id)
{
  //
  // Find the insertion offset
  //
  int offset=d_texts.size();
  for(int i=0;i<d_texts.size();i++) {
    if(affiliate_id<d_ids.at(i)) {
      offset=i;
      break;
    }
  }

  beginInsertRows(QModelIndex(),offset,offset);
  QList<QVariant> list;
  for(int i=0;i<columnCount();i++) {
    list.push_back(QVariant());
  }
  d_ids.push_back(affiliate_id);
  d_texts.insert(offset,list);
  d_icons.insert(offset,QVariant());
  updateRowLine(offset);
  endInsertRows();

  return createIndex(offset,0);
}


void AffiliateListModel::removeAffiliate(const QModelIndex &row)
{
  beginRemoveRows(QModelIndex(),row.row(),row.row());

  d_ids.removeAt(row.row());
  d_texts.removeAt(row.row());
  d_icons.removeAt(row.row());

  endRemoveRows();
}


void AffiliateListModel::removeAffiliate(int affiliate_id)
{
  for(int i=0;i<d_texts.size();i++) {
    if(d_ids.at(i)==affiliate_id) {
      removeAffiliate(createIndex(i,0));
      return;
    }
  }
}


void AffiliateListModel::refresh(const QModelIndex &row)
{
  if(row.row()<d_texts.size()) {
    QString sql=sqlFields()+
      "from `AFFILIATES` "+
      "where `AFFILIATES`.`ID`="+QString::asprintf("%d",d_ids.at(row.row()));
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    if(q->first()) {
      updateRow(row.row(),q);
      emit dataChanged(createIndex(row.row(),0),
		       createIndex(row.row(),columnCount()));
    }
    delete q;
  }
}


void AffiliateListModel::refresh(int affiliate_id)
{
  for(int i=0;i<d_texts.size();i++) {
    if(d_ids.at(i)==affiliate_id) {
      updateRowLine(i);
      return;
    }
  }
}


void AffiliateListModel::refresh(bool affl_only,const QDate &affl_missing,
				 const QString &call_filter)
{
  d_affiliates_only=affl_only;
  d_affiliates_missing=affl_missing;
  d_call_filter=call_filter;

  updateModel();
}


void AffiliateListModel::updateModel()
{
  QList<QVariant> texts; 
  QList<QVariant> icons;
  QString sql;
  QString where;

  int prev_rows=rowCount();
  if(d_affiliates_only) {
    where+=QString("(`AFFILIATES`.`IS_AFFILIATE`='Y')&&");
  }
  if(!d_affiliates_missing.isNull()) {
    sql=QString("select ")+
      "`AFFILIATES`.`ID` "+  // 00
      "from `AFFILIATES` where `AFFILIATES`.`AFFIDAVIT_ACTIVE`='Y'";
    DvtSqlQuery *q=new DvtSqlQuery(sql);
    while(q->next()) {
      if(!DvtAffidavitNeeded(q->value(0).toInt(),d_affiliates_missing)) {
	where+=QString::asprintf("(ID!=%d)&&",q->value(0).toInt());
      }
    }
    delete q;
  }
  if(!d_call_filter.isEmpty()) {
    where+="(`AFFILIATES`.`STATION_CALL` like "+
      DvtSqlQuery::escape(d_call_filter+"%")+")&&";
  }
  where=where.left(where.length()-2);
  DvtSqlQuery *q=NULL;
  sql=sqlFields();
  sql+="from `AFFILIATES` ";
  if(!where.isEmpty()) {
    sql+="where "+where;
  }
  sql+="order by `AFFILIATES`.`STATION_CALL` ";
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
  if(prev_rows!=rowCount()) {
    emit rowCountChanged(rowCount());
  }
}


void AffiliateListModel::updateRowLine(int line)
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


void AffiliateListModel::updateRow(int row,DvtSqlQuery *q)
{
  QList<QVariant> texts;

  // Call
  if((q->value(5).toString()=="A")||(q->value(5).toString()=="F")) {
    texts.push_back(q->value(1).toString()+"-"+q->value(5).toString()+"M");
  }
  else {
    texts.push_back(q->value(1));
  }
  if(q->value(8).toString()=="Y") {
    if(q->value(9).toString()=="Y") {
      d_icons[row]=QPixmap(greenball_xpm);
    }
    else {
      d_icons[row]=QPixmap(redball_xpm);
    }
  }
  else {
    d_icons[row]=QPixmap(whiteball_xpm);
  }

  // City of License
  if(q->value(3).toString().isEmpty()) {
    texts.push_back(q->value(2).toString());
  }
  else {
    texts.push_back(q->value(2).toString()+", "+
		    q->value(3).toString().toUpper());
  }

  // Type
  if(q->value(5).toString()=="F") {
    texts.push_back(tr("FM"));
  }
  else {
    if(q->value(5).toString()=="A") {
      texts.push_back(tr("AM"));
    }
    else {
      texts.push_back(tr("Internet"));
    }
  }    

  // DMA
  texts.push_back(q->value(7));

  // Business Name
  texts.push_back(q->value(6));

  d_texts[row]=texts;
}


QString AffiliateListModel::sqlFields() const
{
  return QString("select ")+
    "`AFFILIATES`.`ID`,"+                // 00
    "`AFFILIATES`.`STATION_CALL`,"+      // 01
    "`AFFILIATES`.`LICENSE_CITY`,"+      // 02
    "`AFFILIATES`.`LICENSE_STATE`,"+     // 03
    "`AFFILIATES`.`LICENSE_COUNTRY`,"+   // 04
    "`AFFILIATES`.`STATION_TYPE`,"+      // 05
    "`AFFILIATES`.`BUSINESS_NAME`,"+     // 06
    "`AFFILIATES`.`DMA_NAME`,"+          // 07
    "`AFFILIATES`.`IS_AFFILIATE`,"+      // 08
    "`AFFILIATES`.`AFFIDAVIT_ACTIVE` ";  // 09
  /*
                         from AFFILIATES %s \
                         order by AFFILIATES.STATION_CALL desc",
			(const char *)FilterSql(list_callfilter_edit->text(),
						list_filter_box->isChecked(),
						affidavit_date));
  */
}
