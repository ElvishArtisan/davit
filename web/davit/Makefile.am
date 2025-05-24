## Makefile.am
##
## Makefile.am for rivendell/web/davit
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

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -Wno-strict-aliasing -std=c++17 -fPIC -I$(top_srcdir)/lib @QT6_CFLAGS@
LIBS = -L$(top_srcdir)/lib
MOC = @QT6_MOC@

libexec_PROGRAMS = davit.cgi

dist_davit_cgi_SOURCES = davit.cpp davit.h
davit_cgi_LDADD = @QT6_LIBS@ @VMIME_LIBS@ -ldvt

EXTRA_DIST = davit.pro

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
