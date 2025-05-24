// geometry.cpp
//
// Persistent geometry for Davit
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
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

#ifdef WIN32
#include <qsettings.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#endif  // WIN32

#include <dvtprofile.h>
#include <dvtconf.h>
#include <qdir.h>

#include <geometry.h>

Geometry::Geometry()
{
  for(unsigned i=0;i<Geometry::LastWidget;i++) {
    geo_visible[i]=false;
  }
}


bool Geometry::isVisible(Widget widget) const
{
  if(widget>=Geometry::LastWidget) {
    return true;
  }
  return geo_visible[widget];
}


void Geometry::setVisible(Widget widget,bool state)
{
  if(widget>=Geometry::LastWidget) {
    return;
  }
  geo_visible[widget]=state;
}


QRect Geometry::geometry(Widget widget,const QRect &def) const
{
  if(widget>=Geometry::LastWidget) {
    return def;
  }
  if(geo_rects[widget].width()==0) {
    return def;
  }
  return geo_rects[widget];
}


void Geometry::setGeometry(Widget widget,const QRect &rect)
{
  if(widget>=Geometry::LastWidget) {
    return;
  }
  geo_rects[widget]=rect;
}


QString Geometry::openofficePath() const
{
  return openoffice_path;
}


void Geometry::load()
{
#ifdef WIN32
  QSettings settings;
  settings.setPath("Paravel Systems","Davit",QSettings::User);
  settings.insertSearchPath(QSettings::Windows,"/Paravel Systems/");
  QString key=QString().sprintf("/Davit/Geometry");
  for(int i=0;i<Geometry::LastWidget;i++) {
    geo_visible[i]=settings.
      readNumEntry(key+QString().sprintf("Widget%dVisible",i+1),0);
    geo_rects[i].
      setX(settings.readNumEntry(key+QString().sprintf("Widget%dX",i+1),0));
    geo_rects[i].
      setY(settings.readNumEntry(key+QString().sprintf("Widget%dY",i+1),0));
    geo_rects[i].
      setWidth(settings.readNumEntry(key+QString().
				     sprintf("Widget%dWidth",i+1),0));
    geo_rects[i].
      setHeight(settings.readNumEntry(key+QString().
				     sprintf("Widget%dHeight",i+1),0));
  }
  openoffice_path=settings.readEntry(key+"OpenofficePath",
	              "C:/Program Files/OpenOffice 4/program/soffice");
#else
  DvtProfile *p=new DvtProfile();
  if(!p->setSource(ConfigPath())) {
    fprintf(stderr,"davit: unable to load geometry\n");
  }
  for(unsigned i=0;i<Geometry::LastWidget;i++) {
    geo_visible[i]=
      p->boolValue("DavitGeometry",
		   QString::asprintf("Widget%dVisible",i+1),false);
    geo_rects[i].
      setX(p->intValue("DavitGeometry",QString::asprintf("Widget%dX",i+1),0));
    geo_rects[i].
      setY(p->intValue("DavitGeometry",QString::asprintf("Widget%dY",i+1),0));
    geo_rects[i].
      setWidth(p->intValue("DavitGeometry",
			   QString::asprintf("Widget%dWidth",i+1),0));
    geo_rects[i].
      setHeight(p->intValue("DavitGeometry",
			   QString::asprintf("Widget%dHeight",i+1),0));
    openoffice_path=p->stringValue("DavitGeometry","OpenofficePath","soffice");
  }
  delete p;
#endif  // WIN32
}


void Geometry::save()
{
#ifdef WIN32
  QSettings settings;
  settings.setPath("Paravel Systems","Davit",QSettings::User);
  settings.insertSearchPath(QSettings::Windows,"/Paravel Systems/");
  QString key=QString::asprintf("/Davit/Geometry");
  for(int i=0;i<Geometry::LastWidget;i++) {
    settings.
      writeEntry(key+QString::asprintf("Widget%dVisible",i+1),geo_visible[i]);
    settings.
      writeEntry(key+QString::asprintf("Widget%dX",i+1),geo_rects[i].x());
    settings.
      writeEntry(key+QString::asprintf("Widget%dY",i+1),geo_rects[i].y());
    settings.
      writeEntry(key+QString::asprintf("Widget%dWidth",i+1),
		 geo_rects[i].width());
    settings.
      writeEntry(key+QString::asprintf("Widget%dHeight",i+1),
		 geo_rects[i].height());
  }
#else
  FILE *f=NULL;
  QDir dir(ConfigDir());

  if(!dir.exists()) {
    mkdir(dir.absolutePath().toUtf8(),0700);
  }
  if((f=fopen(ConfigPath().toUtf8(),"w"))==NULL) {
    fprintf(stderr,"davit: unable to save geometry\n");
    return;
  }
  fprintf(f,"[DavitGeometry]\n");
  for(unsigned i=0;i<Geometry::LastWidget;i++) {
    if(geo_visible[i]) {
      fprintf(f,"Widget%dVisible=Yes\n",i+1);
    }
    else {
      fprintf(f,"Widget%dVisible=No\n",i+1);
    }
    fprintf(f,"Widget%dX=%d\n",i+1,geo_rects[i].x());
    fprintf(f,"Widget%dY=%d\n",i+1,geo_rects[i].y());
    fprintf(f,"Widget%dWidth=%d\n",i+1,geo_rects[i].width());
    fprintf(f,"Widget%dHeight=%d\n",i+1,geo_rects[i].height());
  }
  fclose(f);
#endif  // WIN32
}


QString Geometry::ConfigDir()
{
  return DvtGetHomeDir()+"/.davit";
}


QString Geometry::ConfigPath()
{
  return ConfigDir()+"/geometry";
}
