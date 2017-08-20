// ---------------------------------------------------------------------------
// FILE NAME 
// ---------------------------------------------------------------------------
// COMPANY 
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
// This class is used for parsing PXGF streams. The user must register a reset handler 
// and multiple chunk handlers. The reset handler is used to clear the state of the 
// application when synchronisation is lost. The chunk handlers are used to process 
// recognised chunks when they are recognised in the stream.
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2013-12-04 10:05:58 +0200 (Wed, 04 Dec 2013) $ 
// Revision:   $LastChangedRevision: 52567 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGReaderBase.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PXGReaderBase.h"
#include "SwapEndian.h"
#include "exceptions/PxgfException.h"

namespace pxgf {

	using namespace std;

	//=================================================================================
    //	cPXGReaderBase::cPXGReaderBase
	//--------------------------------------------------------------------------------
	//	This method is the constructor for the class cPXGReaderBase.
	//
	//	Return
	//	------
	//	None
	//
	//=================================================================================
	cPXGReaderBase::cPXGReaderBase()
      : m_bRemoveFromStream(true),
        m_pStreamWriter(0),
        m_pResetHandler(0),
		m_iCopyMode(0),
		m_eEndian(Endian_Auto),
		m_bBigEndian(true)
	{}


	//=================================================================================
    //	cPXGReaderBase::registerStreamWriter
	//--------------------------------------------------------------------------------
	//  If you wish to make a copy of the chunks that are being read just before
    //  they are processed then call this method with a non-null value.  By setting 
	//  iCopyMode appropriately, it is possible to control which chunks are copied.
	//  With iCopyMode set to 1, it is the users responsibility to copy registered
	//  chunks (if that is desired). Default mode is 0, copy everything.
	//
	//	Parameter		Dir		Description
	//	---------		---		-----------
	//	pStreamWriter	IN		writer to make a copy of the stream	
	//  iCopyMode		IN		0 to copy all, 1 to only copy unregistered chunks
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGReaderBase::registerStreamWriter(cPXGWriterBase* pStreamWriter, int iCopyMode)
	{
        if (m_bRemoveFromStream)
            m_pStreamWriter = pStreamWriter;
        else 
        {
            if (pStreamWriter != 0)
                if (m_bBigEndian != pStreamWriter->getIsBigEndian()) {
					string strMessage("m_streamReader and m_streamWriter must have the same Endian.");
                    throw cPxgfException(strMessage);
				}
            m_pStreamWriter = pStreamWriter;
        }
		m_iCopyMode = iCopyMode;
	}


	//=================================================================================
    //	cPXGReaderBase::registerResetHandler
	//--------------------------------------------------------------------------------
	//	Use this method to specify what should be called to reset the 
    //  application stream state. The registered handler will be automatically 
	//  called whenever the stream engine resynchronises with the stream.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	pHandler	IN		pointer to a class implementing cResetHandler
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGReaderBase::registerResetHandler(cResetHandler* pHandler) 
	{
        m_pResetHandler = pHandler;
	}

	//=================================================================================
    //	cPXGReaderBase::registerChunkHandler
	//--------------------------------------------------------------------------------
	//  Use this method to register a new chunk handler with the engine. It 
    //	will automatically determine the chunk type from the handler and put 
    //	it into a table of recognised types. Note that you can only have a 
    //	single handler for any one type of chunk. If a handler  already 
    //	exists, it will be replaced by the new one.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//  handler		IN		new handler to be added
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================	
	void cPXGReaderBase::registerChunkHandler(cChunkHandler& handler) 
	{
        m_chunkHandlers.insert(map<int,cChunkHandler*>::value_type(handler.getChunkType(), &handler));
	}


	//=================================================================================
    //	cPXGReaderBase::removeChunkHandler
	//--------------------------------------------------------------------------------
	//	Use this method to remove a chunk handler from the table of recognised types.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	handler		IN		chunk handler to be removed.
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGReaderBase::removeChunkHandler(const cChunkHandler& handler) 
	{
        m_chunkHandlers.erase(handler.getChunkType());
    } 


