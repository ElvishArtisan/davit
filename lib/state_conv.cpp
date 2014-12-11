// state_conv.cpp
//
// Conversion functions for state abreviations.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
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

#include <state_conv.h>

QString StateToAbbreviation(const QString &str)
{
  QString state=str.lower();
  if(state=="alabama") {
    return "al";
  }
  if(state=="alaska") {
    return "ak";
  }
  if(state=="american samoa") {
    return "as";
  }
  if(state=="arizona") {
    return "az";
  }
  if(state=="arkansas") {
    return "ar";
  }
  if(state=="california") {
    return "ca";
  }
  if(state=="colorado") {
    return "co";
  }
  if(state=="connecticut") {
    return "ct";
  }
  if(state=="delaware") {
    return "de";
  }
  if(state=="district of columbia") {
    return "dc";
  }
  if(state=="federated states of micronesia") {
    return "fm";
  }
  if(state=="florida") {
    return "fl";
  }
  if(state=="georgia") {
    return "ga";
  }
  if(state=="guam") {
    return "gu";
  }
  if(state=="hawaii") {
    return "hi";
  }
  if(state=="idaho") {
    return "id";
  }
  if(state=="illinois") {
    return "il";
  }
  if(state=="indiana") {
    return "in";
  }
  if(state=="iowa") {
    return "ia";
  }
  if(state=="kansas") {
    return "ks";
  }
  if(state=="kentucky") {
    return "ky";
  }
  if(state=="louisiana") {
    return "la";
  }
  if(state=="maine") {
    return "me";
  }
  if(state=="marshall islands") {
    return "mh";
  }
  if(state=="maryland") {
    return "md";
  }
  if(state=="massachusetts") {
    return "ma";
  }
  if(state=="michigan") {
    return "mi";
  }
  if(state=="minnesota") {
    return "mn";
  }
  if(state=="mississippi") {
    return "ms";
  }
  if(state=="missouri") {
    return "mo";
  }
  if(state=="montana") {
    return "mt";
  }
  if(state=="nebraska") {
    return "ne";
  }
  if(state=="nevada") {
    return "nv";
  }
  if(state=="new hampshire") {
    return "nh";
  }
  if(state=="new jersey") {
    return "nj";
  }
  if(state=="new mexico") {
    return "nm";
  }
  if(state=="new york") {
    return "ny";
  }
  if(state=="north carolina") {
    return "nc";
  }
  if(state=="north dakota") {
    return "nd";
  }
  if(state=="northern mariana islands") {
    return "mp";
  }
  if(state=="ohio") {
    return "oh";
  }
  if(state=="oklahoma") {
    return "ok";
  }
  if(state=="oregon") {
    return "or";
  }
  if(state=="palau") {
    return "pw";
  }
  if(state=="pennsylvania") {
    return "pa";
  }
  if(state=="puerto rico") {
    return "pr";
  }
  if(state=="rhode island") {
    return "ri";
  }
  if(state=="south carolina") {
    return "sc";
  }
  if(state=="south dakota") {
    return "sd";
  }
  if(state=="tennessee") {
    return "tn";
  }
  if(state=="texas") {
    return "tx";
  }
  if(state=="utah") {
    return "ut";
  }
  if(state=="vermont") {
    return "vt";
  }
  if(state=="virgin islands") {
    return "vi";
  }
  if(state=="virginia") {
    return "va";
  }
  if(state=="washington") {
    return "wa";
  }
  if(state=="west virginia") {
    return "wv";
  }
  if(state=="wisconsin") {
    return "wi";
  }
  if(state=="wyoming") {
    return "wy";
  }
  return "";
}


QString AbbreviationToState(const QString &str)
{
  QString state=str.lower();
  if(state=="al") {
    return "Alabama";
  }
  if(state=="ak") {
    return "Alaska";
  }
  if(state=="as") {
    return "American Samoa";
  }
  if(state=="az") {
    return "Arizona";
  }
  if(state=="ar") {
    return "Arkansas";
  }
  if(state=="ca") {
    return "California";
  }
  if(state=="co") {
    return "Colorado";
  }
  if(state=="ct") {
    return "Connecticut";
  }
  if(state=="de") {
    return "Delaware";
  }
  if(state=="dc") {
    return "District of Columbia";
  }
  if(state=="fm") {
    return "Federated States of Micronesia";
  }
  if(state=="fl") {
    return "Florida";
  }
  if(state=="ga") {
    return "Georgia";
  }
  if(state=="gu") {
    return "Guam";
  }
  if(state=="hi") {
    return "Hawaii";
  }
  if(state=="id") {
    return "Idaho";
  }
  if(state=="il") {
    return "Illinois";
  }
  if(state=="in") {
    return "Indiana";
  }
  if(state=="ia") {
    return "Iowa";
  }
  if(state=="ks") {
    return "Kansas";
  }
  if(state=="ky") {
    return "Kentucky";
  }
  if(state=="la") {
    return "Louisiana";
  }
  if(state=="me") {
    return "Maine";
  }
  if(state=="mh") {
    return "Marshall Islands";
  }
  if(state=="md") {
    return "Maryland";
  }
  if(state=="ma") {
    return "Massachusetts";
  }
  if(state=="mi") {
    return "Michigan";
  }
  if(state=="mn") {
    return "Minnesota";
  }
  if(state=="ms") {
    return "Mississippi";
  }
  if(state=="mo") {
    return "Missouri";
  }
  if(state=="mt") {
    return "Montana";
  }
  if(state=="ne") {
    return "Nebraska";
  }
  if(state=="nv") {
    return "Nevada";
  }
  if(state=="nh") {
    return "New Hampshire";
  }
  if(state=="nj") {
    return "New Jersey";
  }
  if(state=="nm") {
    return "New Mexico";
  }
  if(state=="ny") {
    return "New York";
  }
  if(state=="nc") {
    return "North Carolina";
  }
  if(state=="nd") {
    return "North Dakota";
  }
  if(state=="mp") {
    return "Northern Mariana Islands";
  }
  if(state=="oh") {
    return "Ohio";
  }
  if(state=="ok") {
    return "Oklahoma";
  }
  if(state=="or") {
    return "Oregon";
  }
  if(state=="pw") {
    return "Palau";
  }
  if(state=="pa") {
    return "Pennsylvania";
  }
  if(state=="pr") {
    return "Puerto Rico";
  }
  if(state=="ri") {
    return "Rhode Island";
  }
  if(state=="sc") {
    return "South Carolina";
  }
  if(state=="sd") {
    return "South Dakota";
  }
  if(state=="tn") {
    return "Tennessee";
  }
  if(state=="tx") {
    return "Texas";
  }
  if(state=="ut") {
    return "Utah";
  }
  if(state=="vt") {
    return "Vermont";
  }
  if(state=="vi") {
    return "Virgin Islands";
  }
  if(state=="va") {
    return "Virginia";
  }
  if(state=="wa") {
    return "Washington";
  }
  if(state=="wv") {
    return "West Virginia";
  }
  if(state=="wi") {
    return "Wisconsin";
  }
  if(state=="wy") {
    return "Wyoming";
  }
  return "";
}
