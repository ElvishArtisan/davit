// fontengine.cpp
//
// Engine for calculating fonts in Davit UIs
//
//   (C) Copyright 2019 Fred Gleason <fredg@paravelsystems.com>
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

#include "fontengine.h"

FontEngine::FontEngine(const QFont &default_font,DvtConfig *c)
{
  font_config=c;
  MakeFonts(default_font);
}


FontEngine::~FontEngine()
{
  delete font_button_font_metrics;
  delete font_huge_button_font_metrics;
  delete font_big_button_font_metrics;
  delete font_sub_button_font_metrics;
  delete font_section_label_font_metrics;
  delete font_big_label_font_metrics;
  delete font_label_font_metrics;
  delete font_sub_label_font_metrics;
  delete font_progress_font_metrics;
  delete font_banner_font_metrics;
  delete font_timer_font_metrics;
  delete font_small_timer_font_metrics;
  delete font_default_font_metrics;
}


QFont FontEngine::buttonFont() const
{
  return font_button_font;
}


QFontMetrics *FontEngine::buttonFontMetrics() const
{
  return font_button_font_metrics;
}


QFont FontEngine::hugeButtonFont() const
{
  return font_huge_button_font;
}


QFontMetrics *FontEngine::hugeButtonFontMetrics() const
{
  return font_huge_button_font_metrics;
}


QFont FontEngine::bigButtonFont() const
{
  return font_big_button_font;
}


QFontMetrics *FontEngine::bigButtonFontMetrics() const
{
  return font_big_button_font_metrics;
}


QFont FontEngine::subButtonFont() const
{
  return font_sub_button_font;
}


QFontMetrics *FontEngine::subButtonFontMetrics() const
{
  return font_sub_button_font_metrics;
}


QFont FontEngine::sectionLabelFont() const
{
  return font_section_label_font;
}


QFontMetrics *FontEngine::sectionLabelFontMetrics() const
{
  return font_section_label_font_metrics;
}


QFont FontEngine::bigLabelFont() const
{
  return font_big_label_font;
}


QFontMetrics *FontEngine::bigLabelFontMetrics() const
{
  return font_big_label_font_metrics;
}


QFont FontEngine::labelFont() const
{
  return font_label_font;
}


QFontMetrics *FontEngine::labelFontMetrics() const
{
  return font_label_font_metrics;
}


QFont FontEngine::subLabelFont() const
{
  return font_sub_label_font;
}


QFontMetrics *FontEngine::subLabelFontMetrics() const
{
  return font_sub_label_font_metrics;
}


QFont FontEngine::progressFont() const
{
  return font_progress_font;
}


QFontMetrics *FontEngine::progressFontMetrics() const
{
  return font_progress_font_metrics;
}


QFont FontEngine::bannerFont() const
{
  return font_banner_font;
}


QFontMetrics *FontEngine::bannerFontMetrics() const
{
  return font_banner_font_metrics;
}


QFont FontEngine::timerFont() const
{
  return font_timer_font;
}


QFontMetrics *FontEngine::timerFontMetrics() const
{
  return font_timer_font_metrics;
}


QFont FontEngine::smallTimerFont() const
{
  return font_small_timer_font;
}


QFontMetrics *FontEngine::smallTimerFontMetrics() const
{
  return font_small_timer_font_metrics;
}


QFont FontEngine::defaultFont() const
{
  return font_default_font;
}


QFontMetrics *FontEngine::defaultFontMetrics() const
{
  return font_default_font_metrics;
}


QFont FontEngine::printerHeaderFont() const
{
  return font_big_label_font;
}


QFontMetrics *FontEngine::printerHeaderFontMetrics() const
{
  return font_big_label_font_metrics;
}


QFont FontEngine::printerFont() const
{
  return font_printer_font;
}


QFontMetrics *FontEngine::printerFontMetrics() const
{
  return font_printer_font_metrics;
}


DvtConfig *FontEngine::config() const
{
  return font_config;
}


void FontEngine::MakeFonts(const QFont &default_font)
{
  /*
  printf("family: %s  pixelSize: %d  pointSize: %d\n",
	 (const char *)default_font.family().toUtf8(),
	 default_font.pixelSize(),
	 default_font.pointSize());
  */

  //
  // Default Font Values
  //
  QString family="System";
  int button_size=12;
  int label_size=11;
  int default_size=11;

  //
  // Overrides from rd.conf(5)
  //
  if(!font_config->fontFamily().isEmpty()) {
    family=font_config->fontFamily();
  }
  if(font_config->fontButtonSize()>0) {
    button_size=font_config->fontButtonSize();
  }
  if(font_config->fontLabelSize()>0) {
    label_size=font_config->fontLabelSize();
  }
  if(font_config->fontDefaultSize()>0) {
    default_size=font_config->fontDefaultSize();
  }

  //
  // Generate Fonts
  //
  font_button_font=QFont(family,button_size,QFont::Bold);
  font_button_font.setPixelSize(button_size);
  font_button_font_metrics=new QFontMetrics(font_button_font);

  font_huge_button_font=QFont(family,button_size+24,QFont::DemiBold);
  font_huge_button_font.setPixelSize(button_size+24);
  font_huge_button_font_metrics=new QFontMetrics(font_huge_button_font);

  font_big_button_font=QFont(family,button_size+4,QFont::DemiBold);
  font_big_button_font.setPixelSize(button_size+4);
  font_big_button_font_metrics=new QFontMetrics(font_big_button_font);

  font_sub_button_font=QFont(family,button_size-2,QFont::Normal);
  font_sub_button_font.setPixelSize(button_size-2);
  font_sub_button_font_metrics=new QFontMetrics(font_sub_button_font);

  font_section_label_font=QFont(family,label_size+2,QFont::Bold);
  font_section_label_font.setPixelSize(label_size+2);
  font_section_label_font_metrics=new QFontMetrics(font_section_label_font);

  font_big_label_font=QFont(family,label_size+4,QFont::Bold);
  font_big_label_font.setPixelSize(label_size+4);
  font_big_label_font_metrics=new QFontMetrics(font_big_label_font);

  font_label_font=QFont(family,label_size,QFont::Bold);
  font_label_font.setPixelSize(label_size);
  font_label_font_metrics=new QFontMetrics(font_label_font);

  font_sub_label_font=QFont(family,label_size,QFont::Normal);
  font_sub_label_font.setPixelSize(label_size);
  font_sub_label_font_metrics=new QFontMetrics(font_sub_label_font);

  font_progress_font=QFont(family,label_size+4,QFont::Bold);
  font_progress_font.setPixelSize(label_size+4);
  font_progress_font_metrics=new QFontMetrics(font_progress_font);

  font_banner_font=QFont(family,26,QFont::Normal);
  font_banner_font.setPixelSize(26);
  font_banner_font_metrics=new QFontMetrics(font_banner_font);

  font_timer_font=QFont(family,20,QFont::Normal);
  font_timer_font.setPixelSize(20);
  font_timer_font_metrics=new QFontMetrics(font_timer_font);

  font_small_timer_font=QFont(family,default_size+2,QFont::Normal);
  font_small_timer_font.setPixelSize(default_size+2);
  font_small_timer_font_metrics=new QFontMetrics(font_small_timer_font);

  font_default_font=QFont(family,default_size,QFont::Normal);
  font_default_font.setPixelSize(default_size);
  font_default_font_metrics=new QFontMetrics(font_default_font);

  font_printer_font=QFont(family,label_size+2,QFont::Normal);
  font_printer_font.setPixelSize(label_size+2);
  font_printer_font_metrics=new QFontMetrics(font_printer_font);
}