	//=================================================================================
    //	cPXGReaderBase::removeAllChunkHandlers
	//--------------------------------------------------------------------------------
	//	This method removes all chunk handlers from the StreamReader.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	None
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGReaderBase::removeAllChunkHandlers() 
	{
        m_chunkHandlers.clear();
    } 


	//=================================================================================
    //	cPXGReaderBase::processPXGStream
	//--------------------------------------------------------------------------------
	// Calling this method instructs the engine to start processing the given
    // stream starting at its current location and assuming the endian 
    // ordering specified. It will continuously process chunks until it 
    // either reaches the end-of-file, or you specify that you wish it to 
    // stop by calling removeFromStream().
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	stream		IN		the source of the data
	//	bBigEndian	IN		is the stream big endian byte order?
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGReaderBase::processPXGStream(bool bBigEndian) 
	{
		m_bBigEndian = bBigEndian;
		if (m_bBigEndian)
			m_eEndian = Endian_Big;
		else
			m_eEndian = Endian_Little;

		if (m_pStreamWriter != 0)
		{
            if (bBigEndian != m_pStreamWriter->getIsBigEndian()) 
			{
				string strMessage("m_streamReader and m_streamWriter must have the same Endian.");
                throw cPxgfException(strMessage);
			}
		}

		processPXGStream();
	}

	//=================================================================================
	//	processPXGStream
	//--------------------------------------------------------------------------------
	//	This method replaces the other processPXGStream by allowing a third Endian_Auto
	//	option, where the Endianess of the stream is auto-detected
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	EndianType	IN		Specifies Endian Type
	//
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGReaderBase::processPXGStream(eEndianType EndianType)
	{
		m_eEndian = EndianType;

		if (m_eEndian == Endian_Big)
			m_bBigEndian = true;
		else
			m_bBigEndian = false;	//If Endian is Endian_Auto then it doesn't matter what we set m_bBigEndian to.

		if (m_pStreamWriter != 0)
		{
			if (m_eEndian == Endian_Auto)
			{
				string strMessage("Cannot use streamWriter option without specifing endian of input stream.");
                throw cPxgfException(strMessage);
			}
            else if (m_bBigEndian != m_pStreamWriter->getIsBigEndian()) 
			{
				string strMessage("m_streamReader and m_streamWriter must have the same Endian.");
                throw cPxgfException(strMessage);
			}
		}
		processPXGStream();
	}


	//=================================================================================
	//	cPXGReaderBase::processPXGStream
	//--------------------------------------------------------------------------------
	//	This method is shared between other processStream method and does the main 
	//	parsing work.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	None
	//
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGReaderBase::processPXGStream() 
	{
        // clear the flag so that we don't give up immediately
        m_bRemoveFromStream = false;
        while (true) {
            resynchronise();
            // we have now achieved synchronisation so reset the application state
            if (m_pResetHandler != 0)
                m_pResetHandler->resetState();
            // now process chunks sequentially until something goes wrong
            while (true) {
                if (!processChunk())
                    break;
                if (m_bRemoveFromStream)
                    return;
                // make sure that the synchronisation word is next else we have lost sync
                if (!checkSynchronised())
                    break;
            }
            if (m_bRemoveFromStream)
                return;
        }
	}

	//=================================================================================
    //	cPXGReaderBase::removeFromStream
	//--------------------------------------------------------------------------------
	//  This method specifies that the engine should stop processing the stream as 
	//  soon as it has finished the current chunk.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	None
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGReaderBase::removeFromStream() 
	{
        m_bRemoveFromStream = true;
    }

