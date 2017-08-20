#ifndef ICHUNKHANDLER_H
#define ICHUNKHANDLER_H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandler.h
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
// IChunkHandler.h defines the interface to be implemented by every class which 
// is to be used as a chunk handler.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/ChunkHandler.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#include <vector>
// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES


namespace pxgf {

    /// Interface to be implemented by every class which is to be used as a chunk handler.
    class cChunkHandler 
	{
    public:
        /// Perform the appropriate processing required for this chunk.
        /// @param vchData - the vector of data belonging to the chunk
        /// @param bBigEndian - is the data packed using big endian?
        /// @return true if the chunk was processed successfully
        /// If any problems are detected during parsing of the data then the 
        /// return value should be false.
		virtual bool processChunk(const std::vector<char>& vchData, bool bBigEndian) =0;
      
		/// Get the chunk type identifier that this handler corresponds to.
        /// @return the chunk type
        virtual int getChunkType() const=0;
        
    };

}

#endif
