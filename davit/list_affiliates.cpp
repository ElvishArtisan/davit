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

#include <dvtconf.h>
#include <dvtconfig.h>
#include <dvtsendmail.h>

#include "globals.h"
#include "list_affiliates.h"

//
// Icons
//
#include "../icons/blueball.xpm"
#include "../icons/greenball.xpm"
#include "../icons/redball.xpm"
#include "../icons/whiteball.xpm"

ListAffiliates::ListAffiliates(DvtConfig *c,QWidget *parent)
  : Dialog(c,parent)
{
  QDate today=QDate::currentDate();

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setWindowTitle("Davit - "+tr("List Affiliates"));

  //
  // Load Icons
  //
  list_greenball_map=new QPixmap(greenball_xpm);
  list_redball_map=new QPixmap(redball_xpm);
  list_whiteball_map=new QPixmap(whiteball_xpm);

  //
  // Dialogs
  //
  list_addaffiliate_dialog=new AddAffiliate(c,this);
  list_editaffiliate_dialog=new EditAffiliate(c,this);
  list_generateaffadavit_dialog=new GenerateAffadavit(c,this);

  //
  // Show Affiliates Checkbox
  //
  list_show_affiliates_check=new QCheckBox(this);
  list_show_affiliates_check->setChecked(true);
  list_show_affiliates_label=new QLabel(tr("Show Active Affiliates Only"),this);
  list_show_affiliates_label->setFont(labelFont());
  list_show_affiliates_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  connect(list_show_affiliates_check,SIGNAL(stateChanged(int)),
	  this,SLOT(showAffiliatesChangedData(int)));

  //
  // Show Missing Affadavits Data
  //
  list_missing_affiliates_check=new QCheckBox(this);
  list_missing_affiliates_label=
    new QLabel(tr("Show Affiliates Missing:"),this);
  list_missing_affiliates_label->setFont(labelFont());
  list_missing_affiliates_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  connect(list_missing_affiliates_check,SIGNAL(stateChanged(int)),
	  this,SLOT(missingAffiliateChangedData(int)));
  list_month_box=new QComboBox(this);
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
  list_callfilter_label->setFont(labelFont());
  list_callfilter_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  connect(list_callfilter_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(filterTextChangedData(const QString &)));

  //
  // Counter
  //
  list_showing_edit=new QLineEdit(this);
  list_showing_edit->setReadOnly(true);
  list_showing_label=new QLabel(tr("Showing:"),this);
  list_showing_label->setFont(labelFont());
  list_showing_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // Affiliates List
  //
  list_affiliates_view=new TableView(this);
  list_affiliates_model=new AffiliateListModel(this);
  list_affiliates_model->setFont(defaultFont());
  list_affiliates_model->setPalette(palette());
  list_affiliates_view->setModel(list_affiliates_model);
  connect(list_affiliates_view,SIGNAL(doubleClicked(const QModelIndex &)),
	  this,SLOT(doubleClickedData(const QModelIndex &)));
  connect(list_affiliates_model,SIGNAL(rowCountChanged(int)),
	  this,SLOT(rowCountChangedData(int)));

  //
  //  Add Button
  //
  list_add_button=new QPushButton(this);
  list_add_button->setFont(buttonFont());
  list_add_button->setText("&Add");
  list_add_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  //
  //  Edit Button
  //
  list_edit_button=new QPushButton(this);
  list_edit_button->setFont(buttonFont());
  list_edit_button->setText("&Edit");
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  //  Delete Button
  //
  list_delete_button=new QPushButton(this);
  list_delete_button->setFont(buttonFont());
  list_delete_button->setText("&Delete");
  list_delete_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateEdit));
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  //  Generate Affidavit Button
  //
  list_affadavit_button=new QPushButton(this);
  list_affadavit_button->setFont(buttonFont());
  list_affadavit_button->setText("&Generate\nAffidavit");
  list_affadavit_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivReportView));
  connect(list_affadavit_button,SIGNAL(clicked()),this,SLOT(affadavitData()));

  //
  //  Affidavit Reminder Button
  //
  list_affidavit_reminder_button=new QPushButton(this);
  list_affidavit_reminder_button->setFont(buttonFont());
  list_affidavit_reminder_button->setText("Affidavit\n&Reminders");
  list_affidavit_reminder_button->setDisabled(true);
  connect(list_affidavit_reminder_button,SIGNAL(clicked()),
	  this,SLOT(affidavitReminderData()));
