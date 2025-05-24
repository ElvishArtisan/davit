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

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>

#include <dvtconf.h>

class EditNetwork : public QDialog
{
 Q_OBJECT
 public:
  EditNetwork(QString *network,QWidget *parent=0);
  ~EditNetwork();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void okData();
  void cancelData();

 private:
  QLineEdit *edit_network_edit;
  QString *edit_network;
};


#endif  // EDIT_NETWORK_H
