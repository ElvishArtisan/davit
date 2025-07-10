// viewerprocesslist.h
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

#ifndef VIEWERPROCESSLIST_H
#define VIEWERPROCESSLIST_H

#include <QMap>
#include <QObject>
#include <QTimer>

#include "viewerprocess.h"

#define VIEWERPROCESSLIST_CLEANUP_TIMEOUT 5000

class ViewerProcessList : public QObject
{
  Q_OBJECT
  public:
  ViewerProcessList(QObject *parent);
  ~ViewerProcessList();
  int start(const QStringList &cmdline);
  void deleteOnExit(const QString &pathname);

 signals:
  void reportStartupComplete();
  void cleanupComplete();

 public slots:
  void cleanup();

 private slots:
  void finishedData(int id,int exit_code,QProcess::ExitStatus status,
		    const QString &err_msg);
  void errorOccurredData(int id,QProcess::ProcessError err);
  void cleanupTimerData();

 private:
  QMap<int,ViewerProcess *> d_processes;
  QStringList d_delete_list;
  int d_next_id;
  QTimer *d_cleanup_timer;
};


#endif // VIEWERPROCESS_H
