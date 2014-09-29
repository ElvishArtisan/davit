// davit.h
//
// The Davit Afilliat Manager Interface
//
//   (C) Copyright 2002-2008 Fred Gleason <fredg@paravelsystems.com>
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

#include <qwidget.h>
#include <qsize.h>
#include <qsizepolicy.h>
#include <qsqldatabase.h>
#include <qstringlist.h>

class MainWidget : public QWidget
{
  Q_OBJECT
 public:
  MainWidget(QWidget *parent=0,const char *name=0);
  ~MainWidget();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 private slots:
  void manageUsersData();
  void manageSystemData();
  void manageProvidersData();
  void manageAffiliateData();
  void manageProgramsData();
  void importExternalData();  // precisiontrak.cpp
  //  void importBiaData();
  void manageNetworksData();
  void generateReportsData();
  void quitMainWidget();

 protected:
  void closeEvent(QCloseEvent *e);

 private:
  void ProcessMarketRecord(const QString &table,QString &name,int rank);
  int GetMarketRank(const QString &table,const QString &name,int rank);
  void ImportPrecisionTrakRecord(const QStringList &cols);
};


#endif  // DAVIT_H
