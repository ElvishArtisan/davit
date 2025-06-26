// edit_network.h
//
// Edit a Davit Network.
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

#ifndef EDIT_NETWORK_H
#define EDIT_NETWORK_H

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <dvtconf.h>
#include <dvtdialog.h>

class EditNetwork : public DvtDialog
{
 Q_OBJECT
 public:
  EditNetwork(DvtConfig *c,QWidget *parent=0);
  ~EditNetwork();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(int network_id,bool new_item=false);
				
 private slots:
  void okData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);
  void closeEvent(QCloseEvent *e);

 private:
  QLabel *edit_network_label;
  QLineEdit *edit_network_edit;
  QPushButton *edit_ok_button;
  QPushButton *edit_cancel_button;
  int edit_network_id;
};


#endif  // EDIT_NETWORK_H
