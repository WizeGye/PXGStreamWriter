#ifndef CCHUNKHANDLERGSIQ_H
#define CCHUNKHANDLERGSIQ_H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerGSIQ.h
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
// Chunk handler for the GSIQ chunk. The Group Short IQ chunk stores the
// packed IQ data in the packing defined by the GIQP chunk.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerGSIQ.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"
#include "packings/PackingGIQP.h"

namespace pxgf {

	/// Handler which deals with the Group Short IQ time chunk
    class cChunkHandlerGSIQ: public cChunkHandler
    {
    public:
		/// Callback handler interface
        class cCallbackGSIQ 
		{
        public:
			/// This method is called on the callback handler whenever a GSIQ 
			/// chunk is processed.
			/// @param lTimestamp_microSec - Timestamp of first sample in frame
			/// @param vsIqData - vector of 16bit integer samples. The GIQP chunk is used to 
			/// determine how this is packed
            virtual void callbackGSIQ(int64_t lTimestamp_microSec, const std::vector<short>& vsIqData) =0;
        };

		/// Constructor
        cChunkHandlerGSIQ();
        
		/// Register the object which will provide an implementation of this handler's
		/// callback method.
		/// @param pHandler
        void registerCallbackHandler(cCallbackGSIQ* pHandler);
        
        /// Perform the appropriate processing required for this chunk.
		//
		/// If any problems are detected during parsing of the data then the 
		/// return value should be false.
		/// @param vchData - the array of data belonging to the chunk
		/// @param bBigEndian - is data big endian?
		/// @return true if the chunk was processed successfully
        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
        
		/// Write a GSIQ chunk to the given stream writer
		//
		/// The stream should also contain a GIQP chunk to desrcibe how the data is packed
		/// @param stream - stream to write to
		/// @param lTimestamp_microSec - Timestamp of first sample in frame
		/// @param vsIqData - vector of shorts containing the packed channel time data. 
		void writeChunkGSIQ(cPXGWriterBase& stream, int64_t lTimestamp_microSec, const std::vector<short>& vsIqData) const;
        
		/// Repack the data from the packing specified to the output packing specified
		/// @param oInputPacking - the packing that the data currently has
		/// @param oOutputPacking - the desired packing for the data
		/// @param vsInputIQData - data to be repacked
		/// @param vsOutputIQData - repacked data
		static void repack(const cPackingGIQP& oInputPacking, const cPackingGIQP& oOutputPacking, const std::vector<short>& vsInputIQData, std::vector<short>& vsOutputIQData);

		/// Get the 32bit unique number for this chunk type
		/// @return the unique number for this chunk
        int getChunkType() const;
        
    private:
        cCallbackGSIQ* m_pCallbackGSIQHandler;
        //: Object which implements the callback handler interface
    };

}

#endif
