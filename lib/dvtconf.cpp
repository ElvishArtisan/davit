// dvtconf.c
// A small library for handling common configuration file tasks
// 
// Adopted from conflib
//
//   (C) Copyright 1996-2025 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#else
#include <unistd.h>
#include <netdb.h>
#include <sys/timex.h>
#endif


#include <QDir>
#include <QHostAddress>
#include <QMessageBox>
#include <QObject>
#include <QVariant>
#include <QSqlQuery>

#include "dvtconf.h"
#include "dvt.h"
#include "spread_sheet.h"

#define BUFFER_SIZE 1024

int DvtGetPrivateProfileBool(const char *sFilename,const char *cHeader,
			 const char *cLabel,bool bDefault=false)
{
  char temp[255];

  if(DvtGetPrivateProfileString(sFilename,cHeader,cLabel,temp,"",254)<0) {
    return bDefault;
  }
  if(temp[0]==0) {
    return bDefault;
  }
#ifdef WIN32
  if((!stricmp(temp,"yes"))||(!stricmp(temp,"on"))) {
    return true;
  }
  if((!stricmp(temp,"no"))||(!stricmp(temp,"off"))) {
    return false;
  }
#else
  if((!strcasecmp(temp,"yes"))||(!strcasecmp(temp,"on"))) {
    return true;
  }
  if((!strcasecmp(temp,"no"))||(!strcasecmp(temp,"off"))) {
    return false;
  }
#endif
  return bDefault;
}


int DvtGetPrivateProfileString(const char *sFilename,const char *cHeader,
			    const char *cLabel,char *cValue,
			    const char *cDefault,int dValueLength)
{
  int i;
  
  i=GetIni(sFilename,cHeader,cLabel,cValue,dValueLength);
  if(i==0) {
    return 0;
  }
  else {
    strcpy(cValue,cDefault);
    return -1;
  }
}


int DvtGetPrivateProfileInt(const char *sFilename,const char *cHeader,
			 const char *cLabel,int dDefault)
{
  int c;
  char sNum[12];

  if(GetIni(sFilename,cHeader,cLabel,sNum,11)==0) {
    if(sscanf(sNum,"%d",&c)==1) {
      return c;
    }
    else {
      return dDefault;
    }
  }
  else {
    return dDefault;
  }
}


int DvtGetPrivateProfileHex(const char *sFilename,const char *cHeader,
			 const char *cLabel,int dDefault)
{
  char temp[256];
  int n=dDefault;

  DvtGetPrivateProfileString(sFilename,cHeader,cLabel,temp,"",255);
  sscanf(temp,"0x%x",&n);
  return n;
}


double DvtGetPrivateProfileDouble(const char *sFilename,const char *cHeader,
			 const char *cLabel,double dfDefault)
{
  char temp[256];
  double n=dfDefault;

  DvtGetPrivateProfileString(sFilename,cHeader,cLabel,temp,"",255);
  sscanf(temp,"%lf",&n);
  return n;
}


int GetIni(const char *sFileName,const char *cHeader,const char *cLabel,
	   char *cValue,int dValueLength)	
     /* get a value from the ini file */
     
{
  FILE *cIniName;
  char sName[BUFFER_SIZE];
  char cIniBuffer[BUFFER_SIZE],cIniHeader[80],cIniLabel[80];
  int i,j;
  /*  int iFileStat=NULL;  */
  int iFileStat;

  strcpy(sName,sFileName);
#ifdef WIN32
  for(i=0;i<strlen(sName);i++) {
    if(sName[i]=='/') {
      sName[i]='\\';
    }
  }
#endif  // WIN32
  cIniName=fopen(sName,"r");
  if(cIniName==NULL) {
    return 2;	/* ini file doesn't exist! */
  }
  while(GetIniLine(cIniName,cIniBuffer)!=EOF) {
    if(cIniBuffer[0]=='[') {	/* start of section */
      i=1;
      while(cIniBuffer[i]!=']' && cIniBuffer!=0) {
	cIniHeader[i-1]=cIniBuffer[i];
	i++;
      }
      cIniHeader[i-1]=0;
      if(strcmp(cIniHeader,cHeader)==0) {		/* this is the right section! */
	iFileStat=EOF+1;   /* Make this anything other than EOF! */
	while(iFileStat!=EOF) {
	  iFileStat=GetIniLine(cIniName,cIniBuffer);
	  if(cIniBuffer[0]=='[') return 1;
	  i=0;
	  while(cIniBuffer[i]!='=' && cIniBuffer[i]!=0) {
	    cIniLabel[i]=cIniBuffer[i];
	    i++;
	  }
	  cIniLabel[i++]=0;
	  if(strcmp(cIniLabel,cLabel)==0) {	/* this is the right label! */
	    j=0;
	    while(j<dValueLength && cIniBuffer[i]!=0) {
	      cValue[j++]=cIniBuffer[i++];
	    }
	    cValue[j]=0;
	    fclose(cIniName);
	    return 0;		/* value found! */
	  }
	}
      }
    }
  }
  fclose(cIniName);
  return 1;		/* section or label not found! */
}




