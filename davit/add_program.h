// add_program.h
//
// Add a Davit Program.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: add_program.h,v 1.1 2007/03/21 13:38:36 fredg Exp $
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

#ifndef ADD_PROGRAM_H
#define ADD_PROGRAM_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qcombobox.h>


class AddProgram : public QDialog
{
 Q_OBJECT
 public:
  AddProgram(QString *bname,QString *pname,
	     QWidget *parent=0,const char *name=0);
  ~AddProgram();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void okData();
  void cancelData();

 private:
  QComboBox *add_business_name_box;
  QLineEdit *add_program_name_edit;
  QString *add_business_name;
  QString *add_program_name;
};


#endif  // ADD_PROGRAM_H
