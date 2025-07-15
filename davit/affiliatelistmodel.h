// affiliatelistmodel.h
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

#ifndef AFFILIATELISTMODEL_H
#define AFFILIATELISTMODEL_H

#include <QAbstractTableModel>
#include <QDate>
#include <QFont>
#include <QList>
#include <QPalette>

#include <dvtdb.h>

class AffiliateListModel : public QAbstractTableModel
{
  Q_OBJECT
 public:
  AffiliateListModel(QObject *parent=0);
  ~AffiliateListModel();
  QPalette palette();
  void setPalette(const QPalette &pal);
  void setFont(const QFont &font);
  int columnCount(const QModelIndex &parent=QModelIndex()) const;
  int rowCount(const QModelIndex &parent=QModelIndex()) const;
  QVariant headerData(int section,Qt::Orientation orient,
		      int role=Qt::DisplayRole) const;
  QVariant data(const QModelIndex &index,int role=Qt::DisplayRole) const;
  int affiliateId(const QModelIndex &row) const;
  QModelIndex addAffiliate(int affiliate_id);
  void removeAffiliate(const QModelIndex &row);
  void removeAffiliate(int affiliate_id);
  void refresh(const QModelIndex &row);
  void refresh(int affiliate_id);
  void refresh(bool affl_only,const QDate &affl_missing,
	       const QString &call_filter);

 signals:
  void rowCountChanged(int matches);

 protected:
  void updateModel();
  void updateRowLine(int line);
  void updateRow(int row,DvtSqlQuery *q);
  QString sqlFields() const;

 private:
  QPalette d_palette;
  QFont d_font;
  QStringList d_service_names;
  QList<QVariant> d_headers;
  QList<QVariant> d_alignments;
  QList<QList<QVariant> > d_texts;
  QList<QVariant> d_icons;
  QList<int> d_ids;
  bool d_affiliates_only;
  QDate d_affiliates_missing;
  QString d_call_filter;
};


#endif  // AFFILIATELISTMODEL_H
