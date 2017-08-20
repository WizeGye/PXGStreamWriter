#ifndef CCHUNKHANDLERIQDC_H
#define CCHUNKHANDLERIQDC_H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerIQDC.h
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
// Chunk handler for the IQDC chunk. The IQ DisContinuity chunk acts as
// an indicator that there has been a discontinuity in the data.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerIQDC.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"

namespace pxgf {

	/// Handler which deals with the IQ discontinuity chunk
	//
	/// This chunk is used to indicated that there is a discontinuity in the IQ data
	/// Otherwise the SSIQ or GSIQ stream is assumed to be continuous
    class cChunkHandlerIQDC: public cChunkHandler
    {
    public:
		/// Callback handler interface
        class cCallbackIQDC 
		{
        public:
			/// This method is called on the callback handler whenever an IQDC 
			/// chunk is processed.
            virtual void callbackIQDC() =0;
            
        };

		/// Constructor
        cChunkHandlerIQDC();
        
		/// Register the object which will provide an implementation of this handler's
        /// callback method.
        /// @param pHandler
        void registerCallbackHandler(cCallbackIQDC* pHandler);
        
        /// Perform the appropriate processing required for this chunk.
		//
		/// If any problems are detected during parsing of the data then the 
		/// return value should be false.
		/// @param vchData - the array of data belonging to the chunk
		/// @param bBigEndian - is data big endian?
		/// @return true if the chunk was processed successfully
        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
        
		/// Write a SOH_ chunk to the given stream writer
        /// @param stream - stream to write to
        void writeChunkIQDC(cPXGWriterBase& stream) const;
        
		/// Get the 32bit unique number for this chunk type
		/// @return the unique number for this chunk
		int getChunkType() const;
        
    private:
        cCallbackIQDC* m_pCallbackIQDCHandler;
        //: Object which implements the callback handler interface
    };

}

#endif
