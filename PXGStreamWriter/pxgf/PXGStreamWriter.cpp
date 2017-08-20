// ---------------------------------------------------------------------------
// FILE NAME            : PXGStreamWriter.cpp
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
// This class is used for writing PXG streams. It contains helper functions
// which take almost all the responsibility for dealing with different endian
// orderings on different platforms.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2013-09-04 15:30:08 +0200 (Wed, 04 Sep 2013) $ 
// Revision:   $LastChangedRevision: 51594 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGStreamWriter.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#include <sstream>
// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PXGStreamWriter.h"
#include "SwapEndian.h"
#include "exceptions/PxgfIllegalArgumentException.h"

namespace pxgf {

    using namespace std;

	//=================================================================================
    //	cPXGStreamWriter::cPXGStreamWriter
	//--------------------------------------------------------------------------------
	//	Constructor specifying output stream to write to and endian byte 
    //	ordering to use in writing.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	stream		IN		the output stream to write to
	//	bBigEndian	IN		the endian used in writing to stream
	//
	//	Return
	//	------
	//	None
	//
	//=================================================================================
	cPXGStreamWriter::cPXGStreamWriter(ostream& Stream, const bool bBigEndian)
    :cPXGWriterBase(bBigEndian),
	 m_stream(Stream)
    {} 


	//=================================================================================
    //	cPXGStreamWriter::getOutputStream
	//--------------------------------------------------------------------------------
	//	This method returns the output stream.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	None
	//
	//
	//	Return
	//	------
	//	ostream&	the output stream that is being written to
	//
	//=================================================================================
	ostream& cPXGStreamWriter::getOutputStream() 
	{        
        return m_stream;
    }

	void cPXGStreamWriter::write(const char* pchData, std::streamsize nLength)
	{
		m_stream.write(pchData, nLength);
	}

}
