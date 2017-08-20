// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerGCF_.cpp
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
// Chunk handler for the GCF_ chunk. The Group Centre Frequency chunk
// stores the centre frequency of each channel in the group.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $
// Date:       $LastChangedDate: 2010-09-17 19:30:37 +0200 (Fri, 17 Sep 2010) $
// Revision:   $LastChangedRevision: 39901 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerGCF_.cpp $
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#ifdef __unix__
#include <string.h>
#endif

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
#ifndef NO_LOG4CXX
#include "log4cxx/logger.h"
#endif
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "ChunkHandlerGCF_.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"

namespace pxgf {

    using namespace std;

#ifndef NO_LOG4CXX
	static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.handlers.ChunkHandlerGCF_"));
#endif

	//=================================================================================
	//	cChunkHandlerGCF_::cChunkHandlerGCF_
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
	cChunkHandlerGCF_::cChunkHandlerGCF_()
      : m_pCallbackGCF_Handler(0)
    {}


	//=================================================================================
	//	cChunkHandlerGCF_::registerCallbackHandler
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
	void cChunkHandlerGCF_::registerCallbackHandler(cCallbackGCF_* pHandler)
	{
        m_pCallbackGCF_Handler = pHandler;
    }


	//=================================================================================
	//	cChunkHandlerGCF_::processChunk
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
	bool cChunkHandlerGCF_::processChunk(const vector<char>& vchData, bool bBigEndian)
	{
        if (m_pCallbackGCF_Handler == 0) {
#ifndef NO_LOG4CXX
			LOG4CXX_WARN(g_logger, "Callback not initialized.");
#endif
            return true;
        }

        int iNumChannels = *(int*)&vchData[0];
        if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
            SWAP_INT(iNumChannels);

        // check to see if there is the correct amount of data in the block
	if (iNumChannels != int(vchData.size() - 4)/8 ) {
#ifndef NO_LOG4CXX
		LOG4CXX_ERROR(g_logger, "Could not process chunk due to incorrect length.");
#endif
		return false;
	}

        vector<int64_t> vlCentreFrequencies_microHz (iNumChannels);
        memcpy(&vlCentreFrequencies_microHz[0], &vchData[4], iNumChannels * sizeof(int64_t));
        if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
            for (vector<int64_t>::iterator it = vlCentreFrequencies_microHz.begin(); it != vlCentreFrequencies_microHz.end(); it++)
                SWAP_INT64(*it);
        }

        m_pCallbackGCF_Handler->callbackGCF_(vlCentreFrequencies_microHz);
        return true;
    }


	//=================================================================================
	//	cChunkHandlerGCF_::writeChunkGCF_
	//--------------------------------------------------------------------------------
	//	Write a GCF_ chunk to the given stream writer.
	//
	//	Parameter					Dir	Description
	//	---------					---	-----------
	//	stream						IN	is the output stream to which the chunk is written.
	//	alCentreFrequencies_microHz	IN	an array of the the centre frequencies for each
	//									 channel in the group.
	//
	//=================================================================================
	void cChunkHandlerGCF_::writeChunkGCF_(cPXGWriterBase& stream, const vector<int64_t>& vlCentreFrequencies_microHz) const
	{
        stream.writeChunkHeader(*this, 4 + 8*vlCentreFrequencies_microHz.size());
        stream.writeInteger(vlCentreFrequencies_microHz.size());
        for (unsigned uChannel = 0; uChannel < vlCentreFrequencies_microHz.size(); uChannel++)
            stream.writeInt64(vlCentreFrequencies_microHz[uChannel]);
    }


	//=================================================================================
	//	cChunkHandlerGCF_::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerGCF_::getChunkType() const
	{
        //"GCF_"
        return 0x4743465F;
    }

}
