#ifndef CPXGASYNCIOREADER_H
#define CPXGASYNCIOREADER_H
// ---------------------------------------------------------------------------
// FILE NAME            : PXGAsyncIOReader.h
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
// Declaration for the class cPXGAsyncIOReader used for parsing PXGF files using
// asynchronous file reads
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $
// Date:       $LastChangedDate: 2011-08-04 09:25:42 +0200 (Thu, 04 Aug 2011) $
// Revision:   $LastChangedRevision: 42520 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGAsyncIOReader.h $
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#ifndef __unix__
#include <windows.h>
#pragma warning(disable:4786)
#else
#include <aio.h>
#include <cerrno>
#include "Utilities/WinToLinux/WinToLinux.h"
#define INVALID_HANDLE_VALUE -1
#endif

#include <map>
#include <sstream>
#include <vector>
// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PXGReaderBase.h"
#include "PXGStreamWriter.h"
#include "ResetHandler.h"

namespace pxgf {

	/// Implementation of a PXGReader class that use asyncronous I/O.
	//
	/// This class is used for parsing PXGF streams. The user must register a
	/// reset handler and multiple chunk handlers. The reset handler is used to
	/// clear the state of the application when synchronisation is lost. The chunk
	/// handlers are used to process recognised chunks when they are recognised
	/// in the stream.
	///
	/// Once you have opened an input stream and determined the endian of the
	/// stream you will call the processStream() method. This method will not
	/// return until either you specify somewhere else that you wish to stop
	/// processing by calling removeFromStream(), or it reaches the end of the file.
    class cPXGAsyncIOReader : public cPXGReaderBase
	{
	public:
		/// Constructor
		cPXGAsyncIOReader();

		/// Destructor
		~cPXGAsyncIOReader();

		/// Process an input handle
		//
		/// Calling this stream instructs the engine to start processing the given
		/// stream starting at its current location and assuming the endian
		/// ordering specified. It will continuously process chunks until it
		/// either reaches the end-of-file, or you specify that you wish it to
		/// stop by calling removeFromStream().
		/// @param hFile - the source of the data
		/// @param bBigEndian - is the stream big endian byte order?
		void processStream(HANDLE& hFile, bool bBigEndian);

		/// Process an input handle
		//
		/// Calling this stream instructs the engine to start processing the given
		/// stream starting at its current location and assuming the endian
		/// ordering specified. It will continuously process chunks until it
		/// either reaches the end-of-file, or you specify that you wish it to
		/// stop by calling removeFromStream().
		/// @param hFile - the source of the data
		/// @param EndianType - is the stream big endian, little endian, or should endian be auto-detected
		void processStream(HANDLE& hFile,  eEndianType EndianType);


		/// Reset the buffers and the pointers
		void Reset (void);

		/// Seek to the given offset in the file
		bool Seek (int64_t lOffset);

		/// Called to indicate the completion of an overlapped read.
		void CompleteRead (void);

		/// Set the handle
		void AssignSource (HANDLE& hFile);

		/// Return the effective read offset in the file
		int64_t Tell (void);

		/// Perform a blocking read of the desired length and throw eof exception
		/// @param pData - destination for read data
		/// @param iLength - number of bytes to read
		virtual void blockingRead(char* pData, int iLength);


	protected:

	private:
		unsigned EmptyBuffers (void);
		//: Count the number empty buffers

		void CheckBuffers (void);
		//: Fill up the buffers at any stage when they may be empty

        	HANDLE* m_pHandle;
		//: Pointer to the handle to the file

#ifndef __unix__
		OVERLAPPED m_Overlapped;
		//: OVERLAPPED structure used for overlapped reads
#else
		aiocb m_InputFileCB;
#endif

		unsigned * m_puBufferEmptyFlag;
		//: Flags to record the status of each of the buffers

		unsigned m_uBufferCounter;
		//: The current buffer in which blockingRead will start reading data

		unsigned m_uWriteBufferCounter;
		//: The next buffer to be written to by the file read

		unsigned m_uCurrentBufferOffset;
		//: Offset in the current buffer (used by blockingRead)

		unsigned m_uReadInProgress;
		//: Flag to record whether a read is in progress

		int64_t m_lOffset;
		//: Offset in the current file in bytes

		int64_t m_lUseOffset;
		//: Offset in the file of the current read pointer

		char *m_pchBuffer;
		//std::vector<char> m_pchBuffer;
        //: Buffer used to store data read from disk before it is used by
		//  blockingRead

		bool m_bFileFinished;
		//: Flag says whether a read operation encountered the end of file

		unsigned * m_puLastTransferred;
		//: Array specifying how long a buffer was retrieved from the disk
    };
}

#endif
