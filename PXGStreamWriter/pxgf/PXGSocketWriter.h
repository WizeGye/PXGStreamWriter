#ifndef __PXG_SOCKET_WRITER_H
#define __PXG_SOCKET_WRITER_H

// ---------------------------------------------------------------------------
// FILE NAME            : PXGSocketWriter.h
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
// Declaration of a PXG writer class that uses a TCP socket as the transport
// medium for the PXGF stream.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: dave $
// Date:       $LastChangedDate: 2013-07-19 16:42:45 +0200 (Fri, 19 Jul 2013) $
// Revision:   $LastChangedRevision: 50952 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGSocketWriter.h $
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#ifndef __unix__

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>

#else

#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#endif //unix

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PXGWriterBase.h"
#include "ChunkHandler.h"

namespace pxgf
{

	/// PXG Writer class that uses a socket to write its PXGF stream to
	class cPXGSocketWriter : public cPXGWriterBase
	{
	public:
		/// Constructor
		/// @param hSocket - valid socket to write stream to
		/// @param bBigEndian - endian to write stream in
#ifndef __unix__
		cPXGSocketWriter(SOCKET hSocket, bool bBigEndian);
#else
		cPXGSocketWriter(int hSocket, bool bBigEndian);
#endif
		bool SetDatagramDest(sockaddr_in hSocketDest);
		void SetMTUSize(unsigned uMTU) { m_uUDP_MTU = (uMTU - 40); }
		/// Defines the socket MTU size, default is 1500
		/// @param uMTU - size in bytes of maximum transmission unit

	protected:
		/// Perform a blocking read of the desired length and throw eof exception
		/// @param pData - destination for read data
		/// @param iLength - number of bytes to read
		virtual void write(const char* pData, std::streamsize iLength);
		
	private:
#ifndef __unix__
		SOCKET m_hSocket;
		//: Handle to the system socket
#else //unix
		int m_hSocket;
		//: Handle to the system socket
#endif
		bool m_bIsSocketUDP;
		sockaddr_in m_DatagramDest;
		unsigned m_uUDP_MTU;
		//: UDP parameters to setuup, if socket is not TCP/IP
	};
}
#endif

