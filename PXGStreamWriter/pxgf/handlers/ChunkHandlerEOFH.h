#ifndef CCHUNKHANDLEREOFH_H
#define CCHUNKHANDLEREOFH_H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerEOFH.h
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
// Chunk handler for the EOFH chunk. This chunk should only be used in files, 
// it is used to indicate the End Of Header of the file. This chunk acts 
// as an indicator to the reading application that the header is complete. The 
// header of PXGF files should be used to store information to allow an 
// application to decide whether to continue processing the whole file.
// 
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerEOFH.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"

namespace pxgf {

    class cChunkHandlerEOFH: public cChunkHandler
    {
    public:
        class cCallbackEOFH {
        public:
            virtual void callbackEOFH() =0;
            //: This method is called on the callback handler whenever an EOFH 
            //: chunk is processed.
        };

        cChunkHandlerEOFH();
        //: Creates a new instance of cChunkHandlerEOFH

        void registerCallbackHandler(cCallbackEOFH* pHandler);
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

        void writeChunkEOFH(cPXGWriterBase& stream) const;
        //: Write a SOH_ chunk to the given stream writer
        //!param: stream - 
        
        int getChunkType() const;
        //: Get the 32bit unique number for this chunk type
        //!return: the unique number for this chunk

    private:
        cCallbackEOFH* m_pCallbackEOFHHandler;
        //: Object which implements the callback handler interface
    };

}

#endif
