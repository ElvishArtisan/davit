# lib.pro
#
# The lib/ QMake project file for Davit.
#
# (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
#
#      $Id: lib.pro,v 1.8 2011/03/29 15:38:51 pcvs Exp $
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License version 2 as
#   published by the Free Software Foundation.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public
#   License along with this program; if not, write to the Free Software
#   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

TEMPLATE = lib

win32 {
  DEFINES += WIN32
  DEFINES += VERSION=\"$$[VERSION]\"
  DEFINES += PACKAGE=\"davit\" 
  DEFINES += PACKAGE_VERSION=\"$$[VERSION]\" 
  DEFINES += PACKAGE_NAME=\"davit\"
  DEFINES += PACKAGE_BUGREPORT=\"fredg@paravelsystems.com\"
}

SOURCES += algocombobox.cpp
SOURCES += change_password.cpp
SOURCES += dvtcmdswitch.cpp
SOURCES += dvtconf.cpp
SOURCES += dvtconfig.cpp
SOURCES += dvtdatedialog.cpp
SOURCES += dvtdatepicker.cpp
SOURCES += dvtfeed.cpp
SOURCES += dvtimport.cpp
SOURCES += dvtlabel.cpp
SOURCES += dvtlistselector.cpp
SOURCES += dvtlistviewitem.cpp
SOURCES += dvtprofile.cpp
SOURCES += dvtprofileline.cpp
SOURCES += dvtprofilesection.cpp
SOURCES += dvtsqlquery.cpp
SOURCES += dvttextfile.cpp
SOURCES += dvtuser.cpp
SOURCES += login.cpp
SOURCES += spread_cell.cpp
SOURCES += spread.cpp
SOURCES += spread_sheet.cpp
SOURCES += spread_tab.cpp
SOURCES += statecombobox.cpp
SOURCES += state_conv.cpp
#SOURCES += dvtmail-win32.cpp
SOURCES += dvtsystem.cpp
SOURCES += dvttransportbutton.cpp
x11 {
  SOURCES += dvtweb.cpp
}

HEADERS += algocombobox.h
HEADERS += change_password.h
HEADERS += dvtcmdswitch.h
HEADERS += dvtconf.h
HEADERS += dvtconfig.h
HEADERS += dvtdatedialog.h
HEADERS += dvtdatepicker.h
HEADERS += dvtfeed.h
HEADERS += dvt.h
HEADERS += dvtimport.h
HEADERS += dvtlabel.h 
HEADERS += dvtlistselector.h
HEADERS += dvtlistviewitem.h
HEADERS += dvtprofile.h
HEADERS += dvtprofileline.h
HEADERS += dvtprofilesection.h
HEADERS += dvtsqlquery.h
HEADERS += dvttextfile.h
HEADERS += dvtuser.h
HEADERS += login.h
HEADERS += spread_cell.h
HEADERS += spread.h
HEADERS += spread_sheet.h
HEADERS += spread_tab.h
HEADERS += statecombobox.h
HEADERS += state_conv.h
#HEADERS += dvtmail.h
HEADERS += dvtsystem.h
HEADERS += dvttransportbutton.h
x11 {
  HEADERS += dvtweb.h
}

CONFIG += qt staticlib
