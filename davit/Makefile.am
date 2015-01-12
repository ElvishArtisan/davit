## automake.am
##
## davit/ Makefile.am for Davit
##
## (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
##
##     $Id: Makefile.am,v 1.30 2013/02/26 19:06:17 pcvs Exp $
##
## Use automake to process this into a Makefile.in
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

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -DQTDIR=\"@QT_DIR@\" @QT_CXXFLAGS@
INCLUDES = -I$(top_srcdir)/lib
LIBS = @QT_LIBS@ -L$(top_srcdir)/lib
MOC = @QT_MOC@

# QT's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@

bin_PROGRAMS = davit
dist_davit_SOURCES = activity_report.cpp\
                     add_affiliate.cpp add_affiliate.h\
                     add_program.cpp add_program.h\
                     add_provider.cpp add_provider.h\
                     add_remark.cpp add_remark.h\
                     add_user.cpp add_user.h\
                     addedprograms_report.cpp\
                     affidavit_picker.cpp affidavit_picker.h\
                     affidavit_report.cpp\
                     affiliatesbymarket_report.cpp\
                     affiliatesbyprogram_report.cpp\
                     arbitron_report.cpp\
                     contact.cpp contact.h\
                     contactlistview.cpp contactlistview.h\
                     createdb.cpp createdb.h\
                     davit.cpp davit.h\
                     daypicker.cpp daypicker.h\
                     edit_affiliate.cpp edit_affiliate.h\
                     edit_airing.cpp edit_airing.h\
                     edit_contact.cpp edit_contact.h\
                     edit_feed.cpp edit_feed.h\
                     edit_network.cpp edit_network.h\
                     edit_program.cpp edit_program.h\
                     edit_provider.cpp edit_provider.h\
                     edit_system.cpp edit_system.h\
                     edit_user.cpp edit_user.h\
                     generate_affadavit.cpp generate_affadavit.h\
                     geometry.cpp geometry.h\
                     globals.h\
                     list_airings.cpp list_airings.h\
                     list_affiliates.cpp list_affiliates.h\
                     list_contacts.cpp list_contacts.h\
                     list_networks.cpp list_networks.h\
                     list_programs.cpp list_programs.h\
                     list_providers.cpp list_providers.h\
                     list_remarks.cpp list_remarks.h\
                     list_reports.cpp list_reports.h\
                     list_users.cpp list_users.h\
                     maildialog.cpp maildialog.h\
                     migrate_affiliates.cpp migrate_affiliates.h\
                     missingaffidavit_report.cpp\
                     missingaffidavitcontact_report.cpp\
                     mysql_login.cpp mysql_login.h\
                     opendb.cpp opendb.h\
                     pick_daypart.cpp pick_daypart.h\
                     pick_fields.cpp pick_fields.h\
                     precisiontrak.cpp\
                     preview.cpp preview.h\
                     program_report.cpp\
                     programbymarket_report.cpp\
                     raaffiliate_report.cpp\
                     showaffidavits.cpp showaffidavits.h

nodist_davit_SOURCES = moc_add_affiliate.cpp\
                       moc_generate_affadavit.cpp\
                       moc_add_program.cpp\
                       moc_add_provider.cpp\
                       moc_add_remark.cpp\
                       moc_add_user.cpp\
                       moc_affidavit_picker.cpp\
                       moc_contactlistview.cpp\
                       moc_davit.cpp\
                       moc_daypicker.cpp\
                       moc_edit_affiliate.cpp\
                       moc_edit_airing.cpp\
                       moc_edit_contact.cpp\
                       moc_edit_feed.cpp\
                       moc_edit_network.cpp\
                       moc_edit_program.cpp\
                       moc_edit_provider.cpp\
                       moc_edit_system.cpp\
                       moc_edit_user.cpp\
                       moc_list_affiliates.cpp\
                       moc_list_airings.cpp\
                       moc_list_contacts.cpp\
                       moc_list_networks.cpp\
                       moc_list_programs.cpp\
                       moc_list_providers.cpp\
                       moc_list_remarks.cpp\
                       moc_list_reports.cpp\
                       moc_list_users.cpp\
                       moc_maildialog.cpp\
                       moc_migrate_affiliates.cpp\
                       moc_mysql_login.cpp\
                       moc_pick_daypart.cpp\
                       moc_pick_fields.cpp\
                       moc_preview.cpp\
                       moc_showaffidavits.cpp

davit_LDADD = -ldvt @VMIME_LIBS@ @QT_LIBS@

EXTRA_DIST = davit.pro

CLEANFILES = *~ moc_* *.obj *.exe *.idb *.pdb *.ilk
DISTCLEANFILES = moc_*
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4 moc_* *.tar.gz
