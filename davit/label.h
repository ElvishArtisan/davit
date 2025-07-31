//   label.h
//
//   An label widget with word wrap.
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

#ifndef LABEL_H
#define LABEL_H

#include <QWidget>
#include <QLabel>
#include <QColor>
#include <QPalette>
#include <QSize>
#include <QPixmap>

/**
 * @short A label widget with word wrap.
 * @author Fred Gleason <fredg@wava.com>
 *
 * Label is a label widget almost exactly identical to QLabel with one 
 * addition: the ability to automatically word-wrap text.
 *
 **/
class Label : public QLabel
{
  Q_OBJECT

  public:
  /**
   * Create an Label with the following parameters:
   * @param parent The parent widget
   * @param name The name of the widget
   * @param f The widget flags
   **/
   Label(QWidget *parent);

  /**
   * Create an RSlider with the following parameters:
   * @param text The text of the label
   * @param parent The parent widget
   * @param name The name of the widget
   * @param f The widget flags
   **/
   Label(const QString &text,QWidget *parent);

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



#endif  // LABEL_H
