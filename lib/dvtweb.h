// dvtweb.h
//
// Functions for interfacing with web components using the
// Common Gateway Interface (CGI) Standard 
//
//   (C) Copyright 1996-2007 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef DVTWEB_H
#define DVTWEB_H

#include <qstring.h>
#include <qhostaddress.h>

//
// Data Structure Sizes
//
#define CGI_ACCUM_SIZE 1024
#define DVT_WEB_SESSION_TIMEOUT 900

//
// Function Prototypes
//
extern int DvtReadPost(char *,int);
extern int DvtPutPostString(char *,char *,char *,unsigned);
extern int DvtFindPostString(const char *,const char *,char *,int);
extern int DvtGetPostString(const char *,const char *,char *,int);
extern int DvtGetPostInt(const char *,const char *,int *);
extern int DvtGetPostLongInt(const char *,const char *,long int *);
extern int DvtPurgePostString(char *,char *,int);
extern int DvtEncodeString(char *,int);
extern int DvtEncodeSQLString(char *,int);
extern int DvtDecodeString(char *);
extern int DvtPutPlaintext(char *,int);
extern int DvtPurgePlaintext(char *,int);
extern void DvtCgiError(const char *);
extern int DvtBufferDiff(char *,int,int,int);
extern void DvtPruneAmp(char *);
extern int DvtEscapeQuotes(const char *src,char *dest,int maxlen);
extern long int DvtAuthenticateLogin(const QString &username,
				     const QString &passwd,
				     const QHostAddress &addr,
				     int *affiliate_id);
extern int DvtAuthenticateSession(long int session_id,const QHostAddress &addr);
extern void DvtLogoutSession(long int session_id,const QHostAddress &addr);
extern QString DvtEscapeWebString(const QString &str);

#endif  // DVTWEB_H
