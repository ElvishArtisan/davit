// algocombobox.cpp
//
// QComboBox for choosing a US state.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: algocombobox.cpp,v 1.1 2007/03/21 13:38:44 fredg Exp $
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

#include <state_conv.h>
#include <algocombobox.h>


AlgoComboBox::AlgoComboBox(QWidget *parent,const char *name)
  : QComboBox(parent,name)
{
  insertItem("G.722");
  insertItem("MPEG Layer 2");
  insertItem("MPEG Layer 3");
  insertItem("AAC");
}


QString AlgoComboBox::currentAlgoCode() const
{
  switch(currentItem()) {
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
    setCurrentItem(0);
  }
  if(algo_code=="2") {
    setCurrentItem(1);
  }
  if(algo_code=="3") {
    setCurrentItem(2);
  }
  if(algo_code=="A") {
    setCurrentItem(3);
  }
}
