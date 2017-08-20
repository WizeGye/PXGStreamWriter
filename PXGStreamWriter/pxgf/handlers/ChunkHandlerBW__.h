#ifndef CCHUNKHANDLERBW___H
#define CCHUNKHANDLERBW___H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerBW__.h
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
// Chunk handler for the BW__ chunk which holds the bandwidth of the signal 
// stored in micro Hz. It is strongly recommended that this chunk be sent if the 
// bandwidth is available. The bandwidth must be less than or equal to the 
// sample rate.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerBW__.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"

namespace pxgf {

	/// Handler which deals with the Bandwidth chunk 
    class cChunkHandlerBW__: public cChunkHandler
    {
    public:
		/// Callback handler interface
        class cCallbackBW__ 
		{
        public:
			/// This method is called on the callback handler whenever a BW__ 
			/// chunk is processed.
			/// @param lBandwidth_microHz - Bandwidth in uHz
            virtual void callbackBW__(int64_t lBandwidth_microHz) =0;
        };

		/// Constructor
        cChunkHandlerBW__();
        
		/// Register the object which will provide an implementation of this handler's
		/// callback method.
		/// @param pHandler Pointer to the callback handler that is to be used
		void registerCallbackHandler(cCallbackBW__* pHandler);
        
		/// Perform the appropriate processing required for this chunk.
		//
		/// This method is only to be used by the a PXG Reader class
        /// @param vchData - the array of data belonging to the chunk
        /// @param bBigEndian - Is the stream big endian?
        /// @return true if the chunk was processed successfully
        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
        
		/// Write a BW__ chunk to the given stream writer
        /// @param stream - stream to write to
        /// @param lBandwidth_microHz - Bandwidth in uHz
        void writeChunkBW__(cPXGWriterBase& stream, int64_t lBandwidth_microHz) const;
        
		/// Get the 32bit unique number for this chunk type
        /// @return the unique number for this chunk
        int getChunkType() const;
        
    private:
        cCallbackBW__* m_pCallbackBW__Handler;
        //: Object which implements the callback handler interface
    };

}

#endif
