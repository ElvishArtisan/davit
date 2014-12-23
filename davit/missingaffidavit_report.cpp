// missingaffidavit_report.cpp
//
// Generate an Affiliate Missing Affidavit List Report
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

#include <map>
#include <vector>

#include <qfile.h>
#include <qsqldatabase.h>
#include <qmessagebox.h>

#include <dvttextfile.h>
#include <dvtconf.h>
#include <spread_sheet.h>

#include <affidavit_picker.h>
#include <list_reports.h>


void ListReports::MissingAffidavitReport(SpreadSheet *sheet)
{
  QString s;
  QString sql;
  QSqlQuery *q=NULL;
  QString where;
  std::vector<int> affiliate_ids;
  std::map<int,int> affiliate_counts;
  Dvt::AffidavitStationFilter filter=Dvt::All;
  Dvt::AffidavitSortType sort_type=Dvt::ByCount;
  int program_id=-1;

  //
  // Get Parameters
  //
  QDate end_date=QDate::currentDate();
  end_date.setYMD(end_date.year(),end_date.month(),1);
  QDate start_date=end_date.addYears(-1);
  AffidavitPicker *d=new AffidavitPicker(&filter,&sort_type,&program_id,this);
  if(d->exec()!=0) {
    delete d;
    return;
  }
  delete d;

  //
  // Generate Report
  //
  SpreadTab *tab=sheet->addTab(sheet->tabs()+1);
  tab->addCell(1,1)->setText(tr("Affiliates Missing Affidavits"));
  tab->cell(1,1)->setWidth(120.0);
  tab->addCell(1,2)->setText(tr("Report Date")+":"+
			     QDate::currentDate().toString("MMMM dd, yyyy"));
  switch(filter) {
  case Dvt::All:
    tab->addCell(1,3)->setText(tr("All Programs"));
    break;

  case Dvt::Weekday:
    tab->addCell(1,3)->setText(tr("All Weekday Programs"));
    break;

  case Dvt::Weekend:
    tab->addCell(1,3)->setText(tr("All Weekend Programs"));
    break;

  case Dvt::Program:
    sql=QString("select PROGRAM_NAME from PROGRAMS ")+
      QString().sprintf("where ID=%d",program_id);
    q=new QSqlQuery(sql);
    if(q->first()) {
      tab->addCell(1,3)->setText(q->value(0).toString());
    }
    else {
      tab->addCell(1,3)->setText(tr("INTERNAL ERROR!"));
    }
    delete q;
    break;
  }
  tab->addCell(1,5)->setText(tr("CALL LETTERS"));
  tab->addCell(2,5)->setText(tr("NAME"));
  tab->addCell(3,5)->setText(tr("PHONE"));
  tab->addCell(4,5)->setText(tr("E-MAIL"));
  tab->addCell(5,5)->setText(tr("MISSING"));
  tab->addCell(6,5)->setText(tr("STATE"));
  tab->addCell(7,5)->setText(tr("DMA MARKET"));
  tab->addCell(8,5)->setText(tr("MSA MARKET"));
  int row=6;

  DvtAffidavitNeeded(&affiliate_ids,&affiliate_counts,
		     start_date,end_date,filter,program_id);
  sql=QString("select AFFILIATES.ID,AFFILIATES.STATION_CALL,")+
    "AFFILIATES.STATION_TYPE,"+
    "AFFILIATES.LICENSE_STATE,AFFILIATES.DMA_NAME,AFFILIATES.MARKET_NAME "+
    "from AFFILIATES "+
    "where ";
  for(unsigned i=0;i<affiliate_ids.size();i++) {
    sql+=QString().sprintf("(AFFILIATES.ID=%d)||",affiliate_ids[i]);
  }
  sql=sql.left(sql.length()-2)+" order by LICENSE_STATE";
  q=new QSqlQuery(sql);
  std::vector<int> index;
  for(int i=0;i<q->size();i++) {
    index.push_back(i);
  }

  if(sort_type==Dvt::ByCount) {
    //
    // Build Offender Counts
    //
    std::vector<unsigned> offenders;
    for(int i=0;i<q->size();i++) {
      std::vector<QDate> dates;
      q->seek(index[i]);
      offenders.push_back(DvtAffidavitNeededDates(&dates,q->value(0).toInt(),
						  start_date,end_date));
    }

    //
    // Bubble Sort
    //
    bool modified=true;
    while(modified) {
      modified=false;
      for(int i=1;i<q->size();i++) {
	if(offenders[index[i-1]]<offenders[index[i]]) {
	  int val=index[i-1];
	  index[i-1]=index[i];
	  index[i]=val;
	  modified=true;
	}
      }
    }
  }

  for(int i=0;i<q->size();i++) {
    q->seek(index[i]);
    // Call Letters
    tab->addCell(1,row)->
      setText(DvtStationCallString(q->value(1).toString(),
				   q->value(2).toString()));

    //
    // Contact Info
    //
    QString name;
    QString phone;
    QString email;

    DvtContactInfo(&name,NULL,&email,&phone,NULL,q->value(0).toInt(),
		   Dvt::AffidavitContact);
    tab->addCell(2,row)->setText(name);  // Contact Name
    tab->addCell(3,row)->setText(DvtFormatPhoneNumber(phone));  // Contact Phone
    tab->addCell(4,row)->setText(email);    // Contact E-Mail Address

    // Missing Months
    std::vector<QDate> dates;
    QString date_str="";
    if(DvtAffidavitNeededDates(&dates,q->value(0).toInt(),
			       start_date,end_date)) {
      for(unsigned i=0;i<dates.size();i++) {
	date_str+=dates[i].toString("M/yy")+", ";
      }
      date_str=date_str.left(date_str.length()-2);
    }
    tab->addCell(5,row)->setText(date_str);
    tab->addCell(6,row)->setText(q->value(3).toString().upper());  // State of License
    tab->addCell(7,row)->setText(q->value(4).toString());    // DMA Market
    tab->addCell(8,row)->setText(q->value(5).toString());    // MSA Market

    row++;
  }
  delete q;
}
