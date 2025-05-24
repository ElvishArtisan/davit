// list_affiliates.cpp
//
// List Davit Affiliates.
//
//   (C) Copyright 2007-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <math.h>

#include <QApplication>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QResizeEvent>
#include <QSqlQuery>

#ifndef WIN32
#include <vmime/vmime.hpp>
#endif  // WIN32

#include <dvtconf.h>
#include <dvtconfig.h>
//#include <dvtmail.h>

#include "add_affiliate.h"
#include "edit_affiliate.h"
#include "generate_affadavit.h"
#include "globals.h"
#include "list_affiliates.h"

//
// Icons
//
#include "../icons/greenball.xpm"
#include "../icons/redball.xpm"
#include "../icons/whiteball.xpm"

ListAffiliates::ListAffiliates(QWidget *parent)
  : QDialog(parent)
{
  setModal(true);
  QDate today=QDate::currentDate();

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setWindowTitle("Davit - Affiliates");

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Load Icons
  //
  list_greenball_map=new QPixmap(greenball_xpm);
  list_redball_map=new QPixmap(redball_xpm);
  list_whiteball_map=new QPixmap(whiteball_xpm);

  //
  // Email Progress Dialog
  //
  /*
  list_email_progress=
    new QProgressDialog(tr("Generating e-mail, please wait..."),
			tr("Cancel"),100,this);
  list_email_progress->setCaption("Davit");
  */
  //
  // Filter Checkbox
  //
  list_filter_box=new QCheckBox(this);
  list_filter_label=new QLabel(tr("Show Affiliates Only"),this);
  list_filter_label->setFont(font);
  list_filter_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  connect(list_filter_box,SIGNAL(stateChanged(int)),
	  this,SLOT(filterStateChangedData(int)));

  //
  // Missing Data
  //
  list_missing_box=new QCheckBox(this);
  list_missing_label=
    new QLabel(tr("Show Affiliates Missing:"),this);
  list_missing_label->setFont(font);
  list_missing_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  connect(list_missing_box,SIGNAL(stateChanged(int)),
	  this,SLOT(missingStateChangedData(int)));
  list_month_box=new QComboBox();
  list_month_box->insertItem(0,tr("January"));
  list_month_box->insertItem(1,tr("February"));
  list_month_box->insertItem(2,tr("March"));
  list_month_box->insertItem(3,tr("April"));
  list_month_box->insertItem(4,tr("May"));
  list_month_box->insertItem(5,tr("June"));
  list_month_box->insertItem(6,tr("July"));
  list_month_box->insertItem(7,tr("August"));
  list_month_box->insertItem(8,tr("September"));
  list_month_box->insertItem(9,tr("October"));
  list_month_box->insertItem(10,tr("November"));
  list_month_box->insertItem(11,tr("December"));
  connect(list_month_box,SIGNAL(activated(int)),
	  this,SLOT(monthActivatedData(int)));
  list_year_box=new QComboBox(this);
  int count=0;
  for(int i=2008;i<(today.year()+1);i++) {
    list_year_box->insertItem(count++,QString::asprintf("%d",i));
  }
  connect(list_year_box,SIGNAL(activated(int)),
	  this,SLOT(yearActivatedData(int)));
  QDate last_month=QDate::currentDate().addMonths(-1);
  list_month_box->setCurrentIndex(last_month.month()-1);
  list_year_box->setCurrentIndex(last_month.year()-2008);

  //
  // Call Filter
  //
  list_callfilter_edit=new QLineEdit(this);
  list_callfilter_label=new QLabel(tr("Call Filter:"),this);
  list_callfilter_label->setFont(font);
  list_callfilter_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  connect(list_callfilter_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(filterTextChangedData(const QString &)));

  //
  // Counter
  //
  list_showing_edit=new QLineEdit(this);
  list_showing_edit->setReadOnly(true);
  list_showing_label=new QLabel(tr("Showing:"),this);
  list_showing_label->setFont(font);
  list_showing_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  connect(list_showing_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(filterTextChangedData(const QString &)));

  //
  // Affiliates List
  //
  /*
  list_affiliates_list=new QListView(this);
  list_affiliates_list->setMargin(5);
  list_affiliates_list->setAllColumnsShowFocus(true);
  connect(list_affiliates_list,
	  SIGNAL(doubleClicked(QListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(QListViewItem *,const QPoint &,int)));
  list_affiliates_list->addColumn(" ");
  list_affiliates_list->setColumnAlignment(0,Qt::AlignCenter);
  list_affiliates_list->addColumn("Call");
  list_affiliates_list->setColumnAlignment(1,Qt::AlignCenter);
  list_affiliates_list->addColumn("City of License");
  list_affiliates_list->setColumnAlignment(2,Qt::AlignLeft|Qt::AlignVCenter);
  list_affiliates_list->addColumn("Type");
  list_affiliates_list->setColumnAlignment(3,Qt::AlignCenter);
  list_affiliates_list->addColumn("DMA");
  list_affiliates_list->setColumnAlignment(4,Qt::AlignLeft|Qt::AlignVCenter);
  list_affiliates_list->addColumn("Business Name");
  list_affiliates_list->setColumnAlignment(5,Qt::AlignLeft|Qt::AlignVCenter);
  */
  //
  //  Add Button
  //
  list_add_button=new QPushButton(this);
  list_add_button->setFont(font);
  list_add_button->setText("&Add");
  list_add_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  //
  //  Edit Button
  //
  list_edit_button=new QPushButton(this);
  list_edit_button->setFont(font);
  list_edit_button->setText("&Edit");
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  //  Delete Button
  //
  list_delete_button=new QPushButton(this);
  list_delete_button->setFont(font);
  list_delete_button->setText("&Delete");
  list_delete_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  //  Generate Affidavit Button
  //
  list_affadavit_button=new QPushButton(this);
  list_affadavit_button->setFont(font);
  list_affadavit_button->setText("&Generate\nAffidavit");
  list_affadavit_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivReportView));
  connect(list_affadavit_button,SIGNAL(clicked()),this,SLOT(affadavitData()));

  //
  //  Affidavit Reminder Button
  //
  list_affidavit_reminder_button=new QPushButton(this);
  list_affidavit_reminder_button->setFont(font);
  list_affidavit_reminder_button->setText("Affidavit\n&Reminders");
  list_affidavit_reminder_button->setDisabled(true);
  connect(list_affidavit_reminder_button,SIGNAL(clicked()),
	  this,SLOT(affidavitReminderData()));
