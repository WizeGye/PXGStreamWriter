#ifndef CCHUNKHANDLER_dBTG_H
#define CCHUNKHANDLER_dBTG_H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandler_dBTG.h
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
// Chunk handler for the dBTG chunk. The dB Total Gain chunk stores the
// total gain between the antenna and the A/D.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandler_dBTG.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"

namespace pxgf {
	/// Handler which deals with the TotalGain-dB chunk
    class cChunkHandler_dBTG: public cChunkHandler
    {
    public:
		/// Callback handler interface
        class cCallback_dBTG 
		{
        public:
			/// This method is called on the callback handler whenever a _dBTG 
			/// chunk is processed.
            /// @param fTotalGain_dB - Total system gain in dB
            virtual void callback_dBTG(float fTotalGain_dB) =0;
        };

		/// Constructor
        cChunkHandler_dBTG();
        

		/// Register the object which will provide an implementation of this handler's
        /// callback method.
        /// @param pHandler Pointer to the callback handler that is to be used
        void registerCallbackHandler(cCallback_dBTG* pHandler);
        
		/// Perform the appropriate processing required for this chunk.
		//
		/// This method is only to be used by the a PXG Reader class
		/// @param vchData - the array of data belonging to the chunk
		/// @param bBigEndian - Is the stream big endian?
		/// @return true if the chunk was processed successfully
        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
        

		/// Write a _dBTG chunk to the given stream writer
        /// @param stream - stream to write to
        /// @param fTotalGain_dB - Total system gain in dB
        void writeChunk_dBTG(cPXGWriterBase& stream, float fTotalGain_dB) const;
        
		/// Get the 32bit unique number for this chunk type
        /// @return the unique number for this chunk
        int getChunkType() const;
        
    private:
        cCallback_dBTG* m_pCallback_dBTGHandler;
        //: Object which implements the callback handler interface
    };

}

#endif
