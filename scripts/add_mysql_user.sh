# add_mysql_user.sh
#
# Add a MySQL user for Davit
#
# (C) Copyright 2011 Fred Gleason <fredg@paravelsystems.com>
#
#      $Id: add_mysql_user.sh,v 1.1 2011/01/06 17:22:38 pcvs Exp $
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

USAGE="add_mysql_user.sh <host-entry> [<server-hostname>]"

#
# Arguments
#
if [ -z $1 ] ; then
  echo $USAGE
  exit 256
fi
DAVIT_HOST=$1
if [ $2 ] ; then
  ADMIN_HOST="-h $2"
fi

#
# Site defines
#
ADMIN_USER="root"
DAVIT_USER="dvtuser"
DAVIT_PASSWORD="letmein"
DAVIT_DBNAME="Davit"

#
# SQL Statments
#
USER_SQL="insert into user set Host=\"$DAVIT_HOST\",User=\"$DAVIT_USER\",Password=password(\"$DAVIT_PASSWORD\")"

DB_SQL="insert into db set Host=\"$DAVIT_HOST\",Db=\"$DAVIT_DBNAME\",User=\"$DAVIT_USER\",Select_priv=\"Y\",Insert_priv=\"Y\",Update_priv=\"Y\",Delete_priv=\"Y\",Create_priv=\"Y\",Drop_priv=\"Y\",References_priv=\"Y\",Index_priv=\"Y\",Alter_priv=\"Y\",Create_tmp_table_priv=\"Y\",Lock_tables_priv=\"Y\",Create_view_priv=\"Y\",Show_view_priv=\"Y\",Create_routine_priv=\"Y\",Alter_routine_priv=\"Y\",Execute_priv=\"Y\""

FLUSH_SQL="flush privileges"

SQL="$USER_SQL;\n$DB_SQL;\n$FLUSH_SQL;\n"

echo "$SQL" | mysql $ADMIN_HOST -u $ADMIN_USER -p mysql
