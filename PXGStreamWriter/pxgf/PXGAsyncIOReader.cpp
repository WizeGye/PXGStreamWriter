// ---------------------------------------------------------------------------
// FILE NAME            : PXGAsyncIOReader.cpp
// ---------------------------------------------------------------------------
// COMPANY              : PERALEX ELECTRONICS (PTY) LTD
// ---------------------------------------------------------------------------
// COPYRIGHT NOTICE :
//
// The copyright, manufacturing and patent rights stemming from this document
// in any form are vested in PERALEX ELECTRONICS (PTY) LTD.
//
// (c) PERALEX ELECTRONICS (PTY) LTD 2005
//
// PERALEX ELECTRONICS (PTY) LTD has ceded these rights to its clients
// where contractually agreed.
// ---------------------------------------------------------------------------
// DESCRIPTION :
//
// This class is used for parsing PXGF streams. The user must register a reset handler
// and multiple chunk handlers. The reset handler is used to clear the state of the
// application when synchronisation is lost. The chunk handlers are used to process
// recognised chunks when they are recognised in the stream.
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $
// Date:       $LastChangedDate: 2012-03-30 13:18:06 +0200 (Fri, 30 Mar 2012) $
// Revision:   $LastChangedRevision: 45193 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGAsyncIOReader.cpp $
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
//#include <iostream>
#ifdef __unix__
#include <cstring>
#endif

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
//#include "log4cxx/logger.h"
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PXGAsyncIOReader.h"
#include "SwapEndian.h"
#include "exceptions/PxgfException.h"
#include "exceptions/PxgfEofException.h"

static const unsigned g_uBufferSize = 131072*4;
static const unsigned g_uNumBuffers = 3;

namespace pxgf {


    using namespace std;


	//=================================================================================
	//	cPXGAsyncIOReader::cPXGAsyncIOReader
	//--------------------------------------------------------------------------------
	//	This method is the constructor for the class cPXGAsyncIOReader.
	//
	//	Return
	//	------
	//	None
	//
	//=================================================================================
	cPXGAsyncIOReader::cPXGAsyncIOReader()
	: m_pHandle (NULL),
	m_pchBuffer(NULL),
	m_bFileFinished(false),
	m_puLastTransferred(NULL)
	{
		m_pchBuffer = new char [g_uBufferSize * g_uNumBuffers];
		//m_pchBuffer = vector<char> (g_uBufferSize * g_uNumBuffers);
		m_puBufferEmptyFlag = new unsigned [g_uNumBuffers];
		m_puLastTransferred = new unsigned [g_uNumBuffers];
		Reset ();
	}

	//=================================================================================
    //	cPXGAsyncIOReader::~cPXGAsyncIOReader
	//--------------------------------------------------------------------------------
	//	Destructor
	//
	//	Return
	//	------
	//	None
	//
	//=================================================================================
	cPXGAsyncIOReader::~cPXGAsyncIOReader()
	{
		delete [] m_pchBuffer;
		delete [] m_puBufferEmptyFlag;
		delete [] m_puLastTransferred;
	}


	//=================================================================================
	//	cPXGAsyncIOReader::processStream
	//--------------------------------------------------------------------------------
	// Calling this method instructs the engine to start processing the given
	// stream starting at its current location and assuming the endian
	// ordering specified. It will continuously process chunks until it
	// either reaches the end-of-file, or you specify that you wish it to
	// stop by calling removeFromStream().
	// The file must have been opened with OpenFile and the options
	// This file handle must have been created with the FILE_FLAG_OVERLAPPED flag
	// and with GENERIC_READ access to the file.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	stream		IN		the source of the data
	//	bBigEndian	IN		is the stream big endian byte order?
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void
	cPXGAsyncIOReader::processStream(HANDLE& hFile, bool bBigEndian)
	{
		m_pHandle = &hFile;
#ifndef __unix__
		memset (&m_Overlapped, 0, sizeof(m_Overlapped));
#endif
		m_bFileFinished = false;
		processPXGStream(bBigEndian);
	}

	//=================================================================================
	//	cPXGAsyncIOReader::processStream
	//--------------------------------------------------------------------------------
	// Calling this method instructs the engine to start processing the given
	// stream starting at its current location and assuming the endian
	// ordering specified. It will continuously process chunks until it
	// either reaches the end-of-file, or you specify that you wish it to
	// stop by calling removeFromStream().
	// The file must have been opened with OpenFile and the options
	// This file handle must have been created with the FILE_FLAG_OVERLAPPED flag
	// and with GENERIC_READ access to the file.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	stream		IN		the source of the data
	//	EndianType	IN		big, little, or auto endian
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void
	cPXGAsyncIOReader::processStream(HANDLE& hFile,  eEndianType EndianType)
	{
		m_pHandle = &hFile;
#ifndef __unix__
		memset (&m_Overlapped, 0, sizeof(m_Overlapped));
#endif
		m_bFileFinished = false;
		processPXGStream(EndianType);
	}




