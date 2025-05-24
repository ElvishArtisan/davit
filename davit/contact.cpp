// contact.cpp
//
// Container class for a Davit Contact
//
//   (C) Copyright 2010-2025 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
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

#include "contact.h"

Contact::Contact()
{
  contact_name="";
  contact_title="";
  contact_phone="";
  contact_fax="";
  contact_email="";
  contact_affidavit=false;
  contact_program_director=false;
  contact_general_manager=false;
  contact_locked=false;
}


QString Contact::name() const
{
  return contact_name;
}


void Contact::setName(const QString &str)
{
  contact_name=str;
}


QString Contact::title() const
{
  return contact_title;
}


void Contact::setTitle(const QString &str)
{
  contact_title=str;
}


QString Contact::phone() const
{
  return contact_phone;
}


void Contact::setPhone(const QString &str)
{
  contact_phone=str;
}


QString Contact::fax() const
{
  return contact_fax;
}


void Contact::setFax(const QString &str)
{
  contact_fax=str;
}


QString Contact::email() const
{
  return contact_email;
}


void Contact::setEmail(const QString &str)
{
  contact_email=str;
}


bool Contact::affidavit() const
{
  return contact_affidavit;
}


void Contact::setAffidavit(bool state)
{
  contact_affidavit=state;
}


bool Contact::programDirector() const
{
  return contact_program_director;
}


void Contact::setProgramDirector(bool state)
{
  contact_program_director=state;
}


bool Contact::generalManager() const
{
  return contact_general_manager;
}


void Contact::setGeneralManager(bool state)
{
  contact_general_manager=state;
}


bool Contact::locked() const
{
  return contact_locked;
}


void Contact::setLocked(bool state)
{
  contact_locked=state;
}
