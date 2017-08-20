// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerGSIQ.cpp
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
// Chunk handler for the GSIQ chunk. The Group Short IQ chunk stores the
// packed IQ data in the packing defined by the GIQP chunk.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $
// Date:       $LastChangedDate: 2012-02-29 10:37:00 +0200 (Wed, 29 Feb 2012) $
// Revision:   $LastChangedRevision: 44745 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerGSIQ.cpp $
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
#include "ChunkHandlerGSIQ.h"
#include "PxgfHandlerException.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"

namespace pxgf {

    using namespace std;

#ifndef NO_LOG4CXX
	static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.handlers.ChunkHandlerGSIQ"));
#endif

	//=================================================================================
	//	cChunkHandlerGSIQ::cChunkHandlerGSIQ
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
	cChunkHandlerGSIQ::cChunkHandlerGSIQ()
      : m_pCallbackGSIQHandler(0)
    {}


	//=================================================================================
	//	cChunkHandlerGSIQ::registerCallbackHandler
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
	void cChunkHandlerGSIQ::registerCallbackHandler(cCallbackGSIQ* pHandler)
	{
        m_pCallbackGSIQHandler = pHandler;
    }


	//=================================================================================
	//	cChunkHandlerGSIQ::processChunk
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
	bool cChunkHandlerGSIQ::processChunk(const vector<char>& vchData, bool bBigEndian)
	{
        // check that there is enough data for an even number of shorts
        if (((vchData.size()) & 0x3) != 0) return false;
        if (m_pCallbackGSIQHandler == 0) {
#ifndef NO_LOG4CXX
			LOG4CXX_WARN(g_logger, "Callback not initialized.");
#endif
            return true;
        }

        int64_t lTimestamp_microSec = *(int64_t*)&vchData[0];
        int iLength = (vchData.size() - 8) / 2;
        vector<short> vsIqData (iLength);
        memcpy(&vsIqData[0], &vchData[8], iLength * sizeof(short));
        if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
            SWAP_INT64(lTimestamp_microSec);
            for (vector<short>::iterator it = vsIqData.begin(); it != vsIqData.end(); it++)
                SWAP_SHORT(*it);
        }

        m_pCallbackGSIQHandler->callbackGSIQ(lTimestamp_microSec, vsIqData);
        return true;
    }


	//=================================================================================
	//	cChunkHandlerGSIQ::writeChunkGSIQ
	//--------------------------------------------------------------------------------
	//	Write a GSIQ chunk to the given stream writer.
	//
	//	Parameter		Dir		Description
	//	---------		---		-----------
	//	stream			IN		is the output stream to which the chunk is written.
	//	lTimestamp_us 	IN		is the timestamp in micro seconds measured from the
	//							 beginning of the epoch.
	//	vsIqData 		IN		is the array of IQ data stored using shorts.
	//
	//=================================================================================
	void cChunkHandlerGSIQ::writeChunkGSIQ(cPXGWriterBase& stream, int64_t lTimestamp_microSec, const vector<short>& vsIqData) const
	{
        stream.writeChunkHeader(*this, 8 + vsIqData.size() * 2);
        stream.writeInt64(lTimestamp_microSec);
        stream.writeShortArray(vsIqData);
    }


	//=================================================================================
	//	cChunkHandlerGSIQ::repack
	//--------------------------------------------------------------------------------
	//	Repack the data from the packing specified to the output packing specified.
	//
	//	Parameter		Dir		Description
	//	---------		---		-----------
	//	oInputPacking	IN		the packing that the data currently has.
	//	oOutputPacking	IN		the desired packing for the data.
	//	vsInputIQData	IN		is the input data to be reordered.
	//	vsOutputIQData	OUT		is the output vector.
	//
	//=================================================================================
	void cChunkHandlerGSIQ::repack(const cPackingGIQP& oInputPacking, const cPackingGIQP& oOutputPacking, const vector<short>& vsInputIQData, vector<short>& vsOutputIQData)
	{
        if (&oInputPacking == 0 || &oOutputPacking == 0)
            throw cPxgfHandlerException("Neither input nor output packing may be null!");
        if (oInputPacking.getNumChannels() != oOutputPacking.getNumChannels())
            throw new cPxgfHandlerException("Packing definitions are not compatible!");
        if (oInputPacking.equals(oOutputPacking)) {
            vsOutputIQData = vsInputIQData;
            return;
        }
        // check that there are an even number of elements
        if ((vsInputIQData.size() & 0x1) != 0)
            throw cPxgfHandlerException("The length of awInputIQData must be even.");
        vsOutputIQData.resize(vsInputIQData.size());
        int iNumChannels = oInputPacking.getNumChannels();
        int iNumSamples = vsInputIQData.size() / (iNumChannels * 2);
        int iInputIncr = oInputPacking.getIncrement()*2;
        int iOutputIncr = oOutputPacking.getIncrement()*2;
        int iInputIndex,iOutputIndex;
        for (int iChannel = 0; iChannel < iNumChannels; iChannel++) {
            iInputIndex = oInputPacking.getChannelOffsets()[iChannel]*2;
            iOutputIndex = oOutputPacking.getChannelOffsets()[iChannel]*2;
            for (int iSample = 0; iSample < iNumSamples; iSample++) {
                vsOutputIQData[iOutputIndex] = vsInputIQData[iInputIndex];
				vsOutputIQData[iOutputIndex + 1] = vsInputIQData[iInputIndex + 1];
                iInputIndex += iInputIncr;
                iOutputIndex += iOutputIncr;
            }
        }
        // swap IQ data
        if (oInputPacking.getIsIQPacked() != oOutputPacking.getIsIQPacked()) {
            short sSwap;
            for (unsigned uSample = 0; uSample < vsOutputIQData.size(); uSample+=2) {
                sSwap = vsOutputIQData[uSample];
                vsOutputIQData[uSample] = vsOutputIQData[uSample + 1];
                vsOutputIQData[uSample + 1] = sSwap;
            }
        }
    }


	//=================================================================================
	//	cChunkHandlerGSIQ::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerGSIQ::getChunkType() const
	{
        //"GSIQ"
        return 0x47534951;
    }

}