#ifdef WIN32
  list_affidavit_reminder_button->hide();
#endif  // WIN32

  //
  //  Close Button
  //
  list_close_button=new QPushButton(this);
  list_close_button->setDefault(true);
  list_close_button->setFont(font);
  list_close_button->setText("&Close");
  connect(list_close_button,SIGNAL(clicked()),this,SLOT(closeData()));

  filterStateChangedData(false);
}


ListAffiliates::~ListAffiliates()
{
}


QSize ListAffiliates::sizeHint() const
{
  return QSize(755,480);
} 


QSizePolicy ListAffiliates::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListAffiliates::filterStateChangedData(int state)
{
  list_missing_box->setEnabled(state);
  list_missing_label->setEnabled(state);
  list_month_box->setEnabled(state);
  list_year_box->setEnabled(state);
  if(!state) {
    list_missing_box->setChecked(false);
    list_affidavit_reminder_button->setEnabled(false);
  }
  RefreshList();
}


void ListAffiliates::filterTextChangedData(const QString &str)
{
  RefreshList();
}


void ListAffiliates::missingStateChangedData(int state)
{
  list_affidavit_reminder_button->setEnabled(state&&email_enabled);
  RefreshList();
}


void ListAffiliates::monthActivatedData(int index)
{
  RefreshList();
}


void ListAffiliates::yearActivatedData(int index)
{
  RefreshList();
}


void ListAffiliates::addData()
{
  /*
  QString call;
  QString type;
  QString sql;
  QSqlQuery *q;

  AddAffiliate *add=new AddAffiliate(&call,&type,this);
  if(add->exec()==0) {
    sql=QString::asprintf("insert into AFFILIATES set \
                           STATION_CALL=\"%s\",\
                           STATION_TYPE=\"%s\"",
			  (const char *)call,
			  (const char *)type);
    q=new QSqlQuery(sql);
    delete q;
    sql=QString::asprintf("select ID from AFFILIATES where \
                          (STATION_CALL=\"%s\")&&(STATION_TYPE=\"%s\")",
			  (const char *)call,(const char *)type);
    q=new QSqlQuery(sql);
    if(q->first()) {
      EditAffiliate *edit=new EditAffiliate(q->value(0).toInt(),this);
      if(edit->exec()==0) {
	DvtListViewItem *item=new DvtListViewItem(list_affiliates_list);
	item->setText(0,call);
	UpdateItem(item);
	list_affiliates_list->setSelected(item,true);
	list_affiliates_list->ensureItemVisible(item);
      }
      else {
	DeleteAffiliate(q->value(0).toInt());
      }
      delete edit;
    }
  }
  delete add;
  */
}


