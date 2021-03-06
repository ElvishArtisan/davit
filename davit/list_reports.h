// list_reports.h
//
// List Davit Reports.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: list_reports.h,v 1.16 2014/01/02 19:49:13 pcvs Exp $
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

#ifndef LIST_REPORTS_H
#define LIST_REPORTS_H

#include <vector>

#include <qdialog.h>
#include <qfontmetrics.h>
#include <qpushbutton.h>
#include <qlistview.h>
#include <qstringlist.h>
#include <qdatetime.h>

#include <dvt.h>
#include <dvtlistviewitem.h>
#include <spread_sheet.h>

#include <pick_fields.h>

class ListReports : public QDialog
{
 Q_OBJECT
 public:
  ListReports(QWidget *parent=0,const char *name=0);
  ~ListReports();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void runData();
  void doubleClickedData(QListViewItem *item,const QPoint &pt,int c);
  void closeData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  enum ContactType {ProgramDirectorContact=0x01,AffidavitContact=0x02,
		    GeneralManagerContact=0x04};
  enum Fields {FieldName=0x01,FieldPhone=0x02,FieldFax=0x04,
	       FieldEmail=0x08};

  //
  // activity_report.cpp
  //
  bool ActivityReport(SpreadSheet *sheet);

  //
  // addedprograms_report.cpp
  //
  bool AddedProgramsReport(Dvt::RemarkType type,SpreadSheet *sheet);

  //
  // affidavit_report.cpp
  //
  bool AffidavitReport(SpreadSheet *sheet);

  //
  // missingaffidavit_report.cpp
  //
  bool MissingAffidavitReport(SpreadSheet *sheet);

  //
  // missingaffidavitcontact_report.cpp
  //
  bool MissingAffidavitContactReport(SpreadSheet *sheet);

  //
  // affiliatesbyprogram.cpp
  //
  bool AllAffiliatesReport(SpreadSheet *sheet);
  bool AllAffiliateContacts(SpreadSheet *sheet);
  bool AffiliatesByNetworkReport(SpreadSheet *sheet);
  bool AffiliatesByProgramReport(int contacts,SpreadSheet *sheet);
  bool AffiliatesByDaypartReport(SpreadSheet *sheet);
  bool RenderAffiliateReport(SpreadTab *tab,const QString &where,
			     const QString &title,const QString &sub,
			     bool show_program_name,int contacts);

  //
  // arbitron_report.cpp
  //
  bool ArbitronReport(SpreadSheet *sheet);

  //
  // affiliatesbymarket_report.cpp
  //
  bool AffiliatesByMarketReport(PickFields::MarketType type,SpreadSheet *sheet);

  //
  // programbymarket_report.cpp
  //
  bool ProgramByMarketReport(PickFields::MarketType type,SpreadSheet *sheet);

  //
  // raaffiliate_report.cpp
  //
  bool RAAffiliateReport(SpreadSheet *sheet);

  void ForkViewer(const QString &filename,const QString &data="");
  void ContactFields(int affiliate_id,ContactType type,int fields,
		     SpreadTab *tab,int colnum,int rownum,
		     QFontMetrics *fm=NULL);
  QListView *list_reports_list;
  QPushButton *list_run_button;
  QPushButton *list_close_button;
};


#endif  // LIST_REPORTS_H
