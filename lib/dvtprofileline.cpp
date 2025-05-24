// dvtprofileline.cpp
//
// A container class for profile lines.
//
// (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include "dvtprofileline.h"

DvtProfileLine::DvtProfileLine()
{
  clear();
}


QString DvtProfileLine::tag() const
{
  return line_tag;
}


void DvtProfileLine::setTag(QString tag)
{
  line_tag=tag;
}


QString DvtProfileLine::value() const
{
  return line_value;
}


void DvtProfileLine::setValue(QString value)
{
  line_value=value;
}


void DvtProfileLine::clear()
{
  line_tag="";
  line_value="";
}
