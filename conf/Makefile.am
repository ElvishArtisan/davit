## Makefile.am
##
## (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
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
##
## Use automake to process this into a Makefile.in

install-exec-am:	
	mkdir -p $(DESTDIR)/$(sysconfdir)
	cp dvt-bin.conf $(DESTDIR)/$(sysconfdir)/
	mkdir -p $(DESTDIR)$(prefix)/share/davit
	cp davit-sample.conf $(DESTDIR)$(prefix)/share/davit/

uninstall:	
	rm -f $(DESTDIR)/$(sysconfdir)/dvt-bin.conf
	rm -f $(DESTDIR)$(prefix)/share/davit/davit-sample.conf

EXTRA_DIST = davit-sample.conf\
             dvt-bin.conf.in

CLEANFILES = *~
DISTCLEANFILES = 
MAINTAINERCLEANFILES = *~\
                       aclocal.m4\
                       configure\
                       Makefile.in
