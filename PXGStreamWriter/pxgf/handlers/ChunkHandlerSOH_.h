#ifndef CCHUNKHANDLERSOH__H
#define CCHUNKHANDLERSOH__H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerSOH_.h
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
// This chunk has been deprecated, use SOFH chunk instead.
//
// Chunk handler for the SOH_ chunk. The Start Of Header chunk should be the
// first chunk in a file. It is used to indicate the start of the header of the file
// and to give the format used in the file which is the chunk ID of the primary data chunk
// used for that stream. An EOH_ chunk must be send to indicate the end of the header.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerSOH_.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"

namespace pxgf {

	/// Handler which deals with the SOH chunk. This chunk is deprecated,
	/// use the SOFH chunk instead.
    class cChunkHandlerSOH_: public cChunkHandler
    {
    public:
        class cCallbackSOH_ 
		{
        public:
            virtual void callbackSOH_(int iformat) =0;
            //: This method is called on the callback handler whenever a SOH_ 
            //: chunk is processed.
            //!param: iFormat - 
        };

        cChunkHandlerSOH_();
        //: Creates a new instance of cChunkHandlerSOH_

        void registerCallbackHandler(cCallbackSOH_* pHandler);
        //: Register the object which will provide an implementation of this handler's
        //: callback method.
        //!param: pHandler

        bool processChunk(const std::vector<char>& vchData, bool bBigEndian);
        //: Perform the appropriate processing required for this chunk.
        //!param: vchData - the array of data belonging to the chunk
        //!param: bBigEndian - 
        //!return: true if the chunk was processed successfully
        // If any problems are detected during parsing of the data then the 
        // return value should be false.

        void writeChunkSOH_(cPXGWriterBase& stream, int iFormat) const;
        //: Write a SOH_ chunk to the given stream writer
        //!param: stream - 
        //!param: iFormat - 

        int getChunkType() const;
        //: Get the 32bit unique number for this chunk type
        //!return: the unique number for this chunk

    private:
        cCallbackSOH_* m_pCallbackSOH_Handler;
        //: Object which implements the callback handler interface
    };

}

#endif
