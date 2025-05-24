// davit.cpp
//
// An Web-based affiliate manager
//
//   (C) Copyright 2008-2025 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <vector>

#include <vmime/vmime.hpp>
#include <vmime/platforms/posix/posixHandler.hpp>

#include <QApplication>
#include <QDateTime>
#include <QSqlQuery>

#include <dvtconfig.h>
#include <dvtweb.h>
#include <dvtconf.h>
#include <dvtmail.h>

#include "davit.h"

//
// Define this to allow logins with "broken" usernames
// --i.e. with no hyphen between call letters and station type.
//
#define DAVIT_WEB_LOGIN_HACK

MainObject::MainObject(QObject *parent)
  :QObject(parent)
{
  /*
   * FIXME: Port to Qt6
   *
  //
  // Initialize Variables
  //
  cast_session_id=-1;

  //
  // Read Configuration
  //
  cast_config=new DvtConfig();
  cast_config->load();

  //
  // Initialize VMime
  //
  vmime::platform::setHandler<vmime::platforms::posix::posixHandler>();

  //
  // Open Database
  //
  QSqlDatabase *db=QSqlDatabase::addDatabase(cast_config->mysqlDbtype());
  if(!db) {
    printf("Content-type: text/html\n\n");
    printf("rdfeed: unable to initialize connection to database\n");
    exit(0);
  }
  db->setDatabaseName(cast_config->mysqlDbname());
  db->setUserName(cast_config->mysqlUsername());
  db->setPassword(cast_config->mysqlPassword());
  db->setHostName(cast_config->mysqlHostname());
  if(!db->open()) {
    printf("Content-type: text/html\n\n");
    printf("davit: unable to connect to database\n");
    db->removeDatabase(cast_config->mysqlDbname());
    exit(0);
  }

  //
  // Read Post Variables and Dispatch 
  //
  switch(AuthenticatePost()) {
    case DAVIT_COMMAND_LOGIN:
      ServeLogin();
      break;

    case DAVIT_COMMAND_LOGOUT:
      ServeLogout();
      break;

    case DAVIT_COMMAND_MONTH_PICKER:
      ServeMonthPicker();
      break;

    case DAVIT_COMMAND_SERVE_PROGRAM_LIST:
      ServeProgramList();
      break;

    case DAVIT_COMMAND_PROCESS_PROGRAMS:
      ProcessPrograms();
      break;

    default:
      DvtCgiError("Invalid post data!");
      break;
  }
  */
  exit(0);
}


int MainObject::AuthenticatePost()
{
  int cmd=0;
  char str[1024];
  QString username;

  DvtReadPost(cast_post,DAVIT_POST_SIZE);

  //
  // Check for Command Value
  //
  if(DvtGetPostInt(cast_post,"COMMAND",&cmd)<0) {
    return DAVIT_COMMAND_LOGIN;
  }

  //
  // Validate Remote Address
  //
  if(getenv("REMOTE_ADDR")==NULL) {
    DvtCgiError("Invalid CGI request!");
    exit(0);
  }
  cast_client_addr.setAddress(getenv("REMOTE_ADDR"));
  if(cast_client_addr.isNull()) {
    DvtCgiError("Invalid client IP address!");
    exit(0);
  }

  //
  // Authenticate New Login
  //
  if((DvtGetPostLongInt(cast_post,"SESSION_ID",&cast_session_id))<0) {
    if((DvtGetPostString(cast_post,"USERNAME",str,1024))<0) {
      return DAVIT_COMMAND_LOGIN;
    }
    username=str;
#ifdef DAVIT_WEB_LOGIN_HACK
    if(((username.right(2).toLower()=="am")||
	(username.right(2).toLower()=="fm"))&&
       username.mid(username.length()-3,1)!="-") {
      username=username.left(username.length()-2)+"-"+username.right(2);
    }
#endif  // DAVIT_WEB_LOGIN_HACK
    if((DvtGetPostString(cast_post,"PASSWORD",str,1024))<0) {
      return DAVIT_COMMAND_LOGIN;
    }
    if((cast_session_id=
	DvtAuthenticateLogin(username,str,cast_client_addr,
			     &cast_affiliate_id))<0) {
      return DAVIT_COMMAND_LOGIN;
    }
    return cmd;
  }

  //
  // Authenticate Existing Session
  //
  if((cast_affiliate_id=
      DvtAuthenticateSession(cast_session_id,cast_client_addr))<0) {
    return DAVIT_COMMAND_LOGIN;
  }

  return cmd;
}


