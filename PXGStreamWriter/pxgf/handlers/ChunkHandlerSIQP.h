#ifndef CCHUNKHANDLERSIQP_H
#define CCHUNKHANDLERSIQP_H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerSIQP.h
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
// Chunk handler for the SIQP chunk. The Singlechannel IQ Packing chunk
// stores the packing used for the IQData in SSIQ chunks.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerSIQP.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"
#include "packings/PackingSIQP.h"

namespace pxgf {

	/// Handler which deals with the SSIQ packing chunk
	//
	/// This chunk describes how the data in a SSIQ chunk is packed
    class cChunkHandlerSIQP: public cChunkHandler
    {
    public:
		/// Callback handler interface
        class cCallbackSIQP 
		{
        public:
			/// This method is called on the callback handler whenever a SIQP 
			/// chunk is processed.
			/// @param oPacking - Object describing whether the data is IQ or QI packed
            virtual void callbackSIQP(const cPackingSIQP& oPacking) =0;
        };

		/// Constructor
        cChunkHandlerSIQP();
        
		/// Register the object which will provide an implementation of this handler's
		/// callback method.
		/// @param pHandler
        void registerCallbackHandler(cCallbackSIQP* pHandler);
        
        
		/// Perform the appropriate processing required for this chunk.
		//
		/// If any problems are detected during parsing of the data then the 
		/// return value should be false.
		/// @param vchData - the array of data belonging to the chunk
		/// @param bBigEndian - is data big endian?
		/// @return true if the chunk was processed successfully
        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
        
		/// Write a SIQP chunk to the given stream writer
		/// @param stream - stream to write to
		/// @param oPacking - Object describing whether the data is IQ or QI packed
		void writeChunkSIQP(cPXGWriterBase& stream, const cPackingSIQP& oPacking) const;
        
		/// Get the 32bit unique number for this chunk type
		/// @return the unique number for this chunk
		int getChunkType() const;
        
    private:
        cCallbackSIQP* m_pCallbackSIQPHandler;
        //: Object which implements the callback handler interface

        cPackingSIQP m_oIQPacking;
        cPackingSIQP m_oQIPacking;
    };

}

#endif
