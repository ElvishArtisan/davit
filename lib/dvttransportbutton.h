//   dvttransportbutton.h
//
//   An audio transport button widget.
//
//   (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef DVTTRANSPORTBUTTON_H
#define DVTTRANSPORTBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QPixmap>
#include <QColor>
#include <QResizeEvent>

#define DVTTRANSPORTBUTTON_DEFAULT_ON_COLOR Qt::green

class DvtTransportButton : public QPushButton
{
  Q_OBJECT
 public:
  enum TransType {Play=0,Stop=1,Record=2,FastForward=3,Rewind=4,Eject=5,
		  Pause=6,PlayFrom=7,PlayBetween=8,Loop=9,Up=10,Down=11,
		  PlayTo=12};
  enum TransState {On=0,Off=1,Flashing=2}; 
  DvtTransportButton(DvtTransportButton::TransType type,QWidget *parent=0);
  DvtTransportButton::TransType getType() const;
  void setType(DvtTransportButton::TransType type);
  QColor onColor() const;
  void setOnColor(QColor color);
  QColor accentColor() const;
  void setAccentColor(QColor color);
  void setState(DvtTransportButton::TransState state);
  
 public slots:
  void on();
  void off();
  void flash();

 protected:
  virtual void resizeEvent(QResizeEvent *);
  virtual void enabledChange(bool oldEnabled);
  
 private slots:
  void flashClock();

 private:
 void updateCaps();
 void drawMask(QPixmap *cap);
 void drawOnCap();
 void drawOffCap();
 void flashOn();
 void flashOff();
 QSize capSize() const;
 bool flash_state;
 DvtTransportButton::TransType button_type;
 DvtTransportButton::TransState button_state;
 QColor on_color;
 QColor accent_color;
 QPixmap *on_cap;
 QPixmap *off_cap;
 QTimer *flash_timer;
};


#endif  // DVTTRANSPORTBUTTON_H
