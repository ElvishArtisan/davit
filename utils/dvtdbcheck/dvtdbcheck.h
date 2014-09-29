// dvtdbcheck.h
//
// A Database Check/Repair Tool for Davit
//
//   (C) Copyright 2011 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: dvtdbcheck.h,v 1.1 2011/04/29 21:04:59 pcvs Exp $
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


#ifndef DVTDBCHECK_H
#define DVTDBCHECK_H

#include <list>

#include <qobject.h>
#include <qsqldatabase.h>

#include <dvtconfig.h>
#include <dvtcmdswitch.h>

#define DVTDBCHECK_USAGE "[options]\n\nCheck the Davit database for consistency and correctness.\n\n--yes\n     Answer all questions with 'yes'\n\n--no\n     Answer all questions with 'no'\n"

//
// Global Variables
//
DvtConfig *dvtconfig;

class MainObject : public QObject
{
 public:
  MainObject(QObject *parent=0,const char *name=0);

 private:
  void CheckDuplicateAffiliates();
  void MergeAffiliates(int dest_id,int src_id);
  bool UserResponse();
  bool check_yes;
  bool check_no;
};


#endif  // DVTDBCHECK_H