#ifdef WIN32
  list_affidavit_reminder_button->hide();
#endif  // WIN32

  //
  // Icon Legend
  //
  list_greenball_labels[0]=new QLabel(this);
  list_greenball_labels[0]->setPixmap(QPixmap(greenball_xpm));
  list_greenball_labels[1]=
    new QLabel("- "+tr("All Affidavits Up-To-Date"),this);
  list_greenball_labels[1]->setFont(defaultFont());
  
  list_redball_labels[0]=new QLabel(this);
  list_redball_labels[0]->setPixmap(QPixmap(redball_xpm));
  list_redball_labels[1]=new QLabel("- "+tr("Missing Affidavit(s)"),this);
  list_redball_labels[1]->setFont(defaultFont());

  list_blueball_labels[0]=new QLabel(this);
  list_blueball_labels[0]->setPixmap(QPixmap(blueball_xpm));
  list_blueball_labels[1]=new QLabel("- "+tr("Affidavits Not Tracked"),this);
  list_blueball_labels[1]->setFont(defaultFont());

  list_whiteball_labels[0]=new QLabel(this);
  list_whiteball_labels[0]->setPixmap(QPixmap(whiteball_xpm));
  list_whiteball_labels[1]=new QLabel("- "+tr("Not An Active Affiliate"),this);
  list_whiteball_labels[1]->setFont(defaultFont());

  //
  //  Close Button
  //
  list_close_button=new QPushButton(this);
  list_close_button->setDefault(true);
  list_close_button->setFont(buttonFont());
  list_close_button->setText("&Close");
  connect(list_close_button,SIGNAL(clicked()),this,SLOT(closeData()));

  showAffiliatesChangedData(true);
  missingAffiliateChangedData(false);
}


ListAffiliates::~ListAffiliates()
{
}


QSize ListAffiliates::sizeHint() const
{
  return QSize(850,480);
} 


QSizePolicy ListAffiliates::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListAffiliates::showAffiliatesChangedData(int state)
{
  list_missing_affiliates_check->setEnabled(state);
  list_missing_affiliates_label->setEnabled(state);
  list_month_box->setEnabled(state);
  list_year_box->setEnabled(state);
  if(!state) {
    list_missing_affiliates_check->setChecked(false);
    list_affidavit_reminder_button->setEnabled(false);
  }
  list_affiliates_model->refresh(state,MissingAffiliatesDate(),
				 list_callfilter_edit->text());
  list_affiliates_view->resizeColumnsToContents();
}


void ListAffiliates::filterTextChangedData(const QString &str)
{
  list_affiliates_model->refresh(list_show_affiliates_check->isChecked(),
				 MissingAffiliatesDate(),str);
  list_affiliates_view->resizeColumnsToContents();
}


void ListAffiliates::missingAffiliateChangedData(int state)
{
  list_month_box->setEnabled(state);
  list_year_box->setEnabled(state);
  list_affidavit_reminder_button->setEnabled(state);
  list_affiliates_model->refresh(true,MissingAffiliatesDate(),
				 list_callfilter_edit->text());
  list_affiliates_view->resizeColumnsToContents();
}


void ListAffiliates::monthActivatedData(int index)
{
  list_affiliates_model->refresh(true,MissingAffiliatesDate(),
				 list_callfilter_edit->text());
  list_affiliates_view->resizeColumnsToContents();
}


void ListAffiliates::yearActivatedData(int index)
{
  list_affiliates_model->refresh(true,MissingAffiliatesDate(),
				 list_callfilter_edit->text());
  list_affiliates_view->resizeColumnsToContents();
}


void ListAffiliates::addData()
{
  QString call;
  QString type;

  if(list_addaffiliate_dialog->exec(&call,&type)) {
    QString sql=QString("insert into `AFFILIATES` set ")+
      "`STATION_CALL`="+DvtSqlQuery::escape(call)+","+
      "`STATION_TYPE`="+DvtSqlQuery::escape(type);
    int affiliate_id=DvtSqlQuery::run(sql).toInt();
    if(list_editaffiliate_dialog->exec(affiliate_id)) {
      QModelIndex index=list_affiliates_model->addAffiliate(affiliate_id);
      if(index.isValid()) {
	list_affiliates_view->
	  selectionModel()->select(index,QItemSelectionModel::ClearAndSelect|
				   QItemSelectionModel::Rows);
      }
    }
    else {
      sql=QString("delete from `AFFILIATES` where ")+
	QString::asprintf("`AFFILIATES`.`ID`=%d ",affiliate_id);
      DvtSqlQuery::apply(sql);
    }
  }
}


