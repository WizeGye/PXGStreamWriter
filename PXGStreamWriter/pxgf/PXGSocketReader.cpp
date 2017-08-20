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
// Implementation of a class derived from PXGReaderBase that allows reading a PXGF
// stream from a socket.
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: jeremy $ 
// Date:       $LastChangedDate: 2009-01-20 14:22:58 +0200 (Tue, 20 Jan 2009) $ 
// Revision:   $LastChangedRevision: 34205 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGSocketReader.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PXGSocketReader.h"
#include "exceptions/PxgfException.h"

namespace pxgf
{


	//=================================================================================
	//	cPXGSocketReader::cPXGSocketReader
	//--------------------------------------------------------------------------------
	//	Constructor
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	None
	//
	//
	//	Return
	//	------
	//	None
	//
	//=================================================================================
	cPXGSocketReader::cPXGSocketReader()
	:m_hSocket(INVALID_SOCKET)
	{

	}
#ifndef __unix__
	cPXGSocketReader::cPXGSocketReader(SOCKET hSocket)
	:m_hSocket(hSocket)
	{}
#else
	cPXGSocketReader::cPXGSocketReader(int hSocket)
	:m_hSocket(hSocket)
	{}
#endif

	//=================================================================================
	//	cPXGSocketReader::processStream
	//--------------------------------------------------------------------------------
	//	This method starts processing a PXGF stream arriving on a socket
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	hSocket		IN		Handle to a socket connected to a PXGF stream
	//	bBigEndian	IN		Whether to use big endian or not
	//
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	#ifndef __unix__
	void cPXGSocketReader::processStream(SOCKET hSocket, bool bBigEndian)
	#else
	void cPXGSocketReader::processStream(int hSocket, bool bBigEndian)
	#endif
	{
		m_hSocket = hSocket;
		processPXGStream(bBigEndian);
		m_hSocket = INVALID_SOCKET;
	}

	//=================================================================================
	//	cPXGSocketReader::processStream
	//--------------------------------------------------------------------------------
	//	This method replaces the other processPXGStream by allowing a third Endian_Auto
	//	option, where the Endianess of the stream is auto-detected
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	hSocket		IN		Handle to a socket connected to a PXGF stream
	//	EndianType	IN		Specifies Endian type
	//
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	#ifndef __unix__
	void cPXGSocketReader::processStream(SOCKET hSocket, eEndianType EndianType)
	#else
	void cPXGSocketReader::processStream(int hSocket, eEndianType EndianType)
	#endif
	{
		m_hSocket = hSocket;
		processPXGStream(EndianType);
	}

	//=================================================================================
	//	cPXGSocketReader::blockingRead
	//--------------------------------------------------------------------------------
	//	This method overides the virtual function from PXGReaderBase and provides the
	//	method for reading data from the PXGF stream
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	pData		IN		Buffer to read into
	//	iLength		IN		Number of bytes to read
	//
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGSocketReader::blockingRead(char* pData, int iLength) 
	{

		int iRead;
		if (m_hSocket == INVALID_SOCKET)
			throw cPxgfException("Invalid Socket");

		while (iLength) 
		{
			iRead = recv (m_hSocket, pData, iLength, 0);
			if (iRead == 0 || iRead == SOCKET_ERROR)
			{
				m_hSocket = INVALID_SOCKET;
				throw cPxgfException("Socket closed");
			}
			iLength -= iRead;
			pData += iRead;
		}

	}
}
