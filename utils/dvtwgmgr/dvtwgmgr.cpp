// dvtwgmgr.cpp
//
// Davit helper for managing wireguard tunnels
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <QCoreApplication>
#include <QProcess>

#include <dvtcmdswitch.h>

#include "dvtwgmgr.h"

MainObject::MainObject(QObject *parent)
  : QObject(parent)
{
  d_up=false;
  d_down=false;

  DvtCmdSwitch *cmd=new DvtCmdSwitch("dvtwgmgr",DVTWGMGR_USAGE);
  for(int i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--down") {
      d_down=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--up") {
      d_up=true;
      cmd->setProcessed(i,true);
    }
    if(!cmd->processed(i)) {
      fprintf(stderr,"dvtwgmgr: unrecognized option \"%s\"\n",
	      cmd->key(i).toUtf8().constData());
      exit(1);
    }
  }
  if((!d_down)&&(!d_up)) {
    fprintf(stderr,"\"--down\" or \"--up\" must be specified\n");
    exit(1);
  }
  if(d_down&&d_up) {
    fprintf(stderr,"\"--down\" or \"--up\" are mutually exclusive\n");
    exit(1);
  }

  //
  // Load Configuration
  //
  d_config=new DvtConfig();
  d_config->load();

  if(d_up) {
    d_wireguard_configurations=d_config->wireguardConfigurations();
    for(int i=0;i<d_wireguard_configurations.size();i++) {
      StartTunnel(d_wireguard_configurations.at(i));
    }
  }
  if(d_down) {
    d_wireguard_configurations=d_config->wireguardConfigurations();
    for(int i=0;i<d_wireguard_configurations.size();i++) {
      StopTunnel(d_wireguard_configurations.at(i));
    }
  }
  exit(0);
}


void MainObject::StartTunnel(const QString &config)
{
  QStringList args;

  args.push_back("up");
  args.push_back(config);
  QProcess *proc=new QProcess(this);
  proc->start("wg-quick",args);
  proc->waitForFinished();
  if(proc->exitStatus()!=QProcess::NormalExit) {
    fprintf(stderr,"dvtwgmgr: wireguard configuration \"%s\" startup crashed\n",
	    config.toUtf8().constData());
  }
  else {
    if(proc->exitCode()!=0) {
      fprintf(stderr,
	      "dvtwgmgr: wireguard configuration \"%s\" startup returned error %d [%s]\n",
	      config.toUtf8().constData(),proc->exitCode(),
	      proc->readAllStandardError().constData());
    }
  }
  delete proc;
}


void MainObject::StopTunnel(const QString &config)
{
    QStringList args;

    args.push_back("down");
    args.push_back(config);
    QProcess *proc=new QProcess();
    proc->start("wg-quick",args);
    proc->waitForFinished();
    if(proc->exitStatus()!=QProcess::NormalExit) {
      fprintf(stderr,
	      "dvtwgmgr: wireguard configuration \"%s\" shutdown crashed\n",
	      config.toUtf8().constData());
    }
    else {
      if(proc->exitCode()!=0) {
	fprintf(stderr,
		"dvtwgmgr: wireguard configuration \"%s\" startup returned error %d [%s]\n",
		config.toUtf8().constData(),
		proc->exitCode(),
		proc->readAllStandardError().constData());
    }
  }
  delete proc;
}


int main(int argc,char *argv[])
{
  //
  // Check for root permissions
  //
  if(geteuid()!=0) {
    fprintf(stderr,"dvtwgmgr: this program must be run as root\n");
    exit(1);
  }
  if(setuid(0)!=0) {
    fprintf(stderr,"dvtwgmgr: setuid() failed [%s]\n",strerror(errno));
    exit(1);
  }

  QCoreApplication a(argc,argv);

  new MainObject();

  return a.exec();
}
