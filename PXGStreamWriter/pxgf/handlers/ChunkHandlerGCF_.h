#ifndef CCHUNKHANDLERGCF__H
#define CCHUNKHANDLERGCF__H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerGCF_.h
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
// Chunk handler for the GCF_ chunk. The Group Centre Frequency chunk
// stores the centre frequency of each channel in the group.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2010-09-17 19:30:37 +0200 (Fri, 17 Sep 2010) $ 
// Revision:   $LastChangedRevision: 39901 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerGCF_.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"

namespace pxgf {

	/// Handler which deals with the group channel centre frequency chunk
	//
	/// This chunk is used to describe the centre frequency of each channel
    class cChunkHandlerGCF_: public cChunkHandler
    {
    public:
		/// Callback handler interface
        class cCallbackGCF_ 
		{
        public:
			/// This method is called on the callback handler whenever a GCF_ 
			/// chunk is processed.
			/// @param alCentreFrequencies_microHz - vector containing the centre frequencies of each channel
            virtual void callbackGCF_(const std::vector<int64_t>& alCentreFrequencies_microHz) =0;
        
        };

		/// Constructor
        cChunkHandlerGCF_();
        

		/// Register the object which will provide an implementation of this handler's
		/// callback method.
		/// @param pHandler
        void registerCallbackHandler(cCallbackGCF_* pHandler);
        
		/// Perform the appropriate processing required for this chunk.
		//
		/// If any problems are detected during parsing of the data then the 
		/// return value should be false.
		/// @param vchData - the array of data belonging to the chunk
		/// @param bBigEndian - is data big endian?
		/// @return true if the chunk was processed successfully
        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
        
		/// Write a GCF_ chunk to the given stream writer
		/// @param stream - stream to write to
		/// @param vlCentreFrequencies_microHz - vector containing the centre frequencies of each channel
		void writeChunkGCF_(cPXGWriterBase& stream, const std::vector<int64_t>& vlCentreFrequencies_microHz) const;
        
		/// Get the 32bit unique number for this chunk type
		/// @return the unique number for this chunk
        int getChunkType() const;
        
    private:
        cCallbackGCF_* m_pCallbackGCF_Handler;
        //: Object which implements the callback handler interface
    };

}

#endif
