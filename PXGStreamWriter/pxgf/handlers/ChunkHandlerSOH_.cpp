// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerSOH_.cpp
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
// Chunk handler for the SOH_ chunk. The Start Of Header chunk should be the
// first chunk in a file. It is used to indicate the start of the header of the file
// and to give the format used in the file which is the chunk ID of the primary data chunk
// used for that stream. An EOH_ chunk must be send to indicate the end of the header.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerSOH_.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
#ifndef NO_LOG4CXX
#include "log4cxx/logger.h"
#endif
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "ChunkHandlerSOH_.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"

namespace pxgf {

    using namespace std;

#ifndef NO_LOG4CXX
	static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.handlers.ChunkHandlerSOH_"));
#endif

	//=================================================================================
	//	cChunkHandlerSOH_::cChunkHandlerSOH_
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
	cChunkHandlerSOH_::cChunkHandlerSOH_()
      : m_pCallbackSOH_Handler(0)
    {}
    
    
	//=================================================================================
	//	cChunkHandlerSOH_::registerCallbackHandler
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
	void cChunkHandlerSOH_::registerCallbackHandler(cCallbackSOH_* pHandler) 
	{
        m_pCallbackSOH_Handler = pHandler;
    } 
    
    
	//=================================================================================
	//	cChunkHandlerSOH_::processChunk
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
	bool cChunkHandlerSOH_::processChunk(const vector<char>& vchData, bool bBigEndian)
	{
        if (vchData.size() != 4) {
#ifndef NO_LOG4CXX
			LOG4CXX_ERROR(g_logger, "Could not process chunk due to incorrect length.");
#endif
			return false; 
		}
        if (m_pCallbackSOH_Handler == 0) {
#ifndef NO_LOG4CXX
			LOG4CXX_WARN(g_logger, "Callback not initialized.");
#endif
            return true;
        }
        
        int iFormat = *(int*)&vchData[0];
        if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
            SWAP_INT(iFormat);
        
        m_pCallbackSOH_Handler->callbackSOH_(iFormat);
        return true; 
    } 
    
    
	//=================================================================================
	//	cChunkHandlerSOH_::writeChunkSOH_
	//--------------------------------------------------------------------------------
	//	Write a SOH_ chunk to the given stream writer. 
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	stream		IN		is the output stream to which the chunk is written.
	//	iFormat		IN		is a number describing the format used. It is recommended 
	//						 that the chunk type of the primary data chunk be used.
	//
	//=================================================================================
	void cChunkHandlerSOH_::writeChunkSOH_(cPXGWriterBase& stream, int iFormat) const 
	{
        stream.writeChunkHeader(*this, 4);
        stream.writeInteger(iFormat);
    } 
    
    
	//=================================================================================
	//	cChunkHandlerSOH_::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerSOH_::getChunkType() const 
	{
        //"SOFH"
        return 0x534F4648;
    } 
    
}
