// dvtmail-win32.cpp
//
// Routines for process e-mail messages on win32 architectures.
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: dvtmail-win32.cpp,v 1.2 2011/03/29 19:41:18 pcvs Exp $
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

#include <fstream>
#include <iostream>

#include <stdlib.h>
#include <stdio.h>

#include <qmessagebox.h>
#include <qsqldatabase.h>

#include <dvt.h>
#include <dvtconf.h>

#include <dvtmail.h>

bool DvtSendMail(const QStringList &to_addrs,const QStringList &cc_addrs,
		 const QStringList &bcc_addrs,const QString &from_addr,
		 const QString &reply_addr,const QString &subj,
		 const QString &msg,QWidget *parent)
{
  QString tempfile;
  FILE *f=NULL;

  //
  // Validate Addresses
  //
  for(unsigned i=0;i<to_addrs.size();i++) {
    if(!DvtIsEmailAddress(to_addrs[i])) {
      return false;
    }
  }
  for(unsigned i=0;i<cc_addrs.size();i++) {
    if(!DvtIsEmailAddress(cc_addrs[i])) {
      return false;
    }
  }
  for(unsigned i=0;i<bcc_addrs.size();i++) {
    if(!DvtIsEmailAddress(bcc_addrs[i])) {
      return false;
    }
  }
  if(!DvtIsEmailAddress(from_addr)) {
    return false;
  }
  if(!DvtIsEmailAddress(reply_addr)) {
    return false;
  }

  //
  // Generate Temporary File
  //
  if((f=DvtTempFile(&tempfile,"w"))==NULL) {
    return false;
  }

  //
  // Generate Message
  //
  if(!from_addr.isEmpty()) {
    fprintf(f,"From: %s\n",(const char *)from_addr);
  }
  if(!reply_addr.isEmpty()) {
    fprintf(f,"Reply-To: %s\n",(const char *)reply_addr);
  }
  if(to_addrs.size()>0) {
    fprintf(f,"To: ");
    for(unsigned i=0;i<to_addrs.size();i++) {
      fprintf(f,"%s;",(const char *)to_addrs[i]);
    }
    fprintf(f,"\n");
  }
  if(cc_addrs.size()>0) {
    fprintf(f,"Cc: ");
    for(unsigned i=0;i<cc_addrs.size();i++) {
      fprintf(f,"%s;",(const char *)cc_addrs[i]);
    }
    fprintf(f,"\n");
  }
  if(bcc_addrs.size()>0) {
    fprintf(f,"Bcc: ");
    for(unsigned i=0;i<bcc_addrs.size();i++) {
      fprintf(f,"%s;",(const char *)bcc_addrs[i]);
    }
    fprintf(f,"\n");
  }
  if(!subj.isEmpty()) {
    fprintf(f,"Subject: %s\n",(const char *)subj);
  }
  fprintf(f,"X-Davit-Message:\n");
  fprintf(f,"\n");
  fprintf(f,"%s",(const char *)msg);
  fclose(f);

  system(QString().sprintf("%s -t < %s",DVT_SENDMAIL_PATH,
			   (const char *)tempfile));
  DvtDeleteFile(tempfile);

  return true;
}
