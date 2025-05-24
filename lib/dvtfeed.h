// dvtfeed.h
//
// A Container Class for Davit Feeds
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
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

#ifndef DVTFEED_H
#define DVTFEED_H

#include <QDateTime>

class DvtFeed
{
 public:
  DvtFeed();
  QString name() const;
  void setName(const QString &name);
  QTime startTime() const;
  void setStartTime(const QTime &time);
  void setStartTime(const QString &time);
  int length() const;
  QTime endTime() const;
  void setLength(int len);
  void setLength(const QString &time);
  bool dowActive(int dow) const;
  void setDowActive(int dow,bool state);
  int addToAffiliate(int affiliate_id);
  int modifyAffiliate(int airing_id);
  void clear();
  
 private:
  QString slot_name;
  QTime slot_start_time;
  int slot_length;
  bool slot_dow_active[7];
};

#endif  // DVTFEED_H
