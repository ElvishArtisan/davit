## automake.am
##
## Top-level Makefile.am for Davit
##
## Use automake to process this into a Makefile.in
##
##   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
##
##     $Id: Makefile.am,v 1.12 2014/05/13 11:20:37 pcvs Exp $
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

SUBDIRS = helpers scripts docs icons lib davit web utils

rpm:	dist
	cp davit-$(VERSION).tar.gz $(RPM_ROOT)/SOURCES/
	$(RPMBUILD) -ba davit.spec
	mv $(RPM_ROOT)/RPMS/$(ARCH)/davit-*.rpm $(top_srcdir)/
	mv $(RPM_ROOT)/SRPMS/davit-*.src.rpm $(top_srcdir)/
	rm $(RPM_ROOT)/SOURCES/davit-$(VERSION).tar.gz
	rm -rf $(RPM_ROOT)/BUILD/davit-$(VERSION)

EXTRA_DIST = davit.spec.in davit.pro autogen.sh davit.conf-sample\
             get_target.sh dvt-bin.conf.in get_target.sh get_distro.sh\
             davit.ism build_win32.bat PACKAGE_VERSION davit-sample.ini\
             davit.ini kdmrc-example

zip:	dist
	tar -zxf davit-@VERSION@.tar.gz
	zip -r davit-@VERSION@.zip davit-@VERSION@
	rm -rf davit-@VERSION@

CLEANFILES = *~ moc_* *.tgz *.mo *.rpm *.zip
DISTCLEANFILES = *~ moc_* *.tar.gz *.spec dvt-bin.conf
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4 moc_* *.tar.gz\
                       depcomp install-sh missing mkinstalldirs\
                       config.guess config.sub ltmain.sh