	//=================================================================================
	//	cPXGAsyncIOReader::AssignSource
	//--------------------------------------------------------------------------------
	//	Assign a handle to the reader.  This is to allow the reader to be used for
	//  general purpose reading, seeking and telling
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	hFile		IN		handle to the file to be manipulated
	//
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void
	cPXGAsyncIOReader::AssignSource (HANDLE& hFile)
	{
		m_pHandle = &hFile;
	}

	//=================================================================================
	//	cPXGAsyncIOReader::Reset
	//--------------------------------------------------------------------------------
	//	Resets the read buffers - to be used when a seek is performed and on startup
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void
	cPXGAsyncIOReader::Reset (void)
	{
#ifndef __unix__
		memset (&m_Overlapped, 0, sizeof(m_Overlapped));
#endif

		for (unsigned uCounter = 0; uCounter < g_uNumBuffers; uCounter++) {
			m_puBufferEmptyFlag[uCounter] = 1;
			m_puLastTransferred[uCounter] = 0;
		}

		m_uBufferCounter = 0;
		m_uWriteBufferCounter = 0;
		m_uCurrentBufferOffset = 0;
		m_uReadInProgress = 0;
		m_lOffset = 0;
		m_lUseOffset = 0;

		//This shouldn't be required - use in emergencies only
		memset (m_pchBuffer, 0, g_uBufferSize * g_uNumBuffers);
	}


	//=================================================================================
	//	cPXGAsyncIOReader::Seek
	//--------------------------------------------------------------------------------
	//	Set the read pointer to the relevant spot
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	lOffset		IN		Offset into the file at which to start the next read
	//
	//
	//	Return
	//	------
	//	true if no error
	//
	//=================================================================================
	bool
	cPXGAsyncIOReader::Seek (int64_t lOffset)
	{
		//Should probably test to determine whether this seek will seek past the
		//       end of the file
		if (lOffset >= 0)
		{
			if (*m_pHandle != INVALID_HANDLE_VALUE) {
				CompleteRead ();
			}
			Reset();
			m_lOffset = lOffset;
			m_lUseOffset = lOffset;
			m_bFileFinished = false;
			return true;
		}
		else
			return false;
	}

	//=================================================================================
	//	cPXGAsyncIOReader::Tell
	//--------------------------------------------------------------------------------
	//	This method
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	void		IN
	//
	//
	//	Return
	//	------
	//	int64_t is the offset
	//
	//=================================================================================
	int64_t
	cPXGAsyncIOReader::Tell (void)
	{
		return m_lUseOffset;
	}

	//=================================================================================
	//	cPXGAsyncIOReader::EmptyBuffers
	//--------------------------------------------------------------------------------
	//	Counts the number of empty buffers
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//
	//
	//	Return
	//	------
	//	unsigned  = the number of empty buffers
	//
	//=================================================================================
	unsigned
	cPXGAsyncIOReader::EmptyBuffers (void)
	{
		//Count the number of empty buffers
		unsigned uCounter, uEmptyBuffers = 0;
		for (uCounter = 0; uCounter < g_uNumBuffers; uCounter++)
			uEmptyBuffers += m_puBufferEmptyFlag[uCounter];

		return uEmptyBuffers;
	}


	//=================================================================================
	//	cPXGAsyncIOReader::CheckBuffers
	//--------------------------------------------------------------------------------
	//	Reads data into all but one of the empty buffers
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void
	cPXGAsyncIOReader::CheckBuffers (void)
	{
		unsigned uEmptyBuffers = EmptyBuffers();
		//unsigned long uTransferred = 0;

		//If there is one empty buffer, don't worry about it - it'll be filled
		//   while the next 'read' is under way.

		//If there are two empty buffers, fill one of them
		while (uEmptyBuffers-- > 1 && !m_bFileFinished)
		{
			m_uReadInProgress = 1;
#ifndef __unix__
			m_Overlapped.Offset = m_lOffset & 0x00000000ffffffff;
			m_Overlapped.OffsetHigh = (m_lOffset >> 32);
			m_Overlapped.hEvent = 0;
			if (ReadFile (*m_pHandle,
				&m_pchBuffer[g_uBufferSize * m_uWriteBufferCounter],
				g_uBufferSize,
				NULL,
				&m_Overlapped) == false)
			{

				unsigned uError;
				if ((uError = GetLastError()) == ERROR_HANDLE_EOF)
				{
					//throw cPxgfEofException();
					m_bFileFinished = true;
					return;
				}
				else if (uError == ERROR_IO_PENDING)
				{
				}
				else
				{
					throw cPxgfException ("Asynchronous file read error");
				}

			}

#else
			memset (&m_InputFileCB, 0, sizeof(m_InputFileCB));
			m_InputFileCB.aio_fildes = *m_pHandle;
			m_InputFileCB.aio_buf	 = &m_pchBuffer[g_uBufferSize * m_uWriteBufferCounter];
			m_InputFileCB.aio_nbytes = g_uBufferSize;
			m_InputFileCB.aio_offset = m_lOffset;
			if (aio_read (&m_InputFileCB))
			{
				if (errno == EOVERFLOW)
					throw cPxgfEofException();
				else
					throw cPxgfException ("Asynchronous file read error");
			}
#endif

#ifndef __unix__
			if (GetLastError () == ERROR_SUCCESS)
			{
				m_puBufferEmptyFlag[m_uWriteBufferCounter] = 0;
				m_uWriteBufferCounter = (++m_uWriteBufferCounter)%g_uNumBuffers;
				m_uReadInProgress = 0;
				m_lOffset += g_uBufferSize;
			}
			else
#endif
			{
				CompleteRead();
			}
		}
	}

