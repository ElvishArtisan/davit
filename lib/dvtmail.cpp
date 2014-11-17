// dvtmail.cpp
//
// Routines for processing e-mail messages on POSIX architectures.
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: dvtmail.cpp,v 1.10 2011/03/22 15:12:53 pcvs Exp $
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

#include <fstream>
#include <iostream>

#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>

#include <qmessagebox.h>
#include <qsqldatabase.h>

#include <dvt.h>
#include <dvtconf.h>
#include <dvtweb.h>
#include <dvtmail.h>

void __MailError(QString msg,QWidget *parent)
{
  if(parent==NULL) {
    syslog(LOG_ERR,msg);
  }
  else {
    QMessageBox::warning(parent,"Davit - Davit Error",msg);
  }
}


bool DvtSendMail(const QStringList &to_addrs,const QStringList &cc_addrs,
		 const QStringList &bcc_addrs,const QString &from_addr,
		 const QString &reply_addr,const QString &subj,
		 const QString &msg,QWidget *parent,
		 vmime::ref<vmime::net::transport> batch_transport)
{
  QString sql;
  QSqlQuery *q;
  bool ret=true;

  try {
    //
    // Generate Message
    //
    vmime::messageBuilder mb;
    
    mb.setExpeditor(vmime::mailbox(from_addr));
    for(unsigned i=0;i<to_addrs.size();i++) {
      mb.getRecipients().
	appendAddress(vmime::create<vmime::mailbox>(to_addrs[i]));
    }
    for(unsigned i=0;i<cc_addrs.size();i++) {
      mb.getCopyRecipients().
	appendAddress(vmime::create<vmime::mailbox>(cc_addrs[i]));
    }
    for(unsigned i=0;i<bcc_addrs.size();i++) {
      mb.getBlindCopyRecipients().
	appendAddress(vmime::create<vmime::mailbox>(bcc_addrs[i]));
    }
    mb.setSubject(vmime::text(subj));
    mb.getTextPart()->setText(vmime::create<vmime::stringContentHandler>(msg));
    vmime::ref<vmime::message> vmsg=mb.construct();
    vmime::utility::outputStreamAdapter out(std::cout);
    vmime::headerFieldFactory *factory=
      vmime::headerFieldFactory::getInstance();

    vmime::ref<vmime::headerField> replyto=
      factory->create(vmime::fields::REPLY_TO);
    replyto->setValue(vmime::mailbox(reply_addr));
    vmsg->getHeader()->appendField(replyto);
    vmsg->getHeader()->
      getField("X-Davit-Message")->setValue(vmime::text("yes"));
    
    //
    // Send Message
    //
    // vmsg->generate(out);
    if(batch_transport==NULL) {
      sql="select SMTP_HOSTNAME,SMTP_PORT from SYSTEM";
      q=new QSqlQuery(sql);
      if(!q->first()) {
	__MailError("Unable to load SMTP server configuration!",parent);
	delete q;
	return false;
      }
      if(q->value(0).toString().isEmpty()) {
	__MailError("Missing SMTP server configuration!",parent);
	delete q;
	return false;
      }
      vmime::ref<vmime::net::session> sess=
	vmime::create<vmime::net::session>();
      sess->getProperties()["transport.smtp.server.port"]=q->value(1).toInt();
      vmime::utility::url url((const char *)QString().sprintf("smtp://%s",
				(const char *)q->value(0).toString()));
      vmime::ref<vmime::net::transport> trans=sess->getTransport(url); 
      trans->connect();
      trans->send(vmsg);
      trans->disconnect();
      delete q;
    }
    else {
      batch_transport->send(vmsg);
    }
  }
  catch(vmime::exception &e) {
    if(batch_transport==NULL) {
      __MailError(e.what(),parent);
    }
    ret=false;
  }
  return ret;
}


bool DvtSendAlert(const QString &subj,const QString &msg)
{
  QString sql;
  QSqlQuery *q;
  bool ret=false;
  QStringList dummy_addr;


  //
  // Get Addresses
  //
  dummy_addr.clear();
  sql="select ORIGIN_EMAIL,ALERT_EMAIL from SYSTEM";
  q=new QSqlQuery(sql);
  if(q->first()) {
    ret=DvtSendMail(QStringList(q->value(1).toString()),dummy_addr,dummy_addr,
		    q->value(0).toString(),q->value(0).toString(),subj,msg);
  }
  delete q;

  return ret;
}
