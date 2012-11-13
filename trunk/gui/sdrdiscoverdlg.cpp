/////////////////////////////////////////////////////////////////////
// sdrdiscoverdlg.cpp: implementation of the CSdrDiscoverDlg class.
//
//	This class implements a discover widget to find RFSPACE SDR's
//connected to a network using a simple UDP broadcast method.
//
// History:
//	2010-09-15  Initial creation MSW
//	2011-03-27  Initial release
//	2011-05-26  Added support for In Use Status
//	2012-02-11  Qt 4.8 finally fixed UDP socket close rebind bug
/////////////////////////////////////////////////////////////////////

//==========================================================================================
// + + +   This Software is released under the "Simplified BSD License"  + + +
//Copyright 2010 Moe Wheatley. All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification, are
//permitted provided that the following conditions are met:
//
//   1. Redistributions of source code must retain the above copyright notice, this list of
//	  conditions and the following disclaimer.
//
//   2. Redistributions in binary form must reproduce the above copyright notice, this list
//	  of conditions and the following disclaimer in the documentation and/or other materials
//	  provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY Moe Wheatley ``AS IS'' AND ANY EXPRESS OR IMPLIED
//WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
//FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Moe Wheatley OR
//CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
//ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//The views and conclusions contained in the software and documentation are those of the
//authors and should not be interpreted as representing official policies, either expressed
//or implied, of Moe Wheatley.
//==========================================================================================
#include "gui/sdrdiscoverdlg.h"
#include <QDebug>

/*---------------------------------------------------------------------------*/
/*--------------------> L O C A L   D E F I N E S <--------------------------*/
/*---------------------------------------------------------------------------*/
/* UDP port numbers for Discover */
#define	DISCOVER_SERVER_PORT 48321	/* PC client Tx port, SDR Server Rx Port */
#define DISCOVER_CLIENT_PORT 48322	/* PC client Rx port, SDR Server Tx Port */

#define KEY0 0x5A
#define KEY1 0xA5
#define MSG_REQ 0
#define MSG_RESP 1
#define MSG_SET 2

//////////////////////////////////////////////////////////////////////////////
//Constructor/Destructor
//////////////////////////////////////////////////////////////////////////////
CSdrDiscoverDlg::CSdrDiscoverDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSdrDiscoverDlg)
{
    ui->setupUi(this);
	ui->listWidget->clear();
    m_Name = "";
	m_NameFilter = "";
	m_pUdpDiscoverSocket = new QUdpSocket(this);
	connect(m_pUdpDiscoverSocket, SIGNAL(readyRead()), this, SLOT(ReadUDPMessages()));
	m_UdpOpen = false;
}

CSdrDiscoverDlg::~CSdrDiscoverDlg()
{
	if(m_pUdpDiscoverSocket)
		delete m_pUdpDiscoverSocket;
    delete ui;
}

//Fill in initial data
void CSdrDiscoverDlg::InitDlg()
{
	OnFind();
}

//////////////////////////////////////////////////////////////////////////////
//Called when Find button pressed
//////////////////////////////////////////////////////////////////////////////
void CSdrDiscoverDlg::OnFind()
{
	ui->listWidget->clear();	//clear screen then delay sending to create visible clear
	m_Timer.singleShot(250, this, SLOT( SendDiscoverRequest()) );
}

void CSdrDiscoverDlg::CloseUdp()
{
	if(m_UdpOpen)
	{
		m_pUdpDiscoverSocket->close();
		m_UdpOpen = false;
qDebug("UDP Close");
	}
}

void CSdrDiscoverDlg::SendDiscoverRequest()
{
tDiscover_COMMONMSG reqmsg;
	//bind UDP socket with receive port value
	if(!m_UdpOpen)
		m_pUdpDiscoverSocket->bind(DISCOVER_CLIENT_PORT);
	m_UdpOpen = true;
	qint64 length = sizeof(tDiscover_COMMONMSG);
	memset((void*)&reqmsg, 0, length);
	reqmsg.length[0] = length&0xFF;
	reqmsg.length[1] = (length>>8)&0xFF;
	reqmsg.key[0] = KEY0;
	reqmsg.key[1] = KEY1;
	reqmsg.op = MSG_REQ;
	for(int i=0; i<m_NameFilter.length(); i++)
		reqmsg.name[i] = m_NameFilter[i].toAscii();
	m_pUdpDiscoverSocket->writeDatagram( (const char*)&reqmsg, length, QHostAddress::Broadcast, DISCOVER_SERVER_PORT);
	m_CloseTimer.singleShot(250, this, SLOT( CloseUdp()) );
}

