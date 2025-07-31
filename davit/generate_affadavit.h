// generate_affadavit.h
//
// Generate an Affadavit.
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QPrinter>
#include <QPushButton>

#include "dialog.h"

#define AFFADAVIT_WIDTH 80

class GenerateAffadavit : public Dialog
{
  Q_OBJECT
 public:
  enum ReportType {ReportAffiliate=0,ReportProgram=1};
  GenerateAffadavit(DvtConfig *c,QWidget *parent=0);
  ~GenerateAffadavit();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int execAffiliate(int affiliate_id);
  int execProgram(int program_id);

 private slots:
  void updateGenerateButtonData(int index);
  void generateData();
  void closeData();
  void paintAffiliateReport(QPrinter *printer); // affidavitaffiliatereport.cpp
  void paintProgramReport(QPrinter *printer);   // affidavitprogramreport.cpp

 protected:
  void resizeEvent(QResizeEvent *e);
  void closeEvent(QCloseEvent *e);

 private:
  //  void RenderAffidavit(QPainter*p);
  //  void PrintAffidavit();
  void AffiliateReport(int id,const QDate &start_date,
		       const QDate &end_date,bool pgms);
  void ProgramReport(int id,const QDate &start_date,
  		     const QDate &end_date,bool affiliates);
  bool HasAffidavits() const;
  QDate SelectedDate() const;
  QString Center(const QString &s);
  void NewLine(QPrinter *printer,QPainter *p,int *ypos);
  int PrintText(int ypos,const QString &str,QPainter *p);
  QLabel *edit_date_label;
  QComboBox *edit_month_box;
  QComboBox *edit_year_box;
  QLabel *edit_program_label;
  QComboBox *edit_program_box;
  QCheckBox *edit_airings_check;
  QLabel *edit_airings_label;
  QPushButton *edit_generate_button;
  QPushButton *edit_close_button;
  ReportType edit_type;
  int edit_affiliate_id;
  int edit_program_id;
};


#endif  // GENERATE_AFFADAVIT_H
