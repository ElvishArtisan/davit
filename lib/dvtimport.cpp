// dvtimport.cpp
//
// Data Import Routines for Davit
//
// (C) Copyright 2007-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <stdio.h>

#include <QSqlDatabase>
#include <QSqlQuery>

#include "dvtimport.h"

QStringList DvtGetCommaList(const QString params,bool strip_quotes)
{
  int start=0;
  QStringList sl;
  bool quoted=false;

  for(int i=0;i<params.length();i++) {
    if(params.at(i)=='"') {
      quoted=!quoted;
    }
    if((params.at(i)==',')&&(!quoted)) {
      sl.push_back(params.mid(start,i-start));
      start=i+1;
    }
  }
  sl.push_back(params.mid(start,params.length()+1));

  if(strip_quotes) {
    for(int i=0;i<sl.size();i++) {
      if(sl[i].left(1)=="\"") {
	sl[i]=sl[i].right(sl[i].length()-1);
      }
      if(sl[i].right(1)=="\"") {
	sl[i]=sl[i].left(sl[i].length()-1);
      }
    }
  }

  return sl;
}


bool DvtImportTable(const QString &srcfile,const QString &desttable,
		    const QStringList &fieldnames)
{
  QString sql;
  char buf[1024];
  unsigned size=0;
  QStringList values;
  QSqlQuery *q;

  FILE *f=fopen(srcfile.toUtf8(),"r");
  if(f==NULL) {
    return false;
  }

  while(fgets(buf,1024,f)!=NULL) {
    values=DvtGetCommaList(buf);
    if(values.size()<fieldnames.size()) {
      size=values.size();
    }
    else {
      size=fieldnames.size();
    }
    sql=QString().sprintf("insert into %s set ",desttable.toUtf8().constData());
    for(unsigned i=0;i<size;i++) {
      if(!fieldnames[i].isEmpty()) {
	sql+=QString().sprintf("%s=%s,",fieldnames[i].toUtf8().constData(),
			       values[i].toUtf8().constData());
      }
    }
    sql=sql.left(sql.length()-1);
    q=new QSqlQuery(sql);
    delete q;
  }
  fclose(f);
  return true;
}
