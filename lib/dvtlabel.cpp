//   dvtlabel.cpp
//
//   An audio- and touchscreen-friendly slider widget.
//
//   (C) Copyright 2002 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: dvtlabel.cpp,v 1.1 2007/11/19 16:53:34 fredg Exp $
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

#include <stdlib.h>
#include <stdio.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qpalette.h>
#include <qsize.h>
#include <qrect.h>
#include <dvtlabel.h>
#include <qpixmap.h>


DvtLabel::DvtLabel(QWidget *parent,const char *name,WFlags f)
  : QLabel(parent,name,f)
{
  label_wrap=false;
}


DvtLabel::DvtLabel(const QString &text,QWidget *parent=0,const char *name=0,
	       WFlags f)  : QLabel(text,parent,name,f)
{
  label_wrap=false;
  label_text=text;
  QLabel::setText(WrapText());
}


DvtLabel::DvtLabel(QWidget *buddy,const QString &text,QWidget *parent,
	       const char *name,WFlags f): QLabel(buddy,text,parent,name,f)
{
  label_wrap=false;
  label_text=text;
  QLabel::setText(WrapText());
}


QString DvtLabel::text() const
{
  return label_text;
}


void DvtLabel::setFont(const QFont &font)
{
  label_font=font;
  QLabel::setFont(font);
  QLabel::setText(WrapText());
}


bool DvtLabel::wordWrapEnabled() const
{
  return label_wrap;
}


void DvtLabel::setWordWrapEnabled(bool state)
{
  label_wrap=state;
  QLabel::setText(WrapText());
}


void DvtLabel::setText(const QString &string)
{
  label_text=string;
  QLabel::setText(WrapText());
}


QString DvtLabel::WrapText()
{
  QFontMetrics fm(label_font);
  QString str;
  QString residue=label_text;
  bool space_found=false;
  int l;

  if(label_wrap&&!label_text.isEmpty()) {
    while(!residue.isEmpty()) {
      space_found=false;
      for(unsigned i=residue.length();i>=0;i--) {
	if((i==(residue.length()))||(residue.at(i).isSpace())) {
	  if(fm.boundingRect(residue.left(i)).width()<=width()) {
	    space_found=true;
	    if(!str.isEmpty()) {
	      str+="\n";
	    }
	    str+=residue.left(i);
	    if(i==residue.length()) {
	      return str;
	    }
	    residue=residue.right(residue.length()-i-1);
	  }
	}
      }
      if(!space_found) {
	l=residue.length();
	for(unsigned i=l;i>=0;i--) {
	  if(fm.boundingRect(residue.left(i)).width()<=width()) {
	    if(!str.isEmpty()) {
	      str+="\n";
	    }
	    str+=residue.left(i);
	    if(i==residue.length()) {
	      return str;
	    }
	    residue=residue.right(residue.length()-i-1);
	  }
	}
      }
    }
  }
  return label_text;
}
