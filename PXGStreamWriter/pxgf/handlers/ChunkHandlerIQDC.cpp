// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerIQDC.cpp
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
// Chunk handler for the IQDC chunk. The IQ DisContinuity chunk acts as
// an indicator that there has been a discontinuity in the data.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerIQDC.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
#ifndef NO_LOG4CXX
#include "log4cxx/logger.h"
#endif
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "ChunkHandlerIQDC.h"

namespace pxgf {

    using namespace std;

#ifndef NO_LOG4CXX
	static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.handlers.ChunkHandlerIQDC"));
#endif

	//=================================================================================
	//	cChunkHandlerIQDC::cChunkHandlerIQDC
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
	cChunkHandlerIQDC::cChunkHandlerIQDC()
      : m_pCallbackIQDCHandler(0)
    {}
    
    
	//=================================================================================
	//	cChunkHandlerIQDC::registerCallbackHandler
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
	void cChunkHandlerIQDC::registerCallbackHandler(cCallbackIQDC* pHandler) 
	{
        m_pCallbackIQDCHandler = pHandler;
    } 
    
    
	//=================================================================================
	//	cChunkHandlerIQDC::processChunk
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
	bool cChunkHandlerIQDC::processChunk(const vector<char>& vchData, bool bBigEndian) 
	{
        if (vchData.size() != 0) {
#ifndef NO_LOG4CXX
			LOG4CXX_ERROR(g_logger, "Could not process chunk due to incorrect length.");
#endif
			return false; 
		}
        if (m_pCallbackIQDCHandler == 0) {
#ifndef NO_LOG4CXX
			LOG4CXX_WARN(g_logger, "Callback not initialized.");
#endif
            return true;
        }
        
        m_pCallbackIQDCHandler->callbackIQDC();
        return true; 
    } 
    
    
	//=================================================================================
	//	cChunkHandlerIQDC::writeChunkIQDC
	//--------------------------------------------------------------------------------
	//	Write a IQDC chunk to the given stream writer. 
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	stream		IN		is the output stream to which the chunk is written.
	//
	//=================================================================================
	void cChunkHandlerIQDC::writeChunkIQDC(cPXGWriterBase& stream) const 
	{
        stream.writeChunkHeader(*this, 0);
    } 
    
    
	//=================================================================================
	//	cChunkHandlerIQDC::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerIQDC::getChunkType() const 
	{
        //"IQDC"
        return 0x49514443;
    } 
    
}
