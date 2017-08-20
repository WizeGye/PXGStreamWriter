// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerEOH_.cpp
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
// Chunk handler for the EOH_ chunk. This chunk should only be used in files, 
// it is used to indicate the End Of Header of the file. This chunk acts 
// as an indicator to the reading application that the header is complete. The 
// header of PXGF files should be used to store information to allow an 
// application to decide whether to continue processing the whole file.
// 
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2013-11-29 15:06:40 +0200 (Fri, 29 Nov 2013) $ 
// Revision:   $LastChangedRevision: 52524 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerEOH_.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "ChunkHandlerEOH_.h"

namespace pxgf {

    using namespace std;

	//=================================================================================
	//	cChunkHandlerEOH_::cChunkHandlerEOH_
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
	cChunkHandlerEOH_::cChunkHandlerEOH_()
      : m_pCallbackEOH_Handler(0)
    {}
    
    
	//=================================================================================
	//	cChunkHandlerEOH_::registerCallbackHandler
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
	void cChunkHandlerEOH_::registerCallbackHandler(cCallbackEOH_* pHandler) 
	{
        m_pCallbackEOH_Handler = pHandler;
    } 
    
    
	//=================================================================================
	//	cChunkHandlerEOH_::processChunk
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
	bool cChunkHandlerEOH_::processChunk(const vector<char>& vchData, bool bBigEndian) 
	{
        if (vchData.size() != 0) {
			return false; 
		}
        if (m_pCallbackEOH_Handler == 0) {
            return true;
        }
        
        m_pCallbackEOH_Handler->callbackEOH_();
        return true; 
    } 
    
    
	//=================================================================================
	//	cChunkHandlerEOH_::writeChunkEOH_
	//--------------------------------------------------------------------------------
	//	Write a EOH_ chunk to the given stream writer. 
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	stream		IN		is the output stream to which the chunk is written.
	//
	//=================================================================================
	void cChunkHandlerEOH_::writeChunkEOH_(cPXGWriterBase& stream) const 
	{
        stream.writeChunkHeader(*this, 0);
    } 
    
    
	//=================================================================================
	//	cChunkHandlerEOH_::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerEOH_::getChunkType() const 
	{
        //"EOFH"
        return 0x454F4648;
    } 
    
}
