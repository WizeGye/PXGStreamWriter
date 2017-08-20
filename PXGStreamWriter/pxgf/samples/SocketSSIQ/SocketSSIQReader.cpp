// ---------------------------------------------------------------------------
// FILE NAME            : SocketSSIQReader.cpp
// ---------------------------------------------------------------------------
// COMPANY              : PERALEX ELECTRONICS (PTY) LTD
// ---------------------------------------------------------------------------
// COPYRIGHT NOTICE :
//
// The copyright, manufacturing and patent rights stemming from this document
// in any form are vested in PERALEX ELECTRONICS (PTY) LTD.
//
// (c) PERALEX ELECTRONICS (PTY) LTD 2006
//
// PERALEX ELECTRONICS (PTY) LTD has ceded these rights to its clients
// where contractually agreed.
// ---------------------------------------------------------------------------
// DESCRIPTION :
//
// This console application connects to a PXGF SSIQ socket stream using the ip 
// address and port number specified on the command line, and prints out information
// about the received stream.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/samples/SocketSSIQ/SocketSSIQReader.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#include <iostream>
#include <math.h>
#include <time.h>

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "SocketSSIQReader.h"

#include "../../exceptions/PxgfException.h"

using namespace std;
using namespace pxgf;

void cSocketSSIQReader::callbackSIQP(const pxgf::cPackingSIQP& oPacking)
{
	m_bIsIQPacked = oPacking.getIsIQPacked();
}

void cSocketSSIQReader::callbackCF__(int64_t lCentreFrequency_microHz)
{
	m_uCenterFrequency_Hz = lCentreFrequency_microHz/1000000;
}

void cSocketSSIQReader::callbackBW__(int64_t lBandwidth_microHz)
{
	m_uBandwidth_Hz = lBandwidth_microHz/1000000;
}

void cSocketSSIQReader::callback_dBFS(float f_dBFullScale_dBm)
{
	m_fFullScale_dBm = f_dBFullScale_dBm;
}

void cSocketSSIQReader::callback_dBTG(float fTotalGain_dB)
{
	m_fTotalGain_dB = fTotalGain_dB;
}

void cSocketSSIQReader::callbackIQDC()
{
	cout << "Discontinuity detected" << endl;
}

void cSocketSSIQReader::callbackSR__(int64_t lSampleRate_microHz)
{
	m_uSampleRate_Hz = lSampleRate_microHz/1000000;
}

void cSocketSSIQReader::callbackSSIQ(int64_t lTimestamp_microSec, const std::vector<short>& vsIqData)
{
	unsigned uCurrentTick = GetTickCount();
	//check tickcount for overflow
	if (uCurrentTick < m_uLastTick)
		m_uLastTick = uCurrentTick;
	if ((uCurrentTick - m_uLastTick) > 500) //restrict printing info to every 1/2 a sec
	{
		m_uLastTick = uCurrentTick;

		time_t TimeStamp_sec = (unsigned)(lTimestamp_microSec/1000000);
		char *pchTimeString = ctime(&TimeStamp_sec);
		pchTimeString[24] = 0; //remove trailing \n
		cout	<< "(" << pchTimeString << " + " << (unsigned)(lTimestamp_microSec%1000000) << " us): \n\t"
				<< "CenterFrequency: " << m_uCenterFrequency_Hz << " Hz \n\t"
				<< "Bandwidth:       " << m_uBandwidth_Hz << " Hz \n\t"
				<< "Sample Rate:     " << m_uSampleRate_Hz << " Hz\n\t"
				<< "Signal level:    " << CalculateRMSPower(vsIqData, m_fFullScale_dBm, m_fTotalGain_dB) << "dBm (measured over " << vsIqData.size()/2 << " samples)"
				<< endl << endl;
	}
}

float cSocketSSIQReader::CalculateRMSPower(std::vector<short> vsIQData, int iFullScale_dBm, int iTotalGain_dB)
{
	float fAccum = 0;
	float fTemp;
	for (unsigned i = 0; i < vsIQData.size(); i++)
	{
		fTemp = ((float)vsIQData[i])/32768.0f; //Fullscale is +-MAX_SHORT = 2^15 = 32768
		fAccum += fTemp*fTemp;
	}
	fAccum /= (float)vsIQData.size();
	return 10.0f*log10(fAccum) + iFullScale_dBm - iTotalGain_dB; 
}

