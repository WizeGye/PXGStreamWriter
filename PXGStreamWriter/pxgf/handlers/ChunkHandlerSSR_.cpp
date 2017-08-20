// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerSSR_.cpp
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
// Chunk handler for the SSR_ chunk. The Single channel Short IQ chunk
// stores the sampled IQ data.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: ryan $
// Date:       $LastChangedDate: 2009-12-08 10:38:34 +0200 (Tue, 08 Dec 2009) $
// Revision:   $LastChangedRevision: 36884 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerSSR_.cpp $
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
#include "ChunkHandlerSSR_.h"
#include "PxgfHandlerException.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"

namespace pxgf {

    using namespace std;

#ifndef NO_LOG4CXX
	static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.handlers.ChunkHandlerSSR_"));
#endif

	//=================================================================================
	//	cChunkHandlerSSR_::cChunkHandlerSSR_
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
	cChunkHandlerSSR_::cChunkHandlerSSR_()
      : m_pCallbackSSR_Handler(0)
    {}


	//=================================================================================
	//	cChunkHandlerSSR_::registerCallbackHandler
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
	void cChunkHandlerSSR_::registerCallbackHandler(cCallbackSSR_* pHandler)
	{
        m_pCallbackSSR_Handler = pHandler;
    }


	//=================================================================================
	//	cChunkHandlerSSR_::processChunk
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
	bool cChunkHandlerSSR_::processChunk(const vector<char>& vchData, bool bBigEndian)
	{
        // check that there is enough data for an even number of shorts
        if (((vchData.size()) & 0x3) != 0) return false;
        if (m_pCallbackSSR_Handler == 0) {
#ifndef NO_LOG4CXX
			LOG4CXX_WARN(g_logger, "Callback not initialized.");
#endif
            return true;
        }

        int64_t lTimestamp_microSec = *(int64_t*)&vchData[0];
        int iLength = (vchData.size() - 8) / 2;
        //vector<short> vsIqData (iLength);
		m_vsIqData.resize (iLength);
        memcpy(&m_vsIqData[0], &vchData[8], iLength * sizeof(short));
        if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
            SWAP_INT64(lTimestamp_microSec);
            for (vector<short>::iterator it = m_vsIqData.begin(); it != m_vsIqData.end(); it++)
                SWAP_SHORT(*it);
        }

        m_pCallbackSSR_Handler->callbackSSR_(lTimestamp_microSec, m_vsIqData);
        return true;
    }


	//=================================================================================
	//	cChunkHandlerSSR_::writeChunkSSR_
	//--------------------------------------------------------------------------------
	//	Write a SSR_ chunk to the given stream writer.
	//
	//	Parameter		Dir		Description
	//	---------		---		-----------
	//	stream			IN		is the output stream to which the chunk is written.
	//	lTimestamp_us 	IN		is the timestamp in micro seconds measured from the
	//							 beginning of the epoch.
	//	awIqData 		IN		is the array of IQ data stored using shorts.
	//
	//=================================================================================
	void cChunkHandlerSSR_::writeChunkSSR_(cPXGWriterBase& stream, int64_t lTimestamp_microSec, const vector<short>& vsRealData) const
	{
        stream.writeChunkHeader(*this, 8 + vsRealData.size() * 2);
        stream.writeInt64(lTimestamp_microSec);

		// check that there are an even number of elements
        if ((vsRealData.size() & 0x1) != 0)
            throw cPxgfHandlerException("The length of vsIqData must be even.");
		stream.writeShortArray(vsRealData);
    }


	//=================================================================================
	//	cChunkHandlerSSR_::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerSSR_::getChunkType() const
	{
        //"SSR_"
        return 0x5353525f;
    }

}
