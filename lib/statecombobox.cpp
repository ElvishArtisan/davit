// statecombobox.cpp
//
// QComboBox for choosing a US state.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: statecombobox.cpp,v 1.2 2007/03/19 11:20:33 fredg Exp $
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
#include <statecombobox.h>


StateComboBox::StateComboBox(QWidget *parent,const char *name)
  : QComboBox(parent,name)
{
  insertItem("");
  insertItem(tr("Alabama"));
  insertItem(tr("Alaska"));
  insertItem(tr("America Samoa"));
  insertItem(tr("Arizona"));
  insertItem(tr("Arkansas"));
  insertItem(tr("California"));
  insertItem(tr("Colorado"));
  insertItem(tr("Connecticut"));
  insertItem(tr("Delaware"));
  insertItem(tr("District of Columbia"));
  insertItem(tr("Fedrated States of Micronesia"));
  insertItem(tr("Florida"));
  insertItem(tr("Georgia"));
  insertItem(tr("Guam"));
  insertItem(tr("Hawaii"));
  insertItem(tr("Idaho"));
  insertItem(tr("Illinois"));
  insertItem(tr("Indiana"));
  insertItem(tr("Iowa"));
  insertItem(tr("Kansas"));
  insertItem(tr("Kentucky"));
  insertItem(tr("Louisiana"));
  insertItem(tr("Maine"));
  insertItem(tr("Marshall Islands"));
  insertItem(tr("Maryland"));
  insertItem(tr("Massachusetts"));
  insertItem(tr("Michigan"));
  insertItem(tr("Minnesota"));
  insertItem(tr("Mississippi"));
  insertItem(tr("Missouri"));
  insertItem(tr("Montana"));
  insertItem(tr("Nebraska"));
  insertItem(tr("Nevada"));
  insertItem(tr("New Hampshire"));
  insertItem(tr("New Jersey"));
  insertItem(tr("New Mexico"));
  insertItem(tr("New York"));
  insertItem(tr("North Carolina"));
  insertItem(tr("North Dakota"));
  insertItem(tr("Northern Mariana Islands"));
  insertItem(tr("Ohio"));
  insertItem(tr("Oklahoma"));
  insertItem(tr("Oregon"));
  insertItem(tr("Palau"));
  insertItem(tr("Pennsylvania"));
  insertItem(tr("Puerto Rico"));
  insertItem(tr("Rhode Island"));
  insertItem(tr("South Carolina"));
  insertItem(tr("South Dakota"));
  insertItem(tr("Tennessee"));
  insertItem(tr("Texas"));
  insertItem(tr("Utah"));
  insertItem(tr("Vermont"));
  insertItem(tr("Virgin Islands"));
  insertItem(tr("Virginia"));
  insertItem(tr("Washington"));
  insertItem(tr("West Virginia"));
  insertItem(tr("Wisconsin"));
  insertItem(tr("Wyoming"));
}


QString StateComboBox::currentStateCode() const
{
  return StateToAbbreviation(currentText());
}


void StateComboBox::setCurrentStateCode(const QString &state_code)
{
  QString state=AbbreviationToState(state_code);
  for(int i=0;i<count();i++) {
    printf("|%s| : |%s|\n",(const char *)AbbreviationToState(state_code),(const char *)text(i));
    if(state==text(i)) {
      printf("  MATCH!\n");
      setCurrentItem(i);
      return;
    }
  }
}
