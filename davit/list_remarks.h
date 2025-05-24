// list_remarks.h
//
// List Davit Remarks.
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

#ifndef LIST_REMARKS_H
#define LIST_REMARKS_H

#include <QDialog>
#include <QPushButton>
#include <QTextEdit>

#include <contactlistview.h>

class ListRemarks : public QDialog
{
 Q_OBJECT
 public:
  ListRemarks(int id,QWidget *parent=0);
  ~ListRemarks();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  void show();
  void hide();

 public slots:
  void refresh();

 signals:
  void visibilityChanged(bool visible);

 private slots:
  void addData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  void RefreshList();
  QTextEdit *list_remarks_edit;
  QPushButton *list_add_button;
  int list_id;
};


#endif  // LIST_REMARKS_H