void MainObject::ServeLogin()
{
  printf("Content-type: text/html\n\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("<title>Radio America Affiliate Login</title>\n");
  printf("</head>\n");

  printf("<body bgcolor=\"%s\">\n",DVT_WEB_BACKGROUND_COLOR);
  printf("<table border=\"0\" cellpadding=\"5\" cellspacing=\"0\">\n");
  printf("<form action=\"davit.cgi\" method=\"post\">\n");
  printf("<input type=\"hidden\" name=\"COMMAND\" value=\"%d\">",
	 DAVIT_COMMAND_MONTH_PICKER);

  printf("<tr>\n");
  printf("<td align=\"center\" colspan=\"2\"><big>RadioAmerica Affiliate Login</big></td>\n");
  printf("</tr>\n");

  printf("<tr>\n");
  printf("<td bgcolor=\"%s\" align=\"right\">Login Name:</td>\n",
	 DVT_WEB_LINE_COLOR1);
  printf("<td bgcolor=\"%s\"  align=\"left\"><input type=\"text\" name=\"USERNAME\" length=\"8\" maxlength=\"8\"></td>\n",
	 DVT_WEB_LINE_COLOR1);
  printf("</tr>\n");

  printf("<tr>\n");
  printf("<td bgcolor=\"%s\"  align=\"right\">Password:</td>\n",
	 DVT_WEB_LINE_COLOR1);
  printf("<td bgcolor=\"%s\"  align=\"left\"><input type=\"password\" name=\"PASSWORD\" length=\"32\" maxlength=\"32\"></td>\n",
	 DVT_WEB_LINE_COLOR1);
  printf("</tr>\n");

  printf("<tr>\n");
  printf("<td colspan=\"2\">&nbsp;</td>\n");
  printf("</tr>\n");

  printf("<tr>\n");
  printf("<td>&nbsp;</td>\n");
  printf("<td align=\"right\"><input type=\"submit\" value=\"OK\"></td>\n");
  printf("</tr>\n");

  printf("</form>\n");
  printf("</table>\n");
  printf("</body>\n");
  printf("</html>\n");
  exit(0);
}


void MainObject::ServeLogout()
{
  DvtLogoutSession(cast_session_id,cast_client_addr);
  ServeLogin();
}


void MainObject::ServeMonthPicker()
{
  QString sql;
  QSqlQuery *q;

  printf("Content-type: text/html\n\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("<title>Affiliate Affidavit Reporter</title>\n");
  printf("</head>\n");
  printf("<body bgcolor=\"%s\">\n",DVT_WEB_BACKGROUND_COLOR);
  printf("<table border=\"0\" cellpadding=\"5\" cellspacing=\"0\">\n");
  printf("<tr><td colspan=\"2\"><big><strong><center>RadioAmerica Affiliate Reporter</center></strong></big></td></tr>\n");
  QDate today=QDate::currentDate();
  QDate date=today.addMonths(-12);
  bool data_needed=false;
  bool first=true;
  if(date<QDate(2008,1,1)) {
    date=QDate(2008,1,31);
  }
  while(date<today) {
    sql=QString().sprintf("select ID from AIRED where (AFFILIATE_ID=%d)&&\
                           (STATE=%d)&&\
                           (AIR_DATETIME>=\"%s-01 00:00:00\")&&\
                           (AIR_DATETIME<\"%s-01 00:00:00\")",
			  cast_affiliate_id,
			  Dvt::AiredStateScheduled,
			  date.toString("yyyy-MM").toUtf8().constData(),
			  date.addMonths(1).toString("yyyy-MM").
			  toUtf8().constData());
    q=new QSqlQuery(sql);
    if(q->first()) {
      data_needed=true;
      if(first) {
	printf("<tr><td colspan=\"2\"><center>Click a button to enter clearance data for the indicated month/year.</center></td></tr>\n");
	first=false;
      }
      printf("<tr><td align=\"center\">\n");
      printf("<form action=\"davit.cgi\" method=\"post\">\n");
      SetContext(DAVIT_COMMAND_SERVE_PROGRAM_LIST);
      printf("<input type=\"hidden\" name=\"LAST_MONTH_MONTH\" value=\"%s\">\n",
	     date.toString("MM").toUtf8().constData());
      printf("<input type=\"hidden\" name=\"LAST_MONTH_YEAR\" value=\"%s\">\n",
	     date.toString("yyyy").toUtf8().constData());
      printf("<input type=\"submit\" value=\"%s\" size=\"20\">\n",
	     date.toString("MMMM yyyy").toUtf8().constData());
      printf("</form>\n");
      printf("</td></tr>\n");
    }
    delete q;
    date=date.addMonths(1);
  }
  printf("<tr>\n");
  printf("<td align=\"center\">\n");
  if(!data_needed) {
    printf("Our records show that your clearance data are currently up to date.  Thank you!\n");
    printf("</td></tr>\n");
    printf("<tr>\n");
    printf("<td align=\"center\">\n");
  }
  if(!cast_config->contactAddress().isEmpty()) {
    printf("Questions?  Please feel free to contact us at\n");
    printf("<a href=\"mailto:%s\">%s</a>.\n",
	   cast_config->contactAddress().toUtf8().constData(),
	   cast_config->contactAddress().toUtf8().constData());
    printf("</td>\n");
    printf("</tr>\n");
    
  }
  printf("</table>\n");
  printf("</body>\n");
  printf("</html>\n");
}


void MainObject::ServeProgramList()
{
  QString sql;
  QSqlQuery *q;
  int last_month;
  int last_year;

  if((DvtGetPostInt(cast_post,"LAST_MONTH_MONTH",&last_month))<0) {
    DvtCgiError("Missing LAST_MONTH_MONTH");
    exit(0);
  }
  if((DvtGetPostInt(cast_post,"LAST_MONTH_YEAR",&last_year))<0) {
    DvtCgiError("Missing LAST_MONTH_YEAR");
    exit(0);
  }
  QDate date(last_year,last_month,1);
  QDate today=QDate::currentDate();

  printf("Content-type: text/html\n\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("<title>Affiliate Affidavit Reporter</title>\n");
  printf("</head>\n");
  printf("<body bgcolor=\"%s\">\n",DVT_WEB_BACKGROUND_COLOR);
  printf("<table border=\"0\" cellpadding=\"5\" cellspacing=\"0\">\n");
  sql=QString().sprintf("select STATION_CALL,STATION_TYPE from\
                           AFFILIATES where ID=%d",cast_affiliate_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    printf("<tr>\n");
    printf("<td colspan=\"6\" align=\"center\"><big><strong>\n");
    if(q->value(1).toString()=="A") {
      printf("Scheduled Programs for %s-AM",
	     q->value(0).toString().toUtf8().constData());
    }
    if(q->value(1).toString()=="F") {
      printf("Scheduled Programs for %s-FM",
	     q->value(0).toString().toUtf8().constData());
    }
    if(q->value(1).toString()=="I") {
      printf("Scheduled Programs for %s-IN",
	     q->value(0).toString().toUtf8().constData());
    }
    printf("</td>\n");
    printf("</tr>\n");
    delete q;

    printf("<tr>\n");
    printf("<td colspan=\"6\" align=\"center\">\n");
    printf("If your station aired the programs as listed below, then simply\n");
    printf("click <strong>OK</strong>.<br>If start and/or end times are incorrect, please change them accordingly.<br>If a program was not aired at all,\n");
    printf("indicate &quot;No&quot; for it.\n");
    printf("</td>\n");
    printf("</tr>\n");

    printf("<tr>\n");
    printf("<td colspan=\"6\" align=\"center\"><big>\n");
    printf("%s",QDate(last_year,last_month,1).toString("MMMM yyyy").
	   toUtf8().constData());
    printf("</big></td>\n");
    printf("</tr>\n");

    printf("<form action=\"davit.cgi\" method=\"post\">\n");
    SetContext(DAVIT_COMMAND_PROCESS_PROGRAMS);
    printf("<input type=\"hidden\" name=\"LAST_MONTH\" value=\"%d\">\n",
	   last_month);
    printf("<input type=\"hidden\" name=\"LAST_YEAR\" value=\"%d\">\n",
	   last_year);
    printf("<input type=\"hidden\" name=\"THIS_MONTH\" value=\"%d\">\n",
	   today.month());
    printf("<input type=\"hidden\" name=\"THIS_YEAR\" value=\"%d\">\n",
	   today.year());
    printf("<tr>\n");
    printf("<th>Program Name</th>\n");
    printf("<th>Air Date</th>\n");
    printf("<th>Start Time</th>\n");
    printf("<th>&nbsp;</th>\n");
    printf("<th>End Time</th>\n");
    printf("<th>Aired?</th>\n");
    printf("</tr>\n");
    QString line_colors[2];
    line_colors[0]=DVT_WEB_LINE_COLOR1;
    line_colors[1]=DVT_WEB_LINE_COLOR2;
    int current_color=0;
    sql=QString().sprintf("select PROGRAMS.PROGRAM_NAME,AIRED.AIR_DATETIME,\
                           AIRED.AIR_LENGTH,AIRED.ID from PROGRAMS \
                           left join AIRED on AIRED.PROGRAM_ID=PROGRAMS.ID \
                           where (AIRED.AFFILIATE_ID=%d)&&\
                           (AIRED.STATE=%d)&&\
                           (AIRED.AIR_DATETIME>=\"%s-01 00:00:00\")&&\
                           (AIRED.AIR_DATETIME<\"%s-01 00:00:00\") \
                           order by PROGRAMS.PROGRAM_NAME,AIRED.AIR_DATETIME",
			  cast_affiliate_id,
			  Dvt::AiredStateScheduled,
			  date.toString("yyyy-MM").toUtf8().constData(),
			  date.addMonths(1).toString("yyyy-MM").
			  toUtf8().constData());
    q=new QSqlQuery(sql);
    while(q->next()) {
      printf("<tr>\n");
      printf("<td bgcolor=\"%s\" align=\"left\">%s</td>\n",
	     line_colors[current_color].toUtf8().constData(),
	     q->value(0).toString().toUtf8().constData());
      printf("<td bgcolor=\"%s\" align=\"center\">%s</td>",
	     line_colors[current_color].toUtf8().constData(),
	     q->value(1).toDateTime().toString("MM/dd/yyyy").
	     toUtf8().constData());
      printf("<td bgcolor=\"%s\" align=\"center\">%s</td>\n",
	     line_colors[current_color].toUtf8().constData(),
	     TimeWidget(q->value(3).toInt(),"START",q->value(1).toTime()).
	     toUtf8().constData());
      printf("<td bgcolor=\"%s\" align=\"center\">&nbsp;</td>\n",
	     line_colors[current_color].toUtf8().constData());
      printf("<td bgcolor=\"%s\" align=\"center\">%s</td>\n",
	     line_colors[current_color].toUtf8().constData(),
	     TimeWidget(q->value(3).toInt(),"END",q->value(1).toTime().
			addSecs(q->value(2).toInt())).toUtf8().constData());
      printf("<td bgcolor=\"%s\" align=\"center\">",
	     line_colors[current_color].toUtf8().constData());
      printf("<select name=\"CORRECT_%d\">\n",q->value(3).toInt());
      printf("  <option value=\"Yes\" selected>Yes</option>\n");
      printf("  <option value=\"No\">No</option>\n");
      printf("</select>\n");

      printf("</tr>\n");
      if(current_color==0) {
	current_color=1;
      }
      else {
	current_color=0;
      }
    }
    delete q;
  }
  printf("<tr>\n");
  printf("<td colspan=\"6\">&nbsp</td>\n");
  printf("</tr>\n");
  printf("<td colspan=\"5\">&nbsp</td>\n");
  printf("<td align=\"center\"><input type=\"submit\" value=\"OK\"></td>\n");
  printf("</tr>\n");

  printf("</form>\n");

  exit(0);
}


void MainObject::ProcessPrograms()
{
  /*
   * FIXME: Port to Qt6
   *
  char str[1024];
  char var[1024];
  int last_month;
  int last_year;
  int this_month;
  int this_year;
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
  std::vector<int> bad_airings;
  QString name="[unknown]";
  QString phone="[unknown]";
  QString fax="[unknown]";
  QString email="[unknown]";
  Dvt::AiredState state;
  QTime start_time;
  int air_length;
  int discreps=0;

  //
  // Get Post Variables
  //
  if(DvtGetPostInt(cast_post,"LAST_MONTH",&last_month)<0) {
    DvtCgiError("Missing LAST_MONTH");
  }
  if(DvtGetPostInt(cast_post,"LAST_YEAR",&last_year)<0) {
    DvtCgiError("Missing LAST_MONTH");
  }
  QDate date(last_year,last_month,1);
  if(DvtGetPostInt(cast_post,"THIS_MONTH",&this_month)<0) {
    DvtCgiError("Missing THIS_MONTH");
  }
  if(DvtGetPostInt(cast_post,"THIS_YEAR",&this_year)<0) {
    DvtCgiError("Missing THIS_YEAR");
  }

  //
  // Get Signing Credentials
  //
  sql=QString().sprintf("select NAME,PHONE,FAX,EMAIL from CONTACTS \
                         where (AFFILIATE_ID=%d)&&(AFFIDAVIT=\"Y\")",
			cast_affiliate_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    name=q->value(0).toString();
    phone=q->value(1).toString();
    fax=q->value(2).toString();
    email=q->value(3).toString();
  }
  delete q;

  sql=QString().sprintf("select AIRED.ID,AIRED.AIR_DATETIME,AIRED.AIR_LENGTH \
                         from AIRED right join PROGRAMS \
                         on AIRED.PROGRAM_ID=PROGRAMS.ID \
                         where (AIRED.AFFILIATE_ID=%d)&&\
                         (AIRED.AIR_DATETIME>=\"%s-01 00:00:00\")&&\
                         (AIRED.AIR_DATETIME<\"%s-01 00:00:00\")&&\
                         (AIRED.STATE=%d) order by AIR_DATETIME",
			cast_affiliate_id,
			date.toString("yyyy-MM").toUtf8().constData(),
			date.addMonths(1).toString("yyyy-MM").
			toUtf8().constData(),
			Dvt::AiredStateScheduled);
  q=new QSqlQuery(sql);
  while(q->next()) {
    sprintf(var,"CORRECT_%d",q->value(0).toInt());
    start_time=GetTime(q->value(0).toInt(),"START");
    air_length=start_time.secsTo(GetTime(q->value(0).toInt(),"END"));
    if(DvtGetPostString(cast_post,var,str,1024)<0) {
      DvtCgiError(QString().sprintf("Missing CORRECT_%d",q->value(0).toInt()));
    }
    if(strcasecmp(str,"Yes")==0) {
      if((start_time!=q->value(1).toTime())||
	 (air_length!=q->value(2).toInt())) {
	state=Dvt::AiredStateModified;
	discreps++;
      }
      else {
	state=Dvt::AiredStateConfirmed;
      }
    }
    else {
      state=Dvt::AiredStateDenied;
    }
    sql=QString().sprintf("update AIRED set \
                           AIR_DATETIME=\"%s %s\",\
                           AIR_LENGTH=%d,\
                           STATE=%d,\
                           CONTACT_NAME=\"%s\",\
                           CONTACT_PHONE=\"%s\",\
                           CONTACT_FAX=\"%s\",\
                           CONTACT_EMAIL=\"%s\",\
                           ORIGIN_DATETIME=now() \
                           where ID=%d",
			  q->value(1).toDateTime().toString("yyyy-MM-dd").
			  toUtf8().constData(),
			  start_time.toString("hh:mm:ss").toUtf8().constData(),
			  air_length,
			  state,
			  DvtEscapeString(name).toUtf8().constData(),
			  DvtEscapeString(phone).toUtf8().constData(),
			  DvtEscapeString(fax).toUtf8().constData(),
			  DvtEscapeString(email).toUtf8().constData(),
			  q->value(0).toInt());
    q1=new QSqlQuery(sql);
    delete q1;
  }

  //
  // Serve Page
  //
  if(discreps>0) {
    SendDiscrepancyAlert(cast_affiliate_id,date,discreps);
  }
  ServeMonthPicker();
  */
  exit(0);
}


void MainObject::SetContext(int cmd) const
{
  printf("<input type=\"hidden\" name=\"COMMAND\" value=\"%d\">\n",
	 cmd);
  printf("<input type=\"hidden\" name=\"SESSION_ID\" value=\"%ld\">\n",
	 cast_session_id);
}


bool MainObject::GetContact(int affiliate_id,QString *name,QString *email,
			    QString *phone,QString *fax)
{
  QString sql;
  QSqlQuery *q;
  bool ret;

  sql=QString().sprintf("select NAME,EMAIL,PHONE,FAX from CONTACTS \
                         where (AFFILIATE_ID=%d)&&(AFFIDAVIT=\"Y\")",
			affiliate_id);
  q=new QSqlQuery(sql);
  if((ret=q->first())) {
    *name=q->value(0).toString();
    *email=q->value(1).toString();
    *phone=q->value(2).toString();
    *fax=q->value(3).toString();
  }
  delete q;

  return ret;
}


QString MainObject::TimeWidget(int id,const QString &name,
			       const QTime &time) const
{
  QString ret;

  ret+=QString().sprintf("<input type=\"text\" maxlength=\"2\" size=\"2\" name=\"%s_HOUR_%d\" value=\"%s\">",
			 name.toUtf8().constData(),
			 id,
			 time.toString("hh").toUtf8().constData());
  ret+=":";
  ret+=QString().sprintf("<input type=\"text\" maxlength=\"2\" size=\"2\" name=\"%s_MINUTE_%d\" value=\"%s\">",
			 name.toUtf8().constData(),
			 id,
			 time.toString("mm").toUtf8().constData());
  ret+=":";
  ret+=QString().sprintf("<input type=\"text\" maxlength=\"2\" size=\"2\" name=\"%s_SECOND_%d\" value=\"%s\">",
			 name.toUtf8().constData(),
			 id,
			 time.toString("ss").toUtf8().constData());

  return ret;
}


QTime MainObject::GetTime(int id,const QString &name)
{
  /*
   * FIXME: Port to Qt6
   *
  int hour;
  int minute;
  int second;
  QString tag;

  tag=QString().sprintf("%s_HOUR_%d",name.toUtf8().constData(),id);
  if(DvtGetPostInt(cast_post,tag,&hour)<0) {
    DvtCgiError(QString("Missing ")+tag);
  }
  tag=QString().sprintf("%s_MINUTE_%d",name.toUtf8().constData(),id);
  if(DvtGetPostInt(cast_post,tag,&minute)<0) {
    DvtCgiError(QString("Missing ")+tag);
  }
  tag=QString().sprintf("%s_SECOND_%d",name.toUtf8().constData(),id);
  if(DvtGetPostInt(cast_post,tag,&second)<0) {
    DvtCgiError(QString("Missing ")+tag);
  }
  
  return QTime(hour,minute,second);
  */
  return QTime();
}


void MainObject::SendDiscrepancyAlert(int affiliate_id,QDate &date,int quan)
{
  QString msg=QString()+
    "Affiliate "+DvtStationCallString(affiliate_id)+
    " has submitted an affidavit report for "+date.toString("MMMM yyyy")+
    " containing "+QString().sprintf("%d",quan)+" exceptions.";
  QString subj=QString()+
    "Affidavit exception alert for "+DvtStationCallString(affiliate_id);

  DvtSendAlert(subj,msg);
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv,false);
  new MainObject();
  return a.exec();
}
