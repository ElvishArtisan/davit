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
  void ActivityReport();
  void AddedProgramsReport(Dvt::RemarkType type);
  void AffidavitReport();

  //
  // missingaffidavit_report.cpp
  //
  void MissingAffidavitReport();

  //
  // missingaffidavitcontact_report.cpp
  //
  void MissingAffidavitContactReport();

  //
  // affiliatesbyprogram.cpp
  //
  void AllAffiliatesReport();
  void AllAffiliateContacts();
  void AffiliatesByNetworkReport();
  void AffiliatesByProgramReport(int contacts);
  void AffiliatesByDaypartReport();
  void RenderAffiliateReport(FILE *f,const QString &where,
			     const QString &title,const QString &sub,
			     bool show_program_name,int contacts);

  //
  // arbitron_report.cpp
  //
  void ArbitronReport();

  //
  // affiliatesbymarket_report.cpp
  //
  void AffiliatesByMarketReport(PickFields::MarketType type);

  //
  // programbymarket_report.cpp
  //
  void ProgramByMarketReport(PickFields::MarketType type);

  //
  // raaffiliate_report.cpp
  //
  void RAAffiliateReport();
  QString GetEmailContactList(int affiliate_id,bool affidavit,bool progdir);
  QString GetNameContactList(int affiliate_id,bool affidavit,bool progdir);
  QString GetPhoneNumberContactList(int affiliate_id,bool affidavit,
				    bool progdir);
  FILE *GetTempFile(QString *filename);
  QString GetTempDir();
  void ForkViewer(const QString &filename,const QString &data="");
  QString StringField(const QVariant &v);
  QString PhoneField(const QVariant &v);
  QString PhoneString(const QString &pnum);
  QString AddressField(const QVariant &v1,const QVariant &v2);
  QString FrequencyField(const QVariant &v);
  QString TypeField(const QVariant &v);
  QString ContactFields(int affiliate_id,ContactType type,int fields);
  void ContactFields(int affiliate_id,ContactType type,int fields,
		     SpreadTab *tab,int colnum,int rownum,
		     QFontMetrics *fm=NULL);
  QString EmptyField();
  QListView *list_reports_list;
  QPushButton *list_run_button;
  QPushButton *list_close_button;
};


#endif  // LIST_REPORTS_H
