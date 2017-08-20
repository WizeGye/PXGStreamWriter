#ifndef CCHUNKHANDLERSSIQ_H
#define CCHUNKHANDLERSSIQ_H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerSSIQ.h
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
// Chunk handler for the SSIQ chunk. The Single channel Short IQ chunk
// stores the sampled IQ data.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerSSIQ.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#include <vector>

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"
#include "packings/PackingSIQP.h"

namespace pxgf {

	/// Handler which deals with the Single channel Short IQ time chunk
	class cChunkHandlerSSIQ: public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackSSIQ 
		{
		public:
			/// This method is called on the callback handler whenever a SSIQ 
			/// chunk is processed.
			/// @param lTimestamp_microSec - Timestamp of first sample in frame
			/// @param vsIqData - vector of 16bit integer samples. The SIQP chunk is used to 
			/// determine how this is packed
			virtual void callbackSSIQ(int64_t lTimestamp_microSec, const std::vector<short>& vsIqData) =0;
			
		};
	
		/// Constructor
		cChunkHandlerSSIQ();
		
		/// Destructor
		virtual ~cChunkHandlerSSIQ() {};
		
		/// Register the object which will provide an implementation of this handler's
		/// callback method.
		/// param pHandler
		void registerCallbackHandler(cCallbackSSIQ* pHandler);
		
		/// Perform the appropriate processing required for this chunk.
		//
		/// If any problems are detected during parsing of the data then the 
		/// return value should be false.
		/// @param vchData - the array of data belonging to the chunk
		/// @param bBigEndian - is data big endian?
		/// @return true if the chunk was processed successfully
        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
		
		/// Write a SSIQ chunk to the given stream writer
		/// @param stream - stream to write to
		/// @param lTimestamp_microSec - Timestamp of first sample in frame
		/// @param vsIqData - vector of shorts containing the packed channel time data. 
		/// The stream should also contain a SIQP chunk to desrcibe how the data is packed
		void writeChunkSSIQ(cPXGWriterBase& stream, int64_t lTimestamp_microSec, const std::vector<short>& vsIqData) const;
		
		/// Repack the data from the packing specified to the output packing specified
		/// @param oInputPacking - the packing that the data currently has
		/// @param oOutputPacking - the desired packing for the data
		/// @param vsInputIQData - data to be repacked
		/// @param vsOutputData - repacked data
		static void repack(const cPackingSIQP& oInputPacking, const cPackingSIQP& oOutputPacking, const std::vector<short>& vsInputIQData, std::vector<short>& vsOutputData);
		
		/// Get the 32bit unique number for this chunk type
		/// @return the unique number for this chunk
		int getChunkType() const;
		
		private:
		cCallbackSSIQ* m_pCallbackSSIQHandler;
		//: Pointer to object that implements the callback handler interface
		
		std::vector<short> m_vsIqData;
		//: Vector used to change endian-ness if required, and change from char to short
	};

}

#endif
