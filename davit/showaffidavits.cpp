// showaffidavits.cpp
//
// Show an affiliates affidavit status.
//
//   (C) Copyright 2010-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <QDateTime>
#include <QHeaderView>

#include <dvtconf.h>
#include <dvtdb.h>

#include "globals.h"
#include "showaffidavits.h"

ShowAffidavits::ShowAffidavits(DvtConfig *c,QWidget *parent)
  : DvtWidget(c,parent)
{
  QDate now(QDate::currentDate().year(),QDate::currentDate().month(),1);
  QDate month=now.addMonths(-11);

  show_id=-1;

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  show_view=new QTableWidget(1,11,this);
  for(int i=0;i<show_view->rowCount();i++) {
    for(int j=0;j<show_view->columnCount();j++) {
      show_view->setItem(i,j,new QTableWidgetItem(QTableWidgetItem::Type));
      show_view->item(i,j)->
	setData(Qt::TextAlignmentRole,(int)Qt::AlignVCenter|Qt::AlignCenter);
    }
  }
  show_view->setSelectionBehavior(QAbstractItemView::SelectRows);
  show_view->setSelectionMode(QAbstractItemView::SingleSelection);
  show_view->setShowGrid(false);
  show_view->setSortingEnabled(false);
  show_view->setWordWrap(false);
  show_view->verticalHeader()->setVisible(false);
  show_view->horizontalHeader()->setVisible(true);
  QStringList f0;
  for(int i=0;i<11;i++) {
    f0.push_back(month.toString("MMM"));
    month=month.addMonths(1);
  }
  show_view->setHorizontalHeaderLabels(f0);
  show_mail_button=new QPushButton(tr("Send Reminder\nE-Mail"),this);
  show_mail_button->setFont(labelFont());
  connect(show_mail_button,SIGNAL(clicked()),this,SLOT(mailClickedData()));
}


ShowAffidavits::~ShowAffidavits()
{
}


QSize ShowAffidavits::sizeHint() const
{
  return QSize(10,10);
}


QSizePolicy ShowAffidavits::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ShowAffidavits::setAffiliateId(int id)
{
  QDate now(QDate::currentDate().year(),QDate::currentDate().month(),1);
  QDate month=now.addMonths(-11);
  int col=0;
  bool ok=true;

  show_id=id;

  month=now.addMonths(-11);
  while(month<now) {
    if(DvtAffidavitNeeded(id,month)) {
      show_view->item(0,col)->setText("");
      ok=false;
    }
    else {
      show_view->item(0,col)->setText(tr("OK"));
    }
    col++;
    month=month.addMonths(1);
  }
  show_mail_button->setDisabled(ok||(!email_enabled));
}


void ShowAffidavits::setAffiliateStatus(bool state)
{
  if(!state) {
    for(int i=0;i<show_view->model()->columnCount();i++) {
      show_view->item(0,i)->setText("");
    }
  }
}


void ShowAffidavits::mailClickedData()
{
  QStringList to_addrs;
  QString sql;
  QSqlQuery *q;

  //
  // Load System Values
  //
  QString origin_email=global_dvtsystem->originEmail();
  QString affidavit_email_subject=global_dvtsystem->affidavitEmailSubject();
  QString affidavit_email_template=global_dvtsystem->affidavitEmailTemplate();

  sql=QString("select ")+
    "`STATION_CALL`,"+   // 00
    "`STATION_TYPE`,"+   // 01
    "`USER_PASSWORD` "+  // 02
    QString::asprintf("from `AFFILIATES` where `ID`=%d",show_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    affidavit_email_subject.replace("%S",q->value(0).toString()+"-"+
				       q->value(1).toString()+"M");
    affidavit_email_template.replace("%S",q->value(0).toString()+"-"+
				     q->value(1).toString()+"M");
    affidavit_email_subject.replace("%U",q->value(0).toString().toLower()+"-"+
				    q->value(1).toString().toLower()+"m");
    affidavit_email_template.replace("%U",q->value(0).toString().toLower()+"-"+
				     q->value(1).toString().toLower()+"m");
    affidavit_email_subject.replace("%P",q->value(2).toString());
    affidavit_email_template.replace("%P",q->value(2).toString());
  }
  delete q;

  sql=QString("select ")+
    "`NAME`,"+   // 00
    "`EMAIL` "+  // 01
    "from `CONTACTS` where "+
    QString::asprintf("`(AFFILIATE_ID`=%d)&&(`AFFIDAVIT`='Y')",show_id);
  q=new QSqlQuery(sql);
  while(q->next()) {
    to_addrs.push_back(DvtFormatEmailAddress(q->value(0).toString(),
					     q->value(1).toString()));
  }
  delete q;

  if(mail_dialog->exec(to_addrs,QStringList(),QStringList(),
		       origin_email,global_dvtuser->email(),
		       affidavit_email_subject,
		       affidavit_email_template)==0) {
    sql=QString("insert into ")+
      "`AFFILIATE_REMARKS` set "+
      QString::asprintf("`EVENT_TYPE=%d,",Dvt::RemarkAffidavitReminder)+
      QString::asprintf("`AFFILIATE_ID`=%d,",show_id)+
      "`REMARK_DATETIME`=now(),"+
      "`USER_NAME`="+DvtSqlQuery::escape(global_dvtuser->name())+","+
      "`REMARK`='"+tr("Sent an affidavit reminder e-mail.")+"'";
    DvtSqlQuery::apply(sql);
    emit remarksUpdated();
  }
}


void ShowAffidavits::resizeEvent(QResizeEvent *e)
{
  show_view->setGeometry(0,0,size().width()-120,size().height());
  for(int i=0;i<show_view->model()->columnCount();i++) {
    show_view->
      setColumnWidth(i,show_view->width()/show_view->model()->columnCount());
  }
  show_mail_button->setGeometry(size().width()-110,5,110,size().height()-10);
}
