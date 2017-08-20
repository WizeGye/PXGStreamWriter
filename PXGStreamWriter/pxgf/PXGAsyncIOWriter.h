#ifndef CPXGASYNCIOWRITER_H
#define CPXGASYNCIOWRITER_H
// ---------------------------------------------------------------------------
// FILE NAME            : PXGStreamWriter.h
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
// The class definition used for writing PXG streams using posix realtime
// extensions' asynchronous IO.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: adrian $
// Date:       $LastChangedDate: 2013-08-22 09:26:38 +0200 (Thu, 22 Aug 2013) $
// Revision:   $LastChangedRevision: 51434 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGAsyncIOWriter.h $
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#include <deque>
#include <vector>
#include <string>
#ifndef __unix__
#include <windows.h>
#else
#include <aio.h>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include "Utilities/WinToLinux/WinToLinux.h"
#define INVALID_HANDLE_VALUE -1
#endif
// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PXGWriterBase.h"
#include "ChunkHandler.h"

namespace pxgf {

    /// This class is used for writing PXG streams.
    class cPXGAsyncIOWriter : public cPXGWriterBase
	{
	public:
#ifdef __unix__
		/// Constructor
		//
		/// Constructor specifies output stream to write to and endian byte
		/// ordering to use in recording.
		/// @param iOutputFile - open file handler to write to
		/// @param bBigEndian - to write stream in big endian?
		cPXGAsyncIOWriter(int &iOutputFile, bool bBigEndian);


	protected:
		virtual void write (const char* pchData, std::streamsize nLength);

	private:
		std::vector<char> m_vchData;
		//: Vector to contain data while it is being written.

		aiocb m_OutputFile;
        //: The output file async IO object.

		int &m_iFile;
		//: The output file descriptor

		bool m_bFirstWrite;
		//: Is this the first write
#else
		/// Constructor
		//
		/// Constructor specifies output stream to write to and endian byte
		/// ordering to use in recording.
		/// @param pFile - pointer to open file handler to write to
		/// @param bBigEndian - to write stream in big endian?
		cPXGAsyncIOWriter(HANDLE pFile, bool bBigEndian, uint32_t uNumOverlappedIO = 32, uint32_t uOverlappedIOBlockSize = 4194304);

		virtual ~cPXGAsyncIOWriter();

	protected:
		virtual void write (const char* pchData, std::streamsize nLength);

	private:
		std::deque< OVERLAPPED > m_queueOverlapped;
		//: OVERLAPPED structure used for overlapped reads

		std::deque< HANDLE > m_queueFreeAsyncIOEvents;

		uint32_t	m_uNumBuffers;

		uint32_t	m_uOverlappedIOBlockSize;

		///Vector of vector of buffers for disk writes
		std::vector<std::vector<char> > m_vvchBuffer;

		///Offset into the buffer.
		std::vector<unsigned> m_vuBufferOffset;

		///Currently active buffer
		unsigned m_uCurrentBlock;

		/// How many bytes have been scheduled for writing into this particular file
		uint64_t m_u64Offset;

		HANDLE m_pOutputFile;
		//: The output file descriptor

		int m_iError;
		//: Keep track of the Error status
#endif
		int64_t m_lRetOffset;
		//: Offset to return to caller in case write is still pending while
		//: offset is being requested.

		int64_t m_lOffset;
		//: Offset in the current file in bytes

	public:
		/// Get the amount of data submitted to be written
		/// @return the number of bytes submitted to be written
		int64_t getOffset();

	};
}

#endif
