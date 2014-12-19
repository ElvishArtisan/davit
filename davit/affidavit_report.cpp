// affidavit_report.cpp
//
// Generate an Affiliate Affidavit Contact List Report
//
//   (C) Copyright 2010-2014 Fred Gleason <fredg@paravelsystems.com>
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

#include <qfile.h>
#include <qfontmetrics.h>
#include <qsqldatabase.h>
#include <qmessagebox.h>

#include <dvttextfile.h>
#include <dvtconf.h>
#include <spread_sheet.h>

#include <list_reports.h>


void ListReports::AffidavitReport()
{
  QString s;
  QString sql;
  QSqlQuery *q;
  QString outfile;
  FILE *f=NULL;
  int row=4;
  std::vector<int> ids;
  QString str;

  //
  // Generate Fonts
  //
  QFont main_font("arial",10,QFont::Normal);
  QFontMetrics *fm=new QFontMetrics(main_font);

  //
  // Generate Report
  //
  if((f=GetTempFile(&outfile))==NULL) {
    return;
  }
  fclose(f);
  SpreadSheet *sheet=new SpreadSheet();
  SpreadTab *tab=sheet->addTab(1);
  tab->setName(tr("Affidavit Contacts"));

  tab->addCell(1,1)->setText(tr("Affiliate Affidavit Contacts"),fm);
  tab->addCell(1,2)->setText(tr("Report Date")+": "+
			     QDate::currentDate().toString("MM/dd/yyyy"),fm);
  tab->addCell(1,3)->setText(tr("CALL LETTERS"),fm);
  tab->addCell(2,3)->setText(tr("AFFIDAVIT CONTACT NAME"),fm);
  tab->addCell(3,3)->setText(tr("AFFIDAVIT CONTACT PHONE"),fm);
  tab->addCell(4,3)->setText(tr("AFFIDAVIT CONTACT E-MAIL"),fm);
  sql="select ID,STATION_CALL,STATION_TYPE from AFFILIATES \
       where (AFFIDAVIT_ACTIVE=\"Y\") order by STATION_CALL desc";
  q=new QSqlQuery(sql);
  while(q->next()) {
    // Call Letters
    tab->addCell(1,row)->
      setText(DvtStationCallString(q->value(1).toString(),
				   q->value(2).toString()),fm);
    ContactFields(q->value(0).toInt(),
		  ListReports::AffidavitContact,
		  ListReports::FieldEmail|
		  ListReports::FieldName|
		  ListReports::FieldPhone,tab,2,row,fm);
    row++;
  }
  delete q;
  delete fm;
  ForkViewer(outfile,sheet->write(SpreadObject::ExcelXmlFormat));
}
