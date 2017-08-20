// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerSOFH.cpp
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
// Chunk handler for the SOFH chunk. The Start Of Header chunk should be the
// first chunk in a file. It is used to indicate the start of the header of the file
// and to give the format used in the file which is the chunk ID of the primary data chunk
// used for that stream. An EOH_ chunk must be send to indicate the end of the header.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2013-11-29 15:06:40 +0200 (Fri, 29 Nov 2013) $ 
// Revision:   $LastChangedRevision: 52524 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerSOFH.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
#ifndef NO_LOG4CXX
#include "log4cxx/logger.h"
#endif
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "ChunkHandlerSOFH.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"

namespace pxgf {

    using namespace std;
	//=================================================================================
	//	cChunkHandlerSOFH::cChunkHandlerSOFH
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
	cChunkHandlerSOFH::cChunkHandlerSOFH()
      : m_pCallbackSOFHHandler(0)
    {}
    
    
	//=================================================================================
	//	cChunkHandlerSOFH::registerCallbackHandler
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
	void cChunkHandlerSOFH::registerCallbackHandler(cCallbackSOFH* pHandler) 
	{
        m_pCallbackSOFHHandler = pHandler;
    } 
    
    
	//=================================================================================
	//	cChunkHandlerSOFH::processChunk
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
	bool cChunkHandlerSOFH::processChunk(const vector<char>& vchData, bool bBigEndian)
	{
        if (vchData.size() != 4) {
			return false; 
		}
        if (m_pCallbackSOFHHandler == 0) {
            return true;
        }
        
        int iFormat = *(int*)&vchData[0];
        if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
            SWAP_INT(iFormat);
        
        m_pCallbackSOFHHandler->callbackSOFH(iFormat);
        return true; 
    } 
    
    
	//=================================================================================
	//	cChunkHandlerSOFH::writeChunkSOFH
	//--------------------------------------------------------------------------------
	//	Write a SOFH chunk to the given stream writer. 
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	stream		IN		is the output stream to which the chunk is written.
	//	iFormat		IN		is a number describing the format used. It is recommended 
	//						 that the chunk type of the primary data chunk be used.
	//
	//=================================================================================
	void cChunkHandlerSOFH::writeChunkSOFH(cPXGWriterBase& Writer, int iFormat) const
	{
		Writer.writeChunkHeader(*this, 4);
		Writer.writeInteger(iFormat);
    } 
    
    
	//=================================================================================
	//	cChunkHandlerSOFH::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerSOFH::getChunkType() const 
	{
        //"SOFH"
        return 0x534F4648;
    } 
    
}
