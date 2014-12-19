#!/usr/bin/perl -Wall

# xfer_contacts.pl
#
# Transfer contact info between Davit databases
#
# (C) Copyright 2014 Fred Gleason <fgleason@radioamerica.com>
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License version 2 as
#   published by the Free Software Foundation.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public
#   License along with this program; if not, write to the Free Software
#   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

use DBI;

sub EscapeString {
    $_=$_[0];
    s{\(}{\\(}g;
    s{\)}{\\)}g;
    s{\[}{\\[}g;
    s{\]}{\\]}g;
    s{\"}{\\"}g;
    s{\'}{\\'}g;
    s{\`}{\\`}g;
    s{\?}{\\?}g;
    s{\&}{\\&}g;
    s{\;}{\\;}g;
    s{\<}{\\<}g;
    s{\>}{\\>}g;
    s{\!}{\\!}g;

    return $_;
}


#
# Validate Arguments
#
my $usage="xfer_contacts <dst-database> <src-database>";
if(scalar @ARGV lt 2) {
    print $usage."\n";
    exit 256;
}

#
# Open the databases
#
my $src_dbh=DBI->connect("dbi:mysql:$ARGV[1]:localhost","dvtuser","");
if(!$src_dbh) {
    print "unable to open source database\n";
    exit 256;
}
my $dst_dbh=DBI->connect("dbi:mysql:$ARGV[0]:localhost","dvtuser","");
if(!$dst_dbh) {
    print "unable to open destination database\n";
    exit 256;
}

my $sql="select ID from CONTACTS where AFFIDAVIT=\"Y\"";
my $q=$dst_dbh->prepare($sql);
$q->execute();
my $count=0;
while(($row=$q->fetchrow_arrayref)) {
    $count++;
}
$q->finish();
printf("Affidavit contacts in destination: %d\n",$count);

$q=$src_dbh->prepare($sql);
$q->execute();
$count=0;
while(($row=$q->fetchrow_arrayref)) {
    $count++;
}
$q->finish();
printf("Affidavit contacts in source: %d\n",$count);

$count=0;
$sql="select ID,AFFILIATE_ID,NAME,TITLE,PHONE,FAX,EMAIL,AFFIDAVIT,PROGRAM_DIRECTOR,GENERAL_MANAGER,LOCKED from CONTACTS where AFFIDAVIT=\"Y\"";
$q=$src_dbh->prepare($sql);
$q->execute();
while(($row=$q->fetchrow_arrayref)) {
    $merge=0;
    $sql="select EMAIL from CONTACTS where ".
	sprintf("(AFFILIATE_ID=%d)&&",@$row[1]).
	"(AFFIDAVIT=\"Y\")";
    my $q1=$dst_dbh->prepare($sql);
    $q1->execute();
    if(($row1=$q1->fetchrow_arrayref)) {
	if(@$row1[0] eq "") {
	    $merge=1;
	}
    }
    else {
	$merge=1;
    }
    $q1->finish();
    if($merge) {
	$count++;
	$sql="select ID from CONTACTS where ID=".sprintf("%d",@$row[0]);
	my $q1=$dst_dbh->prepare($sql);
	$q1->execute();
	if($q1->fetchrow_arrayref) {  # Record exists, update
	    $sql="update CONTACTS set ".
		"AFFILIATE_ID=".sprintf("%d",@$row[1]).",".
		"NAME=\"".&EscapeString(@$row[2])."\",".
		"TITLE=\"".&EscapeString(@$row[3])."\",".
		"PHONE=\"".&EscapeString(@$row[4])."\",".
		"FAX=\"".&EscapeString(@$row[5])."\",".
		"EMAIL=\"".&EscapeString(@$row[6])."\",".
		"AFFIDAVIT=\"".&EscapeString(@$row[7])."\",".
		"PROGRAM_DIRECTOR=\"".&EscapeString(@$row[8])."\",".
		"GENERAL_MANAGER=\"".&EscapeString(@$row[9])."\",".
		"LOCKED=\"".&EscapeString(@$row[10])."\" ".
		"where ID=".sprintf("%d",@$row[0]);
	}
	else {                              # Create new record
	    $sql="insert into CONTACTS set ".
		"AFFILIATE_ID=".sprintf("%d",@$row[1]).",".
		"NAME=\"".&EscapeString(@$row[2])."\",".
		"TITLE=\"".&EscapeString(@$row[3])."\",".
		"PHONE=\"".&EscapeString(@$row[4])."\",".
		"FAX=\"".&EscapeString(@$row[5])."\",".
		"EMAIL=\"".&EscapeString(@$row[6])."\",".
		"AFFIDAVIT=\"".&EscapeString(@$row[7])."\",".
		"PROGRAM_DIRECTOR=\"".&EscapeString(@$row[8])."\",".
		"GENERAL_MANAGER=\"".&EscapeString(@$row[9])."\",".
		"LOCKED=\"".&EscapeString(@$row[10])."\"";
	}
	$q1=$dst_dbh->prepare($sql);
	$q1->execute();
	$q1->finish();
    }
}
$q->finish();

printf("Contacts transferred: %d\n",$count);

