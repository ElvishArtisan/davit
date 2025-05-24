// statecombobox.cpp
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

#include "state_conv.h"
#include "statecombobox.h"

StateComboBox::StateComboBox(bool incl_all,QWidget *parent)
  : QComboBox(parent)
{
  insertItem(count(),tr("ALL"));
  insertItem(count(),tr("Alabama"));
  insertItem(count(),tr("Alaska"));
  insertItem(count(),tr("America Samoa"));
  insertItem(count(),tr("Arizona"));
  insertItem(count(),tr("Arkansas"));
  insertItem(count(),tr("California"));
  insertItem(count(),tr("Colorado"));
  insertItem(count(),tr("Connecticut"));
  insertItem(count(),tr("Delaware"));
  insertItem(count(),tr("District of Columbia"));
  insertItem(count(),tr("Fedrated States of Micronesia"));
  insertItem(count(),tr("Florida"));
  insertItem(count(),tr("Georgia"));
  insertItem(count(),tr("Guam"));
  insertItem(count(),tr("Hawaii"));
  insertItem(count(),tr("Idaho"));
  insertItem(count(),tr("Illinois"));
  insertItem(count(),tr("Indiana"));
  insertItem(count(),tr("Iowa"));
  insertItem(count(),tr("Kansas"));
  insertItem(count(),tr("Kentucky"));
  insertItem(count(),tr("Louisiana"));
  insertItem(count(),tr("Maine"));
  insertItem(count(),tr("Marshall Islands"));
  insertItem(count(),tr("Maryland"));
  insertItem(count(),tr("Massachusetts"));
  insertItem(count(),tr("Michigan"));
  insertItem(count(),tr("Minnesota"));
  insertItem(count(),tr("Mississippi"));
  insertItem(count(),tr("Missouri"));
  insertItem(count(),tr("Montana"));
  insertItem(count(),tr("Nebraska"));
  insertItem(count(),tr("Nevada"));
  insertItem(count(),tr("New Hampshire"));
  insertItem(count(),tr("New Jersey"));
  insertItem(count(),tr("New Mexico"));
  insertItem(count(),tr("New York"));
  insertItem(count(),tr("North Carolina"));
  insertItem(count(),tr("North Dakota"));
  insertItem(count(),tr("Northern Mariana Islands"));
  insertItem(count(),tr("Ohio"));
  insertItem(count(),tr("Oklahoma"));
  insertItem(count(),tr("Oregon"));
  insertItem(count(),tr("Palau"));
  insertItem(count(),tr("Pennsylvania"));
  insertItem(count(),tr("Puerto Rico"));
  insertItem(count(),tr("Rhode Island"));
  insertItem(count(),tr("South Carolina"));
  insertItem(count(),tr("South Dakota"));
  insertItem(count(),tr("Tennessee"));
  insertItem(count(),tr("Texas"));
  insertItem(count(),tr("Utah"));
  insertItem(count(),tr("Vermont"));
  insertItem(count(),tr("Virgin Islands"));
  insertItem(count(),tr("Virginia"));
  insertItem(count(),tr("Washington"));
  insertItem(count(),tr("West Virginia"));
  insertItem(count(),tr("Wisconsin"));
  insertItem(count(),tr("Wyoming"));

  connect(this,SIGNAL(activated(const QString &)),
	  this,SLOT(activatedData(const QString &)));
}


QString StateComboBox::currentStateCode() const
{
  return StateToAbbreviation(currentText());
}


void StateComboBox::setCurrentStateCode(const QString &state_code)
{
  QString state=AbbreviationToState(state_code);
  for(int i=0;i<count();i++) {
    if(state==itemText(i)) {
      setCurrentIndex(i);
      return;
    }
  }
}


void StateComboBox::activatedData(const QString &str)
{
  emit stateCodeChanged(StateToAbbreviation(str));
}
