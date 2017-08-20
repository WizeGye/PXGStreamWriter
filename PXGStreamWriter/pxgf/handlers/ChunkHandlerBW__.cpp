// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerBW__.cpp
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
// Chunk handler for the BW__ chunk which holds the bandwidth of the signal 
// stored in micro Hz. It is strongly recommended that this chunk be sent if the 
// bandwidth is available. The bandwidth must be less than or equal to the 
// sample rate.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerBW__.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
#ifndef NO_LOG4CXX
#include "log4cxx/logger.h"
#endif
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "ChunkHandlerBW__.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"

namespace pxgf {

    using namespace std;

#ifndef NO_LOG4CXX
	static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.handlers.ChunkHandlerBW__"));
#endif

	//=================================================================================
	//	cChunkHandlerBW__::cChunkHandlerBW__
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
    cChunkHandlerBW__::cChunkHandlerBW__()
      : m_pCallbackBW__Handler(0)
    {}
    
    
	//=================================================================================
	//	cChunkHandlerBW__::registerCallbackHandler
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
	void cChunkHandlerBW__::registerCallbackHandler(cCallbackBW__* pHandler) 
	{
	    m_pCallbackBW__Handler = pHandler;
	} 
	
	
	//=================================================================================
	//	cChunkHandlerBW__::processChunk
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
	bool cChunkHandlerBW__::processChunk(const vector<char>& vchData, bool bBigEndian) 
	{
	    if (vchData.size() != 8) {
#ifndef NO_LOG4CXX
			LOG4CXX_ERROR(g_logger, "Could not process chunk due to incorrect length.");
#endif
			return false; 
		}
	    if (m_pCallbackBW__Handler == 0) {
#ifndef NO_LOG4CXX
			LOG4CXX_WARN(g_logger, "Callback not initialized.");
#endif
	        return true;
	    }
	    
	    int64_t lBandwidth_microHz = *(int64_t*)&vchData[0];
	    if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
	        SWAP_INT64(lBandwidth_microHz);
	    
	    m_pCallbackBW__Handler->callbackBW__(lBandwidth_microHz);
	    return true; 
	} 
	
	
	//=================================================================================
	//	cChunkHandlerBW__::writeChunkBW__
	//--------------------------------------------------------------------------------
	//	Write a BW__ chunk to the given stream writer. 
	//
	//	Parameter			Dir		Description
	//	---------			---		-----------
	//	stream				IN		is the output stream to which the chunk is written.
	//	lBandwidth_microHz	IN		is the Bandwidth of the channel. 
	//
	//=================================================================================
	void cChunkHandlerBW__::writeChunkBW__(cPXGWriterBase& stream, int64_t lBandwidth_microHz) const 
	{
	    stream.writeChunkHeader(*this, 8);
	    stream.writeInt64(lBandwidth_microHz);
	} 
	
	
	//=================================================================================
	//	cChunkHandlerBW__::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerBW__::getChunkType() const 
	{
	    //"BW__"
	    return 0x42575f5f;
	} 
    
}
