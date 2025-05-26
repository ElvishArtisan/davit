// dvt.h
//
// System-Wide Values for Davit.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef DVT_H
#define DVT_H

/*
 * Current Database Version
 */
#define DVT_VERSION_DATABASE 22

/*
 * Registry Keys
 */
#define DVT_REGISTRY_PATH "/ParavelSystems/"
#define DVT_REGISTRY_INSTALL_DIR "/Davit/InstallDir"

/*
 * Default mySQL Settings
 */
#define DVT_DEFAULT_MYSQL_HOSTNAME ""
#define DVT_DEFAULT_MYSQL_DATABASE "Davit"
#define DVT_DEFAULT_MYSQL_USERNAME "dvtuser"
#define DVT_DEFAULT_MYSQL_PASSWORD "letmein"
#define DVT_DEFAULT_MYSQL_DBTYPE "QMYSQL"

//
// Default User Name
//
#define DVT_DEFAULT_LOGIN_NAME "admin"
#define DVT_DEFAULT_FULL_NAME "Affiliate Administrator"
#define DVT_DEFAULT_DESCRIPTION_NAME "Default User"

//
// Default Configuration File
//
#define DVT_DEFAULT_CONF_FILE "/etc/davit.conf"
#define DVT_DEFAULT_WIN_CONF_FILE "davit.ini"

//
// Default External Editors
//
#define DVT_LINUX_EDITOR "xterm -e vi"
#define DVT_WIN32_EDITOR "notepad"

//
// Oldest possible year in Davit
//
#define DVT_ORIGIN_YEAR 2007

//
// Path to the Sendmail(1) binary
//
#ifdef WIN32
#define DVT_SENDMAIL_PATH "C:\\usr\\lib\\sendmail.exe"
#else
#define DVT_SENDMAIL_PATH "/usr/lib/sendmail"
#endif

class Dvt
{
 public:
  enum RemarkType {RemarkNarrative=0,RemarkProgramAdd=1,RemarkProgramDelete=2,
		   RemarkAffidavitReminder=3};
  enum AiredState {AiredStateUnknown=0, AiredStateScheduled=1,
		   AiredStateConfirmed=2,AiredStateDenied=3,
		   AiredStateModified=4};
  enum AffidavitStationFilter {All=0,Weekday=1,Weekend=2,Program=3};
  enum AffidavitSortType {ByState=0,ByCount=1};
  enum ContactType {AffidavitContact=0,ProgramDirectorContact=1,
		    GeneralManagerContact=2};
};

#endif  // DVT_H
