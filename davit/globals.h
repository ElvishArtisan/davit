// globals.h
//
// Global Definitions for Davit
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


#ifndef GLOBALS_H
#define GLOBALS_H

#include <QStringList>

#include <dvtconfig.h>
#include <dvtuser.h>
#include <dvtsystem.h>

#include "geometry.h"
#include "maildialog.h"
#include "viewerprocesslist.h"

//
// Global Classes
//
extern DvtConfig *config;
extern DvtUser *global_dvtuser;
extern DvtSystem *global_dvtsystem;
extern QString openoffice_path;
extern QStringList temp_files;
extern Geometry *global_geometry;
extern MailDialog *mail_dialog;
extern ViewerProcessList *global_viewer_list;
extern bool global_email_dry_run;

#endif  // GLOBALS_H
