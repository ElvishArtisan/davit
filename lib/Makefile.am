## Makefile.am
##
## Makefile.am for lib/
##
##   (C) Copyright 2007-2025 Fred Gleason <fredg@paravelsystems.com>
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License version 2 as
##   published by the Free Software Foundation.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" @QT6_CFLAGS@ -std=c++17 -fPIC
MOC = @QT6_MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

#instdir = @LOCAL_PREFIX@/lib

lib_LTLIBRARIES = libdvt.la
dist_libdvt_la_SOURCES = algocombobox.cpp algocombobox.h\
                         change_password.cpp change_password.h\
                         dvt.h\
                         dvtcmdswitch.cpp dvtcmdswitch.h\
                         dvtconf.cpp dvtconf.h\
                         dvtconfig.cpp dvtconfig.h\
                         dvtdatedialog.cpp dvtdatedialog.h\
                         dvtdatepicker.cpp dvtdatepicker.h\
                         dvtdb.cpp dvtdb.h\
                         dvtdialog.cpp dvtdialog.h\
                         dvtfontengine.cpp dvtfontengine.h\
                         dvtimport.cpp dvtimport.h\
                         dvtlabel.cpp dvtlabel.h\
                         dvtlistselector.cpp dvtlistselector.h\
                         dvtprofile.cpp dvtprofile.h\
                         dvtprofileline.cpp dvtprofileline.h\
                         dvtprofilesection.cpp dvtprofilesection.h\
                         dvtsystem.cpp dvtsystem.h\
                         dvttableview.cpp dvttableview.h\
                         dvttablewidgetitem.cpp dvttablewidgetitem.h\
                         dvttextfile.cpp dvttextfile.h\
                         dvttransportbutton.cpp dvttransportbutton.h\
                         dvtuser.cpp dvtuser.h\
                         dvtweb.cpp dvtweb.h\
                         dvtwidget.cpp dvtwidget.h\
                         login.cpp login.h\
                         spread.cpp spread.h\
                         spread_cell.cpp spread_cell.h\
                         spread_sheet.cpp spread_sheet.h\
                         spread_tab.cpp spread_tab.h\
                         state_conv.cpp state_conv.h\
                         statecombobox.cpp statecombobox.h

nodist_libdvt_la_SOURCES = moc_algocombobox.cpp\
                           moc_change_password.cpp\
                           moc_dvtdatedialog.cpp\
                           moc_dvtdatepicker.cpp\
                           moc_dvtdialog.cpp\
                           moc_dvtlabel.cpp\
                           moc_dvtlistselector.cpp\
                           moc_dvttableview.cpp\
                           moc_dvttransportbutton.cpp\
                           moc_dvtwidget.cpp\
                           moc_login.cpp\
                           moc_statecombobox.cpp

libdvt_la_LDFLAGS = -release $(VERSION)

EXTRA_DIST = lib.pro\
             dvtmail-win32.cpp

CLEANFILES = *~\
             *.idb\
             *.ilk\
             *.lib\
             *.obj\
             *.pdb\
             moc_*

MAINTAINERCLEANFILES = *~\
                       *.tar.gz\
                       aclocal.m4\
                       configure\
                       Makefile.in\
                       moc_*
