//   dvttransportbutton.cpp
//
//   An audio transport button widget.
//
//   (C) Copyright 2002 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: dvttransportbutton.cpp,v 1.1 2011/03/29 15:38:51 pcvs Exp $
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
//

#include <qpushbutton.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <qpointarray.h>
#include <qtimer.h>
#include <qpalette.h>

#include <dvttransportbutton.h>


DvtTransportButton::DvtTransportButton(DvtTransportButton::TransType type,
				   QWidget *parent,const char *name) :
  QPushButton(parent,name)
{
  button_type=type;
  button_state=DvtTransportButton::Off;
  on_color=QColor(DVTTRANSPORTBUTTON_DEFAULT_ON_COLOR);
  accent_color=QColor(colorGroup().shadow());
  on_cap=new QPixmap();
  drawOnCap();
  off_cap=new QPixmap();
  drawOffCap();
  setPixmap(*off_cap);

  flash_timer=new QTimer(this,"flash_timer");
  connect(flash_timer,SIGNAL(timeout()),this,SLOT(flashClock()));
  flash_state=false;
}


DvtTransportButton::TransType DvtTransportButton::getType() const
{
  return button_type;
}


void DvtTransportButton::setType(DvtTransportButton::TransType type)
{
  button_type=type;
}


QColor DvtTransportButton::onColor() const
{
  return on_color;
}


void DvtTransportButton::setOnColor(QColor color)
{
  if(color!=on_color) {
    on_color=color;
    drawOnCap();
    drawOffCap();
    updateCaps();
  }
}

QColor DvtTransportButton::accentColor() const
{
  return accent_color;
}


void DvtTransportButton::setAccentColor(QColor color)
{
  if(color!=accent_color) {
    accent_color=color;
    drawOnCap();
    drawOffCap();
    updateCaps();
  }
}

void DvtTransportButton::setState(DvtTransportButton::TransState state)
{
  QKeySequence a=accel();
  button_state=state;
  switch(button_state) {
      case DvtTransportButton::On:
	flashOff();
	if(isEnabled()) {
	  setPixmap(*on_cap);
	}
	break;
      case DvtTransportButton::Off:
	flashOff();
	if(isEnabled()) {
	  setPixmap(*off_cap);
	}
	break;
      case DvtTransportButton::Flashing:
	if(isEnabled()) {
	  flashOn();
	}
	break;
  }
  setAccel(a);
}


void DvtTransportButton::on()
{
  setState(DvtTransportButton::On);
}


void DvtTransportButton::off()
{
  setState(DvtTransportButton::Off);
}


void DvtTransportButton::flash()
{
  setState(DvtTransportButton::Flashing);
}


void DvtTransportButton::resizeEvent(QResizeEvent *event)
{
  QKeySequence a=accel();
  drawOnCap();
  drawOffCap();
  switch(button_state) {
      case DvtTransportButton::Off:
	setPixmap(*off_cap);
	break;
      case DvtTransportButton::On:
	setPixmap(*on_cap);
	break;
      default:
	setPixmap(*off_cap);
	break;
  }
  setAccel(a);
}


void DvtTransportButton::enabledChange(bool oldEnabled)
{
  QKeySequence a=accel();
  if(isEnabled()&&!oldEnabled) {
    setState(button_state);
    update();
  }
  if(!isEnabled()&&oldEnabled) {
//    setPixmap(*grey_cap);
    update();
    setAccel(a);
  }
}
 

void DvtTransportButton::flashClock()
{
  QKeySequence a=accel();
  if(flash_state) {
    flash_state=false;
    setPixmap(*off_cap);
  }
  else {
    flash_state=true;
    setPixmap(*on_cap);
  }
  setAccel(a);
}


void DvtTransportButton::updateCaps()
{
  switch(button_state) {
      case DvtTransportButton::On:
	setPixmap(*on_cap);
	break;

      case DvtTransportButton::Flashing:
	if(flash_state) {
	  setPixmap(*on_cap);
	}
	else {
	  setPixmap(*off_cap);
	}
	break;

      case DvtTransportButton::Off:
	setPixmap(*off_cap);
	break;
  }
}


