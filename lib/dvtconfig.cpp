// dvtconfig.cpp
//
// A container class for mld(8) Configuration
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

#include <stdio.h>

#include <QCoreApplication>
#include <QSettings>

#include "dvt.h"
#include "dvtconfig.h"
#include "dvtprofile.h"


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


QString DvtConfig::mysqlServertype() const
{
  return conf_mysql_server_type;
}


int DvtConfig::mysqlConnectionTimeout() const
{
  return conf_mysql_connection_timeout;
}


bool DvtConfig::mysqlParamsOveridden() const
{
  return conf_mysql_params_overidden;
}


QString DvtConfig::fontFamily() const
{
  return conf_font_family;
}


int DvtConfig::fontButtonSize() const
{
  return conf_font_button_size;
}


int DvtConfig::fontLabelSize() const
{
  return conf_font_label_size;
}


int DvtConfig::fontDefaultSize() const
{
  return conf_font_default_size;
}


QString DvtConfig::contactAddress() const
{
  return conf_contact_address;
}


QString DvtConfig::wireguardConfiguration() const
{
  return conf_wireguard_configuration;
}


DvtConfig::ConnectionPriority DvtConfig::wireguardConnectionPriority() const
{
  return conf_wireguard_connection_priority;
}


void DvtConfig::dumpConfig(FILE *stream)
{
  if(!mysqlHostname().isEmpty()) {
    fprintf(stream,"[mySQL]\n");
    fprintf(stream,
	    "  Hostname = %s\n",mysqlHostname().toUtf8().constData());
    fprintf(stream,
	    "  Loginname = %s\n",mysqlUsername().toUtf8().constData());
    fprintf(stream,
	    "  Database = %s\n",mysqlDbname().toUtf8().constData());
    fprintf(stream,
	    "  Password = %s\n",mysqlPassword().toUtf8().constData());
    fprintf(stream,"\n");
  }
  fprintf(stream,"[Wireguard]\n");
  fprintf(stream,"  ConnectionPriority=%s\n",
	  DvtConfig::connectionPriorityText(wireguardConnectionPriority()).
	  toUtf8().constData());
  fprintf(stream,"\n");
}


bool DvtConfig::load(DvtCmdSwitch *cmd)
{
  QString group;
  QString str;
  QString err_msg;
  bool ok=false;

  //
  // Host Name
  //
  DvtProfile *profile=new DvtProfile();
  if(!profile->loadFile(conf_filename,&err_msg)) {
    fprintf(stderr,"%s: failed to load configuration file [%s]\n",
	    QCoreApplication::applicationName().toUtf8().constData(),
	    err_msg.toUtf8().constData());
    exit(1);
  }

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
  conf_mysql_server_type=
    profile->stringValue("mySQL","ServerType",DVT_DEFAULT_MYSQL_DBTYPE);
  conf_mysql_connection_timeout=
    profile->intValue("mySQL","ConnectionTimeout",
		      DVT_DEFAULT_MYSQL_CONNECTION_TIMEOUT);
  conf_mysql_params_overidden=false;
  for(int i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--mysql-hostname") {
      conf_mysql_hostname=cmd->value(i);
      conf_mysql_params_overidden=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--mysql-loginname") {
      conf_mysql_username=cmd->value(i);
      conf_mysql_params_overidden=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--mysql-password") {
      conf_mysql_password=cmd->value(i);
      conf_mysql_params_overidden=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--mysql-database") {
      conf_mysql_dbname=cmd->value(i);
      conf_mysql_params_overidden=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--mysql-servertype") {
      conf_mysql_server_type=cmd->value(i);
      conf_mysql_params_overidden=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--mysql-connection-timeout") {
      conf_mysql_connection_timeout=cmd->value(i).toInt(&ok);
      if(!ok) {
	fprintf(stderr,"--mysql-connection-timeout value must be an integer\n");
	exit(1);
      }
      conf_mysql_params_overidden=true;
      cmd->setProcessed(i,true);
    }
  }

  //
  // [Fonts] Section
  //
  conf_font_family=profile->stringValue("Fonts","Family");
  conf_font_button_size=profile->intValue("Fonts","ButtonSize",-1);
  conf_font_label_size=profile->intValue("Fonts","LabelSize",-1);
  conf_font_default_size=profile->intValue("Fonts","DefaultSize",-1);

  //
  // [Site] Section
  //
  conf_contact_address=profile->stringValue("Site","ContactAddress","");

  //
  // [Wireguard] Section
  //
  conf_wireguard_configuration=
    profile->stringValue("Wireguard","ConfigurationPath");
  str=profile->stringValue("Wireguard","ConnectionPriority",
			   DVT_DEFAULT_WIREGUARD_CONNECTION_PRIORITY).toLower();
  for(int i=0;i<DvtConfig::ConnectionLast;i++) {
    ConnectionPriority prio=(DvtConfig::ConnectionPriority)i;
    if(str==DvtConfig::connectionPriorityText(prio).toLower()) {
      conf_wireguard_connection_priority=prio;
      break;
    }
  }

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
		      settings.readEntry("/Davit/InstallDir","C:\\etc").
		      toUtf8().constData(),
		      DVT_DEFAULT_WIN_CONF_FILE);
#else
  conf_filename=DVT_DEFAULT_CONF_FILE;
#endif  // WIN32
  conf_mysql_hostname="";
  conf_mysql_username="";
  conf_mysql_password="";
  conf_mysql_dbname="";
  conf_mysql_server_type="";
  conf_mysql_params_overidden=false;
  conf_mysql_connection_timeout=-1;
  conf_font_family="";
  conf_font_button_size=-1;
  conf_font_label_size=-1;
  conf_font_default_size=-1;
  conf_contact_address="";
  conf_wireguard_connection_priority=DvtConfig::LocalFirst;;
}


QString DvtConfig::connectionPriorityText(DvtConfig::ConnectionPriority prio)
{
  QString ret="unknown";

  switch(prio) {
  case DvtConfig::LocalOnly:
    ret="LocalOnly";
    break;

  case DvtConfig::LocalFirst:
    ret="LocalFirst";
    break;

  case DvtConfig::VpnFirst:
    ret="VpnFirst";
    break;

  case DvtConfig::ConnectionLast:
    break;

  }
  
  return ret;
}