bool cSocketSSIQReader::Connect(const string &strAddress, unsigned short usPort)
{
	//first create socket	
	if ((m_hSocket = socket (AF_INET, SOCK_STREAM, 0 )) == INVALID_SOCKET) 
	{
        if (WSAGetLastError() == WSANOTINITIALISED) 
		{
            WSADATA wsaData;
            WORD wVersionRequested = MAKEWORD (1,1);
            if (WSAStartup(wVersionRequested, &wsaData) != 0)
				throw string("Failed to initialise WSA");
            if ((m_hSocket = socket (AF_INET, SOCK_STREAM, 0 )) == INVALID_SOCKET)
				throw string("Failed to create socket!");
        }
	}

	// disable TCP/IP delay
	int iTrue = 1;
	setsockopt (m_hSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&iTrue, sizeof(int));
	
	//Then connect
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons (usPort);
	if (isdigit (strAddress[0]))
		address.sin_addr.s_addr = inet_addr (strAddress.c_str());
	else 
	{
		//Use this on windows - windows doesn't do getaddrinfo very well (VC6) and its
		//    gethostbyname is thread safe
		hostent *pHostent = gethostbyname (strAddress.c_str());
		if (!pHostent)
			throw string("Host lookup failure for " + strAddress);
		address.sin_addr.s_addr = *(unsigned long*)pHostent->h_addr_list[0];
	}
	if (address.sin_addr.s_addr == INADDR_NONE)
		return false;

	if (connect (m_hSocket, (sockaddr*)&address, sizeof (sockaddr_in)) != SOCKET_ERROR)
		return true;
	else
		return false;
}

cSocketSSIQReader::cSocketSSIQReader(string strHost, unsigned short usPortNo)
:m_hSocket(INVALID_SOCKET),
m_fFullScale_dBm(0), 
m_fTotalGain_dB(0)
{
	//Connect to socket
	if (!Connect(strHost, usPortNo))
	{
		stringstream strstrError;
		strstrError << "Connection to " << strHost << ":" << usPortNo << " failed!";
		throw strstrError.str();
	}
	
	//m_hSocket is now ready to use
	
	//register all the appropriate chunk handlers to the instance of this class
	m_PXGHandlerSSIQ.registerCallbackHandler(this);
	m_PXGHandlerSIQP.registerCallbackHandler(this);
	m_PXGHandlerCF__.registerCallbackHandler(this);
	m_PXGHandlerBW__.registerCallbackHandler(this);
	m_PXGHandlerSR__.registerCallbackHandler(this);
	m_PXGHandler_dBTG.registerCallbackHandler(this);
	m_PXGHandler_dBFS.registerCallbackHandler(this);
	m_PXGHandlerIQDC.registerCallbackHandler(this);

	//attach all the appropriate chunk handlers to the PXGF socket reader
	m_PXGReader.registerChunkHandler(m_PXGHandlerSSIQ);
	m_PXGReader.registerChunkHandler(m_PXGHandlerSIQP);
	m_PXGReader.registerChunkHandler(m_PXGHandlerCF__);
	m_PXGReader.registerChunkHandler(m_PXGHandlerBW__);
	m_PXGReader.registerChunkHandler(m_PXGHandlerSR__);
	m_PXGReader.registerChunkHandler(m_PXGHandler_dBTG);
	m_PXGReader.registerChunkHandler(m_PXGHandler_dBFS);
	m_PXGReader.registerChunkHandler(m_PXGHandlerIQDC);

	//Init last tick variable
	m_uLastTick = GetTickCount();

	//The PXG Reader is now reay to accept a stream
	try
	{
		//We pass it the cPXGReaderBase::Endian_Auto parameter to allow the library to automatically detect the endian
		m_PXGReader.processStream(m_hSocket, cPXGReaderBase::Endian_Auto);
	}
	catch (cPxgfException &e)
	{
		cout << e.toString() << endl;
	}

}

cSocketSSIQReader::~cSocketSSIQReader()
{

}

int main(int argc, char* argv[])
{
	string strHost;
	unsigned short usPort;

	//Validate command line arguments
	if (argc > 3)
	{
		cout << "Usage: \n\t" << argv[0] << " HostName|IpAddress PortNumber" << endl;
		return 1;
	}
	else if (argc == 3)
	{
		strHost = argv[1];
		usPort = atoi(argv[2]);
		if (usPort == 0)
		{
			cout << "Usage: \n\t" << argv[0] << " HostName|IpAddress PortNumber" << endl;
			return 1;
		}
	}
	else if (argc == 2) 
	{
		strHost = argv[1];
		cout << "Enter port number for host " << strHost << ":" << endl;
		cin >> usPort;
	}
	else if (argc == 1)
	{
		cout << "Enter host name / IP Address of PXGF stream:" << endl;
		cin >> strHost;
		cout << "Enter port number for host " << strHost << ":" << endl;
		cin >> usPort;
	}
	
	try
	{
		cSocketSSIQReader(strHost, usPort);
	}
	catch (string strError)
	{
		cout << strError << endl;
		cout << "Enter x to exit:" << endl;
		char chDummy;
		cin >> chDummy;
		return 1;
	}
	return 0;
}