void DvtTransportButton::drawMask(QPixmap *cap)
{
  QPointArray triangle=QPointArray(3);
  QPainter b;
  QBitmap *bitmap=new QBitmap(size());
  int edge;

  if(height()<width()) {
    edge=height();
  }
  else {
    edge=width();
  }
  cap->resize(size());
  b.begin(bitmap);
  b.fillRect(0,0,size().width(),size().height(),QColor(color0));
  b.setPen(QColor(color1));
  b.setBrush(QColor(color1));

  switch(button_type) {
      case DvtTransportButton::Play:
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      case DvtTransportButton::Stop:
	b.fillRect(width()/2-edge*3/10,height()/2-edge*3/10,
		   edge*3/5,edge*3/5,QColor(color1));
	break;
      case DvtTransportButton::Record:
	b.drawEllipse(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		      (3*edge)/5,(3*edge)/5);
	break;
      case DvtTransportButton::FastForward:
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      case DvtTransportButton::Rewind:
	triangle.setPoint(0,width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2,height()/2);
	triangle.setPoint(2,width()/2+(3*edge)/10,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2-(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      case DvtTransportButton::Eject:
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2);
	b.drawPolygon(triangle);
	b.fillRect(width()/2-(3*edge)/10,height()/2+edge/10,
		   (3*edge)/5,edge/5,QColor(color1));
	break;
      case DvtTransportButton::Pause:
	b.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   (3*edge)/15,(3*edge)/5,QColor(color1));
	b.fillRect(width()/2+(3*edge)/30,height()/2-(3*edge)/10,
		   (3*edge)/15,(3*edge)/5,QColor(color1));
	break;
      case DvtTransportButton::PlayFrom:
	b.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(color1));
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      case DvtTransportButton::PlayBetween:
	b.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(color1));
	b.fillRect(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(color1));
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(2*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      case DvtTransportButton::Loop:
	b.moveTo(width()/2-(2*edge)/10+1,height()/2+(edge)/4);
	b.moveTo(width()/2+(edge)/10+1,height()/2-edge/10);
	b.moveTo(width()/2-(2*edge)/10+1,height()/2+(edge)/4);
	b.drawArc(width()/6,height()/2-edge/9,2*width()/3,
		  height()/3+edge/10,1440,5760);
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(edge)/4);
	triangle.setPoint(1,width()/2+(edge)/10+1,height()/2-edge/10);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+edge/20);
	b.drawPolygon(triangle);
	break;
      case DvtTransportButton::Up:
	triangle.setPoint(0,width()/2,(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()-(3*edge)/10);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()-(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      case DvtTransportButton::Down:
	triangle.setPoint(0,width()/2,height()-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,(3*edge)/10);
	triangle.setPoint(2,width()/2-(3*edge)/10,(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      case DvtTransportButton::PlayTo:
	b.fillRect(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(color1));
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(2*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	b.drawPolygon(triangle);
	break;
      default:
	b.fillRect(0,0,width(),height(),QColor(color1));
  }
  b.end();
  cap->setMask(*bitmap);

  delete bitmap;
}


void DvtTransportButton::drawOnCap()
{
  QPainter p;
  QPointArray triangle=QPointArray(3);
  int edge;

  if(height()<width()) {
    edge=height();
  }
  else {
    edge=width();
  }
  drawMask(on_cap);
  p.begin(on_cap);
  p.setPen(on_color);
  p.setBrush(on_color);
  switch(button_type) {
      case DvtTransportButton::Play:
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.lineTo(width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2+(3*edge)/10,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	break;
      case DvtTransportButton::Stop:
	p.fillRect(width()/2-edge*3/10,height()/2-edge*3/10,
		   edge*3/5,edge*3/5,QColor(on_color));
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-edge*3/10,height()/2+edge*3/10);
	p.lineTo(width()/2-edge*3/10,height()/2-edge*3/10);
	p.lineTo(width()/2+edge*3/10,height()/2-edge*3/10);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2+edge*3/10,height()/2+edge*3/10);
	p.lineTo(width()/2-edge*3/10,height()/2+edge*3/10);
	break;
      case DvtTransportButton::Record:
	p.setPen(QColor(red));
	p.setBrush(QColor(red));
	p.drawEllipse(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		      (3*edge)/5,(3*edge)/5);
	break;
      case DvtTransportButton::FastForward:
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.lineTo(width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2,height()/2+(3*edge)/10);
	p.lineTo(width()/2,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2+(3*edge)/10,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2,height()/2+(3*edge)/10);
	break;
      case DvtTransportButton::Rewind:
	triangle.setPoint(0,width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2,height()/2);
	triangle.setPoint(2,width()/2+(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	p.lineTo(width()/2,height()/2);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2+(3*edge)/10,height()/2+(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2-(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2,height()/2-(3*edge)/10);
	p.lineTo(width()/2-(3*edge)/10,height()/2);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2,height()/2+(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2,height()/2-(3*edge)/10);
	break;
      case DvtTransportButton::Eject:
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(3*edge)/10,height()/2);
	p.lineTo(width()/2,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2+(3*edge)/10,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(3*edge)/10,height()/2);
	p.fillRect(width()/2-(3*edge)/10,height()/2+edge/10,
		   (3*edge)/5,edge/5,on_color);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(3*edge)/10,height()/2+edge/10+edge/5);
	p.lineTo(width()/2-(3*edge)/10,height()/2+edge/10);
	p.lineTo(width()/2+(3*edge)/10,height()/2+edge/10);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2+(3*edge)/10,height()/2+edge/10+edge/5);
	p.lineTo(width()/2-(3*edge)/10,height()/2+edge/10+edge/5);
	break;
      case DvtTransportButton::Pause:
	p.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   (3*edge)/15,(3*edge)/5,on_color);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.lineTo(width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.lineTo(width()/2-(3*edge)/10+(3*edge)/15,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(3*edge)/10+(3*edge)/15,height()/2+(3*edge)/10);
	p.lineTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.fillRect(width()/2+(3*edge)/30,height()/2-(3*edge)/10,
		   (3*edge)/15,(3*edge)/5,on_color);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2+(3*edge)/30,height()/2+(3*edge)/10);
	p.lineTo(width()/2+(3*edge)/30,height()/2-(3*edge)/10);
	p.lineTo(width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2+(3*edge)/10,height()/2+(3*edge)/10);
	p.lineTo(width()/2+(3*edge)/30,height()/2+(3*edge)/10);
	break;
      case DvtTransportButton::PlayFrom:
	p.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	p.lineTo(width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2+(3*edge)/10+1,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	break;
      case DvtTransportButton::PlayBetween:
	p.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	p.fillRect(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(2*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	p.lineTo(width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2+(2*edge)/10+1,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	break;
      case DvtTransportButton::Loop:
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(2*edge)/10+1,height()/2+(edge)/4);
	p.lineTo(width()/2+(edge)/10+1,height()/2-edge/10);
	p.setPen(QColor(colorGroup().dark()));
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(2*edge)/10+1,height()/2+(edge)/4);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawArc(width()/6,height()/2-edge/9,2*width()/3,
		  height()/3+edge/10,1440,5760);
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(edge)/4);
	triangle.setPoint(1,width()/2+(edge)/10+1,height()/2-edge/10);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+edge/20);
	p.drawPolygon(triangle);
	break;
      case DvtTransportButton::Up:
	triangle.setPoint(0,width()/2,(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()-(3*edge)/10);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()-(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(3*edge)/10,height()-(3*edge)/10);
	p.lineTo(width()/2,(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2+(3*edge)/10,height()-(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(3*edge)/10,height()-(3*edge)/10);
	break;
      case DvtTransportButton::Down:
	triangle.setPoint(0,width()/2,height()-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,(3*edge)/10);
	triangle.setPoint(2,width()/2-(3*edge)/10,(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().dark()));
	p.moveTo(width()/2-(3*edge)/10,(3*edge)/10);
	p.lineTo(width()/2,height()-(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2+(3*edge)/10,(3*edge)/10);
	p.setPen(QColor(colorGroup().shadow()));
	p.lineTo(width()/2-(3*edge)/10,(3*edge)/10);
	break;
      case DvtTransportButton::PlayTo:
	p.fillRect(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(2*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.lineTo(width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2+(2*edge)/10+1,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	break;
  }  
  p.end();
}


void DvtTransportButton::drawOffCap()
{
  QPainter p;
  QPointArray triangle=QPointArray(3);
  int edge;

  if(height()<width()) {
    edge=height();
  }
  else {
    edge=width();
  }
  drawMask(off_cap);
  p.begin(off_cap);
  p.setPen(QColor(black));
  p.setBrush(QColor(black));
  switch(button_type) {
      case DvtTransportButton::Play:
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.lineTo(width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2+(3*edge)/10,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	break;
      case DvtTransportButton::Stop:
	p.fillRect(width()/2-edge*3/10,height()/2-edge*3/10,
		   edge*3/5,edge*3/5,QColor(colorGroup().shadow()));
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-edge*3/10,height()/2+edge*3/10);
	p.lineTo(width()/2-edge*3/10,height()/2-edge*3/10);
	p.lineTo(width()/2+edge*3/10,height()/2-edge*3/10);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2+edge*3/10,height()/2+edge*3/10);
	p.lineTo(width()/2-edge*3/10,height()/2+edge*3/10);
	break;
      case DvtTransportButton::Record:
	p.setPen(QColor(darkRed));
	p.setBrush(QColor(darkRed));
	p.drawEllipse(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		      (3*edge)/5,(3*edge)/5);
	break;
      case DvtTransportButton::FastForward:
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.lineTo(width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2,height()/2+(3*edge)/10);
	p.lineTo(width()/2,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2+(3*edge)/10,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2,height()/2+(3*edge)/10);
	break;
      case DvtTransportButton::Rewind:
	triangle.setPoint(0,width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2,height()/2);
	triangle.setPoint(2,width()/2+(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	p.lineTo(width()/2,height()/2);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2+(3*edge)/10,height()/2+(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2-(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2,height()/2-(3*edge)/10);
	p.lineTo(width()/2-(3*edge)/10,height()/2);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2,height()/2+(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2,height()/2-(3*edge)/10);
	break;
      case DvtTransportButton::Eject:
	triangle.setPoint(0,width()/2,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2);
	p.drawPolygon(triangle);
	p.fillRect(width()/2-(3*edge)/10,height()/2+edge/10,
		   (3*edge)/5,edge/5,QColor(black));		   
	break;
      case DvtTransportButton::Pause:
	p.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   (3*edge)/15,(3*edge)/5,QColor(black));
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.lineTo(width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.lineTo(width()/2-(3*edge)/10+(3*edge)/15,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(3*edge)/10+(3*edge)/15,height()/2+(3*edge)/10);
	p.lineTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.fillRect(width()/2+(3*edge)/30,height()/2-(3*edge)/10,
		   (3*edge)/15,(3*edge)/5,QColor(black));
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2+(3*edge)/30,height()/2+(3*edge)/10);
	p.lineTo(width()/2+(3*edge)/30,height()/2-(3*edge)/10);
	p.lineTo(width()/2+(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2+(3*edge)/10,height()/2+(3*edge)/10);
	p.lineTo(width()/2+(3*edge)/30,height()/2+(3*edge)/10);
	break;
      case DvtTransportButton::PlayFrom:
	p.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	p.lineTo(width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2+(3*edge)/10+1,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	break;
      case DvtTransportButton::PlayBetween:
	p.fillRect(width()/2-(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	p.fillRect(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(2*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	p.lineTo(width()/2-(2*edge)/10+1,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2+(2*edge)/10+1,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(2*edge)/10+1,height()/2+(3*edge)/10);
	break;
      case DvtTransportButton::Loop:
	triangle.setPoint(0,width()/2-(2*edge)/10+1,height()/2-(edge)/4);
	triangle.setPoint(1,width()/2+(edge)/10+1,height()/2-edge/10);
	triangle.setPoint(2,width()/2-(2*edge)/10+1,height()/2+edge/20);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(2*edge)/10+1,height()/2+(edge)/4);
	p.lineTo(width()/2+(edge)/10+1,height()/2-edge/10);
	p.setPen(QColor(colorGroup().dark()));
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(2*edge)/10+1,height()/2+(edge)/4);
	p.setPen(QColor(colorGroup().shadow()));
	p.drawArc(width()/6,height()/2-edge/9,2*width()/3,
		  height()/3+edge/10,1440,5760);
	break;
      case DvtTransportButton::Up:
	triangle.setPoint(0,width()/2,(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,height()-(3*edge)/10);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()-(3*edge)/10);
	p.drawPolygon(triangle);
	break;
      case DvtTransportButton::Down:
	triangle.setPoint(0,width()/2,height()-(3*edge)/10);
	triangle.setPoint(1,width()/2+(3*edge)/10,(3*edge)/10);
	triangle.setPoint(2,width()/2-(3*edge)/10,(3*edge)/10);
	p.drawPolygon(triangle);
	break;
      case DvtTransportButton::PlayTo:
	p.fillRect(width()/2+(3*edge)/10,height()/2-(3*edge)/10,
		   3,(3*edge)/5,QBrush(accent_color));
	triangle.setPoint(0,width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	triangle.setPoint(1,width()/2+(2*edge)/10+1,height()/2);
	triangle.setPoint(2,width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.drawPolygon(triangle);
	p.setPen(QColor(colorGroup().shadow()));
	p.moveTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	p.lineTo(width()/2-(3*edge)/10,height()/2-(3*edge)/10);
	p.setPen(QColor(colorGroup().dark()));
	p.lineTo(width()/2+(2*edge)/10+1,height()/2);
	p.setPen(QColor(colorGroup().light()));
	p.lineTo(width()/2-(3*edge)/10,height()/2+(3*edge)/10);
	break;
  }  
  p.end();
}

void DvtTransportButton::flashOn()
{
  if(!flash_timer->isActive()) {
    flash_timer->start(500);
  }
}


void DvtTransportButton::flashOff()
{
  if(flash_timer->isActive()) {
    flash_timer->stop();
  }
}



