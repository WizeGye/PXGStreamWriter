#ifndef CCHUNKHANDLERCF___H
#define CCHUNKHANDLERCF___H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerCF__.h
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
// Date:       $LastChangedDate: 2010-09-17 19:30:37 +0200 (Fri, 17 Sep 2010) $ 
// Revision:   $LastChangedRevision: 39901 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerCF__.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"

namespace pxgf {

	/// Handler which deals with the Centre Frequency chunk
    class cChunkHandlerCF__: public cChunkHandler
    {
    public:
		/// Callback handler interface
        class cCallbackCF__ 
		{
        public:
			/// This method is called on the callback handler whenever a CF__ 
            /// chunk is processed.
            /// @param lCentreFrequency_microHz - Centre frequency of receiver in uHz
            virtual void callbackCF__(int64_t lCentreFrequency_microHz) =0;
            
        };

		/// Constructor
        cChunkHandlerCF__();
        

		/// Register the object which will provide an implementation of this handler's
		/// callback method.
		/// @param pHandler
        void registerCallbackHandler(cCallbackCF__* pHandler);
        
		/// Perform the appropriate processing required for this chunk.
		//
		/// If any problems are detected during parsing of the data then the 
		/// return value should be false.
		/// @param vchData - the array of data belonging to the chunk
		/// @param bBigEndian - is data big endian?
		/// @return true if the chunk was processed successfully
        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
        
		/// Write a CF__ chunk to the given stream writer
		/// @param stream - stream to write to
		/// @param lCentreFrequency_microHz - Centre Frequency of receiver in uHz
        void writeChunkCF__(cPXGWriterBase& stream, int64_t lCentreFrequency_microHz) const;
        
		/// Get the 32bit unique number for this chunk type
		/// @return the unique number for this chunk
        int getChunkType() const;
        
    private:
        cCallbackCF__* m_pCallbackCF__Handler;
        //: Object which implements the callback handler interface
    };

}

#endif
