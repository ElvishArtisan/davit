// generate_affadavit.h
//
// Generate an Affadavit.
//
//   (C) Copyright 2002-2008 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: generate_affadavit.h,v 1.6 2011/03/29 15:38:51 pcvs Exp $
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

#ifndef GENERATE_AFFADAVIT_H
#define GENERATE_AFFADAVIT_H

#include <qdialog.h>
#include <qsqldatabase.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qpushbutton.h>

#define AFFADAVIT_WIDTH 80

class GenerateAffadavit : public QDialog
{
  Q_OBJECT
 public:
  enum ReportType {ReportAffiliate=0,ReportProgram=1};
  GenerateAffadavit(ReportType type,int id,
		    QWidget *parent=0,const char *name=0);
  ~GenerateAffadavit();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 private slots:
  void validateDateData(int index);
  void generateData();
  void closeData();

 private:
  //  void RenderAffidavit(QPainter*p);
  //  void PrintAffidavit();
  void AffiliateReport(int id,const QDate &start_date,
		       const QDate &end_date,bool pgms);
  void ProgramReport(int id,const QDate &start_date,
		     const QDate &end_date,bool affiliates);
  QString Center(const QString &s);
  QComboBox *edit_month_box;
  QComboBox *edit_year_box;
  QComboBox *edit_program_box;
  QCheckBox *edit_airings_check;
  QPushButton *edit_generate_button;
  ReportType edit_type;
  int edit_id;
};


#endif  // GENERATE_AFFADAVIT_H
