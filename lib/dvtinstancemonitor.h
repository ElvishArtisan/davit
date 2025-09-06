// dvtinstancemonitor.h
//
// Monitor instances of a binary
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef DVTINSTANCEMONITOR_H
#define DVTINSTANCEMONITOR_H

#include <sys/types.h>

#include <QDir>

#include <QList>
#include <QString>

class DvtInstance
{
 public:
  DvtInstance(pid_t pid);
  QString command() const;
  pid_t processId() const;
  QString dump() const;

 private:
  QString d_command;
  pid_t d_process_id;
};




class DvtInstanceMonitor
{
 public:
  DvtInstanceMonitor();
  ~DvtInstanceMonitor();
  QString command() const;
  pid_t processId() const;
  QList<pid_t> processIds(bool incl_self=false) const;

 private:
  QString d_command;
  pid_t d_process_id;
  QDir *d_proc_dir;
};


#endif  // DVTINSTANCEMONITOR_H