	//=================================================================================
    //	cPXGReaderBase::processChunk
	//--------------------------------------------------------------------------------
	//  This method is called with a stream in which the synchronisation 
    //  number has just been read and it must organise processing the 
    //  remainder of the chunk. If a problem is detected processing the chunk
    //  then it returns false indicating that synchronisation has been lost
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//  NONE
	//
	//	Return
	//	------
	//	true if the chunk was successfully processed
	//
	//=================================================================================
	bool cPXGReaderBase::processChunk() 
	{
        // read the chunk type and length
        int iType = readInteger();
        int iLength = readInteger();
        // If the length is too great then we must somehow have lost synchronisation.  If length is negative, then
		//    this probably indicates loss of sync or corruption
        if (iLength > cPXGWriterBase::getMaxChunkLength() || iLength < 0) {
            return false;
		}
        if ((iLength & 0x3) != 0) {
            return false;
        }
        // read the data for the 
        //vector<char> m_vchData (iLength);
		m_vchData.resize(iLength);
		if (iLength)
			blockingRead(&m_vchData[0], iLength);

        // lookup handler and process if possible
        map<int,cChunkHandler*>::iterator it = m_chunkHandlers.find(iType);
        if (it != m_chunkHandlers.end()) // chunk is registered
		{
		     // make a copy of the chunk if required
			if ((m_pStreamWriter != 0) && (m_iCopyMode == 0))
				m_pStreamWriter->writeRawChunk(iType, m_vchData);
            (*it).second->processChunk(m_vchData, m_bBigEndian);
		}
		else // chunk is NOT registered
		{
			// make a copy of the chunk if required
			if (m_pStreamWriter != 0)
				m_pStreamWriter->writeRawChunk(iType, m_vchData);
		}
        // otherwise just skip over the chunk
        return true;
    } 


	//=================================================================================
    //	cPXGReaderBase::resynchronise
	//--------------------------------------------------------------------------------
	//	This method searches through the stream for the next occurrence of the
    //	sync word or the end of file. If the end-of-file is found it throws 
    //	IOException.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	None
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGReaderBase::resynchronise() 
	{
        unsigned uBigEndianSync = 0;
		unsigned uLittleEndianSync = 0;

        bool bSync = false;
        int iNext = 0;
        while(!bSync && !m_bRemoveFromStream)
		{
			unsigned char ucByte;
			blockingRead((char*)&ucByte,1);
			iNext = ucByte;
            
			uBigEndianSync <<= 8;
            uBigEndianSync |= iNext;
            uLittleEndianSync >>= 8;
            uLittleEndianSync |= ((iNext) << 24);
			
			if (m_eEndian == Endian_Big)
			{
				bSync = (uBigEndianSync == unsigned(cPXGWriterBase::getSynchronisationWord()));
			}
			else if (m_eEndian == Endian_Little)
			{
				bSync = (uLittleEndianSync == unsigned(cPXGWriterBase::getSynchronisationWord()));
			}
			else if (m_eEndian == Endian_Auto)
			{
				if (uBigEndianSync == unsigned(cPXGWriterBase::getSynchronisationWord()))
				{
					m_bBigEndian = true;
					bSync = true;
				}
				else if (uLittleEndianSync == unsigned(cPXGWriterBase::getSynchronisationWord()))
				{
					m_bBigEndian = false;
					bSync = true;
				}
			}
			else
				throw cPxgfException("Invalid endian specification.");


		}
	}


	//=================================================================================
	//	cPXGReaderBase::checkSynchronised
	//--------------------------------------------------------------------------------
	//	Check if the stream is still synchronised. The next word read from the stream 
	//  should be the synchronisation word. If it is not then we must return false to 
	//  indicate that we are not synchronised.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	None
	//
	//	Return
	//	------
	//	bool = true if we have just read the sync word
	//
	//=================================================================================
	bool cPXGReaderBase::checkSynchronised() 
	{
        int iNextWord = readInteger();
        return iNextWord == cPXGWriterBase::getSynchronisationWord();
	}


	//=================================================================================
	//	cPXGReaderBase::readInteger
	//--------------------------------------------------------------------------------
	//	This method reads an integer using the specified endian ordering from the stream.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//
	//	Return
	//	------
	//	int		The next integer from the stream in native endian format		
	//
	//=================================================================================
	int cPXGReaderBase::readInteger() 
	{
        int iValue;
        blockingRead((char*)&iValue, sizeof(int));
        if (cPXGWriterBase::getIsLocalBigEndian() != m_bBigEndian)
            SWAP_INT(iValue);
        return iValue;
	}

}
