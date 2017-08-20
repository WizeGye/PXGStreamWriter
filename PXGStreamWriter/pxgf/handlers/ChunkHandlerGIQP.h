#ifndef CCHUNKHANDLERGIQP_H
#define CCHUNKHANDLERGIQP_H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerGIQP.h
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
// Chunk handler for the GIQP chunk. The Group IQ Packing chunk stores the
// packing used for the IQ Data in the GSIQ chunks.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerGIQP.h $ 
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

	/// Handler which deals with the group channel packing chunk
    class cChunkHandlerGIQP: public cChunkHandler
    {
    public:
		/// Callback handler interface
        class cCallbackGIQP 
		{
        public:
			/// This method is called on the callback handler whenever a GIQP 
			/// chunk is processed.
			/// @param oPacking - Object describing how the IQ data is packed
			/// in the GSIQ stream
            virtual void callbackGIQP(const cPackingGIQP& oPacking) =0;
            
        };

		/// Contructor
        cChunkHandlerGIQP();
        

		/// Register the object which will provide an implementation of this handler's
		/// callback method.
		/// param pHandler
        void registerCallbackHandler(cCallbackGIQP* pHandler);
        
		/// Perform the appropriate processing required for this chunk.
		//
		/// If any problems are detected during parsing of the data then the 
		/// return value should be false.
		/// @param vchData - the array of data belonging to the chunk
		/// @param bBigEndian - is data big endian?
		/// @return true if the chunk was processed successfully
        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
        
		/// Write a GIQP chunk to the given stream writer
        /// @param stream - stream to write to
        /// @param oPacking - Object describing how the IQ data is packed
		/// in the GSIQ stream
        void writeChunkGIQP(cPXGWriterBase& stream, const cPackingGIQP& oPacking) const;
        
		/// Get the 32bit unique number for this chunk type
		/// @return the unique number for this chunk
		int getChunkType() const;
        
    private:
        cCallbackGIQP* m_pCallbackGIQPHandler;
        //: Object which implements the callback handler interface
    };

}

#endif
