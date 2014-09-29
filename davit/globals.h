// globals.h
//
// Global Definitions for Davit
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: globals.h,v 1.8 2011/02/03 21:55:43 pcvs Exp $
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


#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>

#include <dvtconfig.h>
#include <dvtuser.h>
#include <dvtsystem.h>

#include <geometry.h>
#include <maildialog.h>

//
// Global Classes
//
extern DvtConfig *config;
extern DvtUser *global_dvtuser;
extern DvtSystem *global_dvtsystem;
extern QString openoffice_path;
extern std::vector<QString> temp_files;
extern bool email_enabled;
extern Geometry *global_geometry;
extern MailDialog *mail_dialog;


#endif  // GLOBALS_H
