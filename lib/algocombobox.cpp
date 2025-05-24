// algocombobox.cpp
//
// QComboBox for choosing a US state.
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

#include "algocombobox.h"
#include "state_conv.h"

AlgoComboBox::AlgoComboBox(QWidget *parent)
  : QComboBox(parent)
{
  insertItem(0,"G.722");
  insertItem(1,"MPEG Layer 2");
  insertItem(2,"MPEG Layer 3");
  insertItem(3,"AAC");
}


QString AlgoComboBox::currentAlgoCode() const
{
  switch(currentIndex()) {
      case 0:
	return "G";

      case 1:
	return "2";

      case 2:
	return "3";
	
      case 3:
	return "A";
  }
  return "G";
}


void AlgoComboBox::setCurrentAlgoCode(const QString &algo_code)
{
  if(algo_code=="G") {
    setCurrentIndex(0);
  }
  if(algo_code=="2") {
    setCurrentIndex(1);
  }
  if(algo_code=="3") {
    setCurrentIndex(2);
  }
  if(algo_code=="A") {
    setCurrentIndex(3);
  }
}
