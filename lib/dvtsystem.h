// dvtsystem.h
//
// Abstract system-wide Davit settings.
//
// (C) Copyright 2010-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef DVTSYSTEM_H
#define DVTSYSTEM_H

#include <QString>

class DvtSystem
{
 public:
  DvtSystem();
  QString originEmail() const;
  void setOriginEmail(const QString &str);
  QString affidavitEmailSubject() const;
  void setAffidavitEmailSubject(const QString &str);
  QString affidavitEmailTemplate() const;
  void setAffidavitEmailTemplate(const QString &str);
  void load();
  void save();
  void clear();

 private:
  QString sys_origin_email;
  QString sys_affidavit_email_subject;
  QString sys_affidavit_email_template;
};


#endif  // DVTSYSTEM_H
