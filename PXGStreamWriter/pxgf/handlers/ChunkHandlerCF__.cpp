// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerCF__.cpp
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
// Chunk handler for the CF__ chunk which stores the Centre Frequency,
// that is the frequency to which the receiver is tuned.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2013-11-29 15:06:40 +0200 (Fri, 29 Nov 2013) $ 
// Revision:   $LastChangedRevision: 52524 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerCF__.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "ChunkHandlerCF__.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"

namespace pxgf {

    using namespace std;

	//=================================================================================
	//	cChunkHandlerCF__::cChunkHandlerCF__
	//--------------------------------------------------------------------------------
	//	Constructor.
	//
	//=================================================================================
	cChunkHandlerCF__::cChunkHandlerCF__()
      : m_pCallbackCF__Handler(0)
    {}
    
    
	//=================================================================================
	//	cChunkHandlerCF__::registerCallbackHandler
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
	void cChunkHandlerCF__::registerCallbackHandler(cCallbackCF__* pHandler) 
	{
        m_pCallbackCF__Handler = pHandler;
    } 
    
    
	//=================================================================================
	//	cChunkHandlerCF__::processChunk
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
	bool cChunkHandlerCF__::processChunk(const vector<char>& vchData, bool bBigEndian) 
	{
        if (vchData.size() != 8) {
			return false; 
		}
        if (m_pCallbackCF__Handler == 0) {
            return true;
        }
        
        int64_t lCentreFrequency_microHz = *(int64_t*)&vchData[0];
        if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
            SWAP_INT64(lCentreFrequency_microHz);
        
        m_pCallbackCF__Handler->callbackCF__(lCentreFrequency_microHz);
        return true; 
    } 
    
    
	//=================================================================================
	//	cChunkHandlerCF__::writeChunkCF__
	//--------------------------------------------------------------------------------
	//	Write a CF__ chunk to the given stream writer. 
	//
	//	Parameter					Dir		Description
	//	---------					---		-----------
	//	stream						IN		is the output stream to which the chunk is written.
	//	lCentreFrequency_microHz	IN		is the centre frequency in micro Hz.
	//
	//=================================================================================
	void cChunkHandlerCF__::writeChunkCF__(cPXGWriterBase& stream, int64_t lCentreFrequency_microHz) const 
	{
        stream.writeChunkHeader(*this, 8);
        stream.writeInt64(lCentreFrequency_microHz);
    } 
    
    
	//=================================================================================
	//	cChunkHandlerCF__::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerCF__::getChunkType() const 
	{
        //"CF__"
        return 0x43465f5f;
    } 
    
}
