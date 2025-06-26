// edit_network.cpp
//
// Edit a Davit Network.
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

#include <QMessageBox>

#include <dvtdb.h>

#include "edit_network.h"


EditNetwork::EditNetwork(DvtConfig *c,QWidget *parent)
  : DvtDialog(c,parent)
{
  setModal(true);
  edit_network_id=-1;

  //
  // Fix the Window Size
  //
  setMinimumSize(sizeHint());
  setMaximumSize(sizeHint());

  //
  // Network Name
  //
  edit_network_edit=new QLineEdit(this);
  edit_network_edit->setMaxLength(64);
  edit_network_label=new QLabel("Network Name:",this);
  edit_network_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_network_label->setFont(labelFont());

  //
  //  OK Button
  //
  edit_ok_button=new QPushButton(this);
  edit_ok_button->setDefault(true);
  edit_ok_button->setFont(buttonFont());
  edit_ok_button->setText("&OK");
  connect(edit_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  edit_cancel_button=new QPushButton(this);
  edit_cancel_button->setFont(buttonFont());
  edit_cancel_button->setText("&Cancel");
  connect(edit_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


EditNetwork::~EditNetwork()
{
}


QSize EditNetwork::sizeHint() const
{
  return QSize(300,110);
} 


QSizePolicy EditNetwork::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


int EditNetwork::exec(int network_id,bool new_entry)
{
  edit_network_id=network_id;

  setWindowTitle("Davit - "+tr("Edit Network")+
		 QString::asprintf(" [Id: %d]",network_id));

  QString sql=QString("select ")+
    "`NAME` "+  // 00
    "from `NETWORKS` where "+
    QString::asprintf("`ID`=%d",network_id);
  DvtSqlQuery *q=new DvtSqlQuery(sql);
  if(q->first()) {
    edit_network_edit->setText(q->value(0).toString());
    if(new_entry) {
      edit_network_edit->selectAll();
    }
  }
  delete q;

  return QDialog::exec();
}


void EditNetwork::okData()
{
  QString sql=
    QString::asprintf("update `NETWORKS` set ")+
    "`NAME`="+DvtSqlQuery::escape(edit_network_edit->text())+" where "+
    QString::asprintf("`ID`=%d",edit_network_id);
  DvtSqlQuery::apply(sql);

  done(true);
}


void EditNetwork::cancelData()
{
  done(false);
}


void EditNetwork::resizeEvent(QResizeEvent *e)
{
  int w=size().width();
  int h=size().height();

  edit_network_label->setGeometry(10,10,95,20);
  edit_network_edit->setGeometry(110,10,w-120,20);
  edit_ok_button->setGeometry(w-180,h-60,80,50);
  edit_cancel_button->setGeometry(w-90,h-60,80,50);
}


void EditNetwork::closeEvent(QCloseEvent *e)
{
  cancelData();
}
