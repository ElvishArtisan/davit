// precisiontrak.cpp
//
// PrecisionTrak import routines for Davit
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
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

#include <vector>

#include <qmessagebox.h>
#include <qfiledialog.h>

#include <dvtimport.h>
#include <dvtconf.h>

#include <davit.h>

void MainWidget::importExternalData()
{
  QString sql;
  QSqlQuery *q;

  if(QMessageBox::question(this,tr("Davit - External Data Import"),
			   tr("This will import an external data set generated from\nPrecisionTrak using the \"Davit-v2\" template.\n\nProceed?"),
			   QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
    return;
  }

  //
  // Open Source File
  //
  char buffer[1024];
  QString filename=
    QFileDialog::getOpenFileName("","CSV Files (*.csv)",this);
  if(filename.isNull()) {
    return;
  }
  FILE *f=fopen(filename,"r");
  if(f==NULL) {
    QMessageBox::warning(this,tr("Davit - File Error"),
			 tr("Unable to open file!"));
    return;
  }

  //
  // Verify Column Layout
  //
  std::vector<QStringList> rows;
  QStringList cols;
  if(fgets(buffer,1024,f)==NULL) {
    QMessageBox::warning(this,tr("Davit - File Error"),
			 tr("Source file contains no data!"));
    fclose(f);
    return;
  }
  buffer[strlen(buffer)-2]=0;  // Remove LF
  cols=cols.split(",",buffer);
  if(cols.size()!=33) {
    QMessageBox::warning(this,tr("Davit - File Error"),
	tr("Source file is unrecognized (incorrect number of data columns)."));
    fclose(f);
    return;
  }
  for(unsigned i=0;i<cols.size();i++) {
    cols[i].replace("\"","");
  }
  if((cols[0]!="Radio - Station")||
     (cols[1]!=" Freq")||
     (cols[2]!=" Call Letters")||
     (cols[3]!=" City of License")||
     (cols[4]!=" State of License")||
     (cols[5]!=" Phone")||
     (cols[6]!=" Fax")||
     (cols[7]!=" Email")||
     (cols[8]!=" Website")||
     (cols[9]!=" Home Arb MSA Rank")||
     (cols[10]!=" Geo Home Market")||
     (cols[11]!=" Nielsen DMA")||
     (cols[12]!=" Nielsen DMA Rank")||
     (cols[13]!=" Country")||
     (cols[14]!=" Primary Format")||
     (cols[15]!=" Licensee")||
     (cols[16]!=" Mailing Address")||
     (cols[17]!=" Mailing Address2")||
     (cols[18]!=" Mailing City")||
     (cols[19]!=" Mailing State")||
     (cols[20]!=" Mailing Zip")||
     (cols[21]!=" Genl Mgr")||
     (cols[22]!=" Genl Mgr Title")||
     (cols[23]!=" Genl Mgr Email")||
     (cols[24]!=" Prog Dir")||
     (cols[25]!=" Prog Dir Title")||
     (cols[26]!=" Prog Dir Email")||
     (cols[27]!=" Traffic Mgr")||
     (cols[28]!=" Traffic Mgr Email")||
     (cols[29]!=" Traffic Phone")||
     (cols[30]!=" Traffic Fax")||
     (cols[31]!=" Day/Power")||
     (cols[32]!=" Nite/Power")) {
    QMessageBox::warning(this,tr("Davit - File Error"),
       tr("Source file is unrecognized (unexpected names in column header)."));
    fclose(f);
    return;
  }

  //
  // Import Rows
  //
  int lineno=2;
  bool ok=false;
  while(fgets(buffer,1024,f)!=NULL) {
    buffer[strlen(buffer)-2]=0;  // Remove LF
    cols=DvtGetCommaList(buffer,true);
    if(cols.size()==33) {
      cols[2].toInt(&ok);  // Ignore CPs
      if(!ok) {
	rows.push_back(cols);
	//ImportPrecisionTrakRecord(cols);
      }
    }
    else {
      fprintf(stderr,"line %d [%s] malformatted, skipping\n",
	      lineno,(const char *)cols[2]);
    }

    lineno++;
  }

  fclose(f);

  //
  // Pass 1: Build Market Tables
  //
  sql="delete from MSA_MARKETS";
  q=new QSqlQuery(sql);
  delete q;
  sql="delete from DMA_MARKETS";
  q=new QSqlQuery(sql);
  delete q;
  for(unsigned i=0;i<rows.size();i++) {
    ProcessMarketRecord("MSA_MARKETS",rows[i][10],rows[i][9].toInt());
    ProcessMarketRecord("DMA_MARKETS",rows[i][11],rows[i][12].toInt());
  }

  //
  // Pass 2: Import Affiliate Data
  //
  for(unsigned i=0;i<rows.size();i++) {
    ImportPrecisionTrakRecord(rows[i]);
  }
}


void MainWidget::ProcessMarketRecord(const QString &table,QString &name,
				     int rank)
{
  QString sql;
  QSqlQuery *q;

  if(rank<=0) {
    return;
  }

  sql=QString().sprintf("select ID from %s where NAME=\"%s\"",
			(const char *)table,
			(const char *)DvtEscapeString(name.stripWhiteSpace()));
  q=new QSqlQuery(sql);
  if(!q->first()) {
    delete q;
    sql=QString().sprintf("insert into %s set NAME=\"%s\",RANK=%d",
			  (const char *)table,
			  (const char *)name.stripWhiteSpace(),
			  rank);
    q=new QSqlQuery(sql);
  }
  delete q;
}


int MainWidget::GetMarketRank(const QString &table,const QString &name,
			      int rank)
{
  QString sql;
  QSqlQuery *q;

  if(rank>0) {
    return rank;
  }
  sql=QString().sprintf("select RANK from %s where NAME=\"%s\"",
			(const char *)DvtEscapeString(table),
			(const char *)DvtEscapeString(name.stripWhiteSpace()));
  q=new QSqlQuery(sql);
  if(q->first()) {
    rank=q->value(0).toInt();
  }
  delete q;
  return rank;
}


void MainWidget::ImportPrecisionTrakRecord(const QStringList &cols)
{
  QString msg;
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
  int affiliate_id=-1;
  QString type;
  int msa_rank;
  int dma_rank;

  //
  // Get Station Type
  //
  QStringList fields=fields.split("-",cols[0]);
  if(fields.size()==2) {
    type=fields[1].left(1);
  }

  //
  // Get Market Data
  //
  msa_rank=GetMarketRank("MSA_MARKETS",cols[10],cols[9].toInt());
  dma_rank=GetMarketRank("DMA_MARKETS",cols[11],cols[12].toInt());

  //
  // Find (or Create) Matching Affiliate Record
  //
  sql=QString().sprintf("select ID from AFFILIATES where \
                         (STATION_FREQUENCY=%6.1f)&&\
                         (LICENSE_CITY=\"%s\")&&\
                         (LICENSE_STATE=\"%s\")",
			cols[1].toDouble(),
			(const char *)DvtEscapeString(cols[3]),
			(const char *)DvtEscapeString(cols[4]));
  q=new QSqlQuery(sql);
  if(q->first()) {
    affiliate_id=q->value(0).toInt();
  }
  else {
    msg=tr("The following record:\n      ")+cols[2]+", "+cols[1]+" - "+
      cols[3]+", "+cols[4]+"\n"+tr("Couldn't be matched exactly.\n\n");

    // Try to find a near fit
    sql=QString().sprintf("select ID,STATION_CALL,STATION_FREQUENCY,\
                           LICENSE_CITY,LICENSE_STATE from AFFILIATES \
                           where (STATION_CALL=\"%s\")&&(STATION_TYPE=\"%s\")",
			  (const char *)cols[2],(const char *)type);
    q1=new QSqlQuery(sql);
    if(q1->first()) {
      /*
      msg+=tr("Apply it to this record?\n     ")+q1->value(1).toString()+
	", "+QString().sprintf("%6.1lf",q1->value(2).toDouble())+" - "+
	q1->value(3).toString()+", "+q1->value(4).toString();
      if(QMessageBox::question(this,tr("Davit - External Data Import"),
			       msg,QMessageBox::Yes,QMessageBox::No)==
	 QMessageBox::Yes) {
      */

      if(1==1) {
	affiliate_id=q1->value(0).toInt();
      }
      else {
	if(QMessageBox::question(this,tr("Davit - External Data Import"),
		tr("Create a new affiliate record for this entry?"),
		      QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes) {
	  affiliate_id=DvtCreateNewAffiliateRecord();
	}
      }
    }
    else {
      /*
      msg+=tr("Create a new affiliate record for this entry?");
      if(QMessageBox::question(this,tr("Davit - External Data Import"),msg,
		    QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes) {
      */
      if(1==1) {
	affiliate_id=DvtCreateNewAffiliateRecord();
      }
    }
    delete q1;
  }
  delete q;
  if(affiliate_id<0) {
    return;
  }

  //
  // Update Affiliate Record
  //
  sql=QString().sprintf("update AFFILIATES set \
                         STATION_TYPE=\"%s\",\
                         STATION_FREQUENCY=%6.1lf,\
                         STATION_CALL=\"%s\",\
                         LICENSE_CITY=\"%s\",\
                         LICENSE_STATE=\"%s\",\
                         PHONE=\"%s\",\
                         FAX=\"%s\",\
                         EMAIL_ADDR=\"%s\",\
                         WEB_URL=\"%s\",\
                         MARKET_NAME=\"%s\",\
                         MARKET_RANK=%d,\
                         DMA_NAME=\"%s\",\
                         DMA_RANK=%d,\
                         COUNTRY=\"%s\",\
                         STATION_FORMAT=\"%s\",\
                         BUSINESS_NAME=\"%s\",\
                         ADDRESS1=\"%s\",\
                         ADDRESS2=\"%s\",\
                         CITY=\"%s\",\
                         STATE=\"%s\",\
                         ZIPCODE=\"%s\",\
                         STATION_POWER=%d,\
                         STATION_NIGHT_POWER=%d \
                         where ID=%d",
			(const char *)type,
			cols[1].toDouble(),
			(const char *)cols[2],
			(const char *)cols[3],
			(const char *)cols[4],
			(const char *)cols[5],
			(const char *)cols[6],
			(const char *)cols[7],
			(const char *)cols[8],
			(const char *)cols[10],
			msa_rank,
			(const char *)cols[11],
			dma_rank,
			(const char *)cols[13],
			(const char *)cols[14],
			(const char *)cols[15],
			(const char *)cols[16],
			(const char *)cols[17],
			(const char *)cols[18],
			(const char *)cols[19],
			(const char *)cols[20],
			cols[31].toInt(),
			cols[32].toInt(),
			affiliate_id);
  q=new QSqlQuery(sql);
  delete q;

  //
  // Update Contacts
  //
  QString phone;
  QString fax;
  QString program_director="N";
  QString title;

  sql=QString().sprintf("delete from CONTACTS where \
                         (AFFILIATE_ID=%d)&&(LOCKED=\"N\")",affiliate_id);
  q=new QSqlQuery(sql);
  delete q;

  for(unsigned i=0;i<3;i++) {
    phone=cols[5];
    fax=cols[6];
    program_director="N";
    title=cols[21+i*3+1];
    switch(i) {
    case 1:
      program_director="Y";
      break;

    case 2:
      phone=cols[29];
      fax=cols[30];
      title="Traffic Director";
      break;
    }
    sql=QString().sprintf("insert into CONTACTS set \
                           AFFILIATE_ID=%d,\
                           NAME=\"%s\",\
                           TITLE=\"%s\",\
                           EMAIL=\"%s\",\
                           PHONE=\"%s\",\
                           FAX=\"%s\",\
                           PROGRAM_DIRECTOR=\"%s\"",
			  affiliate_id,
			  (const char *)DvtEscapeString(cols[21+i*3]),
			  (const char *)DvtEscapeString(title),
			  (const char *)DvtEscapeString(cols[21+i*3+2]),
			  (const char *)DvtNormalizePhoneNumber(phone),
			  (const char *)DvtNormalizePhoneNumber(fax),
			  (const char *)program_director);
    q=new QSqlQuery(sql);
    delete q;
  }
}
