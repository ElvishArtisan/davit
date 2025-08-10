// dvtdbmgr.h
//
// A Database Check/Repair Tool for Davit
//
//   (C) Copyright 2011-2025 Fred Gleason <fredg@paravelsystems.com>
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


#ifndef DVTDBMGR_H
#define DVTDBMGR_H

#include <list>

#include <QObject>
#include <QSqlDatabase>

#include <dvtconfig.h>
#include <dvtcmdswitch.h>

#define DVTDBMGR_USAGE "[options]\n\n"

class MainObject : public QObject
{
  Q_OBJECT;
 public:
  enum Command {NoCommand=0,ModifyCommand=1,CreateCommand=2,CheckCommand=3};
  MainObject();

 private:
  void Check() const;                                    // check.cpp
  void Create() const;                                   // create.cpp
  bool CreateDb() const;                                 // create.cpp
  bool InitDb() const;                                   // create.cpp
  void Modify() const;                                   // modify.cpp
  void CheckDuplicateAffiliates() const;                 // check.cpp
  void MergeAffiliates(int dest_id,int src_id) const;    // check.cpp
  int GetCurrentSchema() const;                          // dvtdbmgr.cpp
  void PrintStatus() const;                              // dvtdbmgr.cpp
  bool DbIsEmpty() const;                                // dvtdbmgr.cpp
  bool UserResponse() const;                             // dvtdbmgr.cpp
  Command d_command;
  bool d_yes;
  bool d_no;
  QString db_mysql_hostname;
  QString db_mysql_loginname;
  QString db_mysql_password;
  QString db_mysql_database;
  QString db_mysql_servertype;
  DvtConfig *dvtconfig;
};


#endif  // DVTDBMGR_H
