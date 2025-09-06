// dvtconfig.h
//
// A container class for CallCommander mld(8) Configuration
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef DVTCONFIG_H
#define DVTCONFIG_H

#include <vector>

#include <QStringList>

#include "dvt.h"

class DvtConfig
{
 public:
  DvtConfig();
  DvtConfig(QString filename);
  QString filename() const;
  void setFilename(QString filename);
  QString mysqlHostname() const;
  QString mysqlUsername() const;
  QString mysqlPassword() const;
  QString mysqlDbname() const;
  QString mysqlServertype() const;
  QString fontFamily() const;
  int fontButtonSize() const;
  int fontLabelSize() const;
  int fontDefaultSize() const;
  QString contactAddress() const;
  QStringList wireguardConfigurations() const;
  void dumpConfig(FILE *stream);
  bool load();
  void clear();

 private:
  QString conf_filename;
  QString conf_mysql_hostname;
  QString conf_mysql_username;
  QString conf_mysql_password;
  QString conf_mysql_dbname;
  QString conf_mysql_server_type;
  QString conf_font_family;
  int conf_font_button_size;
  int conf_font_label_size;
  int conf_font_default_size;
  QString conf_contact_address;
  QStringList conf_wireguard_configurations;
};


#endif  // DVTCONFIG_H
