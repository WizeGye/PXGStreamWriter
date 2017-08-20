// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandler_dBTG.cpp
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
// Chunk handler for the dBTG chunk. The dB Total Gain chunk stores the
// total gain between the antenna and the A/D.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandler_dBTG.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
#ifndef NO_LOG4CXX
#include "log4cxx/logger.h"
#endif
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "ChunkHandler_dBTG.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"

namespace pxgf {

    using namespace std;

#ifndef NO_LOG4CXX
	static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.handlers.ChunkHandler_dBTG"));
#endif

	//=================================================================================
	//	cChunkHandler_dBTG::cChunkHandler_dBTG
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
    cChunkHandler_dBTG::cChunkHandler_dBTG()
      : m_pCallback_dBTGHandler(0)
    {}
    
    
	//=================================================================================
	//	cChunkHandler_dBTG::registerCallbackHandler
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
	void cChunkHandler_dBTG::registerCallbackHandler(cCallback_dBTG* pHandler) 
	{
        m_pCallback_dBTGHandler = pHandler;
    } 
    
    
	//=================================================================================
	//	cChunkHandler_dBTG::processChunk
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
	bool cChunkHandler_dBTG::processChunk(const vector<char>& vchData, bool bBigEndian) 
	{
        if (vchData.size() != 4) {
#ifndef NO_LOG4CXX
			LOG4CXX_ERROR(g_logger, "Could not process chunk due to incorrect length.");
#endif
			return false; 
		}
        if (m_pCallback_dBTGHandler == 0) {
#ifndef NO_LOG4CXX
			LOG4CXX_WARN(g_logger, "Callback not initialized.");
#endif
            return true;
        }
        
        float fTotalGain_dB = *(float*)&vchData[0];
        if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
            SWAP_FLOAT(fTotalGain_dB);
        
        m_pCallback_dBTGHandler->callback_dBTG(fTotalGain_dB);
        return true; 
    } 
    
    
	//=================================================================================
	//	cChunkHandler_dBTG::writeChunk_dBTG
	//--------------------------------------------------------------------------------
	//	Write a dBTG chunk to the given stream writer. 
	//
	//	Parameter		Dir		Description
	//	---------		---		-----------
	//	stream			IN		is the output stream to which the chunk is written.
	//	fTotalGain_dB	IN		is the total gain from the antenna to the input of the A/D
	//
	//=================================================================================
	void cChunkHandler_dBTG::writeChunk_dBTG(cPXGWriterBase& stream, float fTotalGain_dB) const 
	{
        stream.writeChunkHeader(*this, 4);
        stream.writeFloat(fTotalGain_dB);
    } 
    
    
	//=================================================================================
	//	cChunkHandler_dBTG::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandler_dBTG::getChunkType() const 
	{
        //"_dBTG"
        return 0x64425447;
    } 
    
}
