// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerSIQP.cpp
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
// Chunk handler for the SIQP chunk. The Single channel IQ Packing chunk
// stores the packing used for the IQData in SSIQ chunks.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerSIQP.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#include <vector>

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
#ifndef NO_LOG4CXX
#include "log4cxx/logger.h"
#endif
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "ChunkHandlerSIQP.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"

namespace pxgf {

	using namespace std;

#ifndef NO_LOG4CXX
	static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.handlers.ChunkHandlerSIQP"));
#endif

	//=================================================================================
	//	cChunkHandlerSIQP::cChunkHandlerSIQP
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
	cChunkHandlerSIQP::cChunkHandlerSIQP()
      : m_pCallbackSIQPHandler(0),
        m_oIQPacking(true),
        m_oQIPacking(false)
    {}
    
    
	//=================================================================================
	//	cChunkHandlerSIQP::registerCallbackHandler
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
	void cChunkHandlerSIQP::registerCallbackHandler(cCallbackSIQP* pHandler) 
	{
        m_pCallbackSIQPHandler = pHandler;
    } 
    
    
	//=================================================================================
	//	cChunkHandlerSIQP::processChunk
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
	bool cChunkHandlerSIQP::processChunk(const vector<char>& vchData, bool bBigEndian)
	{
        if (vchData.size() != 4) {
#ifndef NO_LOG4CXX
			LOG4CXX_ERROR(g_logger, "Could not process chunk due to incorrect length.");
#endif
			return false; 
		}
        if (m_pCallbackSIQPHandler == 0) {
#ifndef NO_LOG4CXX
			LOG4CXX_WARN(g_logger, "Callback not initialized.");
#endif
            return true;
        }
        
        int iValue = *(int*)&vchData[0];
        if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
            SWAP_INT(iValue);

        switch(iValue) {
        case 0: m_pCallbackSIQPHandler->callbackSIQP(m_oQIPacking); break;
        case 1: m_pCallbackSIQPHandler->callbackSIQP(m_oIQPacking); break;
        default: return false;
        }
        
        return true; 
    } 
    
    
	//=================================================================================
	//	cChunkHandlerSIQP::writeChunkSIQP
	//--------------------------------------------------------------------------------
	//	Write a SIQP chunk to the given stream writer. 
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	stream		IN		is the output stream to which the chunk is written.
	//	oPacking	IN		is object describing the packing order used for the data chunks.
	//
	//=================================================================================
	void cChunkHandlerSIQP::writeChunkSIQP(cPXGWriterBase& stream, const cPackingSIQP& oPacking) const 
	{
        stream.writeChunkHeader(*this, 4);
        if (oPacking.getIsIQPacked())
            stream.writeInteger(1);
        else
            stream.writeInteger(0);
    } 
    
    
	//=================================================================================
	//	cChunkHandlerSIQP::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerSIQP::getChunkType() const 
	{
        //"SIQP"
        return 0x53495150;
    } 
    
}
