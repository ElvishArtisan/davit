## Makefile.am
##
## docs/manpages/Makefile.am for Davit
##
## (C) Copyright 2025 Fred Gleason <fredg@paravelsystems.com>
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

##
## Build Dependencies
##
%.html:	%.xml
	xsltproc -o $@ $(DOCBOOK_STYLESHEETS)/xhtml/docbook.xsl $<
%.pdf:	%.xml
	xsltproc $(DOCBOOK_STYLESHEETS)/fo/docbook.xsl $< | fop - -pdf $@
%.1:	%.xml
	xsltproc $(DOCBOOK_STYLESHEETS)/manpages/docbook.xsl $<
%.5:	%.xml
	xsltproc $(DOCBOOK_STYLESHEETS)/manpages/docbook.xsl $<
%.7:	%.xml
	xsltproc $(DOCBOOK_STYLESHEETS)/manpages/docbook.xsl $<
%.8:	%.xml
	xsltproc $(DOCBOOK_STYLESHEETS)/manpages/docbook.xsl $<

all-local:	davit.conf.5\
                davit.conf.html\
                davit.conf.pdf\
                dvtdbmgr.8\
                dvtdbmgr.html\
                dvtdbmgr.pdf\
                dvtstamp.8\
                dvtstamp.html\
                dvtstamp.pdf

man_MANS = davit.conf.5\
           dvtdbmgr.8\
           dvtstamp.8

EXTRA_DIST = davit.conf.5\
             davit.conf.html\
             davit.conf.pdf\
             davit.conf.xml\
             dvtdbmgr.8\
             dvtdbmgr.html\
             dvtdbmgr.pdf\
             dvtdbmgr.xml\
             dvtstamp.8\
             dvtstamp.html\
             dvtstamp.pdf\
             dvtstamp.xml

CLEANFILES = *~
MAINTAINERCLEANFILES = *~\
                       *.1\
                       *.5\
                       *.5\
                       *.7\
                       *.8\
                       *.html\
                       *.pdf\
                       aclocal.m4\
                       configure\
                       Makefile.in
