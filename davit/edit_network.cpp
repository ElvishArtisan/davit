// edit_network.cpp
//
// Edit a Davit Network.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: edit_network.cpp,v 1.2 2011/06/14 21:47:07 pcvs Exp $
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

#include <qpushbutton.h>
#include <qlabel.h>
#include <qsqldatabase.h>
#include <qmessagebox.h>

#include <math.h>

#include <edit_network.h>


EditNetwork::EditNetwork(QString *network,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  edit_network=network;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Davit - Edit Network");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // Network Name
  //
  edit_network_edit=new QLineEdit(this,"edit_network_edit");
  edit_network_edit->setGeometry(110,10,sizeHint().width()-120,20);
  edit_network_edit->setFont(font);
  edit_network_edit->setMaxLength(64);
  edit_network_edit->setText(*network);
  QLabel *label=
    new QLabel(edit_network_edit,"Network Name:",this,"edit_network_label");
  label->setGeometry(10,10,95,20);
  label->setAlignment(AlignRight|AlignVCenter);
  label->setFont(label_font);

  //
  //  OK Button
  //
  QPushButton *button=new QPushButton(this,"ok_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(label_font);
  button->setText("&OK");
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this,"cancel_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText("&Cancel");
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));
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


void EditNetwork::okData()
{
  QString sql=
    QString().sprintf("update NETWORKS set NAME=\"%s\" where NAME=\"%s\"",
		      (const char *)DvtEscapeString(edit_network_edit->text()),
		      (const char *)DvtEscapeString(edit_network_edit->text()));
  QSqlQuery *q=new QSqlQuery(sql);
  delete q;
  *edit_network=edit_network_edit->text();
  done(0);
}


void EditNetwork::cancelData()
{
  done(-1);
}
