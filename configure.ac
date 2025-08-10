dnl configure.in
dnl
dnl Autoconf configuration for Davit.
dnl Use autoconf to process this into a configure script
dnl
dnl   (C) Copyright 2007-2025 Fred Gleason <fredg@paravelsystems.com>
dnl
dnl   This program is free software; you can redistribute it and/or modify
dnl   it under the terms of the GNU General Public License version 2 as
dnl   published by the Free Software Foundation.
dnl
dnl   This program is distributed in the hope that it will be useful,
dnl   but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl   GNU General Public License for more details.
dnl
dnl   You should have received a copy of the GNU General Public
dnl   License along with this program; if not, write to the Free Software
dnl   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
dnl

AC_INIT(davit,m4_esyscmd(cat PACKAGE_VERSION))
AM_INIT_AUTOMAKE
AC_SUBST(RPM_RELEASE,1)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])
AC_CONFIG_MACRO_DIR([m4])

#
# Some Fully Resolved Install Paths
# (for the packaging system and friends)
#
if test ${prefix} = "NONE" ; then
  AC_SUBST(LOCAL_PREFIX,/usr/local)
else
  AC_SUBST(LOCAL_PREFIX,${prefix})
fi

#
# Basic Compiler Checks
#
AC_PROG_CXX
AC_LANG(C++)
LT_INIT

#
# Check for Qt
#
AQ_FIND_QT6(QT6GUI,Core Gui Widgets Network Sql PrintSupport,[],[AC_MSG_ERROR([*** Qt6 GUI components not found ***])])
AQ_FIND_QT6(QT6CLI,Core Network Sql PrintSupport,[],[AC_MSG_ERROR([*** Qt6 CLI not found ***])])

#
# DocBook
#
if test $DOCBOOK_STYLESHEETS ; then
   ln -s $DOCBOOK_STYLESHEETS helpers/docbook
else
   AC_MSG_ERROR([*** Unable to find docbook5 stylesheets ***])
fi

#
# Determine the target architecture
#
AR_GCC_TARGET()
AC_SUBST(VENDOR,$ar_gcc_distro)
AC_SUBST(ARCH,$ar_gcc_arch)

#
# Determine number of available CPU cores for building packages
#
AX_COUNT_CPUS([],[])
if test $CPU_COUNT -lt 2 ; then
   AC_SUBST(CPUS_AVAIL,1)
else
   if test $CPU_COUNT -gt 16 ; then
      AC_SUBST(CPUS_AVAIL,16)
   else
      AC_SUBST(CPUS_AVAIL,$(($CPU_COUNT-1)))
   fi
fi

#
# Determine Distro
#
AR_GET_DISTRO()

#
# Configure RPM Build
#
AC_CHECK_PROG(RPMBUILD_FOUND,rpmbuild,[yes],[])
if test -z $RPMBUILD_FOUND ; then
  AC_SUBST(RPMBUILD,rpm)
else
  AC_SUBST(RPMBUILD,rpmbuild)
fi
if test -d /usr/src/redhat ; then
  AC_SUBST(RPM_ROOT,/usr/src/redhat)
else
  AC_SUBST(RPM_ROOT,/usr/src/packages)
fi 
if test $ar_gcc_arch = x86_64 ; then
  AC_SUBST(DAVIT_LIB_PATH,lib64)
else
  AC_SUBST(DAVIT_LIB_PATH,lib)
fi

AC_CONFIG_FILES(Makefile \
  helpers/Makefile \
  scripts/Makefile \
  lib/Makefile \
  davit/Makefile \
  web/Makefile \
  web/davit/Makefile \
  utils/Makefile \
  utils/dvtdbcheck/Makefile \
  utils/dvtdbmgr/Makefile \
  utils/dvtstamp/Makefile \
  utils/vnc/Makefile \
  utils/vnc/davit.xinitrc \
  utils/vnc/start_davit.sh \
  conf/Makefile \
  docs/manpages/Makefile \
  docs/Makefile \
  icons/Makefile \
  davit.spec \
  conf/dvt-bin.conf
) 
AC_OUTPUT()
chmod 755 utils/vnc/davit.xinitrc
chmod 755 utils/vnc/start_davit.sh

