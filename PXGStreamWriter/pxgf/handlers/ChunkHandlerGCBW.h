#ifndef CCHUNKHANDLERGCBW_H
#define CCHUNKHANDLERGCBW_H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerGCBW.h
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
// Chunk handler for the GCBW chunk. The Group Channel BandWidths give the bandwidth
// of each channel in a group, each channel within a group must have the same bandwidth. 
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerGCBW.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"

namespace pxgf {

	/// Handler which deals with the Group channel Bandwidth chunk
	//
	/// This chunk is used to describe the bandwidth of each channel in a GSIQ stream
    class cChunkHandlerGCBW: public cChunkHandler
    {
    public:
		/// Callback handler interface
        class cCallbackGCBW 
		{
        public:
			/// This method is called on the callback handler whenever a GCBW 
            /// chunk is processed.
            /// @param lChannelBandwidth_microHz - Bandwidth per channel
            virtual void callbackGCBW(int64_t lChannelBandwidth_microHz) =0;
            
        };

		/// Constructor
        cChunkHandlerGCBW();
        
		/// Register the object which will provide an implementation of this handler's
        /// callback method.
        /// param pHandler
        void registerCallbackHandler(cCallbackGCBW* pHandler);
        
		/// Perform the appropriate processing required for this chunk.
		//
		/// If any problems are detected during parsing of the data then the 
		/// return value should be false.
		/// @param vchData - the array of data belonging to the chunk
		/// @param bBigEndian - is data big endian?
		/// @return true if the chunk was processed successfully
		bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
        
		/// Write a GCBW chunk to the given stream writer
		/// @param stream - stream to write to
		/// @param lChannelBandwidth_microHz - Bandwidth of each channel in uHz 
        void writeChunkGCBW(cPXGWriterBase& stream, int64_t lChannelBandwidth_microHz) const;
        
		/// Get the 32bit unique number for this chunk type
        /// @return the unique number for this chunk
        int getChunkType() const;
        
    private:
        cCallbackGCBW* m_pCallbackGCBWHandler;
        //: Object which implements the callback handler interface
    };

}

#endif
