// missingaffidavitcontact_report.cpp
//
// Generate an Affiliate Missing Affidavit Contact List Report
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

#include <vector>

#include <QFile>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <dvtconf.h>

#include "list_reports.h"
#include "pick_fields.h"

bool ListReports::MissingAffidavitContactReport(SpreadSheet *sheet)
{
  QString s;
  QString sql;
  QSqlQuery *q=NULL;
  QSqlQuery *q1=NULL;
  QString where;
  int row=4;

  //
  // Generate Report
  //
  setBusyCursor();
  SpreadTab *tab=sheet->addTab(sheet->tabs()+1);
  tab->setName(tr("Missiing Affidavit Contacts"));

  tab->addCell(1,1)->setText(tr("Missing Affidavit Contacts"));
  tab->addCell(2,1)->setText(tr("Report Date")+": "+
			     QDate::currentDate().toString("MMMM dd, yyyy"));
  tab->addCell(1,3)->setText(tr("CALL LETTERS"));
  tab->addCell(2,3)->setText(tr("STATION PHONE"));
  sql=QString("select ID,STATION_CALL,STATION_TYPE,PHONE from AFFILIATES ")+
    "where IS_AFFILIATE=\"Y\"";
  q=new QSqlQuery(sql);
  while(q->next()) {
    sql=QString("select ID from CONTACTS where ")+
      QString::asprintf("(AFFILIATE_ID=%d)&&",q->value(0).toInt())+
      "(CONTACTS.AFFIDAVIT=\"Y\")&&"+
      "((CONTACTS.EMAIL!=\"\")&&(CONTACTS.EMAIL is not null))||"+
      "((CONTACTS.PHONE!=\"\")&&(CONTACTS.PHONE is null))";
    q1=new QSqlQuery(sql);
    if(!q1->first()) {
      // Call Letters
      tab->addCell(1,row)->
	setText(DvtStationCallString(q->value(1).toString(),
				     q->value(2).toString()));
    
      // Phone
      tab->addCell(2,row)->
	setText(DvtFormatPhoneNumber(q->value(3).toString()));
      row++;
    }
    delete q1;
  }
  delete q;

  return true;
}
