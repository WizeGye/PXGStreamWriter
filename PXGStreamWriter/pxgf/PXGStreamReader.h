#ifndef CPXGSTREAMREADER_H
#define CPXGSTREAMREADER_H
// ---------------------------------------------------------------------------
// FILE NAME            : PXGStreamReader.h
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
// Declaration for the class cPXGStreamReader used for parsing PXGF files.
// 
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: jeremy $ 
// Date:       $LastChangedDate: 2009-01-20 14:22:58 +0200 (Tue, 20 Jan 2009) $ 
// Revision:   $LastChangedRevision: 34205 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGStreamReader.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#ifndef __unix__
#pragma warning(disable:4786)
#endif

#include <map>
#include <sstream>
// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PXGReaderBase.h"
#include "PXGStreamWriter.h"
#include "ResetHandler.h"

namespace pxgf {

	/// PXGReader class that reads a PXGF stream from an std::istream.
	//
	/// This class is used for parsing PXGF streams. The user must register a 
	/// reset handler and multiple chunk handlers. The reset handler is used to 
	/// clear the state of the application when synchronisation is lost. The chunk
	/// handlers are used to process recognised chunks when they are recognised 
	/// in the stream.
	///
	/// Once you have opened an input stream and determined the endian of the 
	/// stream you will call the processStream() method. This method will not 
	/// return until either you specify somewhere else that you wish to stop 
	/// processing by calling removeFromStream(), or it reaches the end of the file.
	class cPXGStreamReader : public cPXGReaderBase 
	{
    public:
		/// Constructor
        cPXGStreamReader();
		
		cPXGStreamReader(std::istream& stream);
		
		/// Process an input stream
		//
		/// Calling this stream instructs the engine to start processing the given
		/// stream from the specfied input stream and assuming the endian 
		/// ordering specified. It will continuously process chunks until it 
		/// either reaches the end-of-file, or you specify that you wish it to 
		/// stop by calling removeFromStream(). (This processStream is deprecated
		/// and the other one which specfies endian by the eEndianType enum should
		/// be used instead).
		/// @param stream - a valid istream to read the data stream from
		/// @param bBigEndian - is the stream big endian byte order?
		void processStream(std::istream& stream, bool bBigEndian);

		/// Process an input stream
		//
		/// Calling this stream instructs the engine to start processing the given
		/// stream from the specfied stream and assuming the endian 
		/// ordering specified. It will continuously process chunks until it 
		/// either reaches the end-of-file, or you specify that you wish it to 
		/// stop by calling removeFromStream().
		/// This differs from the other processStream method in that allows one to
		/// specify that the endian should be automatically detected.
		/// @param stream - a valid istream to read the data stream from
		/// @param EndianType - specify the endian type of the input stream. Use Endian_Auto 
		/// for it to be automatically detected.
		void processStream(std::istream& stream, eEndianType EndianType = Endian_Auto);
    
    protected:

        /// Perform a blocking read of the desired length and throw eof exception
        /// @param pData - destination for read data
        /// @param iLength - number of bytes to read
		virtual void blockingRead(char* pData, int iLength);
        

    private:
        std::istream* m_pStream;
        //: This is the input stream used for reading data from
    };

}

#endif
