## Makefile.am
##
## davit/ Makefile.am for Davit
##
## (C) Copyright 2007-2025 Fred Gleason <fredg@paravelsystems.com>
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

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -std=c++17 -fPIC -I$(top_srcdir)/lib @QT6_CFLAGS@
LIBS = -L$(top_srcdir)/lib
MOC = @QT6_MOC@

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
                     affidavitaffiliatereport.cpp\
                     affidavitprogramreport.cpp\
                     affiliatesbymarket_report.cpp\
                     affiliatesbyprogram_report.cpp\
                     affiliatelistmodel.cpp affiliatelistmodel.h\
                     airinglistmodel.cpp airinglistmodel.h\
                     algocombobox.cpp algocombobox.h\
                     arbitron_report.cpp\
                     change_password.cpp change_password.h\
                     contact.cpp contact.h\
                     contactlistmodel.cpp contactlistmodel.h\
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
                     feedlistmodel.cpp feedlistmodel.h\
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
                     networklistmodel.cpp networklistmodel.h\
                     opendb.cpp opendb.h\
                     pick_daypart.cpp pick_daypart.h\
                     pick_fields.cpp pick_fields.h\
                     precisiontrak.cpp\
                     program_report.cpp\
                     programlistmodel.cpp programlistmodel.h\
                     programbymarket_report.cpp\
                     providerlistmodel.cpp providerlistmodel.h\
                     raaffiliate_report.cpp\
                     showaffidavits.cpp showaffidavits.h\
                     spread.cpp spread.h\
                     spread_cell.cpp spread_cell.h\
                     spread_sheet.cpp spread_sheet.h\
                     spread_tab.cpp spread_tab.h\
                     userlistmodel.cpp userlistmodel.h\
                     statecombobox.cpp statecombobox.h\
                     viewerprocess.cpp viewerprocess.h\
                     viewerprocesslist.cpp viewerprocesslist.h

nodist_davit_SOURCES = moc_add_affiliate.cpp\
                       moc_change_password.cpp\
                       moc_generate_affadavit.cpp\
                       moc_add_program.cpp\
                       moc_add_provider.cpp\
                       moc_add_remark.cpp\
                       moc_add_user.cpp\
                       moc_affidavit_picker.cpp\
                       moc_affiliatelistmodel.cpp\
                       moc_airinglistmodel.cpp\
                       moc_algocombobox.cpp\
                       moc_contactlistmodel.cpp\
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
                       moc_feedlistmodel.cpp\
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
                       moc_networklistmodel.cpp\
                       moc_pick_daypart.cpp\
                       moc_pick_fields.cpp\
                       moc_programlistmodel.cpp\
                       moc_providerlistmodel.cpp\
                       moc_showaffidavits.cpp\
                       moc_statecombobox.cpp\
                       moc_userlistmodel.cpp\
                       moc_viewerprocess.cpp\
                       moc_viewerprocesslist.cpp

davit_LDADD = -ldvt @QT6_LIBS@

EXTRA_DIST = davit.pro

CLEANFILES = *~ moc_* *.obj *.exe *.idb *.pdb *.ilk
DISTCLEANFILES = moc_*
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4 moc_* *.tar.gz
