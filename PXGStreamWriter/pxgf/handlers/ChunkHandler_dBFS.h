#ifndef CCHUNKHANDLER_dBFS_H
#define CCHUNKHANDLER_dBFS_H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandler_dBFS.h
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
// Chunk handler for the dBFS chunk. The dB Full Scale chunk stores the
// full scale value in dBs.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandler_dBFS.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGWriterBase.h"

namespace pxgf {

	/// Handler which deals with the Fullscale-dBm chunk
    class cChunkHandler_dBFS: public cChunkHandler
    {
    public:
        /// Callback handler interface
		class cCallback_dBFS 
		{
        public:
			/// This method is called on the callback handler whenever a _dBFS 
			/// chunk is processed.
			/// @param f_dBFullScale_dBm - Level in dBm that a fullscale (i.e. +-SHORT_MAX) 
			///								signal represents
            virtual void callback_dBFS(float f_dBFullScale_dBm) =0;
            
        };

		/// Constructor
        cChunkHandler_dBFS();
        
		/// Register the object which will provide an implementation of this handler's
		/// callback method.
		/// @param pHandler
        void registerCallbackHandler(cCallback_dBFS* pHandler);
        
		/// Perform the appropriate processing required for this chunk.
		//
		/// If any problems are detected during parsing of the data then the 
		/// return value should be false.
		/// @param vchData - the array of data belonging to the chunk
		/// @param bBigEndian - is data big endian?
		/// @return true if the chunk was processed successfully
        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
        
		/// Write a _dBFS chunk to the given stream writer
		/// @param stream - stream to write to
		/// @param f_dBFullScale_dBm - Fullscale value of data in dBm
        void writeChunk_dBFS(cPXGWriterBase& stream, float f_dBFullScale_dBm) const;
        
		/// Get the 32bit unique number for this chunk type
		/// @return the unique number for this chunk
        int getChunkType() const;
        
    private:
        cCallback_dBFS* m_pCallback_dBFSHandler;
        //: Object which implements the callback handler interface
    };

}

#endif
