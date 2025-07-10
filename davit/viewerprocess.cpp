// viewerprocess.cpp
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

#include "viewerprocess.h"

ViewerProcess::ViewerProcess(int id,const QStringList &cmdline,QObject *parent)
  : QObject(parent)
{
  d_id=id;
  d_command_line=cmdline;

  d_process=new QProcess(this);
  connect(d_process,SIGNAL(finished(int,QProcess::ExitStatus)),
	  this,SLOT(finishedData(int,QProcess::ExitStatus)));
  connect(d_process,SIGNAL(errorOccurred(QProcess::ProcessError)),
	  this,SLOT(errorOccurredData(QProcess::ProcessError)));
}


ViewerProcess::~ViewerProcess()
{
  d_process->disconnect();
  d_process->kill();
  d_process->deleteLater();
}


QStringList ViewerProcess::commandLine() const
{
  return d_command_line;
}


void ViewerProcess::start()
{
  QStringList args=d_command_line;
  args.removeFirst();
  d_process->start(d_command_line.first(),args);
}


void ViewerProcess::terminate()
{
  d_process->terminate();
}


void ViewerProcess::finishedData(int exit_code,QProcess::ExitStatus status)
{
  emit finished(d_id,exit_code,status,
		QString::fromUtf8(d_process->readAllStandardOutput()));
}


void ViewerProcess::errorOccurredData(QProcess::ProcessError err)
{
  emit errorOccurred(d_id,err);
}
