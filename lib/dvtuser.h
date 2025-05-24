// dvtuser.h
//
// Abstract a Davit user.
//
// (C) Copyright 2008-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef DVTUSER_H
#define DVTUSER_H

#include <QString>

class DvtUser
{
 public:
  enum Privilege {PrivAdmin=0,PrivAffiliateView=1,PrivAffiliateEdit=2,
		  PrivAffiliateSchedule=3,PrivAffiliateRemark=4,
		  PrivProviderView=5,PrivProviderEdit=6,
		  PrivProgramView=7,PrivProgramEdit=8,
		  PrivReportView=9,PrivLastItem=10};
  DvtUser(const QString &username);
  QString name() const;
  QString fullName() const;
  void setFullName(const QString &str);
  QString description() const;
  void setDescription(const QString &str);
  QString phoneNumber() const;
  void setPhoneNumber(const QString &str);
  QString email() const;
  void setEmail(const QString &str);
  bool privilege(DvtUser::Privilege priv) const;
  void setPrivilege(DvtUser::Privilege priv,bool state);
  void load();
  void save();
  void clear();

 private:
  QString user_name;
  QString user_full_name;
  QString user_description;
  QString user_phone_number;
  QString user_email;
  bool user_privilege[DvtUser::PrivLastItem];
};


#endif  // DVTUSER_H
