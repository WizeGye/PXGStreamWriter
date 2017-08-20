// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandler_dBFS.cpp
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
// Chunk handler for the dBFS chunk. The dB Full Scale chunk stores the
// full scale value in dBs.
// 
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandler_dBFS.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
#ifndef NO_LOG4CXX
#include "log4cxx/logger.h"
#endif 
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "ChunkHandler_dBFS.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"

namespace pxgf {

    using namespace std;

#ifndef NO_LOG4CXX
	static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.handlers.ChunkHandler_dBFS"));
#endif


	//=================================================================================
	//	cChunkHandler_dBFS::cChunkHandler_dBFS
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
	cChunkHandler_dBFS::cChunkHandler_dBFS()
	  : m_pCallback_dBFSHandler(0)
	{}


	//=================================================================================
	//	cChunkHandler_dBFS::registerCallbackHandler
	//--------------------------------------------------------------------------------
	//	Register the object which will provide an implementation of this handler's 
	//	callback method. 
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	pHandler	IN		pointer to the object which will provide an 
	//						implementation of this handler's callback method
	//
	//=================================================================================
	void cChunkHandler_dBFS::registerCallbackHandler(cCallback_dBFS* pHandler) 
	{
		m_pCallback_dBFSHandler = pHandler;
	}


	//=================================================================================
	//	cChunkHandler_dBFS::processChunk
	//--------------------------------------------------------------------------------
	//	Perform the appropriate processing required for this chunk.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	vchData		IN		the array of data belonging to the chunk
	//	bBigEndian	IN		whether the data is BigEndian or not
	//
	//	Return
	//	------
	//	true if the chunk was processed successfully
	//	If any problems are detected during parsing of the data then the 
	//	return value should be false.
	//
	//=================================================================================
	bool cChunkHandler_dBFS::processChunk(const vector<char>& vchData, bool bBigEndian) 
	{
		if (vchData.size() != 4) {
#ifndef NO_LOG4CXX
			LOG4CXX_ERROR(g_logger, "Could not process chunk due to incorrect length.");
#endif
			return false; 
		}
		if (m_pCallback_dBFSHandler == 0) {
#ifndef NO_LOG4CXX
			LOG4CXX_WARN(g_logger, "Callback not initialized.");
#endif
			return true;
		}

		float f_dBFullScale_dBm = *(float*)&vchData[0];
		if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_FLOAT(f_dBFullScale_dBm);

		m_pCallback_dBFSHandler->callback_dBFS(f_dBFullScale_dBm);
		return true; 
	}


	//=================================================================================
	//	cChunkHandler_dBFS::writeChunk_dBFS
	//--------------------------------------------------------------------------------
	//	Write a dBFS chunk to the given stream writer. 
	//
	//	Parameter			Dir		Description
	//	---------			---		-----------
	//	stream				IN		is the output stream to which the chunk is written.
	//	f_dBFullScale_dBm	IN		is the Full Scale value measured in dBs. 
	//
	//=================================================================================
	void cChunkHandler_dBFS::writeChunk_dBFS(cPXGWriterBase& stream, float f_dBFullScale_dBm) const 
	{
		stream.writeChunkHeader(*this, 4);
		stream.writeFloat(f_dBFullScale_dBm);
	}


	//=================================================================================
	//	cChunkHandler_dBFS::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandler_dBFS::getChunkType() const 
	{
		//"_dBFS"
		return 0x64424653;
	}

}