int GetIniLine(FILE *cIniName,char *cIniBuffer)		/* read a line from the ini file */
     
{
  int i;
  
  for(i=0;i<BUFFER_SIZE-1;i++) {
    cIniBuffer[i]=getc(cIniName);
    switch(cIniBuffer[i]) {
      
    case EOF:
      cIniBuffer[i]=0;
      return EOF;
      
    case 10:
      cIniBuffer[i]=0;
      return 0;
    }
  }
  return 0;
}


void Prepend(char *sPathname,char *sFilename)
{
  char sTemp[256];

  if(sPathname[strlen(sPathname)-1]!='/' && sFilename[0]!='/') {
    strcat(sPathname,"/");
  }
  strcpy(sTemp,sPathname);
  strcat(sTemp,sFilename);
  strcpy(sFilename,sTemp);
}

  
#ifndef WIN32
int IncrementIndex(char *sPathname,int dMaxIndex)
{
  int dLockname=-1;
  FILE *hPathname;
  int i;
  char sLockname[256];
  char sAccum[256];
  int dIndex,dNewIndex;

  /* Lock the index */
  strcpy(sLockname,sPathname);
  strcat(sLockname,".LCK");
  i=0;
  while(dLockname<0 && i<MAX_RETRIES) {
    dLockname=open(sLockname,O_WRONLY|O_EXCL|O_CREAT|S_IRUSR|S_IWUSR);
    i++;
  }
  if(dLockname<0) {
    return -1;
  }
  sprintf(sAccum,"%d",getpid());
  write(dLockname,sAccum,strlen(sAccum));
  close(dLockname);

  /* We've got the lock, so read the index */
  hPathname=fopen(sPathname,"r");
  if(hPathname==NULL) {
    unlink(sLockname);
    return -1;
  }
  if(fscanf(hPathname,"%d",&dIndex)!=1) {
    fclose(hPathname);
    unlink(sLockname);
    return -1;
  }
  fclose(hPathname);

  /* Update the index */
  if((dIndex<dMaxIndex) || (dMaxIndex==0)) {
    dNewIndex=dIndex+1;
  }
  else {
    dNewIndex=1;
  }

  /* Write it back */
  hPathname=fopen(sPathname,"w");
  if(hPathname==NULL) {
    unlink(sLockname);
    return -1;
  }
  fprintf(hPathname,"%d",dNewIndex);
  fclose(hPathname);

  /* Release the lock */
  unlink(sLockname);

  /* Ensure a sane value to return and then exit */
  if((dIndex>dMaxIndex)&&(dMaxIndex!=0)) {
    dIndex=1;
  }

  return dIndex;
}
#endif


/*
 * int StripLevel(char *sString)
 *
 * This strips the lower-most level from the pathname pointed to by 'sString'
 */

void StripLevel(char *sString)
{
  int i;                              /* General Purpose Pointer */
  int dString;                        /* Initial Length of 'sString' */

  dString=strlen(sString)-1;
  for(i=dString;i>=0;i--) {
    if(sString[i]=='/') {
      sString[i]=0;
      return;
    }
  }
  sString[0]=0;
}




#ifndef WIN32
bool GetLock(const char *sLockname)
{
  int fd;
  char sAccum[256];

  if((fd=open(sLockname,O_WRONLY|O_EXCL|O_CREAT,S_IRUSR|S_IWUSR))<0) {
    printf("failed!\n");
    if(DvtCheckPid(DvtGetPathPart(sLockname),DvtGetBasePart(sLockname))) {
      return false;
    }
    ClearLock(sLockname);
    if((fd=open(sLockname,O_WRONLY|O_EXCL|O_CREAT,S_IRUSR|S_IWUSR))<0) {
      return false;
    }
  }
  sprintf(sAccum,"%d",getpid());
  write(fd,sAccum,strlen(sAccum));
  close(fd);
  return true;
}
#endif




#ifndef WIN32
void ClearLock(const char *sLockname)
{
  unlink(sLockname);
}
#endif  // WIN32


QString DvtGetPathPart(QString path)
{
  QStringList f0=path.split("/",QString::KeepEmptyParts);
  f0.removeLast();
  return f0.join("/");
}


QString DvtGetBasePart(QString path)
{
  QStringList f0=path.split("/",QString::KeepEmptyParts);

  return f0.last();
}


QString DvtGetShortDate(QDate date)
{
  return QString().sprintf("%02d/%02d/%04d",
			   date.month(),date.day(),date.year());
}

QString DvtGetShortDayNameEN(int weekday)
{
  QString day_name;
  if ( weekday < 1 || weekday > 7 )
    weekday = 1;

  if (weekday == 1)
    day_name = "Mon";
  else if (weekday == 2)
    day_name = "Tue";
  else if (weekday == 3)
    day_name = "Wed";
  else if (weekday == 4)
    day_name = "Thu";
  else if (weekday == 5)
    day_name = "Fri";
  else if (weekday == 6)
    day_name = "Sat";
  else if (weekday == 7)
    day_name = "Sun";
  return day_name;
}

