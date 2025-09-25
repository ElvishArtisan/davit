// signalmonitor.h
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

#ifndef SIGNALMONITOR_H
#define SIGNALMONITOR_H

#include <QList>
#include <QLocalSocket>
#include <QObject>

class SignalMonitor : public QObject
{
 Q_OBJECT
 public:
 SignalMonitor(QObject *parent=0);
  ~SignalMonitor();
  QList<int> monitoredSignals() const;
  void addSignal(int signum);
  void removeSignal(int signum);

 signals:
  void receivedSignal(int signo);

 private slots:
  void readyReadData();
   
 private:
  QList<int> d_signals;
  QLocalSocket *d_notify_socket;
};


#endif  // SIGNALMONITOR_H
