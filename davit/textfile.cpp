// textfile.cpp
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

#include "textfile.h"

bool TextFile(const QString &data)
{
  char tmpfile[256];
  QString editor=DVT_LINUX_EDITOR;
  char cmd[PATH_MAX];
  char *args[64];

  /* 
   * FIXME: Added editor selection
   *
  if(!rda->station()->reportEditorPath().trimmed().isEmpty()) {
    editor=rda->station()->reportEditorPath();
  }
  */
  editor="/usr/bin/emacs";

  memset(args,0,sizeof(args));
  QStringList f0=editor.split(" ",Qt::SkipEmptyParts);
  if(f0.size()>64) {
    QMessageBox::warning(NULL,"File Error",
			 "Too many arguments to report editor!");
    return false;
  }
  strncpy(cmd,f0.at(0).toUtf8(),PATH_MAX-1);
  QStringList f1=f0.at(0).split("/");
  args[0]=(char *)malloc(f1.back().toUtf8().size()+1);
  strcpy(args[0],f1.back().toUtf8());
  for(int i=1;i<f0.size();i++) {
    args[i]=(char *)malloc(f0.at(i).toUtf8().size()+1);
    strcpy(args[i],f0.at(i).toUtf8());
  }
  strcpy(tmpfile,"/tmp/rdreportXXXXXX");
  int fd=mkstemp(tmpfile);
  if(fd<0) {
    QMessageBox::warning(NULL,"File Error","Unable to create temporary file");
    return false;
  }
  if(write(fd,data.toUtf8(),data.toUtf8().length())!=data.toUtf8().length()) {
    fprintf(stderr,"TextFile write lost data [%s]",strerror(errno));
  }
  ::close(fd);

  args[f0.size()]=(char *)malloc(strlen(tmpfile)+1);
  strcpy(args[f0.size()],tmpfile);

  args[f0.size()+1]=(char *)NULL;

  if(fork()==0) {
    execvp(cmd,args);
    _exit(1);
  }

  return true;
}
