// ---------------------------------------------------------------------------
// FILE NAME			: ChunkHandlerGRG_.cpp
// ---------------------------------------------------------------------------
// COMPANY			  : PERALEX ELECTRONICS (PTY) LTD
// ---------------------------------------------------------------------------
// COPYRIGHT NOTICE :
//
// The copyright, manufacturing and patent rights stemming from this document
// in any form are vested in PERALEX ELECTRONICS (PTY) LTD.
//
// (c) PERALEX ELECTRONICS (PTY) LTD 2013
//
// PERALEX ELECTRONICS (PTY) LTD has ceded these rights to its clients
// where contractually agreed.
// ---------------------------------------------------------------------------
// DESCRIPTION :
//
// Chunk handler for the GRG_ chunk. The Group Relative Gain is the total analogue gain in dB
// of each channel from the input of the receiver (usually an antenna) to the input of the ADC,
// relative to the dBTG chunk, which would be the total analogue gain of one of the channels.
// Therefore, the total analogue gain for a particular channel is equal to the sum of the value
// contained in this chunk and the value contained in the dBTG chunk.
//
// ---------------------------------------------------------------------------
// Author:	 $LastChangedBy: ross $
// Date:	   $LastChangedDate: 2013-09-12 17:22:13 +0200 (Thu, 12 Sep 2013) $
// Revision:   $LastChangedRevision: 51694 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerGRG_.cpp $
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
#include "ChunkHandlerGRG_.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"

namespace pxgf {

	using namespace std;

#ifndef NO_LOG4CXX
	static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.handlers.ChunkHandlerGRG_"));
#endif

	//=================================================================================
	//	cChunkHandlerGRG_::cChunkHandlerGRG_
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
	cChunkHandlerGRG_::cChunkHandlerGRG_()
	  : m_pCallbackGRG_Handler(0)
	{}


	//=================================================================================
	//	cChunkHandlerGRG_::registerCallbackHandler
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
	void cChunkHandlerGRG_::registerCallbackHandler(cCallbackGRG_* pHandler)
	{
		m_pCallbackGRG_Handler = pHandler;
	}


	//=================================================================================
	//	cChunkHandlerGRG_::processChunk
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
	bool cChunkHandlerGRG_::processChunk(const vector<char>& vchData, bool bBigEndian)
	{
		if (m_pCallbackGRG_Handler == 0) {
#ifndef NO_LOG4CXX
			LOG4CXX_WARN(g_logger, "Callback not initialized.");
#endif
			return true;
		}

		int iNumChannels = *(int*)&vchData[0];
		if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT(iNumChannels);

		// check to see if there is the correct amount of data in the block
		if (iNumChannels != int(vchData.size() - 4)/4 ) {
#ifndef NO_LOG4CXX
			LOG4CXX_ERROR(g_logger, "Could not process chunk due to incorrect length.");
#endif
			return false;
		}

		vector<float> vfGains_dB(iNumChannels);
		memcpy(&vfGains_dB[0], &vchData[4], iNumChannels * sizeof(float));
		if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
			for (vector<float>::iterator it = vfGains_dB.begin(); it != vfGains_dB.end(); it++)
				SWAP_FLOAT(*it);
		}

		m_pCallbackGRG_Handler->callbackGRG_(vfGains_dB);
		return true;
	}


	//=================================================================================
	//	cChunkHandlerGRG_::writeChunkGRG_
	//--------------------------------------------------------------------------------
	//	Write a GRG_ chunk to the given stream writer.
	//
	//	Parameter		Dir		Description
	//	---------		---		-----------
	//	stream			IN		is the output stream to which the chunk is written.
	//	vfGains_dB		IN		is the vector of total gains for each channel from the
	//							antenna to the input of the A/D
	//
	//=================================================================================
	void cChunkHandlerGRG_::writeChunkGRG_(cPXGWriterBase& stream, vector<float> vfGains_dB) const
	{
		stream.writeChunkHeader(*this, 4 + 4*vfGains_dB.size());
		stream.writeInteger(vfGains_dB.size());
		for (unsigned uChannel = 0; uChannel < vfGains_dB.size(); uChannel++)
			stream.writeFloat(vfGains_dB[uChannel]);
	}


	//=================================================================================
	//	cChunkHandlerGRG_::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerGRG_::getChunkType() const
	{
		//"GRG_"
		return 0x4752475f;
	}

}
