// label.cpp
//
// An audio- and touchscreen-friendly slider widget.
//
// (C) Copyright 2002-2025 Fred Gleason <fredg@paravelsystems.com>
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

#include <stdlib.h>
#include <stdio.h>

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QColor>
#include <QPalette>
#include <QSize>
#include <QRect>
#include <QPixmap>

#include "label.h"

Label::Label(QWidget *parent)
  : QLabel(parent)
{
  label_wrap=false;
}


Label::Label(const QString &text,QWidget *parent=0)
  : QLabel(text,parent)
{
  label_wrap=false;
  label_text=text;
  QLabel::setText(WrapText());
}


QString Label::text() const
{
  return label_text;
}


void Label::setFont(const QFont &font)
{
  label_font=font;
  QLabel::setFont(font);
  QLabel::setText(WrapText());
}


bool Label::wordWrapEnabled() const
{
  return label_wrap;
}


void Label::setWordWrapEnabled(bool state)
{
  label_wrap=state;
  QLabel::setText(WrapText());
}


void Label::setText(const QString &string)
{
  label_text=string;
  QLabel::setText(WrapText());
}


QString Label::WrapText()
{
  QFontMetrics fm(label_font);
  QString str;
  QString residue=label_text;
  bool space_found=false;
  int l;

  if(label_wrap&&!label_text.isEmpty()) {
    while(!residue.isEmpty()) {
      space_found=false;
      for(int i=residue.length();i>=0;i--) {
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
	for(int i=l;i>=0;i--) {
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