QFont::Weight DvtGetFontWeight(QString string)
{
  if(string.contains("Light",Qt::CaseInsensitive)) {
    return QFont::Light;
  }
  if(string.contains("Normal",Qt::CaseInsensitive)) {
    return QFont::Normal;
  }
  if(string.contains("DemiBold",Qt::CaseInsensitive)) {
    return QFont::DemiBold;
  }
  if(string.contains("Bold",Qt::CaseInsensitive)) {
    return QFont::Bold;
  }
  if(string.contains("Black",Qt::CaseInsensitive)) {
    return QFont::Black;
  }
  return QFont::Normal;
}


#ifndef WIN32
bool DvtDetach()
{
  if(daemon(0,0)) {
    return false;
  }
  return true;
}
#endif


bool DvtBool(QString string)
{
  if(string.contains("Y",Qt::CaseInsensitive)) {
    return true;
  }
  return false;
}


QString DvtYesNo(bool state)
{
  if(state) {
    return QString("Y");
  }
  return QString("N");
}


#ifndef WIN32
QHostAddress DvtGetHostAddr()
{
  FILE *file;
  char host_name[256];
  struct hostent *host_ent;
  int host_address;

  if((file=fopen("/etc/HOSTNAME","r"))==NULL) {
    return QHostAddress();
  }
  if(fscanf(file,"%s",host_name)!=1) {
    return QHostAddress();
  }
  if((host_ent=gethostbyname(host_name))==NULL) {
    return QHostAddress();
  }
  host_address=16777216*(host_ent->h_addr_list[0][0]&0xff)+
    65536*(host_ent->h_addr_list[0][1]&0xff)+
    256*(host_ent->h_addr_list[0][2]&0xff)+
    (host_ent->h_addr_list[0][3]&0xff);
  return QHostAddress((uint32_t)host_address);
}
#endif  // WIN32


QString DvtGetDisplay(bool strip_point)
{
#ifdef WIN32
  return QString("win32");
#else
  QString display;
  int l;

  if(getenv("DISPLAY")[0]==':') {
    display=QString().sprintf("%s%s",(const char *)DvtGetHostAddr().toString().
			      toUtf8().constData(),
			     getenv("DISPLAY"));
  }
  else {
    display=QString(getenv("DISPLAY"));
  }
  if(strip_point) {
    l=display.length();
    while(display.at(l)!=':') {
      if(display.at(l--)=='.') {
	return display.left(l+1);
      }    
    }
  }
  return display;
#endif  // WIN32
}


bool DvtDoesRowExist(QString table,QString name,QString test,QSqlDatabase *db)
{
  QSqlQuery *q;
  QString sql;

  sql=QString().sprintf("SELECT %s FROM %s WHERE %s=\"%s\"",
			name.toUtf8().constData(),
			table.toUtf8().constData(),
			name.toUtf8().constData(),
			test.toUtf8().constData());
  q=new QSqlQuery(sql);
  if(q->size()>0) {
    delete q;
    return true;
  }
  delete q;
  return false;
}


bool DvtDoesRowExist(QString table,QString name,unsigned test,QSqlDatabase *db)
{
  QSqlQuery *q;
  QString sql;

  sql=QString().sprintf("SELECT %s FROM %s WHERE %s=%d",
			name.toUtf8().constData(),
			table.toUtf8().constData(),
			name.toUtf8().constData(),
			test);
  q=new QSqlQuery(sql);
  if(q->size()>0) {
    delete q;
    return true;
  }
  delete q;
  return false;
}


QVariant DvtGetSqlValue(QString table,QString name,QString test,
		      QString param,QSqlDatabase *db,bool *valid)
{
  QSqlQuery *q;
  QString sql;
  QVariant v;

  sql=QString().sprintf("SELECT %s FROM %s WHERE %s=\"%s\"",
			param.toUtf8().constData(),
			table.toUtf8().constData(),
			name.toUtf8().constData(),
			test.toUtf8().constData());
  q=new QSqlQuery(sql);
  if(q->isActive()) {
    q->first();
    v=q->value(0);
    if(valid!=NULL) {
      *valid=!q->isNull(0);
    }
    delete q;
    return v;
  }
  delete q;
  return QVariant();
}


bool DvtIsSqlNull(QString table,QString name,QString test,
		QString param,QSqlDatabase *db)
{
  QSqlQuery *q;
  QString sql;

  sql=QString().sprintf("SELECT %s FROM %s WHERE %s=\"%s\"",
			param.toUtf8().constData(),
			table.toUtf8().constData(),
			name.toUtf8().constData(),
			test.toUtf8().constData());
  q=new QSqlQuery(sql);
  if(q->isActive()) {
    q->first();
    if(q->isNull(0)) {
      delete q;
      return true;
    }
    else {
      delete q;
      return false;
    }
  }
  delete q;
  return true;
}


