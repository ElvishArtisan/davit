// list_reports.cpp
//
// List Davit Reports.
//
//   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: list_reports.cpp,v 1.20 2014/01/02 19:49:13 pcvs Exp $
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

#ifndef PATH_MAX
#define PATH_MAX 256
#endif  // PATH_MAX

#include <stdlib.h>
#include <errno.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif  // WIN32

#include <qpushbutton.h>
#include <qlabel.h>
#include <qsqldatabase.h>
#include <math.h>
#include <qmessagebox.h>
#include <qprocess.h>

#include <dvtconfig.h>
#include <list_reports.h>
#include <edit_provider.h>
#include <add_provider.h>
#include <globals.h>
#include <dvtconf.h>
#include <spread_sheet.h>

ListReports::ListReports(QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setCaption("Davit - Reports");

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Reports List
  //
  list_reports_list=new QListView(this,"list_reports_list");
  list_reports_list->setMargin(5);
  list_reports_list->setAllColumnsShowFocus(true);
  connect(list_reports_list,
	  SIGNAL(doubleClicked(QListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(QListViewItem *,const QPoint &,int)));
  list_reports_list->addColumn("Report");
  list_reports_list->setColumnAlignment(0,AlignLeft|AlignVCenter);

  QListViewItem *item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Affidavit Contacts"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Missing Affidavit Data Contacts"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Missing Affidavit Submissions"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("All Affiliates"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("All Affiliate Contacts"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Affiliates by Daypart"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Affiliates by Network"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Affiliates by Program"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Affiliates/Admin Contacts by Program"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Added Programs"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Deleted Programs"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("RadioAmerica Affiliate"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Arbitron"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Affiliate Activity"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Affiliates by Program/DMA Market"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Affiliates by Program/MSA Market"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Programs by DMA Market"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Programs by MSA Market"));
  item=new QListViewItem(list_reports_list);
  item->setText(0,tr("Programs by City/State"));

  //
  //  Run Button
  //
  list_run_button=new QPushButton(this,"list_run_button");
  list_run_button->setFont(font);
  list_run_button->setText("&Run");
  connect(list_run_button,SIGNAL(clicked()),this,SLOT(runData()));

  //
  //  Close Button
  //
  list_close_button=new QPushButton(this,"list_close_button");
  list_close_button->setDefault(true);
  list_close_button->setFont(font);
  list_close_button->setText("&Close");
  connect(list_close_button,SIGNAL(clicked()),this,SLOT(closeData()));
}


ListReports::~ListReports()
{
}


QSize ListReports::sizeHint() const
{
  return QSize(320,300);
} 


QSizePolicy ListReports::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListReports::runData()
{
  SpreadSheet *sheet=new SpreadSheet();
  bool ok=false;

  sheet->setDefaultFont(DvtGetFont("DAVIT_REPORT"));

  QListViewItem *item=list_reports_list->selectedItem();
  if(item==NULL) {
    return;
  }
  if(item->text(0)==tr("Added Programs")) {
    ok=AddedProgramsReport(Dvt::RemarkProgramAdd,sheet);
  }
  if(item->text(0)==tr("Deleted Programs")) {
    ok=AddedProgramsReport(Dvt::RemarkProgramDelete,sheet);
  }
  if(item->text(0)==tr("All Affiliates")) {
    ok=AllAffiliatesReport(sheet);
  }
  if(item->text(0)==tr("All Affiliate Contacts")) {
    ok=AllAffiliateContacts(sheet);
  }
  if(item->text(0)==tr("Affiliates by Daypart")) {
    ok=AffiliatesByDaypartReport(sheet);
  }
  if(item->text(0)==tr("Affiliates by Network")) {
    ok=AffiliatesByNetworkReport(sheet);
  }
  if(item->text(0)==tr("Affiliates by Program")) {
    ok=AffiliatesByProgramReport(0,sheet);
  }
  if(item->text(0)==tr("Affiliates/Contacts by Program")) {
    ok=AffiliatesByProgramReport(ListReports::ProgramDirectorContact|
			      ListReports::AffidavitContact,sheet);
  }
  if(item->text(0)==tr("Affiliates/Admin Contacts by Program")) {
    ok=AffiliatesByProgramReport(ListReports::ProgramDirectorContact|
			      ListReports::GeneralManagerContact,sheet);
  }
  if(item->text(0)==tr("Arbitron")) {
    ok=ArbitronReport(sheet);
  }
  if(item->text(0)==tr("RadioAmerica Affiliate")) {
    ok=RAAffiliateReport(sheet);
  }
  if(item->text(0)==tr("Affiliate Activity")) {
    ok=ActivityReport(sheet);
  }
  if(item->text(0)==tr("Affidavit Contacts")) {
    ok=AffidavitReport(sheet);
  }
  if(item->text(0)==tr("Missing Affidavit Submissions")) {
    ok=MissingAffidavitReport(sheet);
  }
  if(item->text(0)==tr("Missing Affidavit Data Contacts")) {
    ok=MissingAffidavitContactReport(sheet);
  }
  if(item->text(0)==tr("Affiliates by Program/DMA Market")) {
    ok=AffiliatesByMarketReport(PickFields::DmaMarket,sheet);
  }
  if(item->text(0)==tr("Affiliates by Program/MSA Market")) {
    ok=AffiliatesByMarketReport(PickFields::MsaMarket,sheet);
  }
  if(item->text(0)==tr("Programs by DMA Market")) {
    ok=ProgramByMarketReport(PickFields::DmaMarket,sheet);
  }
  if(item->text(0)==tr("Programs by MSA Market")) {
    ok=ProgramByMarketReport(PickFields::MsaMarket,sheet);
  }
  if(item->text(0)==tr("Programs by City/State")) {
    ok=ProgramByMarketReport(PickFields::StateMarket,sheet);
  }

  if(ok) {
    ForkViewer(DvtTempName(Spread::fileExtension(DvtGetSpreadSheetFileFormat("DAVIT_REPORT"))),
	       sheet->write(DvtGetSpreadSheetFileFormat("DAVIT_REPORT")));
    // printf("out: %s\n",(const char *)outfile);
  }
}


void ListReports::doubleClickedData(QListViewItem *item,const QPoint &pt,
				      int c)
{
  runData();
}


void ListReports::closeData()
{
  done(0);
}


void ListReports::resizeEvent(QResizeEvent *e)
{
  list_reports_list->
    setGeometry(10,10,size().width()-20,size().height()-80);
  list_run_button->setGeometry(10,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListReports::ForkViewer(const QString &filename,const QString &data)
{
  if(!data.isEmpty()) {
    FILE *f=NULL;

    if((f=fopen(filename,"w"))==NULL) {
      QMessageBox::warning(this,"Davit - "+tr("Report Error"),
			   tr("Unable to create temporary file")+" \""+
			   filename+"\" ["+strerror(errno)+"].");
      return;
    }
    fprintf(f,"%s",(const char *)data.utf8());
    fclose(f);
  }

  QProcess proc(DvtReportViewerCommand(filename,openoffice_path));
  if(!proc.launch("")) {
    QMessageBox::warning(this,"Davit - "+tr("Error"),
			 tr("Unable to launch report viewer!"));
  }
}


void ListReports::ContactFields(int affiliate_id,ContactType type,int fields,
				SpreadTab *tab,int colnum,int rownum,
				QFontMetrics *fm)
{
  QString field;
  QString sql;
  QSqlQuery *q;
  QString ret;

  switch(type) {
  case ListReports::ProgramDirectorContact:
    field="PROGRAM_DIRECTOR";
    break;

  case ListReports::AffidavitContact:
    field="AFFIDAVIT";
    break;

  case ListReports::GeneralManagerContact:
    field="GENERAL_MANAGER";
    break;
  }
  sql=QString().sprintf("select NAME,PHONE,FAX,EMAIL from CONTACTS \
                         where (AFFILIATE_ID=%d)&&(%s=\"Y\")",
			affiliate_id,(const char *)field);
  q=new QSqlQuery(sql);
  if(q->first()) {
    if((fields&FieldName)!=0) {
      tab->addCell(colnum,rownum)->setText(q->value(0).toString(),fm);
      colnum++;
    }
    if((fields&FieldPhone)!=0) {
      tab->addCell(colnum,rownum)->
	setText(DvtFormatPhoneNumber(q->value(1).toString()),fm);
      colnum++;
    }
    if((fields&FieldFax)!=0) {
      tab->addCell(colnum,rownum)->
	setText(DvtFormatPhoneNumber(q->value(2).toString()),fm);
      colnum++;
    }
    if((fields&FieldEmail)!=0) {
      tab->addCell(colnum,rownum)->setText(q->value(3).toString(),fm);
      colnum++;
    }
  }
  else {
    if((fields&FieldName)!=0) {
      tab->addCell(colnum,rownum);
      colnum++;
    }
    if((fields&FieldPhone)!=0) {
      tab->addCell(colnum,rownum);
      colnum++;
    }
    if((fields&FieldFax)!=0) {
      tab->addCell(colnum,rownum);
      colnum++;
    }
    if((fields&FieldEmail)!=0) {
      tab->addCell(colnum,rownum);
      colnum++;
    }
  }
  delete q;
}
