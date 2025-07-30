// list_reports.cpp
//
// List Davit Reports.
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

#ifndef PATH_MAX
#define PATH_MAX 256
#endif  // PATH_MAX

#include <stdlib.h>
#include <errno.h>
#include <math.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif  // WIN32

#include <QLabel>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>

#include <dvtconfig.h>
#include <dvtconf.h>
#include <dvtdb.h>

#include "add_provider.h"
#include "edit_provider.h"
#include "globals.h"
#include "list_reports.h"
#include "spread_sheet.h"

ListReports::ListReports(DvtConfig *c,QWidget *parent)
  : DvtDialog(c,parent)
{
  setModal(true);
  d_busy_cursor_count=0;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setWindowTitle("Davit - Reports");

  //
  // Reports List
  //
  list_reports_list=new QListWidget(this);
  connect(list_reports_list,SIGNAL(doubleClicked(const QModelIndex &)),
	  this,SLOT(doubleClickedData(const QModelIndex &)));
  QListWidgetItem *item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Affidavit Contacts"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Missing Affidavit Data Contacts"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Missing Affidavit Submissions"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("All Affiliates"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("All Affiliate Contacts"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Affiliates by Daypart"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Affiliates by Network"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Affiliates by Program"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Affiliates/Admin Contacts by Program"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Added Programs"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Deleted Programs"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("RadioAmerica Affiliate"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Arbitron"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Affiliate Activity"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Affiliates by Program/DMA Market"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Affiliates by Program/MSA Market"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Programs by DMA Market"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Programs by MSA Market"));
  item=new QListWidgetItem(list_reports_list);
  item->setText(tr("Programs by City/State"));

  //
  //  Run Button
  //
  list_run_button=new QPushButton(this);
  list_run_button->setFont(buttonFont());
  list_run_button->setText("Run");
  connect(list_run_button,SIGNAL(clicked()),this,SLOT(runData()));

  //
  //  Close Button
  //
  list_close_button=new QPushButton(this);
  list_close_button->setDefault(true);
  list_close_button->setFont(buttonFont());
  list_close_button->setText("Close");
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


void ListReports::clearBusyCursor()
{
  d_busy_cursor_count--;
  if(d_busy_cursor_count==0) {
    unsetCursor();
  }
}


void ListReports::runData()
{
  SpreadSheet *sheet=new SpreadSheet();
  bool ok=false;

  sheet->setDefaultFont(DvtGetFont("DAVIT_REPORT"));

  QList<QListWidgetItem *> items=list_reports_list->selectedItems();
  if(items.size()!=1) {
    return;
  }
  if(items.first()->text()==tr("Added Programs")) {
    ok=AddedProgramsReport(Dvt::RemarkProgramAdd,sheet);
  }
  if(items.first()->text()==tr("Deleted Programs")) {
    ok=AddedProgramsReport(Dvt::RemarkProgramDelete,sheet);
  }
  if(items.first()->text()==tr("All Affiliates")) {
    ok=AllAffiliatesReport(sheet);
  }
  if(items.first()->text()==tr("All Affiliate Contacts")) {
    ok=AllAffiliateContacts(sheet);
  }
  if(items.first()->text()==tr("Affiliates by Daypart")) {
    ok=AffiliatesByDaypartReport(sheet);
  }
  if(items.first()->text()==tr("Affiliates by Network")) {
    ok=AffiliatesByNetworkReport(sheet);
  }
  if(items.first()->text()==tr("Affiliates by Program")) {
    ok=AffiliatesByProgramReport(0,sheet);
  }
  if(items.first()->text()==tr("Affiliates/Contacts by Program")) {
    ok=AffiliatesByProgramReport(ListReports::ProgramDirectorContact|
			      ListReports::AffidavitContact,sheet);
  }
  if(items.first()->text()==tr("Affiliates/Admin Contacts by Program")) {
    ok=AffiliatesByProgramReport(ListReports::ProgramDirectorContact|
			      ListReports::GeneralManagerContact,sheet);
  }
  if(items.first()->text()==tr("Arbitron")) {
    ok=ArbitronReport(sheet);
  }
  if(items.first()->text()==tr("RadioAmerica Affiliate")) {
    ok=RAAffiliateReport(sheet);
  }
  if(items.first()->text()==tr("Affiliate Activity")) {
    ok=ActivityReport(sheet);
  }
  if(items.first()->text()==tr("Affidavit Contacts")) {
    ok=AffidavitReport(sheet);
  }
  if(items.first()->text()==tr("Missing Affidavit Submissions")) {
    ok=MissingAffidavitReport(sheet);
  }
  if(items.first()->text()==tr("Missing Affidavit Data Contacts")) {
    ok=MissingAffidavitContactReport(sheet);
  }
  if(items.first()->text()==tr("Affiliates by Program/DMA Market")) {
    ok=AffiliatesByMarketReport(PickFields::DmaMarket,sheet);
  }
  if(items.first()->text()==tr("Affiliates by Program/MSA Market")) {
    ok=AffiliatesByMarketReport(PickFields::MsaMarket,sheet);
  }
  if(items.first()->text()==tr("Programs by DMA Market")) {
    ok=ProgramByMarketReport(PickFields::DmaMarket,sheet);
  }
  if(items.first()->text()==tr("Programs by MSA Market")) {
    ok=ProgramByMarketReport(PickFields::MsaMarket,sheet);
  }
  if(items.first()->text()==tr("Programs by City/State")) {
    ok=ProgramByMarketReport(PickFields::StateMarket,sheet);
  }

  if(ok) {
    ForkViewer(DvtTempName(Spread::fileExtension(Spread::fileFormat("DAVIT_REPORT"))),
	       sheet->write(Spread::fileFormat("DAVIT_REPORT")));
    // printf("out: %s\n",(const char *)outfile);
  }
}


void ListReports::doubleClickedData(const QModelIndex &index)
{
  runData();
}


void ListReports::closeData()
{
  done(true);
}


void ListReports::setBusyCursor()
{
  emit reportStartupCommenced();
  if(d_busy_cursor_count==0) {
    setCursor(Qt::BusyCursor);
  }
  d_busy_cursor_count++;
}


void ListReports::resizeEvent(QResizeEvent *e)
{
  list_reports_list->setGeometry(10,10,size().width()-20,size().height()-80);
  list_run_button->setGeometry(10,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListReports::ForkViewer(const QString &filename,const QString &data)
{
  printf("filename: %s\n",filename.toUtf8().constData());
  //  printf("data: %s\n",data.toUtf8().constData());

  if(!data.isEmpty()) {
    FILE *f=NULL;

    if((f=fopen(filename.toUtf8(),"w"))==NULL) {
      QMessageBox::warning(this,"Davit - "+tr("Report Error"),
			   tr("Unable to create temporary file")+" \""+
			   filename+"\" ["+strerror(errno)+"].");
      return;
    }
    fprintf(f,"%s",data.toUtf8().constData());
    fclose(f);
  }
  /*
  QProcess proc(DvtReportViewerCommand(filename,openoffice_path));
  if(!proc.launch("")) {
    QMessageBox::warning(this,"Davit - "+tr("Error"),
			 tr("Unable to launch report viewer!"));
  }
  */
  QStringList args=DvtReportViewerCommand(filename,openoffice_path);
  global_viewer_list->start(args);
  
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
  sql=QString::asprintf("select NAME,PHONE,FAX,EMAIL from CONTACTS \
                         where (AFFILIATE_ID=%d)&&(%s=\"Y\")",
			affiliate_id,field.toUtf8().constData());
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
