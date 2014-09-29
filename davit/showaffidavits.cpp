// showaffidavits.cpp
//
// Show an affiliates affidavit status.
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: showaffidavits.cpp,v 1.8 2011/04/29 22:13:25 pcvs Exp $
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

#include <qdatetime.h>
#include <qsqldatabase.h>
#include <qmessagebox.h>

#include <dvtconf.h>

#include <showaffidavits.h>
#include <globals.h>

ShowAffidavits::ShowAffidavits(int id,QWidget *parent,const char *name)
  : QWidget(parent,name)
{
  QDate now(QDate::currentDate().year(),QDate::currentDate().month(),1);
  QDate month=now.addMonths(-11);
  int col=0;
  bool ok=true;
  QListViewItem *item=NULL;

  show_id=id;

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  show_list=new QListView(this,"show_list");
  show_list->setAllColumnsShowFocus(true);
  show_list->setItemMargin(5);
  while(month<now) {
    show_list->addColumn(month.toString("MMM"));
    show_list->setColumnAlignment(col++,AlignCenter);
    month=month.addMonths(1);
  }
  month=now.addMonths(-11);
  col=0;
  item=new QListViewItem(show_list);
  while(month<now) {
    if(DvtAffidavitNeeded(id,month)) {
      ok=false;
    }
    else {
      item->setText(col,"OK");
    }
    col++;
    month=month.addMonths(1);
  }

  show_mail_button=new QPushButton(tr("Send Reminder\nE-Mail"),this);
  show_mail_button->setFont(label_font);
  show_mail_button->setDisabled(ok||(!email_enabled));
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


void ShowAffidavits::setAffiliateStatus(bool state)
{
  if(!state) {
    QListViewItem *item=show_list->firstChild();
    for(int i=0;i<show_list->columns();i++) {
      item->setText(i,"");
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

  sql=QString().sprintf("select STATION_CALL,STATION_TYPE,USER_PASSWORD \
                         from AFFILIATES where ID=%d",show_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    affidavit_email_subject.replace("%S",q->value(0).toString()+"-"+
				       q->value(1).toString()+"M");
    affidavit_email_template.replace("%S",q->value(0).toString()+"-"+
				     q->value(1).toString()+"M");
    affidavit_email_subject.replace("%U",q->value(0).toString().lower()+"-"+
				    q->value(1).toString().lower()+"m");
    affidavit_email_template.replace("%U",q->value(0).toString().lower()+"-"+
				     q->value(1).toString().lower()+"m");
    affidavit_email_subject.replace("%P",q->value(2).toString());
    affidavit_email_template.replace("%P",q->value(2).toString());
  }
  delete q;

  sql=QString().sprintf("select NAME,EMAIL from CONTACTS \
                         where (AFFILIATE_ID=%d)&&(AFFIDAVIT=\"Y\")",
			show_id);
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
    sql=QString().
      sprintf("insert into AFFILIATE_REMARKS set \
               EVENT_TYPE=%d,	 \
               AFFILIATE_ID=%d,\
               REMARK_DATETIME=now(),\
               USER_NAME=\"%s\",\
               REMARK=\"Sent an affidavit reminder e-mail.\"",
	      Dvt::RemarkAffidavitReminder,
	      show_id,
	      (const char *)DvtEscapeString(global_dvtuser->name()));
    q=new QSqlQuery(sql);
    delete q;
    emit remarksUpdated();
  }
}


void ShowAffidavits::resizeEvent(QResizeEvent *e)
{
  show_list->setGeometry(0,0,size().width()-120,size().height());
  show_mail_button->setGeometry(size().width()-110,5,110,size().height()-10);
}
