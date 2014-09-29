// dvt_import.h
//
// A Batch Importer for Rivendell.
//
//   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: dvt_import.h,v 1.1 2008/01/29 16:55:40 fredg Exp $
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


#ifndef DVT_IMPORT_H
#define DVT_IMPORT_H

#include <list>

#include <qobject.h>
#include <qsqldatabase.h>
#include <qfileinfo.h>

#include <dvtconfig.h>
#include <dvtcmdswitch.h>

#define DVT_IMPORT_USAGE "--station-table|--program-table=<filename> \n\nImport a table from the RA Database.\n"


class MainObject : public QObject
{
 public:
  enum TableType {NoTable=0,StationsTable=1,ProgramsTable=2};
  MainObject(QObject *parent=0,const char *name=0);
  DvtConfig *import_config;
  DvtCmdSwitch *import_cmd;

 private:
  void ImportStationsTable(const QString &filename);
  void ImportProgramsTable(const QString &filename);
};


#endif  // DVT_IMPORT_H
