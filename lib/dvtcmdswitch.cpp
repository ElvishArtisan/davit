// dvtcmdswitch.cpp
//
// Process Davit Command-Line Switches
//
//   (C) Copyright 2002-2008 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: dvtcmdswitch.cpp,v 1.3 2013/02/26 19:06:17 pcvs Exp $
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

#include <stdlib.h>

#include <dvtcmdswitch.h>
#include <qmessagebox.h>

DvtCmdSwitch::DvtCmdSwitch(int argc,char *argv[],const char *modname,
			   const char *usage)
{
  unsigned l=0;
  bool handled=false;

  for(int i=1;i<argc;i++) {
#ifndef WIN32
    if(!strcmp(argv[i],"--version")) {
      printf("Davit v%s [%s]\n",VERSION,modname);
      exit(0);
    }
#endif  // WIN32
    if(!strcmp(argv[i],"--help")) {
      printf("\n%s %s\n",modname,usage);
      exit(0);
    }
    l=strlen(argv[i]);
    handled=false;
    for(unsigned j=0;j<l;j++) {
      if(argv[i][j]=='=') {
	switch_keys.push_back(QString(argv[i]).left(j));
	switch_values.push_back(QString(argv[i]).right(l-(j+1)));
	switch_processed.push_back(false);
	j=l;
	handled=true;
      }
    }
    if(!handled) {
      switch_keys.push_back(QString(argv[i]));
      switch_values.push_back(QString(""));
      switch_processed.push_back(false);
    }
  }
}


unsigned DvtCmdSwitch::keys() const
{
  return switch_keys.size();
}


QString DvtCmdSwitch::key(unsigned n) const
{
  return switch_keys[n];
}


QString DvtCmdSwitch::value(unsigned n) const
{
  return switch_values[n];
}


bool DvtCmdSwitch::processed(unsigned n) const
{
  return switch_processed[n];
}


void DvtCmdSwitch::setProcessed(unsigned n,bool state)
{
  switch_processed[n]=state;
}