//Called when UDP messages are received on the client port for parsing
void CSdrDiscoverDlg::ReadUDPMessages()
{
qint64 totallength;
qint64 length;
int index=0;
bool InUse;
tDiscover_COMMONMSG tmpmsg;
char Buf[2048];	//buffer to hold received UDP packet
	while( m_pUdpDiscoverSocket->hasPendingDatagrams() )
	{	//loop and get all UDP packets availaable
		totallength = m_pUdpDiscoverSocket->pendingDatagramSize();
		m_pUdpDiscoverSocket->readDatagram( Buf, totallength);	//read entire UDP packet

		//see if msg cam from same device that is already in the list
		//if so then delete old one so new one will get added
		index = ui->listWidget->count();	//get index to next free position in list
		length = sizeof(tDiscover_COMMONMSG);
		memcpy((void*)&tmpmsg, (void*)Buf, length );	//get tmp copy of new message
		for(int i=0; i<index; i++)
		{
			if( 0==strcmp(m_DiscovermsgCommon[i].sn, tmpmsg.sn) )
			{
				ui->listWidget->takeItem(i);	//remove old duplicate
				break;
			}
		}
		index = ui->listWidget->count();	//get index to next free position in list

		//copy just the common fixed size block into m_DiscovermsgCommon
		memcpy((void*)&m_DiscovermsgCommon[index], (void*)Buf, length );
		if( (KEY0 == m_DiscovermsgCommon[index].key[0]) && (KEY1 == m_DiscovermsgCommon[index].key[1]) && (index<MAX_DEVICES) )
		{
			InUse = FALSE;
			if( (QString(m_DiscovermsgCommon[index].name ) == "SDR-IP" ) ||
				(QString(m_DiscovermsgCommon[index].name ) == "NetSDR" ) )
			{	//get all information from SDR-IP and NetSDR
				memcpy((void*)&m_DiscovermsgNetSDR[index], (void*)Buf, sizeof(tDiscover_NETSDR) );
				if(m_DiscovermsgNetSDR[index].status & (STATUS_BIT_CONNECTED|STATUS_BIT_RUNNING))
					InUse = TRUE;
			}
			else	//get info for SDR-IQ and SDR-14
			{
				memcpy((void*)&m_DiscovermsgSDRxx[index], (void*)Buf,  sizeof(tDiscover_NETSDR) );
				if(m_DiscovermsgSDRxx[index].status & (STATUS_BIT_CONNECTED|STATUS_BIT_RUNNING))
					InUse = TRUE;
			}

			quint16 tmp16 = m_DiscovermsgCommon[index].port[1]; tmp16 <<= 8; tmp16 |= m_DiscovermsgCommon[index].port[0];
			QString str = QString("%1   SN=%2   IP=%3.%4.%5.%6   Port=%7")
					.arg(m_DiscovermsgCommon[index].name)
					.arg(m_DiscovermsgCommon[index].sn)
					.arg(m_DiscovermsgCommon[index].ipaddr[3])
					.arg(m_DiscovermsgCommon[index].ipaddr[2])
					.arg(m_DiscovermsgCommon[index].ipaddr[1])
					.arg(m_DiscovermsgCommon[index].ipaddr[0])
					.arg(tmp16);
			if(InUse)
				str.prepend("(In Use) ");
			ui->listWidget->addItem(str);	//place formated information in listbox
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//Called when someone double clicks on a listbox entry for viewing/editing
//////////////////////////////////////////////////////////////////////////////
void CSdrDiscoverDlg::OnItemDoubleClick( QListWidgetItem * item )
{
	Q_UNUSED(item);
	accept();
}

void CSdrDiscoverDlg::accept()
{	//OK was pressed so get all data from edit controls
int index = ui->listWidget->currentRow();
	if(index>=0)
	{
		m_Name = m_DiscovermsgCommon[index].name;
		m_Port = m_DiscovermsgCommon[index].port[1];
		m_Port <<= 8;
		m_Port |= m_DiscovermsgCommon[index].port[0];
		m_IPAdr = m_DiscovermsgCommon[index].ipaddr[3];
		m_IPAdr<<=8;
		m_IPAdr += m_DiscovermsgCommon[index].ipaddr[2];
		m_IPAdr<<=8;
		m_IPAdr += m_DiscovermsgCommon[index].ipaddr[1];
		m_IPAdr<<=8;
		m_IPAdr += m_DiscovermsgCommon[index].ipaddr[0];
	}
	 else
	{
		 m_Name = "";
	}
	QDialog::accept();		//call base class to exit
}
