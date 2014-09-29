//   dvtimport.h
//
//   Data Import Routines for Davit
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: dvtimport.h,v 1.2 2008/01/29 16:55:37 fredg Exp $
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

#ifndef DVTIMPORT_H
#define DVTIMPORT_H

#include <qstring.h>
#include <qstringlist.h>


QStringList DvtGetCommaList(const QString params,bool strip_quotes=false);
bool DvtImportTable(const QString &srcfile,const QString &desttable,
		    const QStringList &fieldnames);


#endif  // DVTIMPORT_H
