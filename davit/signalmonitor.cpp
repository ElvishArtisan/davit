// signalmonitor.cpp
//
// Catch UNIX-style signals in a thread-safe manner
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

#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "signalmonitor.h"

int __send_sock=-1;
void __SignalMonitor_SignalHandler(int signum)
{
  static char data[4];

  data[0]=(0xff&signum);  // Little Endian!
  data[1]=(0xff&(signum>>8));
  data[2]=(0xff&(signum>>16));
  data[3]=(0xff&(signum>>24));
  send(__send_sock,data,4,0);
}


SignalMonitor::SignalMonitor(QObject *parent)
  : QObject(parent)
{
  int socks[2];

  if(socketpair(AF_UNIX,SOCK_STREAM,0,socks)!=0) {
    fprintf(stderr,"SignalMonitor setup failed [%s]\n",strerror(errno));
    exit(1);
  }
  __send_sock=socks[0];
  d_notify_socket=new QLocalSocket(this);
  connect(d_notify_socket,SIGNAL(readyRead()),this,SLOT(readyReadData()));
  d_notify_socket->setSocketDescriptor(socks[1]);
}


SignalMonitor::~SignalMonitor()
{
  for(int i=0;i<d_signals.size();i++) {
    ::signal(d_signals.at(i),SIG_DFL);
  }
}


QList<int> SignalMonitor::monitoredSignals() const
{
  return d_signals;
}


void SignalMonitor::addSignal(int signum)
{
  if(!d_signals.contains(signum)) {
    struct sigaction sigact;
    sigact.sa_handler=__SignalMonitor_SignalHandler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags=0;
    sigaction(signum,&sigact,NULL);
    d_signals.push_back(signum);
  }
}


void SignalMonitor::removeSignal(int signum)
{
  if(d_signals.contains(signum)) {
    struct sigaction sigact;
    sigact.sa_handler=SIG_DFL;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags=0;
    sigaction(signum,&sigact,NULL);
    d_signals.removeOne(signum);
  }
}


void SignalMonitor::readyReadData()
{
  QByteArray data=d_notify_socket->read(4);
  if(data.size()==4) {
    int signum=(int)((0xff&data[0])+  // Little Endian!
		     ((0xff&data[1])<<8)+
		     ((0xff&data[2])<<16)+
		     ((0xff&data[3])<<24));
    if(d_signals.contains(signum)) {
      ::signal(signum,__SignalMonitor_SignalHandler);
      emit receivedSignal(signum);
    }
  }
  else {
    fprintf(stderr,"SignalMonitor: signal channel threw an error\n");
  }
}
