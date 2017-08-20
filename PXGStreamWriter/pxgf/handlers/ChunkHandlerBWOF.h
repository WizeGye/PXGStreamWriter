#ifndef CCHUNKHANDLERBWOF_H
#define CCHUNKHANDLERBWOF_H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerBWOF.h
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
// Chunk handler for the BandWidth Offset Frequency (BWOF) chunk. This 
// chunk specifies the bandwidth along with the offset frequency of the signal
// band from the centre frequency in microHz.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2010-09-17 19:30:37 +0200 (Fri, 17 Sep 2010) $ 
// Revision:   $LastChangedRevision: 39901 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerBWOF.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"

namespace pxgf {

	/// Handler which deals with the Bandwidth offset chunk
	//
	/// The BWOF chunk defines what offset frequency the filter has if it is non-symetrical.
	///	An example of this might be an IF stream of an SSB signal which has its filter shifted by BW/2
    class cChunkHandlerBWOF: public cChunkHandler
    {
    public:
		/// Callback handler interface
        class cCallbackBWOF 
		{
        public:
			/// This method is called on the callback handler whenever a BWOF 
			/// chunk is processed.
            /// @param lBandwidth_microHz - Bandwidth of filter in uHz
			/// @param lOffsetFrequency_microHz - offset of filter in uHz
            virtual void callbackBWOF(int64_t lBandwidth_microHz, int64_t lOffsetFrequency_microHz) =0;
            
        };

		//Constructor
        cChunkHandlerBWOF();
        

		/// Register the object which will provide an implementation of this handler's
		/// callback method.
		/// @param pHandler Pointer to the callback handler that is to be used
        void registerCallbackHandler(cCallbackBWOF* pHandler);
    
		/// Perform the appropriate processing required for this chunk.
		//
		/// This method is only to be used by the a PXG Reader class
        /// @param vchData - the array of data belonging to the chunk
        /// @param bBigEndian - Is the stream big endian?
        /// @return true if the chunk was processed successfully
        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);

		/// Write a BWOF chunk to the given stream writer
		/// @param stream - stream to write to
		/// @param lBandwidth_microHz - Bandwidth in uHz
		/// @param lOffsetFrequency_microHz - offset in uHz
        void writeChunkBWOF(cPXGWriterBase& stream, int64_t lBandwidth_microHz, int64_t lOffsetFrequency_microHz) const;

		/// Get the 32bit unique number for this chunk type
        /// @return the unique number for this chunk
        int getChunkType() const;
        
    private:
        cCallbackBWOF* m_pCallbackBWOFHandler;
        //: Object which implements the callback handler interface
    };

}

#endif