bool DvtIsSqlNull(QString table,QString name,unsigned test,
		QString param,QSqlDatabase *db)
{
  QSqlQuery *q;
  QString sql;

  sql=QString().sprintf("SELECT %s FROM %s WHERE %s=%d",
			param.toUtf8().constData(),
			table.toUtf8().constData(),
			name.toUtf8().constData(),
			test);
  q=new QSqlQuery(sql);
  if(q->isActive()) {
    q->first();
    if(q->isNull(0)) {
      delete q;
      return true;
    }
    else {
      delete q;
      return false;
    }
  }
  delete q;
  return true;
}


QVariant DvtGetSqlValue(QString table,QString name,unsigned test,
		      QString param,QSqlDatabase *db,bool *valid)
{
  QSqlQuery *q;
  QString sql;
  QVariant v;

  sql=QString().sprintf("SELECT %s FROM %s WHERE %s=%u",
			param.toUtf8().constData(),
			table.toUtf8().constData(),
			name.toUtf8().constData(),
			test);
  q=new QSqlQuery(sql);
  if(q->first()) {
    v=q->value(0);
    if(valid!=NULL) {
      *valid=!q->isNull(0);
    }
    delete q;
    return v;
  }
  delete q;
  return QVariant();
}


QString DvtGetTimeLength(int mseconds,bool leadzero,bool tenths)
{
  int hour,min,seconds,tenthsecs;
  char negative[2];

  if(mseconds<0) {
    mseconds=-mseconds;
    strcpy(negative,"-");
  }
  else {
    negative[0]=0;
  }
  QTime time_length(QTime(0,0,0).addMSecs(mseconds));
  hour = time_length.hour();
  min = time_length.minute();
  seconds = time_length.second();
  mseconds = time_length.msec();
  tenthsecs=mseconds/100;
  if(leadzero) {
    if(tenths) {
     return QString().sprintf("%s%d:%02d:%02d.%d",negative,hour,min,seconds,
			      tenthsecs);
    }
    return QString().sprintf("%s%d:%02d:%02d",negative,hour,min,seconds);
  }
  if((hour==0)&&(min==0)) {
    if(tenths) {
      return QString().sprintf("%s:%02d.%d",negative,seconds,tenthsecs);
    }
    return QString().sprintf("%s:%02d",negative,seconds);
  }
  if(hour==0) {
    if(tenths) {
      return QString().sprintf("%s%2d:%02d.%d",negative,min,seconds,
			       tenthsecs);
    }
    return QString().sprintf("%s%2d:%02d",negative,min,seconds);
  }
  if(tenths) {
    return QString().sprintf("%s%2d:%02d:%02d.%d",negative,hour,min,seconds,
			     tenthsecs);
  }
  return QString().sprintf("%s%2d:%02d:%02d",negative,hour,min,seconds);
}


int DvtSetTimeLength(QString str)
{
  int istate=2;
  QString field;
  int res=0;

  if(str.isEmpty()) {
    return -1;
  }
  for(int i=0;i<str.length();i++) {
    if(str.at(i)==':') {
      istate--;
    }
  }
  if(istate<0) {
    return -1;
  }
  for(int i=0;i<str.length();i++) {
    if(str.at(i).isNumber()) {
      field+=str.at(i);
    }
    else {
      if((str.at(i)==':')||(str.at(i)=='.')) {
	if(field.length()>2) {
	  return -1;
	}
	switch(istate) {
	    case 0:
	      res+=3600000*field.toInt();
	      break;

	    case 1:
	      res+=60000*field.toInt();
	      break;

	    case 2:
	      res+=1000*field.toInt();
	      break;
	}
	istate++;
	field="";
      }
      else {
	if(!str.at(i).isSpace()) {
	  return -2;
	}
      }
    }
  }
  switch(istate) {
      case 2:
	res+=1000*field.toInt();
	break;

      case 3:
	switch(field.length()) {
	    case 1:
	      res+=100*field.toInt();
	      break;

	    case 2:
	      res+=10*field.toInt();
	      break;

	    case 3:
	      res+=field.toInt();
	      break;
	}
  }

  return res;
}


QTime DvtSetTime(const QString &str)
{
  int istate=0;
  QString field;
  int hour=0;
  int minute=0;

  for(int i=0;i<str.length();i++) {
    switch(istate) {
      case 0:  // Hour
	if(str.at(i)==':') {
	  hour=field.toInt();
	  field="";
	  istate++;
	}
	else {
	  field+=str.at(i);
	}
	break;

      case 1:  // Minute
	if(str.at(i)==' ') {
	  minute=field.toInt();
	  istate++;
	}
	else {
	  field+=str.at(i);
	}
	break;

      default:  // AM/PM
	if(str.at(i)=='P') {
	  hour+=12;
	}
	break;
    }
  }

  return QTime(hour,minute,0);
}


