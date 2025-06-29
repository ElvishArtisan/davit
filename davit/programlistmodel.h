// programlistmodel.h
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

#ifndef PROGRAMLISTMODEL_H
#define PROGRAMLISTMODEL_H

#include <QAbstractTableModel>
#include <QFont>
#include <QList>
#include <QPalette>

#include <dvtdb.h>


class ProgramListModel : public QAbstractTableModel
{
  Q_OBJECT
 public:
  ProgramListModel(QObject *parent=0);
  ~ProgramListModel();
  QPalette palette();
  void setPalette(const QPalette &pal);
  void setFont(const QFont &font);
  void setAffiliateId(int id);
  int columnCount(const QModelIndex &parent=QModelIndex()) const;
  int rowCount(const QModelIndex &parent=QModelIndex()) const;
  QVariant headerData(int section,Qt::Orientation orient,
		      int role=Qt::DisplayRole) const;
  QVariant data(const QModelIndex &index,int role=Qt::DisplayRole) const;
  int programId(const QModelIndex &row) const;
  QString programName(const QModelIndex &row) const;
  QModelIndex addProgram(int program_id);
  void removeProgram(const QModelIndex &row);
  void removeProgram(int program_id);
  void refresh(const QModelIndex &row);
  void refresh(int program_id);
  void refresh();

 protected:
  void updateModel();
  void updateRowLine(int line);
  void updateRow(int row,DvtSqlQuery *q);
  QString sqlFields() const;

 private:
  QPalette d_palette;
  QFont d_font;
  QStringList d_service_names;
  int d_affiliate_id;
  QList<QVariant> d_headers;
  QList<QVariant> d_alignments;
  QList<int> d_ids;
  QList<QList<QVariant> > d_texts;
  QList<QVariant> d_icons;
};


#endif  // PROGRAMLISTMODEL_H
