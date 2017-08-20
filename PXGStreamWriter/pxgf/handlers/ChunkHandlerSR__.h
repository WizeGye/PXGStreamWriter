#ifndef CCHUNKHANDLERSR___H
#define CCHUNKHANDLERSR___H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerSR__.h
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
// Chunk handler for the SR__ chunk. The Sample Rate chunk stores the
// sample rate used when sampling data.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerSR__.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGWriterBase.h"

namespace pxgf {

	/// Handler which deals with the Sample Rate chunk
    class cChunkHandlerSR__: public cChunkHandler
    {
    public:
        class cCallbackSR__ 
		{
        public:
			/// This method is called on the callback handler whenever a SR__ 
			/// chunk is processed.
			/// @param lSampleRate_microHz - Sample rate of sampled data in uHz
            virtual void callbackSR__(int64_t lSampleRate_microHz) =0;
        };

		/// Constructor
        cChunkHandlerSR__();
        
		/// Register the object which will provide an implementation of this handler's
		/// callback method.
		/// @param pHandler
        void registerCallbackHandler(cCallbackSR__* pHandler);
        
        /// Perform the appropriate processing required for this chunk.
		//
		/// If any problems are detected during parsing of the data then the 
		/// return value should be false.
		/// @param vchData - the array of data belonging to the chunk
		/// @param bBigEndian - is data big endian?
		/// @return true if the chunk was processed successfully
        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
        
        /// Write a SR__ chunk to the given stream writer
        /// @param stream - 
        /// @param lSampleRate_microHz - Sample rate of sampled data in uHz
		void writeChunkSR__(cPXGWriterBase& stream, int64_t lSampleRate_microHz) const;
        
        /// Get the 32bit unique number for this chunk type
        /// @return the unique number for this chunk
		int getChunkType() const;
        
    private:
        cCallbackSR__* m_pCallbackSR__Handler;
        //: Object which implements the callback handler interface
    };

}

#endif
