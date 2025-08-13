## Makefile.am
##
## Top-level Makefile.am for Davit
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

SUBDIRS = helpers\
          debian\
          lib\
          scripts\
          conf\
          docs\
          icons\
          davit\
          web\
          utils

ACLOCAL_AMFLAGS = -I m4

maintainer-clean-local:	
	rm -rf m4

deb:	dist
	./build_debs.sh

rpm:	dist
	cp davit-$(VERSION).tar.gz $(HOME)/rpmbuild/SOURCES/
	$(RPMBUILD) -ba davit.spec
	mv $(HOME)/rpmbuild/RPMS/$(ARCH)/davit-*.rpm $(top_srcdir)/
	mv $(HOME)/rpmbuild/SRPMS/davit-*.src.rpm $(top_srcdir)/
	rm $(HOME)/rpmbuild/SOURCES/davit-$(VERSION).tar.gz
	rm -rf $(HOME)/rpmbuild/BUILD/davit-$(VERSION)

EXTRA_DIST = autogen.sh\
             build_win32.bat\
             davit-sample.ini\
             davit.ini kdmrc-example\
             davit.ism\
             davit.pro\
             davit.spec.in\
             get_target.sh\
             get_distro.pl\
             PACKAGE_VERSION

zip:	dist
	tar -zxf davit-@VERSION@.tar.gz
	zip -r davit-@VERSION@.zip davit-@VERSION@
	rm -rf davit-@VERSION@

CLEANFILES = *~\
             *.deb\
             *.ddeb\
             *.dsc\
             *.mo\
             *.rpm\
             *.tgz\
             *.zip\
             moc_*

DISTCLEANFILES = *~\
                 *.buildinfo\
                 *.changes\
                 *.spec\
                 *.tar.gz\
                 *.tar.xz\
                 build_debs.sh\
                 dvt-bin.conf\
                 moc_*

MAINTAINERCLEANFILES = *~\
                       *.tar.gz\
                       aclocal.m4\
                       compile\
                       config.guess\
                       config.sub\
                       configure\
                       depcomp\
                       install-sh\
                       ltmain.sh\
                       Makefile.in\
                       missing mkinstalldirs\
                       moc_*
