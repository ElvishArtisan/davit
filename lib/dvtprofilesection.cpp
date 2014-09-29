// dvtprofilesection.cpp
//
// A container class for profile sections.
//
// (C) Copyright 2002-2003 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: dvtprofilesection.cpp,v 1.1 2007/11/19 16:53:38 fredg Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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
//

#include <dvtprofilesection.h>

DvtProfileSection::DvtProfileSection()
{
  clear();
}


QString DvtProfileSection::name() const
{
  return section_name;
}


void DvtProfileSection::setName(QString name)
{
  section_name=name;
}


bool DvtProfileSection::getValue(QString tag,QString *value) const
{
  for(unsigned i=0;i<section_line.size();i++) {
    if(section_line[i].tag()==tag) {
      *value=section_line[i].value();
      return true;
    }
  }
  return false;
}


void DvtProfileSection::addValue(QString tag,QString value)
{
  section_line.push_back(DvtProfileLine());
  section_line.back().setTag(tag);
  section_line.back().setValue(value);
}


void DvtProfileSection::clear()
{
  section_name="";
  section_line.resize(0);
}
