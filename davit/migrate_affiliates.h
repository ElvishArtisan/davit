// migrate_affiliates.h
//
// Migrate Davit Affiliates.
//
//   (C) Copyright 2007,2008 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: migrate_affiliates.h,v 1.1 2008/12/19 17:03:01 fredg Exp $
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

#include <qdialog.h>
#include <qcombobox.h>

#include <dvtlistselector.h>

class MigrateAffiliates : public QDialog
{
 Q_OBJECT
 public:
  MigrateAffiliates(int pgm_id,const QString &pgm_name,
		    QWidget *parent=0,const char *name=0);
  ~MigrateAffiliates();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void programActivatedData(const QString &str);
  void okData();
  void cancelData();

 private:
  int GetProgramId(const QString &str) const;
  int GetAffiliateId(const QString &str) const;
  int edit_id;
  QString edit_program_name;
  QComboBox *edit_program_box;
  DvtListSelector *edit_affiliates_sel;
};


#endif  // MIGRATE_AFFILIATES_H
