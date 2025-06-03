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

#include <dvtconf.h>
#include <dvtdb.h>

#include "add_remark.h"
#include "globals.h"
#include "list_remarks.h"

ListRemarks::ListRemarks(DvtConfig *c,QWidget *parent)
  : DvtDialog(c,parent)
{
  setModal(true);

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
  RefreshList();
}


void ListRemarks::show()
{
  RefreshList();
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

  AddRemark *d=new AddRemark(&remark,this);
  if(d->exec()==0) {
    sql=QString("insert into `AFFILIATE_REMARKS` set ")+
      QString::asprintf("`AFFILIATE_ID`=%d,",list_id)+
      QString::asprintf("`EVENT_TYPE`=%d,",Dvt::RemarkNarrative)+
      "`REMARK_DATETIME`=now(),"+
      "`USER_NAME`="+DvtSqlQuery::escape(global_dvtuser->name())+","+
      "`REMARK`="+DvtSqlQuery::escape(remark);
    DvtSqlQuery::apply(sql);
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
  DvtSqlQuery *q;
  QString remarks;
  sql=QString("select ")+
    "`REMARK_DATETIME`,"+  // 00
    "`USER_NAME`,"+        // 01
    "`REMARK` "+           // 02
    "from `AFFILIATE_REMARKS` where "+
    QString::asprintf("`AFFILIATE_ID`=%d ",list_id)+
    "order by `REMARK_DATETIME` desc";
  q=new DvtSqlQuery(sql);
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
