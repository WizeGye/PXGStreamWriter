// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerBWOF.cpp
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
// Chunk handler for the BandWidth Offset Frequency (BWOF) chunk. This 
// chunk specifies the bandwidth along with the offset frequency of the signal
// band from the centre frequency in microHz.
// 
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2010-09-17 19:30:37 +0200 (Fri, 17 Sep 2010) $ 
// Revision:   $LastChangedRevision: 39901 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerBWOF.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
#ifndef NO_LOG4CXX
#include "log4cxx/logger.h"
#endif
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "ChunkHandlerBWOF.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"

namespace pxgf {

    using namespace std;

#ifndef NO_LOG4CXX
	static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.handlers.ChunkHandlerBWOF"));
#endif

	//=================================================================================
	//	cChunkHandlerBWOF::cChunkHandlerBWOF
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
    cChunkHandlerBWOF::cChunkHandlerBWOF()
      : m_pCallbackBWOFHandler(0)
    {}
    
    
	//=================================================================================
	//	cChunkHandlerBWOF::registerCallbackHandler
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
	void cChunkHandlerBWOF::registerCallbackHandler(cCallbackBWOF* pHandler) 
	{
        m_pCallbackBWOFHandler = pHandler;
    } 
    
    
	//=================================================================================
	//	cChunkHandlerBWOF::processChunk
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
	bool cChunkHandlerBWOF::processChunk(const vector<char>& vchData, bool bBigEndian) 
	{
        if (vchData.size() != 16) {
#ifndef NO_LOG4CXX
			LOG4CXX_ERROR(g_logger, "Could not process chunk due to incorrect length.");
#endif
			return false; 
		}
        if (m_pCallbackBWOFHandler == 0) {
#ifndef NO_LOG4CXX
			LOG4CXX_WARN(g_logger, "Callback not initialized.");
#endif
            return true;
        }
        
        int64_t lBandwidth_microHz = *(int64_t*)&vchData[0];
		int64_t lOffsetFrequency_microHz = *(int64_t*)&vchData[8];
        if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
            SWAP_INT64(lBandwidth_microHz);
			SWAP_INT64(lOffsetFrequency_microHz);
		}
        
        m_pCallbackBWOFHandler->callbackBWOF(lBandwidth_microHz, lOffsetFrequency_microHz);
        return true; 
    } 
    
    
	//=================================================================================
	//	cChunkHandlerBWOF::writeChunkBWOF
	//--------------------------------------------------------------------------------
	//	Write a BWOF chunk to the given stream writer. 
	//
	//	Parameter				Dir		Description
	//	---------				---		-----------
	//	stream					IN		is the output stream to which the chunk is written.
	//	lBandwidth_uHz			IN		is the Bandwidth of the channel.
	//	lOffsetFrequency_uHz	IN		is the offset frequency of the channel from the 
	//									 centre frequency.
	//
	//=================================================================================
	void cChunkHandlerBWOF::writeChunkBWOF(cPXGWriterBase& stream, int64_t lBandwidth_microHz, int64_t lOffsetFrequency_microHz) const 
	{
        stream.writeChunkHeader(*this, 16);
        stream.writeInt64(lBandwidth_microHz);
		stream.writeInt64(lOffsetFrequency_microHz);
    } 
    
    
	//=================================================================================
	//	cChunkHandlerBWOF::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerBWOF::getChunkType() const 
	{
        //"BWOF"
        return 0x42574F46;
    } 
    
}
