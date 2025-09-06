// dvtinstancemonitor.cpp
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

#include <stdio.h>
#include <unistd.h>

#include <QTextStream>

#include "dvtinstancemonitor.h"

DvtInstance::DvtInstance(pid_t pid)
{
  FILE *f=NULL;
  

  if((f=fopen(QString::asprintf("/proc/%u/cmdline",pid).toUtf8(),"r"))==NULL) {
    d_process_id=0;
    return;
  }
  d_process_id=pid;
  QTextStream *strm=new QTextStream(f,QIODeviceBase::ReadOnly);
  QStringList f0=strm->readAll().split(" ",Qt::SkipEmptyParts);
  if(f0.size()>0) {
    QStringList f1=f0.first().split("/",Qt::SkipEmptyParts);
    d_command=f1.last();
  }
  fclose(f);
  delete strm;
}


QString DvtInstance::command() const
{
  return d_command;
}


pid_t DvtInstance::processId() const
{
  return d_process_id;
}


QString DvtInstance::dump() const
{
  return QString::asprintf("processId: %u\n",d_process_id)+d_command;
}




DvtInstanceMonitor::DvtInstanceMonitor()
{
  d_process_id=getpid();
  DvtInstance *inst=new DvtInstance(d_process_id);
  d_command=inst->command();
  delete inst;

  d_proc_dir=new QDir("/proc");
}


DvtInstanceMonitor::~DvtInstanceMonitor()
{
  delete d_proc_dir;
}


QString DvtInstanceMonitor::command() const
{
  return d_command;
}


pid_t DvtInstanceMonitor::processId() const
{
  return d_process_id;
}


QList<pid_t> DvtInstanceMonitor::processIds(bool incl_self) const
{
  QList<pid_t> ret;
  QStringList procdirs=d_proc_dir->entryList(QDir::Dirs|QDir::NoDotAndDotDot);
  for(int i=0;i<procdirs.size();i++) {
    bool ok=false;
    pid_t pid=procdirs.at(i).toInt(&ok);
    if(ok) {
      DvtInstance *inst=new DvtInstance(pid);
      if(inst->command()==d_command) {
	if(incl_self||(pid!=d_process_id)) {
	  ret.push_back(pid);
	}
      }
      delete inst;
    }
  }

  return ret;
}
