// list_remarks.cpp
//
// List Davit Remarks.
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

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <dvtconf.h>

#include "add_remark.h"
#include "globals.h"
#include "list_remarks.h"

ListRemarks::ListRemarks(DvtConfig *c,QWidget *parent)
  : DvtDialog(c,parent)
{
  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  list_remarks_edit=new QTextEdit(this);
  list_remarks_edit->setReadOnly(true);
  list_remarks_edit->setAcceptRichText(true);

  list_add_button=new QPushButton(this);
  list_add_button->setFont(label_font);
  list_add_button->setText(tr("Add"));
  list_add_button->
    setEnabled(global_dvtuser->privilege(DvtUser::PrivAffiliateRemark));
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  RefreshList();
}


ListRemarks::~ListRemarks()
{
}


QSize ListRemarks::sizeHint() const
{
  return QSize(615,300);
}


void ListRemarks::setAffiliateId(int id)
{
  list_id=id;
  setWindowTitle("Davit - "+tr("Affiliate History for")+" "+
		 DvtStationCallString(id));
}


void ListRemarks::show()
{
  QDialog::show();
  emit visibilityChanged(true);
}


void ListRemarks::hide()
{
  QDialog::hide();
  emit visibilityChanged(false);
}


QSizePolicy ListRemarks::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListRemarks::refresh()
{
  RefreshList();
}


void ListRemarks::addData()
{
  QString remark;
  QString sql;
  QSqlQuery *q;

  AddRemark *d=new AddRemark(&remark,this);
  if(d->exec()==0) {
    sql=QString::asprintf("insert into AFFILIATE_REMARKS set \
                           AFFILIATE_ID=%d,\
                           EVENT_TYPE=%d,\
                           REMARK_DATETIME=now(),\
                           USER_NAME=\"%s\",\
                           REMARK=\"%s\"",
			  list_id,
			  Dvt::RemarkNarrative,
			  DvtEscapeString(global_dvtuser->name()).
			  toUtf8().constData(),
			  DvtEscapeString(remark).toUtf8().constData());
    q=new QSqlQuery(sql);
    delete q;
  }
  delete d;
  RefreshList();
}


void ListRemarks::resizeEvent(QResizeEvent *e)
{
  list_remarks_edit->setGeometry(0,0,size().width(),size().height()-40);
  list_add_button->setGeometry(10,size().height()-35,50,30);
}


void ListRemarks::RefreshList()
{
  QString sql;
  QSqlQuery *q;
  QString remarks;
  sql=QString::asprintf("select REMARK_DATETIME,USER_NAME,REMARK \
                         from AFFILIATE_REMARKS where AFFILIATE_ID=%d \
                         order by REMARK_DATETIME desc",list_id);
  q=new QSqlQuery(sql);
  while(q->next()) {
    remarks+=("On <strong>"+q->value(0).toDateTime().toString("MM/dd/yyyy"));
    remarks+=(" @ "+q->value(0).toDateTime().toString("h:mm ap")+
	      "</strong>");
    remarks+=(" by <strong>"+q->value(1).toString()+"</strong>");
    remarks+=("<br>"+q->value(2).toString()+"\n");
    remarks+="<hr>";
  }
  list_remarks_edit->setText(remarks);
  delete q;
}
