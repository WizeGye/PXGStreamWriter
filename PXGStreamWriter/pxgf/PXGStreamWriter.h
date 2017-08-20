#ifndef CPXGSTREAMWRITER_H
#define CPXGSTREAMWRITER_H
// ---------------------------------------------------------------------------
// FILE NAME            : PXGStreamWriter.h
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
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGStreamWriter.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#include <vector>
#include <ostream>
#include <string>
// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PXGWriterBase.h"
#include "ChunkHandler.h"

namespace pxgf {

	/// PXGWriter class that writes its PXGF stream to a std::ostream.
	//
	/// This class is used for writing PXG streams. It contains helper functions
	/// which take almost all the responsibility for dealing with different endian
	/// orderings on different platforms.
    class cPXGStreamWriter : public cPXGWriterBase 
	{
    public:
        /// Constructor
		//
		/// Constructor specifies output stream to write to and endian byte 
        /// ordering to use in recording.
		/// @param stream - Valid ouput stream to write to
        /// @param bBigEndian - Write stream in big endian?
		cPXGStreamWriter(std::ostream& Stream, const bool bBigEndian);
    
		/// Get the output stream
        /// @return: the output stream that is being written to
        std::ostream& getOutputStream();
        
	protected:
		virtual void write(const char* pchData, std::streamsize nLength);

    private:
        std::ostream &m_stream;
        //: The output stream.
    };

}

#endif
