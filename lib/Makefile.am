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

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" @QT6CLI_CFLAGS@ -std=c++17 -fPIC
MOC = @QT6_MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

#instdir = @LOCAL_PREFIX@/lib

lib_LTLIBRARIES = libdvt.la
dist_libdvt_la_SOURCES = dvt.h dvt.cpp\
                         dvtcmdswitch.cpp dvtcmdswitch.h\
                         dvtconf.cpp dvtconf.h\
                         dvtconfig.cpp dvtconfig.h\
                         dvtdb.cpp dvtdb.h\
                         dvtimport.cpp dvtimport.h\
                         dvtprofile.cpp dvtprofile.h\
                         dvtprofileline.cpp dvtprofileline.h\
                         dvtprofilesection.cpp dvtprofilesection.h\
                         dvtsendmail.cpp dvtsendmail.h\
                         dvtsystem.cpp dvtsystem.h\
                         dvtuser.cpp dvtuser.h\
                         dvtweb.cpp dvtweb.h

# nodist_libdvt_la_SOURCES =

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