	//=================================================================================
	//	cPXGAsyncIOReader::CompleteRead
	//--------------------------------------------------------------------------------
	//	Called to wait for an asynchronous read to complete
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void
	cPXGAsyncIOReader::CompleteRead (void)
	{
		if (m_uReadInProgress && !m_bFileFinished)
		{
			unsigned long uTransferred;
#ifndef __unix__
			if (GetOverlappedResult (*m_pHandle, &m_Overlapped, &uTransferred, TRUE) == false)
			{
				unsigned uError = GetLastError ();
				if (uError == ERROR_HANDLE_EOF) {
					//throw cPxgfEofException();
					m_bFileFinished = true;
					return;
				}
				else
					throw cPxgfException ("Asynchronous file read error");
			}
#else
			//**********Get this working like the windows code
			aiocb *pInputFileCB = &m_InputFileCB;
			if (aio_suspend(&pInputFileCB, 1, NULL))
			{
				if (errno == EAGAIN)
					throw cPxgfException ("Timeout");
				else
					throw cPxgfException ("Asynchronous file read error: EINTR");
			}
			if (int iError = aio_error (&m_InputFileCB)) {
				if (iError == ECANCELED)
					throw cPxgfException ("Cancelled");
				else
					throw cPxgfException ("Asynchronous file read error");
			}
			int iAioReturnValue = aio_return(&m_InputFileCB);
			//If a 0 is returned, then the end of file was read
			//If a positive value was returned, this is the number of bytes read
			//If a -1 is returned, this indicates that the aio call has not finished - this
			//   shouldn't happen because the aio_suspend should fix it.
			if (!iAioReturnValue)
				throw cPxgfEofException();

			uTransferred = iAioReturnValue;

#endif
			//Store 0 if a full buffer is transferred, otherwise 1
			if (uTransferred != g_uBufferSize)
				m_puLastTransferred[m_uWriteBufferCounter] = uTransferred;

			m_puBufferEmptyFlag[m_uWriteBufferCounter] = 0;
			m_uWriteBufferCounter = (++m_uWriteBufferCounter)%g_uNumBuffers;
			m_uReadInProgress = 0;
			m_lOffset += g_uBufferSize;
//			cout << "-" << uTransferred << "-";

		}
	}




