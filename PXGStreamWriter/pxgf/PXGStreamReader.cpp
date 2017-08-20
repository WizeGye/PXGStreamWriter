// ---------------------------------------------------------------------------
// FILE NAME            : PXGStreamReader.cpp
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
// This class is used for parsing PXGF streams. The user must register a reset handler 
// and multiple chunk handlers. The reset handler is used to clear the state of the 
// application when synchronisation is lost. The chunk handlers are used to process 
// recognised chunks when they are recognised in the stream.
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: jeremy $ 
// Date:       $LastChangedDate: 2009-01-20 14:22:58 +0200 (Tue, 20 Jan 2009) $ 
// Revision:   $LastChangedRevision: 34205 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGStreamReader.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
//#include "log4cxx/logger.h"
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PXGStreamReader.h"
#include "SwapEndian.h"
#include "exceptions/PxgfException.h"

namespace pxgf {

    using namespace std;

	//static log4cxx::LoggerPtr g_logger(log4cxx::Logger::getLogger("com.peralex.framework.pxgf.PXGStreamReader"));

				
	//=================================================================================
    //	cPXGStreamReader::cPXGStreamReader
	//--------------------------------------------------------------------------------
	//	This method is the constructor for the class cPXGStreamReader.
	//
	//	Return
	//	------
	//	None
	//
	//=================================================================================
	cPXGStreamReader::cPXGStreamReader()
      : m_pStream(0)
    {} 

	cPXGStreamReader::cPXGStreamReader(istream& stream)
	  : m_pStream(&stream)
	{}
	
	//=================================================================================
    //	cPXGStreamReader::processStream
	//--------------------------------------------------------------------------------
	// Calling this method instructs the engine to start processing the given
    // stream starting at its current location and assuming the endian 
    // ordering specified. It will continuously process chunks until it 
    // either reaches the end-of-file, or you specify that you wish it to 
    // stop by calling removeFromStream().
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	stream		IN		the source of the data
	//	bBigEndian	IN		is the stream big endian byte order?
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGStreamReader::processStream(istream& stream, bool bBigEndian) 
	{
        m_pStream = &stream;
		processPXGStream(bBigEndian);
    } 


	//=================================================================================
    //	cPXGStreamReader::processStream
	//--------------------------------------------------------------------------------
	//	This method replaces the other processPXGStream by allowing a third Endian_Auto
	//	option, where the Endianess of the stream is auto-detected
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	stream		IN		the source of the data
	//	EndianType	IN		Specifies Endian type
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGStreamReader::processStream(std::istream& stream, eEndianType EndianType)
	{
		m_pStream = &stream;
		processPXGStream(EndianType);	
	}

	//=================================================================================
    //	cPXGStreamReader::blockingRead
	//--------------------------------------------------------------------------------
	//	This method performs a blocking read of the desired length and throw eof exception
	//	
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	pData		OUT		destination for read data
	//	iLengthRead	IN		number of bytes to read
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGStreamReader::blockingRead(char* pchData, int iLength) 
	{
        int iLengthRead = 0;
        do {
            if (m_pStream->eof())
                throw cPxgfException("End of file");
            m_pStream->read(pchData + iLengthRead, iLength - iLengthRead);
            iLengthRead += m_pStream->gcount();
        } while (iLengthRead < iLength);
    }
        
}
