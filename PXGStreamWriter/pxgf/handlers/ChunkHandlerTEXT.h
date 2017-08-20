#ifndef CCHUNKHANDLERTEXT_H
#define CCHUNKHANDLERTEXT_H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandler_TEXT.h
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
// Chunk handler for TEXT chunks.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerTEXT.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
#include <string>

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES

#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"
#include "com/peralex/Framework/pxgf/exceptions/PxgfIllegalArgumentException.h"

using namespace std;

namespace pxgf {

	/// Handler which deals with the Text chunk
	//
	/// This chunk is used to add arbirty text to the PXGF stream
    class cChunkHandlerTEXT: public cChunkHandler
    {
    public:
		/// Callback handler interface
        class cCallbackTEXT 
		{
        public:
			/// This method is called on the callback handler whenever a TEXT 
            /// chunk is processed.
            /// @param sMessage - the text message.
            virtual void callbackTEXT(string& sMessage) =0;
            
        };

		/// Default Constructor
        cChunkHandlerTEXT();
        
		/// Constructor that allows for custom chunk ID specification
		cChunkHandlerTEXT(string strChunkType) /*throw (exceptions::cPxgfIllegalArgumentException)*/;
		
		/// Register the object which will provide an implementation of this handler's
		/// callback method.
		/// @param pHandler
        void registerCallbackHandler(cCallbackTEXT* pHandler);
        
        /// Perform the appropriate processing required for this chunk.
		//
		/// If any problems are detected during parsing of the data then the 
		/// return value should be false.
		/// @param vchData - the array of data belonging to the chunk
		/// @param bBigEndian - is data big endian?
		/// @return true if the chunk was processed successfully
        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
        
		/// Write a TEXT chunk to the given stream writer
		/// @param stream - the output stream to write the chunk to.
		/// @param strMessage - the text message to be written to the stream. 
		void writeChunkTEXT(cPXGWriterBase& stream, string strMessage) const;
        
        /// Get the 32bit unique number for this chunk type
        /// @return the unique number for this chunk
		int getChunkType() const;
        
		/// get the 32bit number for the input string
		/// the unique 32bit number corresponding to the input string strChunkType
		int getChunkType(string strChunkType) const /*throw (exceptions::cPxgfIllegalArgumentException)*/;
		
    private:
        cCallbackTEXT* m_pCallbackTEXTHandler;
        //: Object which implements the callback handler interface

		int m_iChunkType;
    };

}

#endif
