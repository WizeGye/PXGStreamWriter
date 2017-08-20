// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerTEXT.cpp
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
// Chunk handler for the TEXT chunk which encodes strings using ISO-8859-1
// character encoding. A constructor is provided to make support of different
// text chunks easy although care must be taken with the nameing of chunks.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerTEXT.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
#ifndef NO_LOG4CXX
#include "log4cxx/logger.h"
#endif
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "ChunkHandlerTEXT.h"
#include "com/peralex/Framework/pxgf/SwapEndian.h"


namespace pxgf {

    using namespace std;

#ifndef NO_LOG4CXX
	static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.handlers.ChunkHandlerTEXT"));
#endif

	//=================================================================================
	//	cChunkHandlerTEXT::cChunkHandlerTEXT
	//--------------------------------------------------------------------------------
	//	Default constructor
	//
	//=================================================================================
    cChunkHandlerTEXT::cChunkHandlerTEXT()
      : m_pCallbackTEXTHandler(0)
    {
		m_iChunkType = getChunkType("TEXT");
		//cout << "TEXT" << " = " << m_iChunkType << endl;
		//cout << "TEXT" << " = " << 0x54455854 << endl;
	}
    
	//=================================================================================
	//	cChunkHandlerTEXT::cChunkHandlerTEXT
	//--------------------------------------------------------------------------------
	//	Constructor
	//
	//=================================================================================
    cChunkHandlerTEXT::cChunkHandlerTEXT(string strChunkName)
      : m_pCallbackTEXTHandler(0)
    {
		m_iChunkType = getChunkType(strChunkName);
		//cout << strChunkName << " = " << m_iChunkType;
	}

    
	//=================================================================================
	//	cChunkHandlerTEXT::registerCallbackHandler
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
	void cChunkHandlerTEXT::registerCallbackHandler(cCallbackTEXT* pHandler) 
	{
	    m_pCallbackTEXTHandler = pHandler;
	} 
	
	
	//=================================================================================
	//	cChunkHandlerTEXT::processChunk
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
	bool cChunkHandlerTEXT::processChunk(const vector<char>& vchData, bool bBigEndian)
	{
	    if (m_pCallbackTEXTHandler == 0) {
#ifndef NO_LOG4CXX
			LOG4CXX_WARN(g_logger, "Callback not initialized.");
#endif
	        return true;
	    }
	   
	    int iTextLength = *(int*)&vchData[0];
		if (cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
	        SWAP_INT(iTextLength);
				
		string sMessage(&vchData[0] + 4, &vchData[0] + 4 + iTextLength);				
	    m_pCallbackTEXTHandler->callbackTEXT(sMessage);
	    return true; 
	} 
	
	
	//=================================================================================
	//	cChunkHandlerTEXT::writeChunkTEXT
	//--------------------------------------------------------------------------------
	//	Write a TEXT chunk to the given stream writer. 
	//
	//	Parameter			Dir		Description
	//	---------			---		-----------
	//	stream				IN		is the output stream to which the chunk is written.
	//	sMessage			IN		is the text message 
	//
	//=================================================================================
	void cChunkHandlerTEXT::writeChunkTEXT(cPXGWriterBase& stream, string strMessage) const 
	{
		int iTextLength = strMessage.length();
		int iPaddedLength = 4 * (iTextLength / 4);
        if (iTextLength != iPaddedLength)
        {
            iPaddedLength += 4;
        }        
		stream.writeChunkHeader(*this, iPaddedLength + 4);
		stream.writeInteger(iTextLength);
		
		string strPadded(strMessage);
		strPadded.append(iPaddedLength - iTextLength,'0');
		stream.writeString(strPadded);
	} 
	
	
	//=================================================================================
	//	cChunkHandlerTEXT::getChunkType
	//--------------------------------------------------------------------------------
	//	Get the 32bit unique number for this chunk type
	//
	//	Return
	//	------
	//	int		the 32bit unique number for this chunk type
	//
	//=================================================================================
	int cChunkHandlerTEXT::getChunkType() const 
	{
	    //"TEXT"
	    return m_iChunkType;
	} 
    

	int cChunkHandlerTEXT::getChunkType(string strChunkType) const 
	{
		if (strChunkType.length() != 4)
		{
			throw cPxgfIllegalArgumentException("ID of text chunk must be 4 characters.");
		}		
		return strChunkType[0] << 24 
		     | strChunkType[1] << 16 
			 | strChunkType[2] <<  8 
			 | strChunkType[3];      
	} 

}