	//=================================================================================
	//	cPXGAsyncIOReader::blockingRead
	//--------------------------------------------------------------------------------
	//	This method performs a blocking read of the desired length and throw eof exception
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	pchData		OUT		destination for read data
	//	iLengthRead	IN		number of bytes to read
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void
	cPXGAsyncIOReader::blockingRead (char* pchData, int iLength)
	{

		//unsigned long ulLengthRead = 0;

		//Check if the data is longer than the buffer
		if ((unsigned)iLength > g_uBufferSize)
			throw cPxgfException ("Requested read too long");


		//Wait for previous reads (if any) to complete, but only if there is more
		//     than 1 empty buffer
		if (EmptyBuffers() > 1)
			CompleteRead();


		//Check if the buffers are empty, and if so, read a new buffer or 2
		CheckBuffers();

		//If there is an empty buffer, then start a new read to keep the buffers full
		if (m_puBufferEmptyFlag[m_uWriteBufferCounter] && !m_uReadInProgress && !m_bFileFinished)
		{

			//m_uReadInProgress = 1;
			m_uReadInProgress = 1;
#ifndef __unix__
			unsigned uError;
			m_Overlapped.Offset = m_lOffset & 0x00000000ffffffff;
			m_Overlapped.OffsetHigh = (m_lOffset >> 32);
			m_Overlapped.hEvent = 0;
			if (ReadFile (*m_pHandle,
				&m_pchBuffer[g_uBufferSize * m_uWriteBufferCounter],
				g_uBufferSize,
				NULL,
				&m_Overlapped) == false)
			{

				if ((uError = GetLastError()) == ERROR_HANDLE_EOF)
				{
					//throw cPxgfEofException();
					m_bFileFinished = true;
				}
				else if (uError == ERROR_IO_PENDING)
				{
				}
				else
				{
					throw cPxgfException ("Asynchronous file read error");
				}

			}
			//If the Read completed without going async - apparently this should never
			//   happen, but I can't find definitive documentation on this.  Either way,
			//   uError is uninitialized here, so the write shouldn't be completed here
			/*
			if (uError == ERROR_SUCCESS)
			{
				m_puBufferEmptyFlag[m_uWriteBufferCounter] = 0;
				m_uWriteBufferCounter = (++m_uWriteBufferCounter)%g_uNumBuffers;
				m_uReadInProgress = 0;
				m_lOffset += g_uBufferSize;
			}*/
#else
			memset (&m_InputFileCB, 0, sizeof(m_InputFileCB));
			m_InputFileCB.aio_fildes = *m_pHandle;
			m_InputFileCB.aio_buf	 = &m_pchBuffer[g_uBufferSize * m_uWriteBufferCounter];
			m_InputFileCB.aio_nbytes = g_uBufferSize;
			m_InputFileCB.aio_offset = m_lOffset;
			if (aio_read (&m_InputFileCB))
			{
				if (errno == EOVERFLOW)
					throw cPxgfException ("Asynchronous file read error: Overflow");
				else
					throw cPxgfException ("Asynchronous file read error");
			}
#endif
			//For debugging - wait until the read has completed before continuing
			//CompleteRead();
		}


		//Get the data from the buffer
		//If the required length fits entirely in the buffer
		if (g_uBufferSize - m_uCurrentBufferOffset >= (unsigned)iLength)
		{
			//cout << "'" << m_uBufferCounter << "*" << m_uCurrentBufferOffset << "." << EmptyBuffers() << "v" << m_puLastTransferred[m_uBufferCounter];

			//Is this buffer only partially full - in that case it was the last read from the file
			if (m_puLastTransferred[m_uBufferCounter]) {
				if (m_puLastTransferred[m_uBufferCounter] - m_uCurrentBufferOffset < iLength)
					throw cPxgfEofException();
			}

			if (EmptyBuffers() == 3)
				throw cPxgfEofException();

			memcpy (pchData, &m_pchBuffer[g_uBufferSize * m_uBufferCounter + m_uCurrentBufferOffset], iLength);
			m_uCurrentBufferOffset += iLength;
			if (m_uCurrentBufferOffset >= g_uBufferSize) //could be ==, but shouldn't be >
			{
				m_puBufferEmptyFlag[m_uBufferCounter] = 1;
				if (++m_uBufferCounter >= g_uNumBuffers)
					m_uBufferCounter = 0;

				m_uCurrentBufferOffset -= g_uBufferSize;
			}
		}
		else
		{
			//cout << "'" << m_uBufferCounter << "+" << m_uCurrentBufferOffset << "." << EmptyBuffers() << "v" << m_puLastTransferred[m_uBufferCounter];

			//Is the next buffer only partially full - in that case it was the last read from the file
			if (m_puLastTransferred[(m_uBufferCounter+1)%g_uNumBuffers]) {
				//Check that the chunk requested will fit into the remainder of this buffer and the small
				//      incomplete piece of data in the next available buffer
				if (m_puLastTransferred[(m_uBufferCounter+1)%g_uNumBuffers] + (g_uBufferSize - m_uCurrentBufferOffset) < iLength)
					throw cPxgfEofException ();
			}

			if (EmptyBuffers() == 2)
				throw cPxgfEofException();

			memcpy (pchData, &m_pchBuffer[g_uBufferSize * m_uBufferCounter + m_uCurrentBufferOffset], (g_uBufferSize - m_uCurrentBufferOffset));
			m_puBufferEmptyFlag[m_uBufferCounter] = 1;
			if (++m_uBufferCounter >= g_uNumBuffers)
				m_uBufferCounter = 0;
			memcpy (&pchData[g_uBufferSize - m_uCurrentBufferOffset], &m_pchBuffer[g_uBufferSize * m_uBufferCounter], iLength - (g_uBufferSize - m_uCurrentBufferOffset));
			m_uCurrentBufferOffset = iLength - (g_uBufferSize - m_uCurrentBufferOffset);

		}
		m_lUseOffset += iLength;
    }

}
