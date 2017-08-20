// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerSSIQ.cpp
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
// Chunk handler for the SSIQ chunk. The Single channel Short IQ chunk
// stores the sampled IQ data.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: ryan $
// Date:       $LastChangedDate: 2009-12-08 10:38:34 +0200 (Tue, 08 Dec 2009) $
// Revision:   $LastChangedRevision: 36884 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerSSIQ.cpp $
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
#include "ChunkHandlerSSIQ.h"
#include "PxgfHandlerException.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"

namespace pxgf {

    using namespace std;

#ifndef NO_LOG4CXX
	static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.handlers.ChunkHandlerSSIQ"));
#endif

	//=================================================================================
	//	cChunkHandlerSSIQ::cChunkHandlerSSIQ
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
	cChunkHandlerSSIQ::cChunkHandlerSSIQ()
      : m_pCallbackSSIQHandler(0)
    {}


	//=================================================================================
	//	cChunkHandlerSSIQ::registerCallbackHandler
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
	void cChunkHandlerSSIQ::registerCallbackHandler(cCallbackSSIQ* pHandler)
	{
        m_pCallbackSSIQHandler = pHandler;
    }


	//=================================================================================
	//	cChunkHandlerSSIQ::processChunk
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
	bool cChunkHandlerSSIQ::processChunk(const vector<char>& vchData, bool bBigEndian)
	{
        // check that there is enough data for an even number of shorts
        if (((vchData.size()) & 0x3) != 0) return false;
        if (m_pCallbackSSIQHandler == 0) {
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

        m_pCallbackSSIQHandler->callbackSSIQ(lTimestamp_microSec, m_vsIqData);
        return true;
    }


	//=================================================================================
	//	cChunkHandlerSSIQ::writeChunkSSIQ
	//--------------------------------------------------------------------------------
	//	Write a SSIQ chunk to the given stream writer.
	//
	//	Parameter		Dir		Description
	//	---------		---		-----------
	//	stream			IN		is the output stream to which the chunk is written.
	//	lTimestamp_us 	IN		is the timestamp in micro seconds measured from the
	//							 beginning of the epoch.
	//	vsIqData 		IN		is the array of IQ data stored using shorts.
	//
	//=================================================================================
	void cChunkHandlerSSIQ::writeChunkSSIQ(cPXGWriterBase& stream, int64_t lTimestamp_microSec, const vector<short>& vsIqData) const
	{
        stream.writeChunkHeader(*this, 8 + vsIqData.size() * 2);
        stream.writeInt64(lTimestamp_microSec);
        stream.writeShortArray(vsIqData);
    }


	//=================================================================================
	//	cChunkHandlerSSIQ::repack
	//--------------------------------------------------------------------------------
	//	Repack the data from the packing specified to the output packing specified.
	//
	//	Parameter		Dir		Description
	//	---------		---		-----------
	//	oInputPacking	IN		the packing that the data currently has.
	//	oOutputPacking	IN		the desired packing for the data.
	//	awInputIQData	IN		is the input data to be reordered.
	//	awOutputIQData	OUT		is the output vector.
	//
	//=================================================================================
	void cChunkHandlerSSIQ::repack(const cPackingSIQP& oInputPacking, const cPackingSIQP& oOutputPacking, const vector<short>& vsInputIQData, vector<short>& vsOutputIQData)
	{
        if (&oInputPacking == 0 || &oOutputPacking == 0)
            throw cPxgfHandlerException("Neither input nor output packing may be null!");
        if (oInputPacking.equals(oOutputPacking)) {
            vsOutputIQData = vsInputIQData;
            return;
        }
        // check that there are an even number of elements
        if ((vsInputIQData.size() & 0x1) != 0)
            throw cPxgfHandlerException("The length of vsInputIQData must be even.");

        // swap IQ data
        vsOutputIQData.resize(vsInputIQData.size());
        for (unsigned uSample = 0; uSample < vsInputIQData.size(); uSample+=2)
        {
            vsOutputIQData[uSample] = vsInputIQData[uSample + 1];
            vsOutputIQData[uSample + 1] = vsInputIQData[uSample];
        }
    }


	//=================================================================================
	//	cChunkHandlerSSIQ::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerSSIQ::getChunkType() const
	{
        //"SSIQ"
        return 0x53534951;
    }

}
