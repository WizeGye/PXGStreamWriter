// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerGIQP.cpp
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
// Chunk handler for the GIQP chunk. The Group IQ Packing chunk stores the
// packing used for the IQ Data in the GSIQ chunks.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: ryan $
// Date:       $LastChangedDate: 2009-12-08 10:38:34 +0200 (Tue, 08 Dec 2009) $
// Revision:   $LastChangedRevision: 36884 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerGIQP.cpp $
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
#ifndef NO_LOG4CXX
#include "log4cxx/logger.h"
#endif
#ifdef __unix__
#include <string.h>
#endif

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "ChunkHandlerGIQP.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"

namespace pxgf {

    using namespace std;

#ifndef NO_LOG4CXX
	static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.handlers.ChunkHandlerGIQP"));
#endif

	//=================================================================================
	//	cChunkHandlerGIQP::cChunkHandlerGIQP
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
	cChunkHandlerGIQP::cChunkHandlerGIQP()
      : m_pCallbackGIQPHandler(0)
    {}


	//=================================================================================
	//	cChunkHandlerGIQP::registerCallbackHandler
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
	void cChunkHandlerGIQP::registerCallbackHandler(cCallbackGIQP* pHandler)
	{
        m_pCallbackGIQPHandler = pHandler;
    }


	//=================================================================================
	//	cChunkHandlerGIQP::processChunk
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
	bool cChunkHandlerGIQP::processChunk(const vector<char>& vchData, bool bBigEndian)
	{
        if (m_pCallbackGIQPHandler == 0) {
#ifndef NO_LOG4CXX
			LOG4CXX_WARN(g_logger, "Callback not initialized.");
#endif
            return true;
        }

        int iNumChannels = *(int*)&vchData[0];
		if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT(iNumChannels);
        // check that the block has the correct amount of data
        if (vchData.size() != unsigned(iNumChannels * 4 + 12)) {
#ifndef NO_LOG4CXX
			LOG4CXX_ERROR(g_logger, "Could not process chunk due to incorrect length.");
#endif
            return false;
		}

        bool bIsInputIQPacked;
		int iIQPacking = *(int*)&vchData[4];
		if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT(iIQPacking);
        switch(iIQPacking) {
            case 0: bIsInputIQPacked = false; break;
            case 1: bIsInputIQPacked = true; break;
            default: return false;
        }
        int iIncrement = *(int*)&vchData[8];
		if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT(iIncrement);
        vector<int> aiChannelOffsets (iNumChannels);
        memcpy(&aiChannelOffsets[0], &vchData[12], iNumChannels * sizeof(int));
		if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
            for (vector<int>::iterator it = aiChannelOffsets.begin(); it != aiChannelOffsets.end(); it++)
                SWAP_INT(*it);
		}

        m_pCallbackGIQPHandler->callbackGIQP(cPackingGIQP(bIsInputIQPacked, iIncrement, aiChannelOffsets));
        return true;
    }


	//=================================================================================
	//	cChunkHandlerGIQP::writeChunkGIQP
	//--------------------------------------------------------------------------------
	//	Write a GIQP chunk to the given stream writer.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	stream		IN		is the output stream to which the chunk is written.
	//	oPacking	IN		is the cPackingGIQP object describing the packing used for
	//						 the data.
	//
	//=================================================================================
	void cChunkHandlerGIQP::writeChunkGIQP(cPXGWriterBase& stream, const cPackingGIQP& oPacking) const
	{
        stream.writeChunkHeader(*this, oPacking.getNumChannels() * 4 + 12);
        stream.writeInteger(oPacking.getNumChannels());
        if (oPacking.getIsIQPacked())
            stream.writeInteger(1);
        else
            stream.writeInteger(0);
        stream.writeInteger(oPacking.getIncrement());
        stream.writeIntegerArray(oPacking.getChannelOffsets());
    }


	//=================================================================================
	//	cChunkHandlerGIQP::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerGIQP::getChunkType() const
	{
        //"GIQP"
        return 0x47495150;
    }

}