#ifndef WIN32
bool DvtCopy(QString srcfile,QString destfile)
{
  int src_fd;
  int dest_fd;
  struct stat src_stat;
  struct stat dest_stat;
  char *buf=NULL;
  int n;

  if((src_fd=open(srcfile.toUtf8(),O_RDONLY))<0) {
    return false;
  }
  if(fstat(src_fd,&src_stat)<0) {
    close(src_fd);
    return false;
  }
  if((dest_fd=open(destfile.toUtf8(),O_RDWR|O_CREAT,src_stat.st_mode))
     <0) {
    close(src_fd);
    return false;
  }
  if(fstat(dest_fd,&dest_stat)<0) {
    close(src_fd);
    close(dest_fd);
    return false;
  }
  buf=(char *)malloc(dest_stat.st_blksize);
  while((n=read(src_fd,buf,dest_stat.st_blksize))==dest_stat.st_blksize) {
    write(dest_fd,buf,dest_stat.st_blksize);
  }
  write(dest_fd,buf,n);
  free(buf);
  close(src_fd);
  close(dest_fd);
  return true;
}
#endif  // WIN32


#ifndef WIN32
bool DvtWritePid(QString dirname,QString filename,int owner,int group)
{
  FILE *file;
  mode_t prev_mask;
  QString pathname=QString().sprintf("%s/%s",
				     dirname.toUtf8().constData(),
				     filename.toUtf8().constData());

  prev_mask = umask(0113);      // Set umask so pid files are user and group writable.
  file=fopen(pathname.toUtf8(),"w");
  umask(prev_mask);
  if(file==NULL) {
    return false;
  }
  fprintf(file,"%d",getpid());
  fclose(file);
  chown(pathname.toUtf8().constData(),owner,group);

  return true;
}


void DvtDeletePid(QString dirname,QString filename)
{
  QString pid=QString().sprintf("%s/%s",
				dirname.toUtf8().constData(),
				filename.toUtf8().constData());
  unlink(pid.toUtf8());
}


bool DvtCheckPid(QString dirname,QString filename)
{
  QDir dir;
  QString path;
  path=QString("/proc/")+
    QString().sprintf("%d",DvtGetPid(dirname+QString("/")+filename));
  dir.setPath(path);
  return dir.exists();
}


pid_t DvtGetPid(QString pidfile)
{
  FILE *handle;
  pid_t ret;

  if((handle=fopen(pidfile.toUtf8(),"r"))==NULL) {
    return -1;
  }
  if(fscanf(handle,"%d",&ret)!=1) {
    ret=-1;
  }
  fclose(handle);
  return ret;
}


bool DvtTimeSynced()
{
  struct timex timex;

  memset(&timex,0,sizeof(struct timex));
  if(adjtimex(&timex)==TIME_OK) {
    return true;
  }
  return false;
}
#endif  // WIN32


QString DvtGetHomeDir(bool *found)
{
  if(getenv("HOME")==NULL) {
    if(found!=NULL) {
      *found=false;
    }
    return QString("/");
  }
  if(found!=NULL) {
    *found=true;
  }
  return QString(getenv("HOME"));
}


QString DvtTruncateAfterWord(QString str,int word,bool add_dots)
{
  QString simple=str.simplified();
  int quan=0;
  int point;

  for(int i=0;i<simple.length();i++) {
    if(simple.at(i).isSpace()) {
      quan++;
      point=i;
      if(quan==word) {
	if(add_dots) {
	  return simple.left(point)+QString("...");
	}
	else {
	  return simple.left(point);
	}
      }
    }
  }
  return simple;
}


QString DvtHomeDir()
{
  if(getenv("HOME")==NULL) {
    return QString("/");
  }
  return QString(getenv("HOME"));
}


QString DvtTempDir()
{
#ifdef WIN32
  if(getenv("TEMP")!=NULL) {
    return QString(getenv("TEMP"));
  }
  if(getenv("TMP")!=NULL) {
    return QString(getenv("TMP"));
  }
  return QString("C:\\");
#else
  if(getenv("TMPDIR")!=NULL) {
    return QString(getenv("TMPDIR"));
  }
  return QString("/tmp");
#endif  // WIN32
}


QString DvtTempName(const QString &ext)
{
  QString ret=DvtTempDir();

  ret+="/";
  ret+=QDate::currentDate().toString("yyyyMMdd");
  ret+=QTime::currentTime().toString("hhmmsszzz");
  ret+="."+ext;

  return ret;
}


int DvtDeleteFile(const QString &filename)
{
#ifdef WIN32
  return remove(filename.toUtf8());
#else
  return unlink(filename.toUtf8());
#endif
}



