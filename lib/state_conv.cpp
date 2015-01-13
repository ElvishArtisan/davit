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

#include <qobject.h>

#include <state_conv.h>

QString StateToAbbreviation(const QString &str)
{
  QString state=str.lower();
  if(state==QObject::tr("all")) {
    return "aa";
  }
  if(state==QObject::tr("alabama")) {
    return "al";
  }
  if(state==QObject::tr("alaska")) {
    return "ak";
  }
  if(state==QObject::tr("american samoa")) {
    return "as";
  }
  if(state==QObject::tr("arizona")) {
    return "az";
  }
  if(state==QObject::tr("arkansas")) {
    return "ar";
  }
  if(state==QObject::tr("california")) {
    return "ca";
  }
  if(state==QObject::tr("colorado")) {
    return "co";
  }
  if(state==QObject::tr("connecticut")) {
    return "ct";
  }
  if(state==QObject::tr("delaware")) {
    return "de";
  }
  if(state==QObject::tr("district of columbia")) {
    return "dc";
  }
  if(state==QObject::tr("federated states of micronesia")) {
    return "fm";
  }
  if(state==QObject::tr("florida")) {
    return "fl";
  }
  if(state==QObject::tr("georgia")) {
    return "ga";
  }
  if(state==QObject::tr("guam")) {
    return "gu";
  }
  if(state==QObject::tr("hawaii")) {
    return "hi";
  }
  if(state==QObject::tr("idaho")) {
    return "id";
  }
  if(state==QObject::tr("illinois")) {
    return "il";
  }
  if(state==QObject::tr("indiana")) {
    return "in";
  }
  if(state==QObject::tr("iowa")) {
    return "ia";
  }
  if(state==QObject::tr("kansas")) {
    return "ks";
  }
  if(state==QObject::tr("kentucky")) {
    return "ky";
  }
  if(state==QObject::tr("louisiana")) {
    return "la";
  }
  if(state==QObject::tr("maine")) {
    return "me";
  }
  if(state==QObject::tr("marshall islands")) {
    return "mh";
  }
  if(state==QObject::tr("maryland")) {
    return "md";
  }
  if(state==QObject::tr("massachusetts")) {
    return "ma";
  }
  if(state==QObject::tr("michigan")) {
    return "mi";
  }
  if(state==QObject::tr("minnesota")) {
    return "mn";
  }
  if(state==QObject::tr("mississippi")) {
    return "ms";
  }
  if(state==QObject::tr("missouri")) {
    return "mo";
  }
  if(state==QObject::tr("montana")) {
    return "mt";
  }
  if(state==QObject::tr("nebraska")) {
    return "ne";
  }
  if(state==QObject::tr("nevada")) {
    return "nv";
  }
  if(state==QObject::tr("new hampshire")) {
    return "nh";
  }
  if(state==QObject::tr("new jersey")) {
    return "nj";
  }
  if(state==QObject::tr("new mexico")) {
    return "nm";
  }
  if(state==QObject::tr("new york")) {
    return "ny";
  }
  if(state==QObject::tr("north carolina")) {
    return "nc";
  }
  if(state==QObject::tr("north dakota")) {
    return "nd";
  }
  if(state==QObject::tr("northern mariana islands")) {
    return "mp";
  }
  if(state==QObject::tr("ohio")) {
    return "oh";
  }
  if(state==QObject::tr("oklahoma")) {
    return "ok";
  }
  if(state==QObject::tr("oregon")) {
    return "or";
  }
  if(state==QObject::tr("palau")) {
    return "pw";
  }
  if(state==QObject::tr("pennsylvania")) {
    return "pa";
  }
  if(state==QObject::tr("puerto rico")) {
    return "pr";
  }
  if(state==QObject::tr("rhode island")) {
    return "ri";
  }
  if(state==QObject::tr("south carolina")) {
    return "sc";
  }
  if(state==QObject::tr("south dakota")) {
    return "sd";
  }
  if(state==QObject::tr("tennessee")) {
    return "tn";
  }
  if(state==QObject::tr("texas")) {
    return "tx";
  }
  if(state==QObject::tr("utah")) {
    return "ut";
  }
  if(state==QObject::tr("vermont")) {
    return "vt";
  }
  if(state==QObject::tr("virgin islands")) {
    return "vi";
  }
  if(state==QObject::tr("virginia")) {
    return "va";
  }
  if(state==QObject::tr("washington")) {
    return "wa";
  }
  if(state==QObject::tr("west virginia")) {
    return "wv";
  }
  if(state==QObject::tr("wisconsin")) {
    return "wi";
  }
  if(state==QObject::tr("wyoming")) {
    return "wy";
  }
  return "";
}


