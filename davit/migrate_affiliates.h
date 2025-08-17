// migrate_affiliates.h
//
// Migrate Davit Affiliates.
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

#ifndef MIGRATE_AFFILIATES_H
#define MIGRATE_AFFILIATES_H

#include <vector>

#include <QComboBox>
#include <QLabel>

#include <dvtdb.h>

#include "dialog.h"
#include "listselector.h"

class MigrateAffiliates : public Dialog
{
 Q_OBJECT
 public:
 MigrateAffiliates(DvtConfig *c,QWidget *parent=0);
  ~MigrateAffiliates();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(int pgm_id,const QString &pgm_name);

 private slots:
  void programActivatedData(const QString &str);
  void destItemsQuantityChangedData(int quan);
  void okData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);
  
 private:
  QString GetDowSummary(DvtSqlQuery *q) const;
  int GetProgramId(const QString &str) const;
  int GetAffiliateId(const QString &str) const;
  QPushButton *list_ok_button;
  QPushButton *list_cancel_button;
  int edit_program_id;
  QString edit_program_name;
  QLabel *edit_program_label;
  QComboBox *edit_program_box;
  ListSelector *edit_affiliates_sel;
};


#endif  // MIGRATE_AFFILIATES_H
