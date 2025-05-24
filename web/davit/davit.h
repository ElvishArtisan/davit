// davit.h
//
// Web-Based Affiliate Manager
//
//   (C) Copyright 2008-2025 Fred Gleason <fredg@paravelsystems.com>
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


#ifndef DAVIT_H
#define DAVIT_H

#include <QObject>
#include <QSqlDatabase>
#include <QHostAddress>

//
// CGI Values
//
#define DAVIT_POST_SIZE 1000000
#define DAVIT_COMMAND_LOGIN 0
#define DAVIT_COMMAND_LOGOUT 1
#define DAVIT_COMMAND_MONTH_PICKER 2
#define DAVIT_COMMAND_SERVE_PROGRAM_LIST 3
#define DAVIT_COMMAND_PROCESS_PROGRAMS 4
#define DVT_WEB_BACKGROUND_COLOR "#FFFFFF"
#define DVT_WEB_LINE_COLOR1 "#E0E0E0"
#define DVT_WEB_LINE_COLOR2 "#F0F0F0"

class MainObject : public QObject
{
 public:
  MainObject(QObject *parent=0);

 private:
  int AuthenticatePost();
  void ServeLogin();
  void ServeLogout();
  void ServeMonthPicker();
  void ServeProgramList();
  void ProcessPrograms();
  void SetContext(int cmd) const;
  bool GetContact(int affiliate_id,QString *name,QString *email,
		  QString *phone,QString *fax);
  QString TimeWidget(int id,const QString &name,const QTime &time) const;
  QTime GetTime(int id,const QString &name);
  void SendDiscrepancyAlert(int affiliate_id,QDate &date,int quan);
  DvtConfig *cast_config;
  char cast_post[DAVIT_POST_SIZE];
  int cast_affiliate_id;
  long int cast_session_id;
  QHostAddress cast_client_addr;
};


#endif  // DAVIT_H
