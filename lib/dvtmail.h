// dvtmail.h
//
// Routines for process e-mail messages.
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: dvtmail.h,v 1.4 2011/03/22 15:12:53 pcvs Exp $
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

#ifndef DVTMAIL_H
#define DVTMAIL_H

#ifndef WIN32
#include <vmime/vmime.hpp>
#endif  // WIN32

#include <qstring.h>
#include <qstringlist.h>

#ifdef WIN32
bool DvtSendMail(const QStringList &to_addrs,const QStringList &cc_addrs,
		 const QStringList &bcc_addrs,const QString &from_addr,
		 const QString &reply_addr,const QString &subj,
		 const QString &msg,QWidget *parent=NULL);
#else
bool DvtSendMail(const QStringList &to_addrs,const QStringList &cc_addrs,
		 const QStringList &bcc_addrs,const QString &from_addr,
		 const QString &reply_addr,const QString &subj,
		 const QString &msg,QWidget *parent=NULL,
		 vmime::ref<vmime::net::transport> batch_transport=NULL);
#endif  // WIN32

bool DvtSendAlert(const QString &subj,const QString &msg);


#endif   // DVTMAIL_H
