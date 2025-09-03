## Makefile.am
##
## Makefile.am for icons/
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

install-exec-am:	
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps
	cp davit-16x16.png $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/davit.png
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/22x22/apps
	cp davit-22x22.png $(DESTDIR)@prefix@/share/icons/hicolor/22x22/apps/davit.png
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/32x32/apps
	cp davit-32x32.png $(DESTDIR)@prefix@/share/icons/hicolor/32x32/apps/davit.png
	mkdir -p $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps
	cp davit-48x48.png $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/davit.png

uninstall-local:	
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/16x16/apps/davit.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/22x22/apps/davit.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/32x32/apps/davit.png
	rm -f $(DESTDIR)@prefix@/share/icons/hicolor/48x48/apps/davit.png

EXTRA_DIST = admin.xpm\
             blueball.xpm\
             davit-16x16.png\
             davit-22x22.png\
             davit-22x22.xpm\
             davit-32x32.png\
             davit-48x48.png\
             davit.res\
             davit.ico\
             greenball.xpm\
             network.xpm\
             program.xpm\
             provider.xpm\
             redball.xpm\
             report.xpm\
             user.xpm\
             whiteball.xpm

CLEANFILES = *~
DISTCLEANFILES = moc_*
MAINTAINERCLEANFILES = *~\
                       *.tar.gz\
                       aclocal.m4\
                       configure\
                       Makefile.in\
                       moc_*
