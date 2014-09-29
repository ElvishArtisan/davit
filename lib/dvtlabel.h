//   dvtlabel.h
//
//   An label widget with word wrap.
//
//   (C) Copyright 2002 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: dvtlabel.h,v 1.1 2007/11/19 16:53:35 fredg Exp $
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

#ifndef DVTLABEL_H
#define DVTLABEL_H

#include <qwidget.h>
#include <qlabel.h>
#include <qrangecontrol.h>
#include <qcolor.h>
#include <qpalette.h>
#include <qsize.h>
#include <qpixmap.h>


/**
 * @short A label widget with word wrap.
 * @author Fred Gleason <fredg@wava.com>
 *
 * DvtLabel is a label widget almost exactly identical to QLabel with one 
 * addition: the ability to automatically word-wrap text.
 *
 **/
class DvtLabel : public QLabel
{
  Q_OBJECT

  public:
  /**
   * Create an DvtLabel with the following parameters:
   * @param parent The parent widget
   * @param name The name of the widget
   * @param f The widget flags
   **/
   DvtLabel(QWidget *parent,const char *name,WFlags f=0);

  /**
   * Create an RSlider with the following parameters:
   * @param text The text of the label
   * @param parent The parent widget
   * @param name The name of the widget
   * @param f The widget flags
   **/
   DvtLabel(const QString &text,QWidget *parent,const char *name,WFlags f=0);

  /**
   * Create an RSlider with the following parameters:
   * @param buddy The buddy widget
   * @param text The text of the label
   * @param parent The parent widget
   * @param name The name of the widget
   * @param f The widget flags
   **/
   DvtLabel(QWidget *buddy,const QString &text,QWidget *parent,const char *name,
	   WFlags f=0);

   /**
    * Returns the label text.
    **/
   QString text() const;
   
   /**
    * Set the label font
    **/
   void setFont(const QFont &font);


   /**
    * Returns whether word wrap is enabled or not.
    **/
   bool wordWrapEnabled() const;

   /**
    * Set whether word wrap is enabled or not.
    * @param state True = enable word wrap, false = don't enable
    **/
   void setWordWrapEnabled(bool state);

  public slots:
   void setText(const QString &string);

  private:
   QString WrapText();
   QString label_text;
   QFont label_font;
   bool label_wrap;
};



#endif  // DVTLABEL_H