QString DvtEscapeString(const QString &str)
{
  QString res;

  for(int i=0;i<str.length();i++) {
    switch((str.toUtf8())[i]) {
	case '(':
	  res+=QString("\\\(");
	  break;

	case ')':
	  res+=QString("\\)");
	  break;

	case '{':
	  res+=QString("\\\{");
	  break;

	case '"':
	  res+=QString("\\\"");
	  break;

	case '`':
	  res+=QString("\\`");
	  break;

	case ' ':
	  res+=QString("\\")+QString(" ");
	  break;

	case '[':
	  res+=QString("\\\[");
	  break;

	case '\'':
	  res+=QString("\\\'");
	  break;

	case '?':
	  res+=QString("\\\?");
	  break;

	case '&':
	  res+=QString("\\&");
	  break;

        case ';':
	  res+=QString("\\;");
	  break;

	default:
	  res+=(str.toUtf8())[i];
	  break;
    }
  }
  return res;
}


QString DvtFormatCityState(const QString &city,const QString &state)
{
  QString ret=city+", "+state.toUpper();
  if(city.isEmpty()) {
    ret=state.toUpper();
  }
  if(state.isEmpty()) {
    ret=city;
  }
  if(city.isEmpty()&&state.isEmpty()) {
    ret="";
  }
  return ret;
}


bool DvtNormalizeAddresses(QString str,QStringList *list)
{
  str.replace("\n",";");
  str.replace(",",";");
  *list=str.split(";");
  for(int i=0;i<list->size();i++) {
    if(!DvtIsFullEmailAddress((*list)[i])) { 
      return false;
    }
  }

  return true;
}


bool DvtIsFullEmailAddress(QString str)
{
  QString addr;

  //
  // Name <someone@somewhere.com>
  //
  if((str.contains("<")>0)||(str.contains(">")>0)) {
    if((str.contains("<")!=1)||(str.contains(">")!=1)) {
      return false;
    }
    str.replace(">","<");
    return DvtIsEmailAddress(str.section("<",1,1));
  }

  //
  // "Name" someone@somewhere.com
  //
  if(str.contains("\"")>0) {
    if(str.contains("\"")!=2) {
      return false;
    }
    return DvtIsEmailAddress(str.right(str.length()-str.lastIndexOf("\"")-1));
  }

  //
  // someone@somewhere.com
  //
  return DvtIsEmailAddress(str);
}


bool DvtIsEmailAddress(const QString &str)
{
  return (str.contains("@")==1)&&(str.contains(".")>0);
}


QString DvtFormatEmailAddress(const QString &name,const QString &addr)
{
  if(name.isEmpty()) {
    return addr;
  }
  return name+" <"+addr+">";
}


QString DvtFormatPhoneNumber(const QString &str)
{
  if(str.isEmpty()) {
    return str;
  }
  QString pnum=str.trimmed();
  if(pnum.length()==10) {
    return QString("(")+pnum.left(3)+QString(") ")+
      pnum.mid(3,3)+QString("-")+pnum.right(4);
  }
  return pnum;
}


QString DvtFormatFrequency(double freq)
{
  QString ret="["+QObject::tr("unknown")+"]";

  if(freq>=530.0) {
    ret=QString().sprintf("%d kHz",(int)freq);
  }
  if(freq<108.0) {
    ret=QString().sprintf("%5.1lf MHz",freq);
  }
  if(freq==0.0) {
    ret=QObject::tr("N/A");
  }
  return ret;
}


QString DvtNormalizePhoneNumber(const QString &str,bool *ok)
{
  QString ret=str;

  ret.replace("(","");
  ret.replace(")","");
  ret.replace("-","");
  ret.replace(".","");
  ret.replace("+","");
  ret.replace(" ","");
  if(ok!=NULL) {
    ret.toULongLong(ok);
  }
  return ret;
}


int DvtCreateNewAffiliateRecord()
{
  QString sql;
  QSqlQuery *q;
  int id=-1;

  sql="insert into AFFILIATES set STATION_CALL=\"NEW\"";
  q=new QSqlQuery(sql);
  delete q;
  
  sql="select LAST_INSERT_ID() from AFFILIATES";
  q=new QSqlQuery(sql);
  if(q->first()) {
    id=q->value(0).toInt();
  }
  delete q;
  return id;
}


QString DvtStationTypeString(const QString &type)
{
  return type.toUpper()+"M";
}


QString DvtStationCallString(const QString &call,const QString &type)
{
  return call.toUpper()+"-"+DvtStationTypeString(type);
}


QString DvtStationCallString(int affiliate_id)
{
  QString sql;
  QSqlQuery *q;
  QString ret;

  sql=QString().sprintf("select STATION_CALL,STATION_TYPE \
                         from AFFILIATES where ID=%d",affiliate_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    ret=DvtStationCallString(q->value(0).toString(),q->value(1).toString());
  }
  delete q;

  return ret;
}


QString DvtMarketRankString(int rank)
{
  QString ret="";

  if(rank>0) {
    ret=QString().sprintf("%d",rank);
  }
  return ret;
}


