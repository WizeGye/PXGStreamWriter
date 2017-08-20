// ---------------------------------------------------------------------------
// FILE NAME            : PXGAsyncIOWriter.cpp
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
// This class is used for writing PXG streams.  It performs asynchronous IO
// on the output file
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: adrian $
// Date:       $LastChangedDate: 2013-08-22 09:26:38 +0200 (Thu, 22 Aug 2013) $
// Revision:   $LastChangedRevision: 51434 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGAsyncIOWriter.cpp $
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#include <cerrno>
#ifdef __unix__
#include <cstring>
#endif

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PXGAsyncIOWriter.h"
#include "SwapEndian.h"
#include "exceptions/PxgfIllegalArgumentException.h"

namespace pxgf {

	//=================================================================================
    //	cPXGAsyncIOWriter::cPXGAsyncIOWriter
	//--------------------------------------------------------------------------------
	//	Constructor specifying output stream to write to and endian byte
	//	ordering to use in writing.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	iOutputFile	IN		the output file to write to (open with 'open')
	//	bBigEndian	IN		the endian used in writing to stream
	//
	//	Return
	//	------
	//	None
	//
	//=================================================================================
#ifdef __unix__
	cPXGAsyncIOWriter::cPXGAsyncIOWriter(int &iFile, bool bBigEndian)
	:cPXGWriterBase(bBigEndian),
	m_lOffset(0),
	m_lRetOffset(0),
	m_iFile(iFile),
	m_bFirstWrite(true)
	{
		memset (&m_OutputFile, 0, sizeof(m_OutputFile));
	}
#else
	cPXGAsyncIOWriter::cPXGAsyncIOWriter(HANDLE pFile, bool bBigEndian, uint32_t uNumOverlappedIO, uint32_t uOverlappedIOBlockSize)
	:cPXGWriterBase(bBigEndian),
	 m_pOutputFile(pFile),
	m_lOffset(0),
	m_lRetOffset(0),
	m_iError(ERROR_SUCCESS),
	m_vvchBuffer (uNumOverlappedIO, std::vector<char> (m_uOverlappedIOBlockSize, 0)),
	m_vuBufferOffset (uNumOverlappedIO, 0u),
	m_uCurrentBlock(0),
	m_u64Offset(0),
	m_uNumBuffers(uNumOverlappedIO),
	m_uOverlappedIOBlockSize(uOverlappedIOBlockSize)
	{
		//Pre-allocate the event objects for the buffer blocks
		for (uint32_t uOverlapInitCount = 0; uOverlapInitCount < uNumOverlappedIO; uOverlapInitCount++)
		{
			HANDLE hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
			m_queueFreeAsyncIOEvents.push_back (hEvent);
		}
	}
#endif

#ifndef __unix__
	//=================================================================================
	//	cPXGAsyncIOWriter::!cPXGAsyncIOWriter
	//--------------------------------------------------------------------------------
	//	Destructor, waits until queued ovelapped IO operations completes before
	//	returning from the destructor
	//
	//	Return
	//	------
	//	None
	//
	//=================================================================================
	cPXGAsyncIOWriter::~cPXGAsyncIOWriter()
	{
		//Wait for all pending asynchronous IO writes to complete
		unsigned long uTransferred;
		unsigned uTimeout = 0;
		unsigned uTime = 10000;
		while (m_queueOverlapped.size() > 0) {
			do {
				if(GetOverlappedResult(m_pOutputFile, &m_queueOverlapped.front(), &uTransferred, TRUE))
				{
					m_iError = ERROR_SUCCESS;
					m_lOffset += uTransferred;
				}
				else
					m_iError = GetLastError ();
			} while (m_iError == ERROR_IO_PENDING && uTimeout++ < uTime);

			if (uTimeout >= uTime)
			{
				//What to do here?
			}

			//Store the event handle on the queue of free async IO event handles
			m_queueFreeAsyncIOEvents.push_back(m_queueOverlapped.front().hEvent);
			m_queueOverlapped.pop_front();
		}

		//Now iterate through the pre-allocated event object handles and close them correctly.
		while (m_queueFreeAsyncIOEvents.size() > 0)
		{
			CloseHandle(m_queueFreeAsyncIOEvents.front());
			m_queueFreeAsyncIOEvents.pop_front();
		}

	}
#endif

	//=================================================================================
	//	cPXGAsyncIOWriter::write
	//--------------------------------------------------------------------------------
	//	This method does the actual writing to the output device.  Note that the
	//  memory pointed to by pchData must remain valid until the write is complete,
	//  which will probably be some time after the function call has returned.  If
	//  blocks are being written strictly in the order Header, Body, Header, Body, ...
	//  It should be possible to re-use the memory for the current chunk body once
	//  the call to write the next chunk header has returned.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	pchData		IN		pointer to the array of data to be written
	//	nLength		IN		number of values to be written
	//
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
#ifdef __unix__
	void
	cPXGAsyncIOWriter::write(const char* pchData, std::streamsize nLength)
	{
		//Wait for the previous write to complete

		m_lRetOffset += nLength;
		const struct aiocb *const pWait = &m_OutputFile;

		if (m_bFirstWrite == false)
		{
			int iReturn;
			int iError;

			do
			{
				iReturn = aio_suspend (&pWait, 1, NULL);
				if (iReturn == EINTR) //Interrupted by a signal, try again
					continue;
			}
			while (iReturn == EINTR);

			iError = aio_error (&m_OutputFile);
			if (iError != EINVAL)
			{
				iReturn = aio_return (&m_OutputFile);
				if (iReturn == -1) {
					char pchMessage[64];
					strerror_r (errno, pchMessage, 60);
					throw cPxgfException(std::string ("File error: ") + pchMessage);
				}
				m_lOffset += m_OutputFile.aio_nbytes;
			}
		}

		//Resize the temporary storage to the right length
		if (m_vchData.size() < nLength)
			m_vchData.resize(nLength);

		memcpy (&m_vchData[0], pchData, nLength);

		if (nLength)
		{
			//Start the next request
			//Zero the aio descriptor
			memset (&m_OutputFile, 0, sizeof(m_OutputFile));
			//Fill in the relevant values
			m_OutputFile.aio_offset = m_lOffset;
			m_OutputFile.aio_fildes = m_iFile;
			m_OutputFile.aio_buf = (void *)&m_vchData[0];
			m_OutputFile.aio_nbytes = nLength;
			m_bFirstWrite = false;
			aio_write(&m_OutputFile);

		}
		else {
			memset (&m_OutputFile, 0, sizeof(m_OutputFile));
		}
	}

