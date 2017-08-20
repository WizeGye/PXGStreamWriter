#ifndef CCHUNKHANDLERSSR__H
#define CCHUNKHANDLERSSR__H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerSSR_.h
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
// Chunk handler for the SSR_ chunk. The Single channel Short Real chunk
// stores the sampled real data.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2013-11-07 10:09:14 +0200 (Thu, 07 Nov 2013) $ 
// Revision:   $LastChangedRevision: 52313 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerSSR_.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#include <vector>

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"

namespace pxgf {

	/// Handler which deals with the Single channel Short Real chunk
	//
	/// This chunk handles a data stream containing real samples only.
	/// useful for ADC data or audio data.
	class cChunkHandlerSSR_: public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackSSR_ 
		{
		public:
			/// This method is called on the callback handler whenever a SSR_ 
			/// chunk is processed.
			/// @param lTimestamp_microSec - Timestamp of first sample in frame
			/// @param vsIqData - vector of 16bit samples
			virtual void callbackSSR_(int64_t lTimestamp_microSec, const std::vector<short>& vsRealData) =0;
		};
	
		/// Constructor
		cChunkHandlerSSR_();
		
		/// Destructor
		virtual ~cChunkHandlerSSR_() {};
				
		/// Register the object which will provide an implementation of this handler's
		/// callback method.
		/// @param pHandler
		void registerCallbackHandler(cCallbackSSR_* pHandler);
		
		/// Perform the appropriate processing required for this chunk.
		//
		/// If any problems are detected during parsing of the data then the 
		/// return value should be false.
		/// @param vchData - the array of data belonging to the chunk
		/// @param bBigEndian - is data big endian?
		/// @return true if the chunk was processed successfully
        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
		
		/// Write a SSR_ chunk to the given stream writer
		/// @param stream - stream to write to
		/// @param lTimestamp_microSec - Timestamp of first sample in frame
		/// @param vsRealData - vector of 16bit samples
		void writeChunkSSR_(cPXGWriterBase& stream, int64_t lTimestamp_microSec, const std::vector<short>& vsRealData) const;
		
		/// Get the 32bit unique number for this chunk type
		/// @return the unique number for this chunk
		int getChunkType() const;
		
private:
		cCallbackSSR_* m_pCallbackSSR_Handler;
		//: Pointer to object that implements the callback handler interface
		
		std::vector<short> m_vsIqData;
		//: Vector used to change endian-ness if required, and change from char to short
	};

}

#endif
