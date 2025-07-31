// migrate_affiliates.cpp
//
// Migrate a Davit Affiliates.
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

#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QRadioButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QValidator>

#include <globals.h>
#include <dvt.h>
#include <dvtconf.h>

#include <migrate_affiliates.h>

MigrateAffiliates::MigrateAffiliates(int pgm_id,const QString &pgm_name,
				     QWidget *parent)
  : QDialog(parent)
{
  setModal(true);
  edit_id=pgm_id;
  edit_program_name=pgm_name;
  QString sql;
  QSqlQuery *q;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setWindowTitle("Davit - Migrate Affiliates");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont big_font=QFont("Helvetica",16,QFont::Bold);
  big_font.setPixelSize(16);

  //
  // Source Program
  //
  edit_program_box=new QComboBox(this);
  edit_program_box->setGeometry(200,10,sizeHint().width()-210,20);
  QLabel *label=new QLabel(tr("Migrate Affiliates From:"),this);
  label->setGeometry(10,10,185,20);
  label->setFont(label_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  connect(edit_program_box,SIGNAL(activated(const QString &)),
	  this,SLOT(programActivatedData(const QString &)));

  //
  // Affiliate Selector
  //
  edit_affiliates_sel=new ListSelector(this);
  //  edit_affiliates_sel->
  //    setGeometry(10,40,sizeHint().width()-20,sizeHint().height()-110);
  edit_affiliates_sel->destSetLabel(pgm_name);

  //
  //  OK Button
  //
  QPushButton *button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(label_font);
  button->setText("&OK");
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText("&Cancel");
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Data
  //
  sql=QString::asprintf("select PROGRAM_NAME from PROGRAMS where ID!=%d \
                         order by PROGRAM_NAME",edit_id);
  int count=0;
  q=new QSqlQuery(sql);
  while(q->next()) {
    edit_program_box->insertItem(count++,q->value(0).toString());
  }
  delete q;
  programActivatedData(edit_program_box->currentText());
}


MigrateAffiliates::~MigrateAffiliates()
{
}


QSize MigrateAffiliates::sizeHint() const
{
  return QSize(600,400);
} 


QSizePolicy MigrateAffiliates::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void MigrateAffiliates::programActivatedData(const QString &str)
{
  QString sql;
  QSqlQuery *q1;
  QSqlQuery *q2;
  int pgm_id=0;

  edit_affiliates_sel->clear();
  edit_affiliates_sel->sourceSetLabel(str);
  if((pgm_id=GetProgramId(str))>0) {
    sql="select ID,STATION_CALL,STATION_TYPE,LICENSE_CITY,LICENSE_STATE \
              from AFFILIATES order by STATION_CALL,STATION_TYPE";
    q1=new QSqlQuery(sql);
    while(q1->next()) {
      sql=QString::asprintf("select ID from AIRINGS \
                           where (AFFILIATE_ID=%d)&&(PROGRAM_ID=%d)",
			    q1->value(0).toInt(),pgm_id);
      q2=new QSqlQuery(sql);
      if(q2->size()>0) {
	QString station=q1->value(1).toString();
	if(q1->value(2).toString().toLower()=="a") {
	  station+="-AM";
	}
	if(q1->value(2).toString().toLower()=="f") {
	  station+="-FM";
	}
	station+=" ";
	station+=q1->value(3).toString();
	station+=", ";
	station+=q1->value(4).toString().toUpper();
	edit_affiliates_sel->sourceInsertItem(station);
      }
      delete q2;
    }
    delete q1;
  }
}


void MigrateAffiliates::okData()
{
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
  int pgm_id=0;
  int affiliate_id=0;
  QString dt=QDateTime(QDate::currentDate(),QTime::currentTime()).
    toString("yyyy-MM-dd hh:mm:ss");

  if((pgm_id=GetProgramId(edit_program_box->currentText()))<=0) {
    return;
  }
  for(unsigned i=0;i<edit_affiliates_sel->destCount();i++) {
    affiliate_id=GetAffiliateId(edit_affiliates_sel->destText(i));
    sql=QString::asprintf("insert into AFFILIATE_REMARKS set \
                           REMARK_DATETIME=\"%s\",\
                           EVENT_TYPE=%d,\
                           AFFILIATE_ID=%d,\
                           PROGRAM_ID=%d,\
                           USER_NAME=\"%s\",\
                           REMARK=\"Deleted an airing of %s.\"",
			  dt.toUtf8().constData(),
			  Dvt::RemarkProgramDelete,
			  affiliate_id,
			  pgm_id,
			  global_dvtuser->name().toUtf8().constData(),
			  edit_program_box->currentText().toUtf8().constData());
    q=new QSqlQuery(sql);
    delete q;
    sql=QString::asprintf("insert into AFFILIATE_REMARKS set \
                           REMARK_DATETIME=\"%s\",\
                           EVENT_TYPE=%d,\
                           AFFILIATE_ID=%d,\
                           PROGRAM_ID=%d,\
                           USER_NAME=\"%s\",\
                           REMARK=\"Added an airing of %s.\"",
			  dt.toUtf8().constData(),
			  Dvt::RemarkProgramAdd,
			  affiliate_id,
			  edit_id,
			  global_dvtuser->name().toUtf8().constData(),
			  edit_program_name.toUtf8().constData());
    q=new QSqlQuery(sql);
    delete q;
    sql=QString::asprintf("select AIR_SUN,AIR_MON,AIR_TUE,AIR_WED,AIR_THU,\
                           AIR_FRI,AIR_SAT,AIR_TIME,AIR_LENGTH from AIRINGS \
                           where (PROGRAM_ID=%d)&&(AFFILIATE_ID=%d)",
			  pgm_id,affiliate_id);
    q=new QSqlQuery(sql);
    while(q->next()) {
      sql=QString::asprintf("insert into AIRINGS set \
                             AIR_SUN=\"%s\",\
                             AIR_MON=\"%s\",\
                             AIR_TUE=\"%s\",\
                             AIR_WED=\"%s\",\
                             AIR_THU=\"%s\",\
                             AIR_FRI=\"%s\",\
                             AIR_SAT=\"%s\",\
                             AIR_TIME=\"%s\",\
                             AIR_LENGTH=%d,\
                             PROGRAM_ID=%d,\
                             AFFILIATE_ID=%d",
			    q->value(0).toString().toUtf8().constData(),
			    q->value(1).toString().toUtf8().constData(),
			    q->value(2).toString().toUtf8().constData(),
			    q->value(3).toString().toUtf8().constData(),
			    q->value(4).toString().toUtf8().constData(),
			    q->value(5).toString().toUtf8().constData(),
			    q->value(6).toString().toUtf8().constData(),
			    q->value(7).toTime().
			    toString("hh:mm:ss").toUtf8().constData(),
			    q->value(8).toInt(),
			    edit_id,
			    affiliate_id);
//      printf("SQL: %s\n",sql);
      q1=new QSqlQuery(sql);
      delete q1;
      sql=QString::asprintf("delete from AIRINGS \
                             where (PROGRAM_ID=%d)&&(AFFILIATE_ID=%d)",
			    pgm_id,affiliate_id);
      q1=new QSqlQuery(sql);
      delete q1;
    }
    delete q;
  }
  done(0);
}


void MigrateAffiliates::cancelData()
{
  done(-1);
}


int MigrateAffiliates::GetProgramId(const QString &str) const
{
  QString sql;
  QSqlQuery *q;
  int id=0;
  sql=QString::asprintf("select ID from PROGRAMS where PROGRAM_NAME=\"%s\"",
			DvtEscapeString(str).toUtf8().constData());
  q=new QSqlQuery(sql);
  if(q->first()) {
    id=q->value(0).toInt();
  }
  delete q;
  return id;
}


int MigrateAffiliates::GetAffiliateId(const QString &str) const
{
  QString sql;
  QSqlQuery *q;
  int id=0;
  QString name=str.left(str.indexOf(" "));
  int n=name.indexOf("-");
  if(n<0) {
    //    printf("SQL: %s\n",sql.toUtf8().constData());
    sql=QString::asprintf("select ID from AFFILIATES where STATION_CALL=\"%s\"",
			  DvtEscapeString(name).toUtf8().constData());
    q=new QSqlQuery(sql);
    if(q->first()) {
      id=q->value(0).toInt();
    }
    delete q;
    return id;
  }
  sql=QString::asprintf("select ID from AFFILIATES where (STATION_CALL=\"%s\")\
                         &&(STATION_TYPE=\"%s\")",
			DvtEscapeString(name.left(n)).toUtf8().constData(),
			name.right(2).left(1).toLower().toUtf8().constData());
  q=new QSqlQuery(sql);
  if(q->first()) {
    id=q->value(0).toInt();
  }
  delete q;
  return id;
}
