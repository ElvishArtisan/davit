// dvtconfig.cpp
//
// A container class for mld(8) Configuration
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
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

#include <qmessagebox.h>
#include <qsettings.h>

#include <dvtprofile.h>
#include <dvt.h>
#include <dvtconfig.h>


DvtConfig::DvtConfig()
{
  clear();
}


DvtConfig::DvtConfig(QString filename)
{
  clear();
  conf_filename=filename;
}


QString DvtConfig::filename() const
{
  return conf_filename;
}


void DvtConfig::setFilename(QString filename)
{
  conf_filename=filename;
}


QString DvtConfig::mysqlHostname() const
{
  return conf_mysql_hostname;
}


QString DvtConfig::mysqlUsername() const
{
  return conf_mysql_username;
}


QString DvtConfig::mysqlPassword() const
{
  return conf_mysql_password;
}


QString DvtConfig::mysqlDbname() const
{
  return conf_mysql_dbname;
}


QString DvtConfig::mysqlDbtype() const
{
  return conf_mysql_dbtype;
}


QString DvtConfig::contactAddress() const
{
  return conf_contact_address;
}


void DvtConfig::dumpConfig(FILE *stream)
{
  if(!mysqlHostname().isEmpty()) {
    fprintf(stream,"[mySQL]\n");
    fprintf(stream,
	    "  Hostname = %s\n",(const char *)mysqlHostname());
    fprintf(stream,
	    "  Loginname = %s\n",(const char *)mysqlUsername());
    fprintf(stream,
	    "  Database = %s\n",(const char *)mysqlDbname());
    fprintf(stream,
	    "  Password = %s\n",(const char *)mysqlPassword());
    fprintf(stream,"\n");
  }
  fprintf(stream,"\n");
}


bool DvtConfig::load()
{
  QString group;
  QString str;

  //
  // Host Name
  //
  DvtProfile *profile=new DvtProfile();
  profile->setSource(conf_filename);

  //
  // [mySQL] Section
  //
  conf_mysql_hostname=
    profile->stringValue("mySQL","Hostname",DVT_DEFAULT_MYSQL_HOSTNAME);
  conf_mysql_username=
    profile->stringValue("mySQL","Loginname",DVT_DEFAULT_MYSQL_USERNAME);
  conf_mysql_password=profile->stringValue("mySQL","Password","");
  conf_mysql_dbname=
    profile->stringValue("mySQL","Database",DVT_DEFAULT_MYSQL_DATABASE);
  conf_mysql_dbtype=
    profile->stringValue("mySQL","ServerType",DVT_DEFAULT_MYSQL_DBTYPE);

  //
  // [Site] Section
  //
  conf_contact_address=profile->stringValue("Site","ContactAddress","");

  delete profile;

  return true;
}


void DvtConfig::clear()
{
#ifdef WIN32
  QSettings settings=QSettings();
  settings.insertSearchPath(QSettings::Windows,"/ParavelSystems");
  conf_filename=
    QString().sprintf("%s\\%s",
		      (const char *)settings.
		      readEntry("/Davit/InstallDir","C:\\etc"),
		      DVT_DEFAULT_WIN_CONF_FILE);
#else
  conf_filename=DVT_DEFAULT_CONF_FILE;
#endif  // WIN32
  conf_mysql_hostname="";
  conf_mysql_username="";
  conf_mysql_password="";
  conf_mysql_dbname="";
  conf_mysql_dbtype="";
  conf_contact_address="";
}
