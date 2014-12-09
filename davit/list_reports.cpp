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
  item->setText(0,tr("Affiliates/Contacts by Program"));
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
  QListViewItem *item=list_reports_list->selectedItem();
  if(item==NULL) {
    return;
  }
  if(item->text(0)==tr("Added Programs")) {
    AddedProgramsReport(Dvt::RemarkProgramAdd);
  }
  if(item->text(0)==tr("Deleted Programs")) {
    AddedProgramsReport(Dvt::RemarkProgramDelete);
  }
  if(item->text(0)==tr("All Affiliates")) {
    AllAffiliatesReport();
  }
  if(item->text(0)==tr("All Affiliate Contacts")) {
    AllAffiliateContacts();
  }
  if(item->text(0)==tr("Affiliates by Daypart")) {
    AffiliatesByDaypartReport();
  }
  if(item->text(0)==tr("Affiliates by Network")) {
    AffiliatesByNetworkReport();
  }
  if(item->text(0)==tr("Affiliates by Program")) {
    AffiliatesByProgramReport(0);
  }
  if(item->text(0)==tr("Affiliates/Contacts by Program")) {
    AffiliatesByProgramReport(ListReports::ProgramDirectorContact|
			      ListReports::AffidavitContact);
  }
  if(item->text(0)==tr("Arbitron")) {
    ArbitronReport();
  }
  if(item->text(0)==tr("RadioAmerica Affiliate")) {
    RAAffiliateReport();
  }
  if(item->text(0)==tr("Affiliate Activity")) {
    ActivityReport();
  }
  if(item->text(0)==tr("Affidavit Contacts")) {
    AffidavitReport();
  }
  if(item->text(0)==tr("Missing Affidavit Data Contacts")) {
    MissingAffidavitContactReport();
  }
  if(item->text(0)==tr("Affiliates by Program/DMA Market")) {
    AffiliatesByMarketReport(PickFields::DmaMarket);
  }
  if(item->text(0)==tr("Affiliates by Program/MSA Market")) {
    AffiliatesByMarketReport(PickFields::MsaMarket);
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


FILE *ListReports::GetTempFile(QString *filename)
{
  FILE *f=NULL;
#ifdef WIN32
  QString tempfile=QString().sprintf("%s\\davit-%s.txt",(const char *)GetTempDir(),
	           (const char *)QTime::currentTime().toString("hhmmsszzz"));
  if((f=fopen(tempfile,"w"))==NULL) {
    QMessageBox::warning(this,tr("Davit -- File Error"),
			 tr("Unable to create temporary file at \"")+
			 tempfile+"\".");
    *filename="";
    return NULL;
  }
  temp_files.push_back(tempfile);
  *filename=tempfile;
#else
  char pathname[PATH_MAX];
  int fd=-1;

  strcpy(pathname,"/tmp/davitXXXXXX");
  if((fd=mkstemp(pathname))<0) {
    QMessageBox::warning(this,tr("Davit -- File Error"),
			 tr("Unable to create temporary file at \"")+
			 pathname+"\".");
    *filename="";
    return NULL;
  }
  temp_files.push_back(pathname);
  *filename=pathname;
  f=fdopen(fd,"r+");
#endif  // WIN32
  return f;
}

/*
int ListReports::GetActiveAffiliates(std::vector<int> *affiliate_ids)
{
  QString sql;
  QSqlQuery *q;

  affiliate_ids->push_back(0);
  sql="select AFFILIATE_ID from AIRINGS order by AFFILIATE_ID";
  q=new QSqlQuery(sql);
  while(q->next()) {
    if(affiliate_ids->back()!=q->value(0).toInt()) {
      affiliate_ids->push_back(q->value(0).toInt());
    }
  }
  delete q;
  affiliate_ids->erase(affiliate_ids->begin());
  return affiliate_ids->size();
}
*/

QString ListReports::GetEmailContactList(int affiliate_id,bool affidavit,
					 bool progdir)
{
  QString ret;
  QString sql;
  QSqlQuery *q;

  sql=QString().sprintf("select NAME,EMAIL from CONTACTS \
                         where (AFFILIATE_ID=%d)",
			affiliate_id);
  if(affidavit) {
    sql+="&&(AFFIDAVIT=\"Y\")";
  }
  if(progdir) {
    sql+="&&(PROGRAM_DIRECTOR=\"Y\")";
  }
  q=new QSqlQuery(sql);
  if(q->size()<1) {
    ret="[unknown]";
  }
  else {
    while(q->next()) {
      if(DvtIsEmailAddress(q->value(1).toString())) {
	ret+=DvtFormatEmailAddress(q->value(0).toString(),
				   q->value(1).toString())+", ";
      }
      else {
	if(q->value(1).toString().stripWhiteSpace().isEmpty()) {
	  ret+="[unknown], ";
	}
	else {
	  ret+="[invalid], ";
	}
      }	
    }
    ret=ret.left(ret.length()-2);
  }
  delete q;

  return ret;
}


QString ListReports::GetNameContactList(int affiliate_id,bool affidavit,
					bool progdir)
{
  QString ret;
  QString sql;
  QSqlQuery *q;

  sql=QString().sprintf("select NAME from CONTACTS where (AFFILIATE_ID=%d)",
			affiliate_id);
  if(affidavit) {
    sql+="&&(AFFIDAVIT=\"Y\")";
  }
  if(progdir) {
    sql+="&&(PROGRAM_DIRECTOR=\"Y\")";
  }
  q=new QSqlQuery(sql);
  if(q->size()<1) {
    ret="[unknown]";
  }
  else {
    while(q->next()) {
      if(!q->value(0).toString().isEmpty()) {
	ret+=q->value(0).toString()+", ";
      }
    }
    ret=ret.left(ret.length()-2);
  }
  delete q;

  return ret;
}


QString ListReports::GetPhoneNumberContactList(int affiliate_id,bool affidavit,
					       bool progdir)
{
  QString ret;
  QString sql;
  QSqlQuery *q;

  sql=QString().sprintf("select PHONE from CONTACTS where (AFFILIATE_ID=%d)",
			affiliate_id);
  if(affidavit) {
    sql+="&&(AFFIDAVIT=\"Y\")";
  }
  if(progdir) {
    sql+="&&(PROGRAM_DIRECTOR=\"Y\")";
  }
  q=new QSqlQuery(sql);
  if(q->size()<1) {
    //    ret="[unknown]";
  }
  else {
    while(q->next()) {
      if(!q->value(0).toString().isEmpty()) {
	ret+=PhoneString(q->value(0).toString())+", ";
      }
    }
    ret=ret.left(ret.length()-2);
  }
  delete q;

  return ret;
}


QString ListReports::GetTempDir()
{
#ifdef WIN32
  if(getenv("TEMP")!=NULL) {
    return QString(getenv("TEMP"));
  }
  if(getenv("TMP")!=NULL) {
    return QString(getenv("TMP"));
  }
  return QString("C:\\");
#else
  return QString("/tmp");
#endif  // WIN32
}


void ListReports::ForkViewer(const QString &filename)
{
#ifdef WIN32
  QProcess proc;
  proc.addArgument(openoffice_path);
  proc.addArgument("-calc");
  proc.addArgument("-view");
  proc.addArgument(filename);
  bool ret=proc.launch("");
#else
  if(fork()==0) {
    execlp("soffice","soffice","-calc","-view",(const char *)filename,
	   (char *)0);
    exit(0);
  }
#endif  // WIN32
}


QString ListReports::StringField(const QVariant &v)
{
  if(v.isNull()) {
    return QString("\"[none]\",");
  }
  if(v.toString().isEmpty()) {
    return QString("\"[none]\",");
  }
  return QString("\"")+v.toString()+QString("\",");
}


QString ListReports::PhoneField(const QVariant &v)
{
  if(v.isNull()) {
    return QString("\"[none]\",");
  }
  if(v.toString().isEmpty()) {
    return QString("\"[none]\",");
  }
  /*
  QString pnum=v.toString().stripWhiteSpace();
  if(pnum.length()==10) {
    return QString("\"(")+pnum.left(3)+QString(") ")+
      pnum.mid(3,3)+QString("-")+pnum.right(4)+QString("\",");
  }
  */
  return QString("\"")+PhoneString(v.toString())+QString("\",");
}


QString ListReports::PhoneString(const QString &pnum)
{
  QString ret=pnum.stripWhiteSpace();
  if(ret.length()==10) {
    return QString("(")+ret.left(3)+QString(") ")+
      ret.mid(3,3)+QString("-")+ret.right(4);
  }
  return ret;
}


QString ListReports::AddressField(const QVariant &v1,const QVariant &v2)
{
  QString addr1=v1.toString();
  QString addr2=v2.toString();
  if(addr1.isEmpty()&&addr2.isEmpty()) {
    return QString("\"[none]\",");
  }
  QString ret="\""+addr1;
  if((!addr1.isEmpty())&&(!addr2.isEmpty())) {
    ret+=", ";
  }
  ret+=addr2+"\",";

  return ret;
}


QString ListReports::FrequencyField(const QVariant &v)
{
  bool ok=false;
  double freq=v.toDouble(&ok);
  if((!ok)||(freq==0.0)) {
    return QString("\"[none]\",");
  }
  if(freq<108.1) {
    return QString().sprintf("\"%5.1lf MHz\",",freq);
  }
  return QString().sprintf("\"%4.0f kHz\",",freq);
}


QString ListReports::TypeField(const QVariant &v)
{
  if(v.isNull()) {
    return QString("[none]");
  }
  if(v.toString().lower()=="a") {
    return QString("\"AM\",");
  }
  if(v.toString().lower()=="f") {
    return QString("\"FM\",");
  }
  if(v.toString().lower()=="i") {
    return QString("\"Internet\",");
  }
  return QString("\"[none]\",");
}


QString ListReports::ContactFields(int affiliate_id,ContactType type,int fields)
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
      ret+=StringField(q->value(0).toString());
    }
    if((fields&FieldPhone)!=0) {
      ret+=PhoneField(q->value(1).toString());
    }
    if((fields&FieldFax)!=0) {
      ret+=PhoneField(q->value(2).toString());
    }
    if((fields&FieldEmail)!=0) {
      ret+=StringField(q->value(3).toString());
    }
  }
  else {
    if((fields&FieldName)!=0) {
      ret+=StringField("");
    }
    if((fields&FieldPhone)!=0) {
      ret+=StringField("");
    }
    if((fields&FieldFax)!=0) {
      ret+=StringField("");
    }
    if((fields&FieldEmail)!=0) {
      ret+=StringField("");
    }
  }
  delete q;

  return ret;
}


QString ListReports::EmptyField()
{
  return QString("\"\",");
}
