#ifndef CCHUNKHANDLERSOFH_H
#define CCHUNKHANDLERSOFH_H
// ---------------------------------------------------------------------------
// FILE NAME            : ChunkHandlerSOFH.h
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
// Chunk handler for the SOFH chunk. The Start Of Header chunk should be the
// first chunk in a file. It is used to indicate the start of the header of the file
// and to give the format used in the file which is the chunk ID of the primary data chunk
// used for that stream. An EOH_ chunk must be send to indicate the end of the header.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/ChunkHandlerSOFH.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "com/peralex/Framework/pxgf/ChunkHandler.h"
#include "com/peralex/Framework/pxgf/PXGStreamWriter.h"

namespace pxgf {

    class cChunkHandlerSOFH: public cChunkHandler
    {
    public:
        class cCallbackSOFH {
        public:
            virtual void callbackSOFH(int iformat) =0;
            //: This method is called on the callback handler whenever a SOFH 
            //: chunk is processed.
            //!param: iFormat - 
        };

        cChunkHandlerSOFH();
        //: Creates a new instance of cChunkHandlerSOFH

        void registerCallbackHandler(cCallbackSOFH* pHandler);
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

        void writeChunkSOFH(cPXGWriterBase& stream, int iFormat) const;
        //: Write a SOFH chunk to the given stream writer
        //!param: stream - 
        //!param: iFormat - 

        int getChunkType() const;
        //: Get the 32bit unique number for this chunk type
        //!return: the unique number for this chunk

    private:
        cCallbackSOFH* m_pCallbackSOFHHandler;
        //: Object which implements the callback handler interface
    };

}

#endif