void ListAffiliates::editData()
{
  QModelIndexList rows=list_affiliates_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  if(list_editaffiliate_dialog->
     exec(list_affiliates_model->affiliateId(rows.first()))) {
    list_affiliates_model->refresh(rows.first());
  }
}


void ListAffiliates::deleteData()
{
  QModelIndexList rows=list_affiliates_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  int affiliate_id=list_affiliates_model->affiliateId(rows.first());
  QString sql=QString("select ")+
    "`STATION_CALL`,"+  // 00
    "`STATION_TYPE` "+  // 01
    "from `AFFILIATES` where "+
    QString::asprintf("`ID`=%d",affiliate_id);
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  if(q->first()) {
    QString msg=tr("Are you sure you want to delete the affiliate records for")+
      " "+q->value(0).toString()+"-";
    if(q->value(1).toString()=="I") {
      msg+="Internet?\n\n";
    }
    else {
      msg+=q->value(1).toString()+"M?\n\n";
    }
    msg+=tr("WARNING: All associated data (schedule info, remarks, etc) will also be deleted!");
    delete q;
    if(QMessageBox::question(this,"Davit",msg,
			     QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
      return;
    }
  }
  else {
    QMessageBox::critical(this,"Davit - "+tr("Error"),
			  tr("Unable to find record for affiliate!"));
    return;
  }

  //
  // Delete Contacts
  //
  sql=QString("delete from `CONTACTS` where ")+
    QString::asprintf("`AFFILIATE_ID`=%d",affiliate_id);
  DvtSqlQuery::apply(sql);

  //
  // Delete Airings
  //
  sql=QString("delete from `AIRINGS` where ")+
    QString::asprintf("`AFFILIATE_ID`=%d",affiliate_id);
  DvtSqlQuery::apply(sql);

  //
  // Delete Aired
  //
  sql=QString("delete from `AIRED` where ")+
    QString::asprintf("`AFFILIATE_ID`=%d",affiliate_id);
  DvtSqlQuery::apply(sql);

  //
  // Delete Affiliate Remarks
  //
  sql=QString("delete from `AFFILIATE_REMARKS` where ")+
    QString::asprintf("`AFFILIATE_ID`=%d",affiliate_id);
  DvtSqlQuery::apply(sql);

  sql=QString("delete from `AFFILIATES` where ")+
    QString::asprintf("`AFFILIATES`.`ID`=%d ",affiliate_id);
  DvtSqlQuery::apply(sql);
  list_affiliates_model->removeAffiliate(rows.first());
}


void ListAffiliates::affadavitData()
{
  QModelIndexList rows=list_affiliates_view->selectionModel()->selectedRows();

  if(rows.size()!=1) {
    return;
  }
  list_generateaffadavit_dialog->
    execAffiliate(list_affiliates_model->affiliateId(rows.first()));
}


void ListAffiliates::affidavitReminderData()
{
  //
  // Get Selected Affiliates from the Model
  //
  QString sql=QString("select ")+
    "`AFFILIATES`.`ID`,"+             // 00
    "`CONTACTS`.`NAME`,"+             // 01
    "`CONTACTS`.`EMAIL`,"+            // 02
    "`AFFILIATES`.`STATION_CALL`,"+   // 03
    "`AFFILIATES`.`STATION_TYPE`,"+   // 04
    "`AFFILIATES`.`USER_PASSWORD` "+  // 05
    "from `AFFILIATES` left join `CONTACTS` "+
    "on `AFFILIATES`.`ID`=`CONTACTS`.`AFFILIATE_ID` where "+
    "(`CONTACTS`.`AFFIDAVIT`='Y')&&(";
  for(int i=0;i<list_affiliates_model->rowCount();i++) {
    sql+=QString::asprintf("(`AFFILIATES`.`ID`=%d)||",
			   list_affiliates_model->affiliateId(i));
  }
  sql=sql.left(sql.length()-2);
  sql+=")";
  DvtSqlQuery *q=new DvtSqlQuery(sql);

  //
  // Prompt for Confirmation
  //
  QString msg=tr("This will send an affidavit reminder e-mail to")+
			   QString::asprintf(" %d ",q->size())+
    tr("affiliates")+".\n";
  if(global_email_dry_run) {
    msg+=tr("--email-dry-run is active!")+"\n";
  }
  msg+=tr("Proceed?");
  if(QMessageBox::question(this,"Davit - List Affiliates",msg,
			   QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
    delete q;
    return;
  }

  //
  // Generate and Send Messages
  //
  while(q->next()) {
    QString subject;
    QString msg;
    subject=global_dvtsystem->affidavitEmailSubject();
    subject.replace("%S",DvtStationCallString(q->value(3).toString(),
					      q->value(4).toString()));
    subject.replace("%P",q->value(5).toString());
    subject.replace("%U",q->value(1).toString().toLower()+"-"+
		    q->value(2).toString().toLower()+"m");
    msg=global_dvtsystem->affidavitEmailTemplate();
    msg.replace("%S",DvtStationCallString(q->value(3).toString(),
					  q->value(4).toString()));
    msg.replace("%P",q->value(5).toString());
    msg.replace("%U",q->value(1).toString().toLower()+"-"+
		q->value(2).toString().toLower()+"m");
    QString err_msg;
    if(!DvtSendMail(&err_msg,subject,msg,global_dvtsystem->originEmail(),
		    QStringList(DvtFormatEmailAddress(q->value(1).toString(),
				    q->value(2).toString())),QStringList(),
		    QStringList(global_dvtuser->email()),
		    global_email_dry_run)) {
      QMessageBox::warning(this,"Davit - "+tr("Mailer Error"),
			   tr("An error occurred when sending mail to")+"\n"+
			   DvtFormatEmailAddress(q->value(1).toString(),
						 q->value(2).toString())+
			   ".\n\n"+
			   tr("The error message was")+": \""+err_msg+"\".");
    }
  }
  delete q;
}

void ListAffiliates::doubleClickedData(const QModelIndex &index)
{
  editData();
}


void ListAffiliates::rowCountChangedData(int matches)
{
  list_showing_edit->setText(QString::asprintf("%d",matches));
}


void ListAffiliates::closeData()
{
  done(true);
}


void ListAffiliates::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  list_show_affiliates_check->setGeometry(20,11,15,15);
  list_show_affiliates_label->setGeometry(40,8,1800,20);
  list_missing_affiliates_check->setGeometry(240,11,15,15);
  list_missing_affiliates_label->setGeometry(260,8,150,20);
  list_month_box->setGeometry(410,8,100,20);
  list_year_box->setGeometry(515,8,55,20);
  list_showing_edit->setGeometry(650,7,50,20);
  list_showing_label->setGeometry(585,8,60,20);
  list_callfilter_edit->setGeometry(790,7,50,20);
  list_callfilter_label->setGeometry(715,8,70,20);
  list_affiliates_view->setGeometry(10,32,w-20,h-102);
  list_add_button->setGeometry(10,h-60,80,50);
  list_edit_button->setGeometry(100,h-60,80,50);
  list_delete_button->setGeometry(190,h-60,80,50);
  list_affadavit_button->setGeometry(330,h-60,80,50);
  list_affidavit_reminder_button->setGeometry(430,h-60,80,50);

  list_greenball_labels[0]->setGeometry(550,h-68,20,20);
  list_greenball_labels[1]->setGeometry(570,h-68,w-200,20);
    
  list_redball_labels[0]->setGeometry(550,h-52,20,20);
  list_redball_labels[1]->setGeometry(570,h-52,w-200,20);
    
  list_blueball_labels[0]->setGeometry(550,h-36,20,20);
  list_blueball_labels[1]->setGeometry(570,h-36,w-200,20);
    
  list_whiteball_labels[0]->setGeometry(550,h-20,20,20);
  list_whiteball_labels[1]->setGeometry(570,h-20,w-200,20);
    
  list_close_button->setGeometry(w-90,h-60,80,50);
}


QDate ListAffiliates::MissingAffiliatesDate() const
{
  QDate ret;

  if(list_missing_affiliates_check->isChecked()) {
    ret=QDate(list_year_box->currentText().toInt(),
	      list_month_box->currentText().toInt()+1,1);
  }

  return ret;
}
