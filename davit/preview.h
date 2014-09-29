// preview.h
//
// Preview a print job
//
//   (C) Copyright 2011 Fred Gleason <fredg@paravelsystems.com>
//
//     $Id: preview.h,v 1.2 2011/03/29 15:38:51 pcvs Exp $
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

#ifndef PREVIEW_H
#define PREVIEW_H

#include <vector>

#include <qdialog.h>
#include <qpixmap.h>
#include <qlabel.h>
#include <qdatetime.h>
#include <qprinter.h>
#include <qpaintdevicemetrics.h>

#include <dvttransportbutton.h>

class Preview : public QDialog
{
 Q_OBJECT
 public:
  Preview(QWidget *parent=0,const char *name=0);
  ~Preview();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(int affiliate_id,const QDate &date,bool show_airings);
  int exec(int affiliate_id,int pgm_id,const QDate &date,bool show_airings);
  void setupData();
  void prevData();
  void nextData();
  void printData();
  void cancelData();

 private:
  void Render(bool print);
  QPainter *NewLine(int spacing,QPainter *p,int *ypos,bool print);
  int PrintText(int ypos,QFontMetrics *fm,const QString &str,QPainter *p);
  std::vector<QPixmap *> preview_maps;
  unsigned preview_current_map;
  QLabel *preview_label;
  QLabel *preview_printer_name_label;
  QPrinter *preview_printer;
  int preview_affiliate_id;
  int preview_program_id;
  bool preview_show_airings;
  QDate preview_date;
  QPaintDeviceMetrics *preview_pdm;
  QPushButton *preview_print_button;
  DvtTransportButton *preview_next_button;
  DvtTransportButton *preview_prev_button;
};


#endif  // PREVIEW_H