bool DvtAffidavitNeeded(int affiliate_id,const QDate &date)
{
  QString sql;
  QSqlQuery *q;
  bool ret=false;

  sql=QString().sprintf("select ID from AFFILIATES \
                         where (ID=%d)&&(IS_AFFILIATE=\"Y\")",
			affiliate_id);
  q=new QSqlQuery(sql);
  if(!q->first()) {
    delete q;
    return false;
  }
  delete q;
  sql=QString().sprintf("select ID from AIRED \
                         where (AFFILIATE_ID=%d)&&\
                         (STATE=%d)&&\
                         (AIR_DATETIME>=\"%s-01 00:00:00\")&&\
                         (AIR_DATETIME<=\"%s-%02d 23:59:59\")",
			affiliate_id,
			Dvt::AiredStateScheduled,
			date.toString("yyyy-MM").toUtf8().constData(),
			date.toString("yyyy-MM").toUtf8().constData(),
			date.daysInMonth());
  q=new QSqlQuery(sql);
  if(q->first()) {
    ret=true;
  }
  else {
    ret=false;
  }
  delete q;

  return ret;
}


bool DvtAffidavitNeeded(std::vector<int> *ids,std::map<int,int> *counts,
			const QDate &start_date,const QDate &end_date,
			Dvt::AffidavitStationFilter filter,int program_id)
{
  QString sql;
  QSqlQuery *q;
  int current_id=-1;

  ids->clear();
  counts->clear();
  sql=QString("select AFFILIATE_ID,AIR_DATETIME from AIRED where ")+
    QString().sprintf("(STATE=%d)&&",Dvt::AiredStateScheduled)+
    "(AIR_DATETIME>=\""+start_date.toString("yyyy-MM")+"-01 00:00:00\")&&"+
    "(AIR_DATETIME<\""+end_date.addDays(1).
    toString("yyyy-MM")+"-01 00:00:00\")&&";
  if(filter==Dvt::Program) {
    sql+=QString().sprintf("(PROGRAM_ID=%d)&&",program_id);
  }
  sql=sql.left(sql.length()-2);
  sql+=" order by AFFILIATE_ID";
  q=new QSqlQuery(sql);
  while(q->next()) {
    int dow=q->value(1).toDateTime().date().dayOfWeek();
    if(((filter==Dvt::All)||
	(filter==Dvt::Program)||
	((filter==Dvt::Weekday)&&(dow>=1)&&(dow<=5))||
	((filter==Dvt::Weekend)&&(dow>=6)&&(dow<=7)))) {
      if(q->value(0).toInt()!=current_id) {
	ids->push_back(q->value(0).toInt());
	current_id=q->value(0).toInt();
      }
      (*counts)[q->value(0).toInt()]++;
    }
  }
  delete q;

  return ids->size()!=0;
}


unsigned DvtAffidavitNeededDates(std::vector<QDate> *dates,int affiliate_id,
				 const QDate &start_date,const QDate &end_date)
{
  QString sql;
  QSqlQuery *q;
  int current_month=-1;
  int current_year=-1;

  dates->clear();
  sql=QString("select AIR_DATETIME from AIRED where ")+
    QString().sprintf("(AFFILIATE_ID=%d)&&",affiliate_id)+
    QString().sprintf("(STATE=%d)&&",Dvt::AiredStateScheduled)+
    "(AIR_DATETIME>=\""+start_date.toString("yyyy-MM")+"-01 00:00:00\")&&"+
    "(AIR_DATETIME<\""+end_date.addDays(1).
    toString("yyyy-MM")+"-01 00:00:00\") "+
    "order by AIR_DATETIME";
  q=new QSqlQuery(sql);
  while(q->next()) {
    if((q->value(0).toDateTime().date().month()!=current_month)&&
       (q->value(0).toDateTime().date().year()!=current_year)) {
      current_month=q->value(0).toDateTime().date().month();
      current_year=q->value(0).toDateTime().date().month();
      dates->push_back(q->value(0).toDateTime().date());
    }
  }
  delete q;  

  return dates->size();
}


void DvtUpdateIsAffiliateField()
{
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
  QSqlQuery *q2;

  sql="select ID from AFFILIATES";
  q=new QSqlQuery(sql);
  while(q->next()) {
    sql=QString().sprintf("select ID from AIRINGS where AFFILIATE_ID=%d",
			  q->value(0).toInt());
    q1=new QSqlQuery(sql);
    if(q1->first()) {
      sql=QString().sprintf("update AFFILIATES set IS_AFFILIATE=\"Y\",\
                             AFFIDAVIT_ACTIVE=\"Y\"			\
                             where ID=%d",q->value(0).toInt());
      q2=new QSqlQuery(sql);
      delete q2;
    }
    delete q1;
  }
  delete q;
}


