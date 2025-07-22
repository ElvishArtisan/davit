// dvtstamp.h
//
// Utility for instantiating affiliate schedule data for Davit
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef DVTSTAMP_H
#define DVTSTAMP_H

#include <list>

#include <QObject>
#include <QFileInfo>
#include <QList>

#include <dvtconfig.h>
#include <dvtcmdswitch.h>

#define DVTSTAMP_USAGE "[--date-offset=<days>] [--for-date=<date>]\n\nGenerate per-affiliate program airing records for Davit.\n\n--date-offset=<days>\n     Add <days> days to the current date when calculating the date to use for\n     generating program airing records.  Default is 0.\n\n--for-date=<date>\n     Use the absolute date <date> [format YYYY-MM-DD] for generating program\n     airing records.  If the day part of the date is omitted [format YYYY-MM]\n     then records for the entire month will be created.  Default is to use\n     the current date.\n\n--for-affiliate-id=<affiliate-id>\n     Create records only for the specified affiliate.  Default is to create\n     records for all affiliates.\n\n"

class MainObject : public QObject
{
  Q_OBJECT;
 public:
  enum Command {ListCommand=0,CreateCommand=1,DeleteCommand=2,ClearCommand=3,
    LastCommand=4};
  MainObject(QObject *parent=0);
  DvtConfig *import_config;
  DvtCmdSwitch *import_cmd;
  static QString commandText(Command cmd);

 private:
  void Clear() const;
  void Create() const;
  void Delete() const;
  void List() const;
  void CreateSchedule(int affiliate_id,const QDate &date) const;
  QString FilterSql(const QDate &date,int offset,bool whole_month,
		    const QList<int> &affiliate_ids) const;
  bool ApplySql(const QString &sql) const;
  Command d_command;
  QDate d_filter_for_date;
  bool d_filter_whole_month;
  int d_filter_date_offset;
  bool d_use_numeric_ids;
  bool d_dry_run;
  QList<int> d_affiliate_ids;
};


#endif  // DVTSTAMP_H
