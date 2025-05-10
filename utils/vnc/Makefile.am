## Makefile.am
##
## utils/vnc Directory Makefile.am for Davit
##
## (C) Copyright 2010-2025 Fred Gleason <fredg@paravelsystems.com>
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

install-exec-local:	
	cp davit.xinitrc @prefix@/bin/
	cp start_davit.sh @prefix@/bin/
	mkdir -p @prefix@/share/fvwm
	cp davit-config @prefix@/share/fvwm/

uninstall:	
	rm -f @prefix@/bin/davit.xinitrc
	rm -f @prefix@/bin/start_davit.sh
	rm -f @prefix@/share/fvwm/davit-config

EXTRA_DIST = davit-config\
             davit.xinitrc.in\
             start_davit.sh.in

CLEANFILES = *~

DISTCLEANFILES = davit.xinitrc\
                 start_davit.sh

MAINTAINERCLEANFILES = *~ Makefile.in
