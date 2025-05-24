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
#include <QSqlDatabase>
#include <QFileInfo>

#include <dvtconfig.h>
#include <dvtcmdswitch.h>

#define DVTSTAMP_USAGE "[--date-offset=<days>] [--for-date=<date>]\n\nGenerate per-affiliate program airing records for Davit.\n\n--date-offset=<days>\n     Add <days> days to the current date when calculating the date to use for\n     generating program airing records.  Default is 0.\n\n--for-date=<date>\n     Use the absolute date <date> [format YYYY-MM-DD] for generating program\n     airing records.  If the day part of the date is omitted [format YYYY-MM]\n     then records for the entire month will be created.  Default is to use\n     the current date.\n\n--for-affiliate-id=<affiliate-id>\n     Create records only for the specified affiliate.  Default is to create\n     records for all affiliates.\n\n"

class MainObject : public QObject
{
 public:
  MainObject(QObject *parent=0);
  DvtConfig *import_config;
  DvtCmdSwitch *import_cmd;

 private:
  void GenerateSchedule(int affiliate_id,const QDate &date);
};


#endif  // DVTSTAMP_H