unsigned DvtContactInfo(QString *name,QString *title,QString *email,
			QString *phone,QString *fax,
			int affiliate_id,Dvt::ContactType type)
{
  QString sql;
  QSqlQuery *q;
  unsigned ret=0;

  if(name!=NULL) {
    *name=" ";
  }
  if(phone!=NULL) {
    *phone=" ";
  }
  if(email!=NULL) {
    *email=" ";
  }
  if(fax!=NULL) {
    *fax=" ";
  }
  sql=QString("select NAME,TITLE,EMAIL,PHONE,FAX from CONTACTS where ")+
    QString().sprintf("(AFFILIATE_ID=%d)&&",affiliate_id);
  switch(type) {
  case Dvt::AffidavitContact:
    sql+="(AFFIDAVIT=\"Y\")";
    break;

  case Dvt::ProgramDirectorContact:
    sql+="(PROGRAM_DIRECTOR=\"Y\")";
    break;

  case Dvt::GeneralManagerContact:
    sql+="(GENERAL_MANAGER=\"Y\")";
    break;
  }
  q=new QSqlQuery(sql);
  while(q->next()) {
    if(name!=NULL) {
      if(!q->value(0).toString().isEmpty()) {
	(*name)+=q->value(0).toString()+", ";
      }
    }
    if(title!=NULL) {
      if(!q->value(1).toString().isEmpty()) {
	(*title)+=q->value(1).toString()+", ";
      }
    }
    if(email!=NULL) {
      if(!q->value(2).toString().isEmpty()) {
	(*email)+=q->value(2).toString()+", ";
      }
    }
    if(phone!=NULL) {
      if(!q->value(3).toString().isEmpty()) {
	(*phone)+=DvtFormatPhoneNumber(q->value(3).toString())+", ";
      }
    }
    if(fax!=NULL) {
      if(!q->value(4).toString().isEmpty()) {
	(*fax)+=DvtFormatPhoneNumber(q->value(4).toString())+", ";
      }
    }
  }
  delete q;
  if(name!=NULL) {
    *name=(*name).left((*name).length()-2).trimmed();
  }
  if(title!=NULL) {
    *title=(*name).left((*title).length()-2).trimmed();
  }
  if(phone!=NULL) {
    *phone=(*phone).left((*phone).length()-2).trimmed();
  }
  if(email!=NULL) {
    *email=(*email).left((*email).length()-2).trimmed();
  }
  if(fax!=NULL) {
    *fax=(*fax).left((*fax).length()-2).trimmed();
  }

  return ret;
}


QFont DvtGetFont(const QString &base_name)
{
  QString face=SPREAD_DEFAULT_FONT_FACE;
  int size=SPREAD_DEFAULT_FONT_SIZE;
  QFont::Weight weight=SPREAD_DEFAULT_FONT_WEIGHT;
  bool ok=false;
  int n;

  if(getenv((base_name+"_FONT_FACE").toUtf8())!=NULL) {
    face=getenv((base_name+"_FONT_FACE").toUtf8());
  }
  if(getenv((base_name+"_FONT_SIZE").toUtf8())!=NULL) {
    n=QString(getenv((base_name+"_FONT_SIZE").toUtf8())).toInt(&ok);
    if(ok) {
      size=n;
    }
    else {
      fprintf(stderr,"davit: %s_FONT_SIZE value is malformatted\n",
	      base_name.toUtf8().constData());
    }
  }
  if(getenv((base_name+"_FONT_WEIGHT").toUtf8())!=NULL) {
    QString wt=getenv((base_name+"_FONT_WEIGHT").toUtf8());
    if(wt.toLower()=="normal") {
      weight=QFont::Normal;
    }
    else {
      if(wt.toLower()=="bold") {
	weight=QFont::Bold;
      }
      else {
	fprintf(stderr,"davit: %s_FONT_WEIGHT value is malformatted\n",
		base_name.toUtf8().constData());
      }
    }
  }

  return QFont(face,size,weight);
}


Spread::FileFormat DvtGetSpreadSheetFileFormat(const QString &base_name)
{
  Spread::FileFormat ret=SpreadSheet::ExcelXmlFormat;

  if(getenv((base_name+"_FORMAT").toUtf8())!=NULL) {
    QString str=QString(getenv((base_name+"_FORMAT").toUtf8())).toLower();
    if(str=="slk") {
      ret=Spread::SlkFormat;
    }
    else {
      if(str=="excelxml") {
	ret=Spread::ExcelXmlFormat;
      }
      else {
	fprintf(stderr,"davit: %s_FORMAT value is malformatted\n",
		base_name.toUtf8().constData());
      }
    }
  }

  return ret;
}


QStringList DvtReportViewerCommand(const QString &filename,
				   const QString &ooffice_path)
{
  QStringList ret;

  if(getenv("DAVIT_REPORT_COMMAND")==NULL) {
    ret.push_back(ooffice_path);
    ret.push_back("--calc");
    ret.push_back("-view");
    ret.push_back(filename);
  }
  else {
    ret=QString(getenv("DAVIT_REPORT_COMMAND")).split(" ");
    ret.push_back(filename);
  }
  /*
  QString str="CMD: ";
  for(unsigned i=0;i<ret.size();i++) {
    str+=ret[i];
    str+=" ";
  }
  QMessageBox::information(NULL,"DEBUG",str);
  */
  return ret;
}
