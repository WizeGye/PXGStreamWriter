#ifndef CHUNKHANDLER_GRG__H
#define CHUNKHANDLER_GRG__H
// ---------------------------------------------------------------------------
// FILE NAME			: ChunkHandlerGRG_.h
// ---------------------------------------------------------------------------
// COMPANY			  : PERALEX ELECTRONICS (PTY) LTD
// ---------------------------------------------------------------------------
// COPYRIGHT NOTICE :
//
// The copyright, manufacturing and patent rights stemming from this document
// in any form are vested in PERALEX ELECTRONICS (PTY) LTD.
//
// (c) PERALEX ELECTRONICS (PTY) LTD 2013
//
// PERALEX ELECTRONICS (PTY) LTD has ceded these rights to its clients
// where contractually agreed.
// ---------------------------------------------------------------------------
// DESCRIPTION :
//
// Chunk handler for the GRG_ chunk. The Group Relative Gain is the total analogue gain in dB
// of each channel from the input of the receiver (usually an antenna) to the input of the ADC,
// relative to the dBTG chunk, which would be the total analogue gain of one of the channels.
// Therefore, the total analogue gain for a particular channel is equal to the sum of the value
// contained in this chunk and the value contained in the dBTG chunk.
//
// ---------------------------------------------------------------------------
// Author:	 $LastChangedBy: ross $
// Date:	   $LastChangedDate: 2013-09-12 17:22:13 +0200 (Thu, 12 Sep 2013) $
// Revision:   $LastChangedRevision: 51694 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerGRG_.h $
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"

namespace pxgf {
	/// Handler which deals with the Group Relative Gains chunk
	class cChunkHandlerGRG_: public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackGRG_
		{
		public:
			/// This method is called on the callback handler whenever a GRG_
			/// chunk is processed.
			/// @param fTotalGain_dB - Total system gain in dB
			virtual void callbackGRG_(const std::vector<float>& vfGains_dB) =0;
		};

		/// Constructor
		cChunkHandlerGRG_();


		/// Register the object which will provide an implementation of this handler's
		/// callback method.
		/// @param pHandler Pointer to the callback handler that is to be used
		void registerCallbackHandler(cCallbackGRG_* pHandler);

		/// Perform the appropriate processing required for this chunk.
		//
		/// This method is only to be used by the a PXG Reader class
		/// @param vchData - the array of data belonging to the chunk
		/// @param bBigEndian - Is the stream big endian?
		/// @return true if the chunk was processed successfully
		bool processChunk(const std::vector<char>& vchData, bool bBigEndian);


		/// Write a GRG_ chunk to the given stream writer
		/// @param stream - stream to write to
		/// @param vfTotalGain_dB - vector of total system gains per channel in dB
		void writeChunkGRG_(cPXGWriterBase& stream, std::vector<float> vfGains_dB) const;

		/// Get the 32bit unique number for this chunk type
		/// @return the unique number for this chunk
		int getChunkType() const;

	private:
		cCallbackGRG_* m_pCallbackGRG_Handler;
		//: Object which implements the callback handler interface
	};

}

#endif
