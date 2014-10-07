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
			   tr("This will import an external data set generated from\nPrecisionTrak using the \"Davit-v3\" template.\n\nProceed?"),
			   QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
    return;
  }

  //
  // Open Source File
  //
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
  QString line=GetNextLine(f);
  cols=cols.split(",",line);
  if(cols.size()!=31) {
    QMessageBox::warning(this,tr("Davit - File Error"),
	tr("Source file is unrecognized (incorrect number of data columns)."));
    fclose(f);
    return;
  }
  for(unsigned i=0;i<cols.size();i++) {
    cols[i].replace("\"","");
  }
  QStringList errs;
  VerifyPrecisionTrakRecord(cols[0],"Station (ie. WABC-AM  WCBS-FM)",errs);
  VerifyPrecisionTrakRecord(cols[1]," Freq",errs);
  VerifyPrecisionTrakRecord(cols[2]," Call Letters (ie. WABC   WCBS)",errs);
  VerifyPrecisionTrakRecord(cols[3]," City of License",errs);
  VerifyPrecisionTrakRecord(cols[4]," State of License",errs);
  VerifyPrecisionTrakRecord(cols[5]," Phone",errs);

  VerifyPrecisionTrakRecord(cols[6]," Fax",errs);
     //     (cols[7]!=" Email")||
  VerifyPrecisionTrakRecord(cols[7]," Website",errs);
  VerifyPrecisionTrakRecord(cols[8]," Home Nielsen Audio MSA",errs);
  VerifyPrecisionTrakRecord(cols[9]," Home Nielsen Audio Rank",errs);
  VerifyPrecisionTrakRecord(cols[10]," Nielsen DMA",errs);
  VerifyPrecisionTrakRecord(cols[11]," Nielsen DMA Rank",errs);
  VerifyPrecisionTrakRecord(cols[12]," Country",errs);
  VerifyPrecisionTrakRecord(cols[13]," Primary Format",errs);
  VerifyPrecisionTrakRecord(cols[14]," Licensee",errs);
  VerifyPrecisionTrakRecord(cols[15]," Mailing Address",errs);
  VerifyPrecisionTrakRecord(cols[16]," Mailing Address2",errs);
  VerifyPrecisionTrakRecord(cols[17]," Mailing City",errs);
  VerifyPrecisionTrakRecord(cols[18]," Mailing State",errs);
  VerifyPrecisionTrakRecord(cols[19]," Mailing Zip",errs);
  VerifyPrecisionTrakRecord(cols[20]," Genl Mgr",errs);
  VerifyPrecisionTrakRecord(cols[21]," Genl Mgr Title",errs);
  VerifyPrecisionTrakRecord(cols[22]," Genl Mgr Email",errs);
  VerifyPrecisionTrakRecord(cols[23]," Prog Dir",errs);
  VerifyPrecisionTrakRecord(cols[24]," Prog Dir Title",errs);
  VerifyPrecisionTrakRecord(cols[25]," Prog Dir Email",errs);
  VerifyPrecisionTrakRecord(cols[26]," Traffic Mgr",errs);
  VerifyPrecisionTrakRecord(cols[27]," Traffic Mgr Email",errs);
  VerifyPrecisionTrakRecord(cols[28]," Traffic Phone",errs);
     //     (cols[30]!=" Traffic Fax")||
  VerifyPrecisionTrakRecord(cols[29]," Day/Power",errs);
  VerifyPrecisionTrakRecord(cols[30]," Nite/Power",errs);

  if(errs.size()>0) {
    QString msg=tr("Source file is unrecognized (unexpected names in column header):");
    for(unsigned i=0;i<errs.size();i++) {
      msg+="\n"+errs[i];
    }
    QMessageBox::warning(this,tr("Davit - File Error"),msg);
    fclose(f);
    return;
  }

  //
  // Import Rows
  //
  int lineno=2;
  bool ok=false;
  line=GetNextLine(f);

  while(!line.isEmpty()) {
    cols=DvtGetCommaList(line,true);
    if(cols.size()==32) {
      cols[2].toInt(&ok);  // Ignore CPs
      if(!ok) {
	rows.push_back(cols);
      }
    }
    else {
      fprintf(stderr,"line %d [%s] malformatted, skipping\n",
	      lineno,(const char *)cols[2]);
    }

    lineno++;
    line=GetNextLine(f);
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
    ProcessMarketRecord("MSA_MARKETS",rows[i][8],rows[i][9].toInt());
    ProcessMarketRecord("DMA_MARKETS",rows[i][10],rows[i][11].toInt());
  }

  //
  // Pass 2: Import Affiliate Data
  //
  for(unsigned i=0;i<rows.size();i++) {
    if((i%100)==0) {
      printf("row: %u\n",i);
    }
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
  msa_rank=GetMarketRank("MSA_MARKETS",cols[8],cols[9].toInt());
  dma_rank=GetMarketRank("DMA_MARKETS",cols[10],cols[11].toInt());

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
  sql=QString("update AFFILIATES set ")+
    "STATION_TYPE=\""+type+"\","+
    QString().sprintf("STATION_FREQUENCY=%6.1lf,",cols[1].toDouble())+
    "STATION_CALL=\""+DvtEscapeString(cols[2])+"\","+
    "LICENSE_CITY=\""+DvtEscapeString(cols[3])+"\","+
    "LICENSE_STATE=\""+DvtEscapeString(cols[4])+"\","+
    "PHONE=\""+DvtEscapeString(cols[5])+"\","+
    "FAX=\""+DvtEscapeString(cols[6])+"\","+
    "WEB_URL=\""+DvtEscapeString(cols[7])+"\","+
    "MARKET_NAME=\""+DvtEscapeString(cols[8])+"\","+
    QString().sprintf("MARKET_RANK=%d,",msa_rank)+
    "DMA_NAME=\""+DvtEscapeString(cols[10])+"\","+
    QString().sprintf("DMA_RANK=%d,",dma_rank)+
    "COUNTRY=\""+DvtEscapeString(cols[12])+"\","+
    "STATION_FORMAT=\""+DvtEscapeString(cols[13])+"\","+
    "BUSINESS_NAME=\""+DvtEscapeString(cols[14])+"\","+
    "ADDRESS1=\""+DvtEscapeString(cols[15])+"\","+
    "ADDRESS2=\""+DvtEscapeString(cols[16])+"\","+
    "CITY=\""+DvtEscapeString(cols[17])+"\","+
    "STATE=\""+DvtEscapeString(cols[18])+"\","+
    "ZIPCODE=\""+DvtEscapeString(cols[19])+"\","+
    QString().sprintf("STATION_POWER=%d,",cols[29].toInt())+
    QString().sprintf("STATION_NIGHT_POWER=%d ",cols[30].toInt())+
    QString().sprintf("where ID=%d",affiliate_id);
  q=new QSqlQuery(sql);
  delete q;

  //
  // Update Contacts
  //
  QString phone;
  QString fax;
  QString general_manager="N";
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
    general_manager="N";
    title=cols[20+i*3+1];
    switch(i) {
    case 0:
      general_manager="Y";
      break;

    case 1:
      program_director="Y";
      break;

    case 2:
      phone=cols[29];
      fax=cols[30];
      title="Traffic Director";
      break;
    }
    sql=QString("insert into CONTACTS set ")+
      QString().sprintf("AFFILIATE_ID=%d,",affiliate_id)+
      "NAME=\""+DvtEscapeString(cols[20+i*3])+"\","+
      "TITLE=\""+DvtEscapeString(title)+"\","+
      "EMAIL=\""+DvtEscapeString(cols[20+i*3+2])+"\","+
      "PHONE=\""+DvtNormalizePhoneNumber(phone)+"\","+
      "FAX=\""+DvtNormalizePhoneNumber(fax)+"\","+
      "GENERAL_MANAGER=\""+DvtEscapeString(general_manager)+"\","+
      "PROGRAM_DIRECTOR=\""+DvtEscapeString(program_director)+"\"";
    q=new QSqlQuery(sql);
    delete q;
  }
}


void MainWidget::VerifyPrecisionTrakRecord(const QString &str1,const QString &str2,
					   QStringList &errs)
{
  if(str1!=str2) {
    errs.push_back(str1);
  }
}


QString MainWidget::GetNextLine(FILE *f)
{
  QString ret;
  int c;
  int istate=0;

  while(1==1) {
    c=fgetc(f);
    if(c==EOF) {
      return ret;
    }
    switch(istate) {
    case 0:
      if(c==13) {
	istate=1;
      }
      else {
	if(iscntrl(c)!=0) {
	  c=' ';
	}
	ret+=c;
      }
      break;

    case 1:
      if(c==10) {
	return ret;
      }
      ret+=c;
      istate=0;
      break;
    }
  }
  return QString();
}
