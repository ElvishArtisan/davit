// add_remark.h
//
// Add a Davit Remark.
//
//   (C) Copyright 2008-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef ADD_REMARK_H
#define ADD_REMARK_H

#include <QDialog>
#include <QPushButton>
#include <QTextEdit>

class AddRemark : public QDialog
{
 Q_OBJECT
 public:
AddRemark(QString *remark,QWidget *parent=0);
  ~AddRemark();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 protected:
  void resizeEvent(QResizeEvent *e);

 private slots:
  void okData();
  void cancelData();

 private:
  QTextEdit *add_remark_edit;
  QString *add_remark;
  QPushButton *add_ok_button;
  QPushButton *add_cancel_button;
};


#endif  // ADD_REMARK_H
