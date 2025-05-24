// dvttextfile.cpp
//
// Spawn an external text file viewer.
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

#ifndef WIN32
#include <unistd.h>
#endif  // WIN32
#include <stdlib.h>
#include <stdio.h>

#include <QMessageBox>
#include <QProcess>

#include <dvtconf.h>
#include <dvt.h>
#include <dvttextfile.h>

bool DvtTextFile(const QString &data)
{
  char tmpfile[256];
  QString editor;

  if(getenv("VISUAL")==NULL) {
#ifdef WIN32
    editor=DVT_WIN32_EDITOR;
#else
    editor=DVT_LINUX_EDITOR;
#endif  // WIN32
  }
  else {
    editor=getenv("VISUAL");
  }
#ifdef WIN32
  QString tempfile=QString().sprintf("%s\\dvt-%s",(const char *)DvtTempDir(),
	           (const char *)QTime::currentTime().toString("hhmmsszzz"));
  FILE *f=fopen(tempfile,"w");
  if(f==NULL) {
    QMessageBox::warning(NULL,"File Error",QString().
			 sprintf("Unable to create temporary file \"%s\".",
				 (const char *)tempfile));
    return false;
  }
  fprintf(f,"%s",(const char *)data);
  fclose(f);
  QStringList args;
  args+=editor;
  args+=tempfile;
  QProcess *proc=new QProcess(args);
  proc->launch("");
  delete proc;
#else
  strcpy(tmpfile,"/tmp/dvtreportXXXXXX");
  int fd=mkstemp(tmpfile);
  if(fd<0) {
    QMessageBox::warning(NULL,"File Error",
		   QString::asprintf("Unable to create temporary file \"%s\".",
				 (const char *)tmpfile));
    return false;
  }
  else {
    QByteArray bytes=data.toUtf8();
    write(fd,bytes,bytes.length());
    ::close(fd);
    if(fork()==0) {
      system(QString::asprintf("%s %s",
			       editor.toUtf8().constData(),tmpfile).toUtf8());
      unlink(tmpfile);
      exit(0);
    }
  }
#endif  // WIN32
  return true;
}