void ListAffiliates::editData()
{
  /*
  DvtListViewItem *item=
    (DvtListViewItem *)list_affiliates_list->selectedItem();
  
  if(item==NULL) {
    return;
  }
  EditAffiliate *edit=new EditAffiliate(item->id(),this);
  if(edit->exec()==0) {
    UpdateItem(item);
  }
  delete edit;
  */
}


void ListAffiliates::deleteData()
{
  /*
  DvtListViewItem *item=
    (DvtListViewItem *)list_affiliates_list->selectedItem();

  if(item==NULL) {
    return;
  }
  if(QMessageBox::question(this,"Delete Affiliate",
      QString::asprintf("Are you sure you want to delete the affiliate \"%s\"",
			   (const char *)item->text(0)),
			   QMessageBox::Yes,
			   QMessageBox::No)==QMessageBox::No) {
    return;
  }
  DeleteAffiliate(item->id());
  delete item;
  */
}


void ListAffiliates::affadavitData()
{
  /*
  DvtListViewItem *item=
    (DvtListViewItem *)list_affiliates_list->selectedItem();
  
  if(item==NULL) {
    return;
  }
  GenerateAffadavit *edit=
    new GenerateAffadavit(GenerateAffadavit::ReportAffiliate,item->id(),
			  this,"edit");
  edit->exec();
  delete edit;
  */
}


void ListAffiliates::affidavitReminderData()
{
  /*
  if(QMessageBox::question(this,"Davit - List Affiliates",
			   tr("This will send an affidavit reminder e-mail to")+
			   QString::asprintf(" %d ",list_affiliates_list->childCount())+tr("affiliates.\nProceed?"),QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
    return;
  }
  SendAffidavitReminder();
  */
}

/*
void ListAffiliates::doubleClickedData(QListViewItem *item,const QPoint &pt,
				      int c)
{
  editData();
}
*/

void ListAffiliates::closeData()
{
  done(0);
}


