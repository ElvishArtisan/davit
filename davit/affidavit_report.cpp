// affidavit_report.cpp
//
// Generate an Affiliate Affidavit Contact List Report
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
#include <QFontMetrics>
#include <QMessageBox>

#include <dvtconf.h>
#include <dvtdb.h>
#include <spread_sheet.h>

#include "list_reports.h"

bool ListReports::AffidavitReport(SpreadSheet *sheet)
{
  QString s;
  QString sql;
  DvtSqlQuery *q;
  int row=4;
  std::vector<int> ids;
  QString str;

  //
  // Generate Report
  //
  setBusyCursor();
  SpreadTab *tab=sheet->addTab(sheet->tabs()+1);
  tab->setName(tr("Affidavit Contacts"));

  tab->addCell(1,1)->setText(tr("Affiliate Affidavit Contacts"));
  tab->addCell(1,2)->setText(tr("Report Date")+": "+
			     QDate::currentDate().toString("MM/dd/yyyy"));
  tab->addCell(1,3)->setText(tr("CALL LETTERS"));
  tab->addCell(2,3)->setText(tr("AFFIDAVIT CONTACT NAME"));
  tab->addCell(3,3)->setText(tr("AFFIDAVIT CONTACT PHONE"));
  tab->addCell(4,3)->setText(tr("AFFIDAVIT CONTACT E-MAIL"));
  sql=QString("select ")+
    "`ID`,"+            // 00
    "`STATION_CALL`,"+  // 01
    "`STATION_TYPE` "+  // 02
    "from `AFFILIATES` where "+
    "(`AFFIDAVIT_ACTIVE`='Y') "+
    "order by `STATION_CALL` desc";
  q=new DvtSqlQuery(sql);
  while(q->next()) {
    // Call Letters
    tab->addCell(1,row)->
      setText(DvtStationCallString(q->value(1).toString(),
				   q->value(2).toString()));
    ContactFields(q->value(0).toInt(),
		  ListReports::AffidavitContact,
		  ListReports::FieldEmail|
		  ListReports::FieldName|
		  ListReports::FieldPhone,tab,2,row);
    row++;
  }
  delete q;

  return true;
}
