## Makefile.am
##
## Makefile.am for rivendell/utils/dvtwgmgr
##
##   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
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
## Use automake to process this into a Makefile.in

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -std=c++17 -fPIC -I$(top_srcdir)/lib @QT6CLI_CFLAGS@
LIBS = -L$(top_srcdir)/lib

# QT's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	@QT6CLI_MOC@ $< -o $@

install-exec-am:
	install -d $(DESTDIR)@prefix@/lib/davit
	install -m 4755 -t $(DESTDIR)@prefix@/lib/davit .libs/dvtwgmgr

uninstall-local:
	rm -f $(DESTDIR)@prefix@/lib/davit/dvtwgmgr
	rmdir --ignore-fail-on-non-empty $(DESTDIR)@prefix@/lib/davit

noinst_PROGRAMS = dvtwgmgr

dist_dvtwgmgr_SOURCES = dvtwgmgr.cpp dvtwgmgr.h
nodist_dvtwgmgr_SOURCES = moc_dvtwgmgr.cpp
dvtwgmgr_LDADD = -ldvt @QT6CLI_LIBS@

CLEANFILES = *~\
             *.idb\
             *.ilk\
             *.obj\
             *.pdb\
             *.qm\
             moc_*

MAINTAINERCLEANFILES = *~\
                       *.tar.gz\
                       aclocal.m4\
                       configure\
                       Makefile.in\
                       moc_*
