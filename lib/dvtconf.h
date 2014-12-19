// dvtconf.h
//
// The header file for the dvtconf package
//
//   (C) Copyright 1996-2014 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef DVTCONF_H
#define DVTCONF_H

#define MAX_RETRIES 10

#include <map>
#include <vector>

#include <qstring.h>
#include <qdatetime.h>
#include <qfont.h>
#include <qhostaddress.h>
#include <qsqldatabase.h>
#include <qvariant.h>
#include <stdlib.h>
#include <stdio.h>

#include <dvt.h>

/* Function Prototypes */
int DvtGetPrivateProfileBool(const char *,const char *,const char *,bool);
int DvtGetPrivateProfileString(const char *,const char *,const char *,char *,
			       const char *,int);
int DvtGetPrivateProfileInt(const char *,const char *,const char *,int);
int DvtGetPrivateProfileHex(const char *,const char *,const char *,int);
double DvtGetPrivateProfileDouble(const char *,const char *,const char *,
				  double);
int GetIni(const char *,const char *,const char *,char *,int);
int GetIniLine(FILE *,char *);
void Prepend(char *,char *);
int IncrementIndex(char *,int);
void StripLevel(char *); 
bool GetLock(const char *);
void ClearLock(const char *);
QString DvtGetPathPart(QString path);
QString DvtGetBasePart(QString path);
QString DvtGetShortDate(QDate);
/**
 * Returns the name of the weekday in english regardless of the locale
 * configured.
 * @param weekday Integer value for the weekday; 1 = "Mon", 2 = "Tue", 
 * ... 7 = "Sun".  If the value is out of range 1 is defaulted to.
 **/
QString DvtGetShortDayNameEN(int weekday);
QFont::Weight DvtGetFontWeight(QString);
bool DvtDetach();
bool DvtBool(QString);
QString DvtYesNo(bool);
QHostAddress DvtGetHostAddr();
QString DvtGetDisplay(bool strip_point=false);
bool DvtDoesRowExist(QString table,QString name,QString test,
		   QSqlDatabase *db=0);
bool DvtDoesRowExist(QString table,QString name,unsigned test,
		   QSqlDatabase *db=0);
QVariant DvtGetSqlValue(QString table,QString name,QString test,
		      QString param,QSqlDatabase *db=0,bool *valid=0);
QVariant DvtGetSqlValue(QString table,QString name,unsigned test,
		      QString param,QSqlDatabase *db=0,bool *valid=0);
bool DvtIsSqlNull(QString table,QString name,QString test,
		QString param,QSqlDatabase *db=0);
bool DvtIsSqlNull(QString table,QString name,unsigned test,
		QString param,QSqlDatabase *db=0);
QString DvtGetTimeLength(int mseconds,bool leadzero=false,bool tenths=true);
int DvtSetTimeLength(QString string);
QTime DvtSetTime(const QString &string);
bool DvtCopy(QString srcfile,QString destfile);
#ifndef WIN32
bool DvtWritePid(QString dirname,QString filename,int owner=-1,int group=-1);
void DvtDeletePid(QString dirname,QString filename);
bool DvtCheckPid(QString dirname,QString filename);
pid_t DvtGetPid(QString pidfile);
#endif  // WIN32
QString DvtGetHomeDir(bool *found=0);
bool DvtTimeSynced();
QString DvtTruncateAfterWord(QString str,int word,bool add_dots=false);
QString DvtHomeDir();
QString DvtTempDir();
FILE *DvtTempFile(QString *filename,const char *mode);
int DvtDeleteFile(const QString &filename);
QString DvtEscapeString(const QString &str);
QString DvtFormatCityState(const QString &city,const QString &state);
bool DvtNormalizeAddresses(QString str,QStringList *list);
bool DvtIsFullEmailAddress(QString str);
bool DvtIsEmailAddress(const QString &str);
QString DvtFormatEmailAddress(const QString &name,const QString &addr);
QString DvtFormatPhoneNumber(const QString &str);
QString DvtFormatFrequency(double freq);
QString DvtNormalizePhoneNumber(const QString &str,bool *ok=NULL);
int DvtCreateNewAffiliateRecord();
QString DvtStationTypeString(const QString &type);
QString DvtStationCallString(const QString &call,const QString &type);
QString DvtStationCallString(int affiliate_id);
bool DvtAffidavitNeeded(int affiliate_id,const QDate &date);
bool DvtAffidavitNeeded(std::vector<int> *ids,std::map<int,int> *counts,
			const QDate &start_date,const QDate &end_date,
			Dvt::AffidavitStationFilter filter,int program_id);
unsigned DvtAffidavitNeededDates(std::vector<QDate> *dates,int affiliate_id,
				 const QDate &start_date,const QDate &end_date);
void DvtUpdateIsAffiliateField();
unsigned DvtContactInfo(QString *name,QString *title,QString *email,
			QString *phone,QString *fax,
			int affiliate_id,Dvt::ContactType type);

#endif   // DVTCONF_H
