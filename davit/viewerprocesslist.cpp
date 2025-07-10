// viewerprocesslist.cpp
//
// Viewer process manager for Davit
//
//   (C) Copyright 2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <QFile>

#include "viewerprocesslist.h"

ViewerProcessList::ViewerProcessList(QObject *parent)
  : QObject(parent)
{
  d_next_id=1;

  d_cleanup_timer=new QTimer(this);
  d_cleanup_timer->setSingleShot(true);
  connect(d_cleanup_timer,SIGNAL(timeout()),this,SLOT(cleanupTimerData()));
}


ViewerProcessList::~ViewerProcessList()
{
  for(QMap<int,ViewerProcess *>::const_iterator it=d_processes.begin();
      it!=d_processes.end();it++) {
    delete it.value();
  }
  for(int i=0;i<d_delete_list.size();i++) {
    QFile::remove(d_delete_list.at(i));
  }
}


int ViewerProcessList::start(const QStringList &cmdline)
{
  ViewerProcess *proc=new ViewerProcess(d_next_id,cmdline,this);
  connect(proc,SIGNAL(finished(int,int,QProcess::ExitStatus,const QString &)),
	  this,
	  SLOT(finishedData(int,int,QProcess::ExitStatus,const QString &)));
  connect(proc,SIGNAL(errorOccurred(int,QProcess::ProcessError)),
	  this,SLOT(errorOccurredData(int,QProcess::ProcessError)));
  d_delete_list.push_back(cmdline.last());
  d_processes[d_next_id]=proc;
  proc->start();
  emit reportStartupComplete();
  return d_next_id++;
}


void ViewerProcessList::deleteOnExit(const QString &pathname)
{
  d_delete_list.push_back(pathname);
}


void ViewerProcessList::cleanup()
{
  if(d_processes.size()==0) {
    for(int i=0;i<d_delete_list.size();i++) {
      QFile::remove(d_delete_list.at(i));
    }
    emit cleanupComplete();
  }
  else {
    for(QMap<int,ViewerProcess *>::const_iterator it=d_processes.begin();
	it!=d_processes.end();it++) {
      it.value()->terminate();
    }
    d_cleanup_timer->start(VIEWERPROCESSLIST_CLEANUP_TIMEOUT);
  }
}


void ViewerProcessList::finishedData(int id,int exit_code,
				     QProcess::ExitStatus status,
				     const QString &err_msg)
{
  QString cmdline=d_processes.value(id)->commandLine().join(" ");

  if(status!=QProcess::NormalExit) {
    fprintf(stderr,"viewer process \"%s\"crashed\n",
	    cmdline.toUtf8().constData());
  }
  else {
    if(exit_code!=0) {
      fprintf(stderr,"viewer process \"%s\" returned error: %s\n",
	      cmdline.toUtf8().constData(),
	      err_msg.toUtf8().constData());
    }
  }
  d_processes.value(id)->deleteLater();
  d_processes.remove(id);

  if(d_cleanup_timer->isActive()) {
    if(d_processes.size()==0) {
      for(int i=0;i<d_delete_list.size();i++) {
	QFile::remove(d_delete_list.at(i));
      }
      emit cleanupComplete();
    }
  }
}


void ViewerProcessList::errorOccurredData(int id,QProcess::ProcessError err)
{
  fprintf(stderr,"viewer process \"%s\" returned error %d\n",
	  d_processes.value(id)->commandLine().join(" ").toUtf8().constData(),
	  err);
}


void ViewerProcessList::cleanupTimerData()
{
  for(int i=0;i<d_delete_list.size();i++) {
    QFile::remove(d_delete_list.at(i));
  }
  for(QMap<int,ViewerProcess *>::const_iterator it=d_processes.begin();
      it!=d_processes.end();it++) {
    delete it.value();
  }
  emit cleanupComplete();
}
