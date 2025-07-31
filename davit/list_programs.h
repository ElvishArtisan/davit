// list_programs.h
//
// List Davit Programs.
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

#ifndef LIST_PROGRAMS_H
#define LIST_PROGRAMS_H

#include <QPushButton>

#include <dvtconfig.h>

#include "dialog.h"
#include "edit_program.h"
#include "generate_affadavit.h"
#include "programlistmodel.h"
#include "tableview.h"

class ListPrograms : public Dialog
{
 Q_OBJECT
 public:
  ListPrograms(DvtConfig *c,QWidget *parent=0);
  ~ListPrograms();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(int provider_id=-1);   

 private slots:
  void addData();
  void editData();
  void deleteData();
  void affidavitData();
  void doubleClickedData(const QModelIndex &index);
  void closeData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  TableView *list_programs_view;
  ProgramListModel *list_programs_model;
  QPushButton *list_add_button;
  QPushButton *list_edit_button;
  QPushButton *list_delete_button;
  QPushButton *list_affidavit_button;
  QPushButton *list_close_button;
  int list_provider_id;
  EditProgram *list_editprogram_dialog;
  GenerateAffadavit *list_generateaffadavit_dialog;
};


#endif  // LIST_PROGRAMS_H
