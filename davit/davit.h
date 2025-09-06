// davit.h
//
// The Davit Afilliat Manager Interface
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <QProcess>
#include <QSize>
#include <QSizePolicy>
#include <QSqlDatabase>
#include <QStringList>
#include <QWidget>

#include <dvtinstancemonitor.h>

#include "edit_system.h"
#include "list_affiliates.h"
#include "list_networks.h"
#include "list_programs.h"
#include "list_providers.h"
#include "list_reports.h"
#include "list_users.h"
#include "login.h"

#define DAVIT_USAGE "[--email-dry-run]\n     Don't actually send e-mail, but print it to STDOUT\n\n"

class MainWidget : public QWidget
{
  Q_OBJECT
 public:
  MainWidget(QWidget *parent=0);
  ~MainWidget();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 private slots:
  void manageUsersData();
  void manageSystemData();
  void manageProvidersData();
  void manageAffiliateData();
  void importExternalData();  // precisiontrak.cpp
  void manageNetworksData();
  void generateReportsData();
  void quitMainWidget();

 protected:
  void closeEvent(QCloseEvent *e);

 private:
  void ProcessMarketRecord(const QString &table,QString &name,int rank);
  int GetMarketRank(const QString &table,const QString &name,int rank);
  void ImportPrecisionTrakRecord(const QStringList &cols);
  void VerifyPrecisionTrakRecord(const QString &str1,const QString &str2,
				 QStringList &errs);
  QString GetNextLine(FILE *f);
  bool OpenDb(QString *err_msg,const QString &dbname,const QString &login,
	      const QString &pwd,const QString &host,
	      const QString &srv_type) const;
  void WireguardTunnels(bool start_up);
  void CleanExit(int exit_code);
  Login *d_login_dialog;
  ListUsers *d_users_dialog;
  ListAffiliates *d_affiliates_dialog;
  ListNetworks *d_networks_dialog;
  ListProviders *d_providers_dialog;
  ListReports *d_reports_dialog;
  EditSystem *d_system_dialog;
  DvtInstanceMonitor *d_instance_monitor;
  QProcess *d_wireguard_process;
};


#endif  // DAVIT_H
