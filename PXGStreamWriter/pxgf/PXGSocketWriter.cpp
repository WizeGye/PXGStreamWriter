// ---------------------------------------------------------------------------
// FILE NAME
// ---------------------------------------------------------------------------
// COMPANY
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
// Implementation of a PXG writer class that uses a TCP socket as the transport
// medium for the PXGF stream.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: dave $
// Date:       $LastChangedDate: 2013-07-19 16:42:45 +0200 (Fri, 19 Jul 2013) $
// Revision:   $LastChangedRevision: 50952 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGSocketWriter.cpp $
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "exceptions/PxgfException.h"
#include "PXGSocketWriter.h"

using namespace pxgf;



//=================================================================================
//	cPXGSocketWriter::cPXGSocketWriter
//--------------------------------------------------------------------------------
//	Constructor. This constructor accepts an already connected socket that is to
//  be used as the transport medium.
//
//	Parameter	Dir		Description
//	---------	---		-----------
//	stream		IN		Socket that is connected to a PXG reader
//	bBigEndian	IN		true if stream is to be big endian
//
//
//	Return
//	------
//	None
//
//=================================================================================
#ifndef __unix__
cPXGSocketWriter::cPXGSocketWriter(SOCKET hSocket, bool bBigEndian)
#else
cPXGSocketWriter::cPXGSocketWriter(int hSocket, bool bBigEndian)
#endif
:cPXGWriterBase(bBigEndian),
m_hSocket(hSocket),
m_bIsSocketUDP(false),
m_uUDP_MTU(1460)
{

}

//=================================================================================
//	cPXGSocketWriter::SetDatagramDest
//--------------------------------------------------------------------------------
//	This method copies the socket DatagramDest parameters needed for UDP sendto(...)
//	If this is set we assume UDP socket type
//
//	Parameter		Dir		Description
//	---------		---		-----------
//	sockaddr_in		IN		UDP DatagramDest parameter
//
//
//	Return
//	------
//	None.
//
//=================================================================================
bool cPXGSocketWriter::SetDatagramDest(sockaddr_in hSocketDest)
{
	m_DatagramDest = hSocketDest;
	m_bIsSocketUDP = true;
	return true;
}

//=================================================================================
//	cPXGSocketWriter::write
//--------------------------------------------------------------------------------
//	This method sends the raw data over the socket.
//
//	Parameter	Dir		Description
//	---------	---		-----------
//	pData		IN		Pointer to the data block
//	iLength		IN		Size of the data block in bytes
//
//
//	Return
//	------
//	None.
//
//=================================================================================
void cPXGSocketWriter::write(const char* pcData, std::streamsize iLength)
{
	if (iLength == 0)
		return;
	if (m_hSocket == INVALID_SOCKET)
		throw cPxgfException("Invalid socket");
	int iSent;
	while (iLength) {
		if (m_bIsSocketUDP)
		{
			int iLen = iLength > m_uUDP_MTU ? m_uUDP_MTU : iLength;
			iSent = sendto (m_hSocket, pcData, iLen, 0, (struct sockaddr *) &m_DatagramDest, sizeof(m_DatagramDest));
		}
		else
			iSent = send (m_hSocket, pcData, iLength, 0);

		//If it receives SOCKET_ERROR, the socket is almost certainly closed.  It should
		//   never receive 0 - when the socket is closed on the other end, it should receive
		//   an error.  Leave the check for 0 in here though in case send starts behaving
		//   like recv at some stage.
		if (iSent == SOCKET_ERROR || iSent == 0) {
			//User app is responsible for shutting down and calling close on the socket
			throw cPxgfException("Socket closed or non-blocking");
		}
		iLength  -= iSent;
		pcData += iSent;
	}
}
