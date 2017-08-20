#ifndef __PXGSOCKET_READER_H
#define __PXGSOCKET_READER_H
// ---------------------------------------------------------------------------
// FILE NAME            : PXGSocketReader.h
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
// Declaration of class derived from PXGReaderBase that allows reading a PXGF
// stream from a socket.
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2011-08-04 09:25:42 +0200 (Thu, 04 Aug 2011) $ 
// Revision:   $LastChangedRevision: 42520 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGSocketReader.h $ 
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
#include "PXGReaderBase.h"
#include "PXGStreamWriter.h"
#include "ResetHandler.h"


namespace pxgf
{


	/// PXG Reader class that accepts an input PXGF stream from a socket
	class cPXGSocketReader : public cPXGReaderBase  
	{
	public:
		/// Constructor
		cPXGSocketReader();
#ifndef __unix__
		cPXGSocketReader(SOCKET hSocket);
#else
		cPXGSocketReader(int hSocket);
#endif

		/// Process an input stream
		//
		/// Calling this stream instructs the engine to start processing the given
		/// stream from the specfied socket and assuming the endian 
		/// ordering specified. It will continuously process chunks until the socket
		/// connection is broken, or you specify that you wish it to 
		/// stop by calling removeFromStream().(This processStream is deprecated
		/// and the other one which specfies endian by the eEndianType enum should
		/// be used instead).
		/// @param hSocket - a valid socket to read the data stream from
		/// @param bBigEndian - is the stream big endian byte order?
#ifndef __unix__
		void processStream(SOCKET hSocket, bool bBigEndian);
#else
		void processStream(int hSocket, bool bBigEndian);
#endif
		
		/// Process an input stream
		//
		/// This is an alternative method to the previous processStream method
		/// that allows specifying that the endian should be automatically
		/// detected.
		/// @param hSocket - a valid socket to read the data stream from
		/// @param EndianType - specify the endian type of the input stream. Use Endian_Auto 
		/// for it to be automatically detected.
#ifndef __unix__
		void processStream(SOCKET hSocket, eEndianType EndianType);
#else
		void processStream(int hSocket, eEndianType EndianType);
#endif


	protected:
		/// Perform a blocking read of the desired length and throw eof exception
		/// @param pData - destination for read data
		/// @param iLength - number of bytes to read
		virtual void blockingRead(char* pData, int iLength);
		

	private:

#ifndef __unix__
		SOCKET m_hSocket;
		//: Handle to the system socket
#else //unix
		int m_hSocket;
		//: Handle to the system socket
#endif 
	};
}

#endif
