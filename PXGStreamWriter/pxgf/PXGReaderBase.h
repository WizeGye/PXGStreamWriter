#ifndef CPXGREADERBASE_H
#define CPXGREADERBASE_H
// ---------------------------------------------------------------------------
// FILE NAME            : PXGReaderBase.h
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
// Declaration for the class cPXGReaderBase used for parsing PXGF files.
// 
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: flouw $ 
// Date:       $LastChangedDate: 2010-07-13 13:49:55 +0200 (Tue, 13 Jul 2010) $ 
// Revision:   $LastChangedRevision: 39043 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGReaderBase.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#ifndef __unix__
#pragma warning(disable:4786)
#endif

#include <map>
#include <sstream>
#include <vector>
// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PXGStreamWriter.h"
#include "ResetHandler.h"

namespace pxgf {

    /// Base class that handles parsing of PXGF streams
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
	//
	/// Do not use this class directly, but rather make use of one of the derived classes.
    class cPXGReaderBase 
	{
    public:
	
		/// Enum to specify what endian the input stream is
		typedef enum eEndianType
		{
			Endian_Big = 0,
			Endian_Little,
			Endian_Auto
		} eEndianType;

		/// Constructor
        cPXGReaderBase();
        
		/// Destructor
		virtual ~cPXGReaderBase () {};
		
		/// Register a stream writer
		//
		/// If you wish to make a copy of the chunks that are being read just before
        /// they are processed then call this method with a non-null value. By setting 
		/// iCopyMode appropriately, it is possible to control which chunks are copied.
		/// With iCopyMode set to 1, it is the users responsibility to copy registered
		/// chunks (if that is desired). Default mode is 0, copy everything.
        /// @param pStreamWriter - writer to make a copy of the stream to
		/// @param iCopyMode - 0 to copy everything, 1 to only copy unregistered chunks
        void registerStreamWriter(cPXGWriterBase* pStreamWriter, int iCopyMode=0);
        
		/// Register a reset handler
		//
		/// Use this method to specify what should be called to reset the 
        /// application stream state. This will be automatically called whenever
        /// the stream engine resynchronises with the stream.
        ///
        /// Any previously registered reset handler will be replaced by this one
        /// @param pHandler - pointer to a class implementing cResetHandler
        void registerResetHandler(cResetHandler* pHandler);
        
		/// Register a chunk handler
		//
		/// Use this method to register a new chunk handler with the engine. It 
        /// will automatically determine the chunk type from the handler and put 
        /// it into a table of recognised types. Note that you can only have a 
        /// single handler for any one type of chunk. If a handler  already 
        /// exists, it will be replaced by the new one.
        /// @param handler - new handler to be added
        void registerChunkHandler(cChunkHandler& handler);
        
		/// Use this method to remove a chunk handler from the table of recognised types.
        /// @param handler - handler to be removed
        void removeChunkHandler(const cChunkHandler& handler);
        
		/// This method completely empties the table of recognised chunk types.
        void removeAllChunkHandlers();

        /// Process an input stream
		//
		/// Calling this stream instructs the engine to start processing the given
        /// stream starting at its current location and assuming the endian 
        /// ordering specified. It will continuously process chunks until it 
        /// either reaches the end-of-file, or you specify that you wish it to 
        /// stop by calling removeFromStream().
        /// @param bBigEndian - is the stream big endian byte order?
        void processPXGStream(bool bBigEndian);
        
		/// Process an input stream
		//
		/// Calling this stream instructs the engine to start processing the given
        /// stream starting at its current location and assuming the endian 
        /// ordering specified. It will continuously process chunks until it 
        /// either reaches the end-of-file, or you specify that you wish it to 
        /// stop by calling removeFromStream().
		/// @param EndianType - specifies the endian type
        void processPXGStream(eEndianType EndianType);

		/// Stop processing a stream
		//
        /// Specify that the engine should stop processing the stream as soon as 
        /// it has finished the current chunk.
        void removeFromStream();

		/// Get whether the input stream is big endian or not
		/// When using Endian_Auto this value will only be valid once the stream
		/// has been synced to
		/// @return is the input stream using big endian?
		bool getIsBigEndian() const {return m_bBigEndian; };
        
    private:
        bool processChunk();
        //: Process the next chunk
        //!return: true if the chunk was successfully processed
        // This method is called with a stream in which the synchronisation 
        // number has just been read and it must organise processing the 
        // remainder of the chunk. If a problem is detected processing the chunk
        // then it returns false indicating that synchronisation has been lost

        void resynchronise();
        //: Resynchronise with the stream
        // This method searches through the stream for the next occurrence of the
        // sync word or the end of file. If the end-of-file is found it throws 
        // IOException.

        bool checkSynchronised();
        //: Check if the stream is still synchronised
        //!return: true if we have just read the sync word
        // The next word read from the stream should be the synchronisation word.
        // If it is not then we must return false to indicate that we are not 
        // synchronised.

        int readInteger();
        //: Read an integer
        //!return: The next integer from the stream in native endian format
        // Reads an integer using the specified endian ordering from the stream.

	protected:
        virtual void blockingRead(char* pchData, int iLength) = 0;
        //: Perform a blocking read of the desired length and throw eof exception
        //!param: pData - destination for read data
        //!param: iLength - number of bytes to read
		 
    private:
		void processPXGStream();
		//: Starts the processing of the already setup stream
        // Calling this stream instructs the engine to start processing the given
        // stream starting at its current location and assuming the endian 
        // ordering specified. It will continuously process chunks until it 
        // either reaches the end-of-file, or you specify that you wish it to 
        // stop by calling removeFromStream().


        bool m_bRemoveFromStream;
        //: Indicates when a request has been made to stop processing the input 
        //: stream and whether an input stream has been set.

        std::map<int,cChunkHandler*> m_chunkHandlers;
        //: Stores a table of recognised chunk types and handles to their 
        //: corresponding ChunkHandler implementations.

        bool m_bBigEndian;
        //: Indicates whether the stream is stored in big endian byte order or not.

		eEndianType m_eEndian;
		//: Specifies which endian to search try and syncronise to. Endian_Auto will sync to either

	
        cPXGWriterBase* m_pStreamWriter;
        //: If this stream writer is not null then it will be used for making a 
        //: copy of the chunks being read.

        cResetHandler* m_pResetHandler;
        //: Handler to object used for resetting the application state when the
        //: stream resynchronises.

		int m_iCopyMode;
		//: Copy mode set by registerStreamWriter() and used by processChunk()
		//: to determine which chunks should be copied.

		std::vector<char> m_vchData;
    };

}

#endif
