// contact.h
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

#ifndef CONTACT_H
#define CONTACT_H

#include <QString>

class Contact
{
 public:
  Contact();
  QString name() const;
  void setName(const QString &str);
  QString title() const;
  void setTitle(const QString &str);
  QString phone() const;
  void setPhone(const QString &str);
  QString fax() const;
  void setFax(const QString &str);
  QString email() const;
  void setEmail(const QString &str);
  bool affidavit() const;
  void setAffidavit(bool state);
  bool programDirector() const;
  void setProgramDirector(bool state);
  bool generalManager() const;
  void setGeneralManager(bool state);
  bool locked() const;
  void setLocked(bool state);

 private:
  QString contact_name;
  QString contact_title;
  QString contact_phone;
  QString contact_fax;
  QString contact_email;
  bool contact_affidavit;
  bool contact_program_director;
  bool contact_general_manager;
  bool contact_locked;
};


#endif  // CONTACT_H