void ListAffiliates::resizeEvent(QResizeEvent *e)
{
  list_filter_box->setGeometry(20,10,15,15);
  list_filter_label->setGeometry(40,8,130,20);
  list_missing_box->setGeometry(180,10,15,15);
  list_missing_label->setGeometry(200,8,140,20);
  list_month_box->setGeometry(345,8,95,20);
  list_year_box->setGeometry(445,8,55,20);
  list_showing_edit->setGeometry(570,7,50,20);
  list_showing_label->setGeometry(505,8,60,20);
  list_callfilter_edit->setGeometry(e->size().width()-60,7,50,20);
  list_callfilter_label->setGeometry(e->size().width()-135,8,70,20);
  //  list_affiliates_list->
  //    setGeometry(10,32,size().width()-20,size().height()-102);
  list_add_button->setGeometry(10,size().height()-60,80,50);
  list_edit_button->setGeometry(100,size().height()-60,80,50);
  list_delete_button->setGeometry(190,size().height()-60,80,50);
  list_affadavit_button->setGeometry(330,size().height()-60,80,50);
  list_affidavit_reminder_button->setGeometry(430,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListAffiliates::SendAffidavitReminder()
{
  /*
#ifndef WIN32
  QString sql;
  QSqlQuery *q;
  QString subject;
  QString msg;
  QStringList to_addrs;
  QStringList empty;
  QStringList subaddrs;
  unsigned errs=0;
  int count=0;
  int step=0;
  //  int step_size=list_affiliates_list->childCount()/100;
  QString invalid_addrs;

  //
  // Create Mail Transport
  //
  vmime::ref<vmime::net::session> sess=NULL;
  vmime::ref<vmime::net::transport> trans=NULL;
  try {
    sql="select SMTP_HOSTNAME,SMTP_PORT from SYSTEM";
    q=new QSqlQuery(sql);
    if(!q->first()) {
      QMessageBox::warning(NULL,"Davit - Davit Error",
			   "Unable to load SMTP server configuration!");
      delete q;
      return;
    }
    if(q->value(0).toString().isEmpty()) {
      QMessageBox::warning(NULL,"Davit - Davit Error",
			   "Missing SMTP server configuration!");
      delete q;
      return;
    }
    sess=vmime::create<vmime::net::session>();
    sess->getProperties()["transport.smtp.server.port"]=q->value(1).toInt();
    vmime::utility::url url((const char *)QString::asprintf("smtp://%s",
				   (const char *)q->value(0).toString()));
    trans=sess->getTransport(url); 
    trans->connect();
    delete q;
  }
  catch(vmime::exception &e) {
    QMessageBox::warning(NULL,"Davit - Mailer Error",e.what());
    return;
  }
  catch(std::exception &e) {
    QMessageBox::warning(NULL,"Davit - Davit Error",e.what());
    return;
  }

  //
  // Generate messages
  //
  list_email_progress->setProgress(0);
  DvtListViewItem *item=(DvtListViewItem *)list_affiliates_list->firstChild();
  while((item!=NULL)&&(!list_email_progress->wasCancelled())) {
    sql=QString::asprintf("select STATION_CALL,STATION_TYPE,USER_PASSWORD \
                           from AFFILIATES where ID=%d",item->id());
    q=new QSqlQuery(sql);
    while(q->next()) {
      subject=global_dvtsystem->affidavitEmailSubject();
      subject.
	replace("%S",q->value(0).toString()+"-"+q->value(1).toString()+"M");
      subject.replace("%P",q->value(2).toString());
      subject.replace("%U",q->value(0).toString().lower()+"-"+
		      q->value(1).toString().lower()+"m");
      msg=global_dvtsystem->affidavitEmailTemplate();
      msg.replace("%S",q->value(0).toString()+"-"+q->value(1).toString()+"M");
      msg.replace("%P",q->value(2).toString());
      msg.replace("%U",q->value(0).toString().lower()+"-"+
		  q->value(1).toString().lower()+"m");
    }
    delete q;
    sql=QString::asprintf("select NAME,EMAIL from CONTACTS where \
                           (AFFILIATE_ID=%d)&&(AFFIDAVIT=\"Y\")",item->id());
    q=new QSqlQuery(sql);
    to_addrs.clear();
    empty.clear();
    while(q->next()) {
      subaddrs=subaddrs.split(";",q->value(1).toString());
      for(unsigned i=0;i<subaddrs.size();i++) {
	if(!subaddrs[i].isEmpty()) {
	  to_addrs.push_back(QString());
	}
	if(!q->value(0).toString().isEmpty()) {
	  to_addrs.back()=q->value(0).toString()+" <";
	}
	to_addrs.back()+=subaddrs[i].stripWhiteSpace();
	if(!q->value(0).toString().isEmpty()) {
	  to_addrs.back()+=">";
	}
      }
    }
    delete q;
    for(unsigned i=0;i<to_addrs.size();i++) {
      if(DvtIsFullEmailAddress(to_addrs[i])) {
	//printf("addr: |%s|\n",(const char *)to_addrs[i]);
      }
      else {
	invalid_addrs+=item->text(0)+": "+to_addrs[i]+"\n";
	errs++;
	//printf("BAD ADDR: |%s|\n",(const char *)to_addrs[i]);
      }
    }
    if(to_addrs.size()>0) {
      DvtSendMail(to_addrs,empty,empty,global_dvtsystem->originEmail(),
		  global_dvtuser->email(),subject,msg,this,trans);
      //usleep(50000);
    }
    item=(DvtListViewItem *)item->nextSibling();
    if(++count/step_size>step) {
      list_email_progress->setProgress(++step);
      qApp->processEvents();
    }
  }
  list_email_progress->reset();
  try {
    trans->disconnect();
  }
  catch(vmime::exception &e) {
  }
  if(errs==0) {
    QMessageBox::information(this,"Davit",QString::asprintf("%d ",count-errs)+
			     tr("messages sent!"));
  }
  else {
    QMessageBox::information(this,"Davit",
			     QString::asprintf("%d ",errs)+
			     tr("e-mail addresses were invalid:")+"\n\n"+
			     invalid_addrs);
  }
#endif  // WIN32
  */
}


void ListAffiliates::DeleteAffiliate(int id)
{
  QString sql;
  QSqlQuery *q;

  sql=QString::asprintf("delete from AFFILIATES where ID=%d",id);
  q=new QSqlQuery(sql);
  delete q;
}


void ListAffiliates::RefreshList()
{
  /*
  QString sql;
  QSqlQuery *q;
  DvtListViewItem *item;
  QString call;
  QString type;
  QString suffix;
  QDate affidavit_date;
  int count=0;

  list_affiliates_list->clear();
  if(list_missing_box->isChecked()) {
    affidavit_date=QDate(list_year_box->currentText().toInt(),
			 list_month_box->currentItem()+1,1);
  }
  sql=QString::asprintf("select AFFILIATES.ID,AFFILIATES.STATION_CALL,\
                         AFFILIATES.LICENSE_CITY,AFFILIATES.LICENSE_STATE,\
                         AFFILIATES.LICENSE_COUNTRY,AFFILIATES.STATION_TYPE,\
                         AFFILIATES.BUSINESS_NAME,\
                         AFFILIATES.DMA_NAME,\
                         AFFILIATES.IS_AFFILIATE,AFFILIATES.AFFIDAVIT_ACTIVE\
                         from AFFILIATES %s \
                         order by AFFILIATES.STATION_CALL desc",
			(const char *)FilterSql(list_callfilter_edit->text(),
						list_filter_box->isChecked(),
						affidavit_date));
  q=new QSqlQuery(sql);
  while (q->next()) {
    count++;
    if((call!=q->value(1).toString())||(type!=q->value(5).toString())) {
      item=new DvtListViewItem(list_affiliates_list);
      item->setId(q->value(0).toInt());
      if(q->value(8).toString()=="Y") {
	if(q->value(9).toString()=="Y") {
	  item->setPixmap(0,*list_greenball_map);
	}
	else {
	  item->setPixmap(0,*list_redball_map);
	}
      }
      else {
	item->setPixmap(0,*list_whiteball_map);
      }
      if(q->value(5).toString().lower()=="a") {
	item->setText(3,tr("AM"));
	suffix="-AM";
      }
      else {
	if(q->value(5).toString().lower()=="f") {
	  item->setText(3,tr("FM"));
	  suffix="-FM";
	}
	else {
	  item->setText(3,tr("Internet"));
	  suffix="";
	}
      }
      item->setText(1,q->value(1).toString()+suffix);
      item->setText(2,DvtFormatCityState(q->value(2).toString(),
					 q->value(3).toString()));
      if(q->value(7).toString().isEmpty()) {
	item->setText(4,"[none]");
      }
      else {
	item->setText(4,q->value(7).toString());
      }
      item->setText(5,q->value(6).toString());
      call=q->value(1).toString();
      type=q->value(5).toString();
    }
  }
  delete q;
  list_showing_edit->setText(QString::asprintf("%d",count));
*/
}


void ListAffiliates::UpdateItem(DvtListViewItem *item)
{
  /*
  QString sql;
  QSqlQuery *q;
  QString suffix;

  sql=QString::asprintf("select LICENSE_CITY,LICENSE_STATE,LICENSE_COUNTRY,\
                         STATION_CALL,STATION_TYPE,BUSINESS_NAME,\
                         DMA_NAME,\
                         IS_AFFILIATE,AFFIDAVIT_ACTIVE \
                         from AFFILIATES where ID=%d",item->id());
  q=new QSqlQuery(sql);
  if(q->first()) {
    if(q->value(7).toString()=="Y") {
      if(q->value(8).toString()=="Y") {
	item->setPixmap(0,*list_greenball_map);
      }
      else {
	item->setPixmap(0,*list_redball_map);
      }
    }
    else {
      item->setPixmap(0,*list_whiteball_map);
    }
    item->setText(2,DvtFormatCityState(q->value(0).toString(),
				       q->value(1).toString()));
    if(q->value(4).toString().lower()=="a") {
      item->setText(3,tr("AM"));
      suffix="-AM";
    }
    else {
      if(q->value(4).toString().lower()=="f") {
	item->setText(3,tr("FM"));
	suffix="-FM";
      }
      else {
	item->setText(3,tr("Internet"));
	suffix="";
      }
    }
    item->setText(1,q->value(3).toString()+suffix);
    if(q->value(6).toString().isEmpty()) {
      item->setText(4,"[none]");
    }
    else {
      item->setText(4,q->value(6).toString());
    }
    item->setText(5,q->value(5).toString());
  }
  delete q;
*/
}


QString ListAffiliates::FilterSql(const QString &filter,bool affils_only,
				  const QDate &affidavit_date)
{
  QString ret="";
  QString sql;
  QSqlQuery *q;

  if(filter.isEmpty()&&(affidavit_date.isNull())&&(!affils_only)) {
    return QString("");
  }
  ret="where ";
  if(affils_only) {
    ret+="(IS_AFFILIATE=\"Y\")&&";
  }
  if(affidavit_date.isValid()) {
    sql="select ID from AFFILIATES where AFFIDAVIT_ACTIVE=\"Y\"";
    q=new QSqlQuery(sql);
    while(q->next()) {
      if(!DvtAffidavitNeeded(q->value(0).toInt(),affidavit_date)) {
	ret+=QString::asprintf("(ID!=%d)&&",q->value(0).toInt());
      }
    }
    delete q;
  }
  if(filter.isEmpty()) {
    ret=ret.left(ret.length()-2);
  }
  else {
    ret+=QString::asprintf("(STATION_CALL like \"%%%s%%\")",
			   filter.toUtf8().constData());
  }
  return ret;
}