QString AbbreviationToState(const QString &str)
{
  QString state=str.lower();
  if(state=="aa") {
    return QObject::tr("ALL");
  }
  if(state=="al") {
    return QObject::tr("Alabama");
  }
  if(state=="ak") {
    return QObject::tr("Alaska");
  }
  if(state=="as") {
    return QObject::tr("American Samoa");
  }
  if(state=="az") {
    return QObject::tr("Arizona");
  }
  if(state=="ar") {
    return QObject::tr("Arkansas");
  }
  if(state=="ca") {
    return QObject::tr("California");
  }
  if(state=="co") {
    return QObject::tr("Colorado");
  }
  if(state=="ct") {
    return QObject::tr("Connecticut");
  }
  if(state=="de") {
    return QObject::tr("Delaware");
  }
  if(state=="dc") {
    return QObject::tr("District of Columbia");
  }
  if(state=="fm") {
    return QObject::tr("Federated States of Micronesia");
  }
  if(state=="fl") {
    return QObject::tr("Florida");
  }
  if(state=="ga") {
    return QObject::tr("Georgia");
  }
  if(state=="gu") {
    return QObject::tr("Guam");
  }
  if(state=="hi") {
    return QObject::tr("Hawaii");
  }
  if(state=="id") {
    return QObject::tr("Idaho");
  }
  if(state=="il") {
    return QObject::tr("Illinois");
  }
  if(state=="in") {
    return QObject::tr("Indiana");
  }
  if(state=="ia") {
    return QObject::tr("Iowa");
  }
  if(state=="ks") {
    return QObject::tr("Kansas");
  }
  if(state=="ky") {
    return QObject::tr("Kentucky");
  }
  if(state=="la") {
    return QObject::tr("Louisiana");
  }
  if(state=="me") {
    return QObject::tr("Maine");
  }
  if(state=="mh") {
    return QObject::tr("Marshall Islands");
  }
  if(state=="md") {
    return QObject::tr("Maryland");
  }
  if(state=="ma") {
    return QObject::tr("Massachusetts");
  }
  if(state=="mi") {
    return QObject::tr("Michigan");
  }
  if(state=="mn") {
    return QObject::tr("Minnesota");
  }
  if(state=="ms") {
    return QObject::tr("Mississippi");
  }
  if(state=="mo") {
    return QObject::tr("Missouri");
  }
  if(state=="mt") {
    return QObject::tr("Montana");
  }
  if(state=="ne") {
    return QObject::tr("Nebraska");
  }
  if(state=="nv") {
    return QObject::tr("Nevada");
  }
  if(state=="nh") {
    return QObject::tr("New Hampshire");
  }
  if(state=="nj") {
    return QObject::tr("New Jersey");
  }
  if(state=="nm") {
    return QObject::tr("New Mexico");
  }
  if(state=="ny") {
    return QObject::tr("New York");
  }
  if(state=="nc") {
    return QObject::tr("North Carolina");
  }
  if(state=="nd") {
    return QObject::tr("North Dakota");
  }
  if(state=="mp") {
    return QObject::tr("Northern Mariana Islands");
  }
  if(state=="oh") {
    return QObject::tr("Ohio");
  }
  if(state=="ok") {
    return QObject::tr("Oklahoma");
  }
  if(state=="or") {
    return QObject::tr("Oregon");
  }
  if(state=="pw") {
    return QObject::tr("Palau");
  }
  if(state=="pa") {
    return QObject::tr("Pennsylvania");
  }
  if(state=="pr") {
    return QObject::tr("Puerto Rico");
  }
  if(state=="ri") {
    return QObject::tr("Rhode Island");
  }
  if(state=="sc") {
    return QObject::tr("South Carolina");
  }
  if(state=="sd") {
    return QObject::tr("South Dakota");
  }
  if(state=="tn") {
    return QObject::tr("Tennessee");
  }
  if(state=="tx") {
    return QObject::tr("Texas");
  }
  if(state=="ut") {
    return QObject::tr("Utah");
  }
  if(state=="vt") {
    return QObject::tr("Vermont");
  }
  if(state=="vi") {
    return QObject::tr("Virgin Islands");
  }
  if(state=="va") {
    return QObject::tr("Virginia");
  }
  if(state=="wa") {
    return QObject::tr("Washington");
  }
  if(state=="wv") {
    return QObject::tr("West Virginia");
  }
  if(state=="wi") {
    return QObject::tr("Wisconsin");
  }
  if(state=="wy") {
    return QObject::tr("Wyoming");
  }
  return "";
}
