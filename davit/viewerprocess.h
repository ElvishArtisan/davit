// viewerprocess.h
//
// Process container for a Davit report viewer
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

#ifndef VIEWERPROCESS_H
#define VIEWERPROCESS_H

#include <QObject>
#include <QProcess>
#include <QStringList>

class ViewerProcess : public QObject
{
  Q_OBJECT
  public:
  ViewerProcess(int id,const QStringList &cmdline,QObject *parent);
  ~ViewerProcess();
  QStringList commandLine() const;
  void start();
  void terminate();

 signals:
  void finished(int id,int exit_code,QProcess::ExitStatus status,
		const QString &err_msg);
  void errorOccurred(int id,QProcess::ProcessError err);
  
 private slots:
  void finishedData(int exit_code,QProcess::ExitStatus status);
  void errorOccurredData(QProcess::ProcessError err);

 private:
  int d_id;
  QStringList d_command_line;
  QProcess *d_process;
};


#endif // VIEWERPROCESS_H