	//=================================================================================
	//	cPXGAsyncIOWriter::getOffset
	//--------------------------------------------------------------------------------
	//	Gets the offset into the file for data written using write().  Note that because
	//  data is written asynchronously, it's possible that this data will not actually
	//  have been written to disk, but some of the data writes may be pending
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	none
	//
	//	Return
	//	------
	//	Amount of data written using write() in bytes
	//
	//=================================================================================
	int64_t
	cPXGAsyncIOWriter::getOffset()
	{
		if (m_iFile != -1)
			return m_lRetOffset;
		else
			return 0;
	}

#else
	void
	cPXGAsyncIOWriter::write(const char* pchData, std::streamsize nLength)
	{
		unsigned long uTransferred;
		unsigned uTimeout = 0;
		unsigned uTime = 10000;

		//Check if we have already consumed all async IO memory buffers with previous writes
		if (m_queueOverlapped.size() >= m_uNumBuffers) {
			do {
				if(GetOverlappedResult(m_pOutputFile, &m_queueOverlapped.front(), &uTransferred, TRUE))
				{
					m_iError = ERROR_SUCCESS;
					m_lOffset += uTransferred;
				}
				else
					m_iError = GetLastError ();
			} while (m_iError == ERROR_IO_PENDING && uTimeout++ < uTime);

			if (uTimeout >= uTime)
				throw cPxgfException ("Async write timed out!");

			//Store the event handle on the queue of free async IO event handles
			m_queueFreeAsyncIOEvents.push_back(m_queueOverlapped.front().hEvent);
			m_queueOverlapped.pop_front();
		}

		//Copy the data into a buffer so that it doesn't get overwritten by something else in the mean time
		if (m_vuBufferOffset[m_uCurrentBlock] + nLength <= m_uOverlappedIOBlockSize) {
			memcpy (&m_vvchBuffer[m_uCurrentBlock][m_vuBufferOffset[m_uCurrentBlock]], pchData, nLength);
			m_vuBufferOffset[m_uCurrentBlock] += nLength;
		}
		else {
			//  Send the data
			OVERLAPPED ControlBlock;
			memset (&ControlBlock, 0, sizeof(ControlBlock));
			//Fill in the relevant values
			ControlBlock.Offset = m_u64Offset & 0x00000000ffffffff;
			ControlBlock.OffsetHigh = (m_u64Offset >> 32);
			//Get the event handle off the queue of free async IO event handles
			ControlBlock.hEvent = m_queueFreeAsyncIOEvents.front();
			m_queueFreeAsyncIOEvents.pop_front();
			m_queueOverlapped.push_back (ControlBlock);

			if( WriteFile(m_pOutputFile, (void *)&m_vvchBuffer[m_uCurrentBlock][0], m_vuBufferOffset[m_uCurrentBlock], &uTransferred, &m_queueOverlapped.back()) == FALSE)
			{
				m_iError = GetLastError();
				if (m_iError != ERROR_IO_PENDING)
				{
					m_iError;
					throw cPxgfException ("Asynchronous file write error");
				}
			}
			else {
				m_iError = ERROR_SUCCESS;
				m_lOffset += uTransferred;
			}

			m_u64Offset += m_vuBufferOffset[m_uCurrentBlock];
			//Start queueing the next bunch of data
			m_uCurrentBlock++;
			m_uCurrentBlock %= m_uNumBuffers;
			m_vuBufferOffset[m_uCurrentBlock] = 0;
			//Copy the new data into the next block
			memcpy (&m_vvchBuffer[m_uCurrentBlock][m_vuBufferOffset[m_uCurrentBlock]], pchData, nLength);
			m_vuBufferOffset[m_uCurrentBlock] += nLength;
		}
		m_lRetOffset += nLength;
	}

	//=================================================================================
	//	cPXGAsyncIOWriter::getOffset
	//--------------------------------------------------------------------------------
	//	Gets the offset into the file for data written using write().  Note that because
	//  data is written asynchronously, it's possible that this data will not actually
	//  have been written to disk, but some of the data writes may be pending
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	none
	//
	//	Return
	//	------
	//	Amount of data written using write() in bytes
	//
	//=================================================================================
	int64_t
	cPXGAsyncIOWriter::getOffset()
	{
		if (m_pOutputFile != INVALID_HANDLE_VALUE)
			return m_lRetOffset;
		else
			return 0;
	}
#endif

}


