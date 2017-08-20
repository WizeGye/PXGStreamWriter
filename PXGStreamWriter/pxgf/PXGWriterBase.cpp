// ---------------------------------------------------------------------------
// FILE NAME            : PXGWriterBase.cpp
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
// This class is used for writing PXG streams. It contains helper functions
// which take almost all the responsibility for dealing with different endian
// orderings on different platforms.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2013-09-04 15:30:08 +0200 (Wed, 04 Sep 2013) $ 
// Revision:   $LastChangedRevision: 51594 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGWriterBase.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#include <sstream>
// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PXGWriterBase.h"
#include "SwapEndian.h"
#include "exceptions/PxgfIllegalArgumentException.h"

namespace pxgf {

    using namespace std;

    const int cPXGWriterBase::m_iSynchronisationWord = 0xa1b2c3d4;

    const int cPXGWriterBase::m_iMaxChunkLength = 65536*32;


	//=================================================================================
    //	cPXGWriterBase::cPXGWriterBase
	//--------------------------------------------------------------------------------
	//	Constructor specifying output stream to write to and endian byte 
    //	ordering to use in writing.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	stream		IN		the output stream to write to
	//	bBigEndian	IN		the endian used in writing to stream
	//
	//	Return
	//	------
	//	None
	//
	//=================================================================================
	cPXGWriterBase::cPXGWriterBase(const bool bBigEndian)
    :m_bBigEndian(bBigEndian),
	 m_bSwapEndian( getIsLocalBigEndian() != m_bBigEndian )	
    {} 




	//=================================================================================
    //	cPXGWriterBase::getIsLocalBigEndian
	//--------------------------------------------------------------------------------
	//	This method determines the native endian of a system.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	None
	//
	//	Return
	//	------
	//	TRUE if local endian is big endian.
	//
	//=================================================================================
	bool cPXGWriterBase::getIsLocalBigEndian() 
	{
        long L=1; void *Ptr=&L; char B=*(char*)Ptr;
        return (B==0);
    }


	//=================================================================================
    //	cPXGWriterBase::getIsBigEndian
	//--------------------------------------------------------------------------------
	//	Get whether the output is big endian or not.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	None
	//
	//	Return
	//	------
	//	TRUE if big endian byte ordering is being used to write to the output stream.
	//
	//=================================================================================
	bool cPXGWriterBase::getIsBigEndian() const 
	{
        return m_bBigEndian;
    } 


	//=================================================================================
    //	cPXGWriterBase::writeChunkHeader
	//--------------------------------------------------------------------------------
	//	This method writes the header of a chunk.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	handler		IN		chunk handler used to obtain unique chunk ID
	//	iLength		IN		length of the chunk in bytes
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGWriterBase::writeChunkHeader(const cChunkHandler& handler, int iLength) 
	{
        if (iLength > getMaxChunkLength()) {
            stringstream strstrMessage;
            strstrMessage << "length of chunk must be less than " <<  getMaxChunkLength();
            throw cPxgfIllegalArgumentException(strstrMessage.str());
        }
        // make sure that length is multiple of 4 bytes
        if ((iLength & 0x3) != 0)
            throw cPxgfIllegalArgumentException("length of chunk must fill an integer number of 32-bit words");
		
		m_piChunkHeader[0] = getSynchronisationWord();
		m_piChunkHeader[1] = handler.getChunkType();
		m_piChunkHeader[2] = iLength;
		if (m_bSwapEndian) {
			SWAP_INT(m_piChunkHeader[0]);
			SWAP_INT(m_piChunkHeader[1]);
			SWAP_INT(m_piChunkHeader[2]);
		}
        write((const char*)m_piChunkHeader, 12);  // 12 = sizeof(piChunkHeader)
    } 


	//=================================================================================
    //	cPXGWriterBase::writeInteger
	//--------------------------------------------------------------------------------
	//	This method writes an integer in the specified endian ordering.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	iValue		IN		the integer to be written
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGWriterBase::writeInteger(int iValue) 
	{
        if (m_bSwapEndian)
            SWAP_INT(iValue);
        write((const char*)&iValue, sizeof(int));
    } 


	//=================================================================================
    //	cPXGWriterBase::writeInt64
	//--------------------------------------------------------------------------------
	//	This method writes an int64 value in the specified endian ordering.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	lValue		IN		the int64_t value to be written
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGWriterBase::writeInt64(int64_t lValue) 
	{
        if (m_bSwapEndian)
            SWAP_INT64(lValue);
        write((const char*)&lValue, sizeof(int64_t));
    } 


	//=================================================================================
    //	cPXGWriterBase::writeFloat
	//--------------------------------------------------------------------------------
	//	This method writes a float in the specified endian ordering.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	fValue		IN		float value to be written
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGWriterBase::writeFloat(float fValue) 
	{
        if (m_bSwapEndian)
            SWAP_FLOAT(fValue);
        write((const char*)&fValue, sizeof(float));
    }
    

	//=================================================================================
    //	cPXGWriterBase::writeShortArray
	//--------------------------------------------------------------------------------
	//	Write a short array of values using the desired endian
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	vsData		IN		short values to write
	//
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGWriterBase::writeShortArray(const vector<short>& vsData) 
	{
        if (m_bSwapEndian) {
            vector<short> vsSwappedData (vsData);
            for (unsigned uI = 0; uI < vsSwappedData.size(); uI++)
                SWAP_SHORT(vsSwappedData[uI]);
            write((const char*)&vsSwappedData[0], vsSwappedData.size() * sizeof(short));
        }
        else
            write((const char*)&vsData[0], vsData.size() * sizeof(short));
    } 


	//=================================================================================
    //	cPXGWriterBase::writeIntegerArray
	//--------------------------------------------------------------------------------
	//	Write an integer array of values using the desired endian
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	viData		IN		integer values to write
	//
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGWriterBase::writeIntegerArray(const vector<int>& viData) 
	{
        if (m_bSwapEndian) {
            vector<int> viSwappedData (viData);
            for (unsigned uI = 0; uI < viSwappedData.size(); uI++)
                SWAP_INT(viSwappedData[uI]);
            write((const char*)&viSwappedData[0], viSwappedData.size() * sizeof(int));
        }
        else
            write((const char*)&viData[0], viData.size() * sizeof(int));
    } 


    //=================================================================================
	//	cPXGWriterBase::writeString
	//--------------------------------------------------------------------------------
	//	This method writes an array of chars to the output pxg stream
    //
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	
	//	vchData		IN		char values to write
	//
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGWriterBase::writeCharArray(const std::vector<char>& vchData)
	{
		write((const char*)&vchData[0], vchData.size());
	}


	//=================================================================================
	//	cPXGWriterBase::writeString
	//--------------------------------------------------------------------------------
	//	This method writes a string to the pxgf output stream.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	strMessage	IN		string to be written
	//
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGWriterBase::writeString(const string strMessage)
	{
		write(strMessage.c_str(), strMessage.length());
	}


	//=================================================================================
    //	cPXGWriterBase::writeRawChunk
	//--------------------------------------------------------------------------------
	//	Write a raw chunk to the output stream. Note that this method should only be 
	//	used by cPXGStreamReader to facilitate in copying streams. Note that care 
	//	should be taken when calling this method as the byte ordering of data in 
	//	vchData must be correct for the output endian type otherwise you will have 
	//	BIG problems!
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	iType		IN		the chunk type
	//	vchData		IN		the binary data in the chunk
	//
	//	Return
	//	------
	//	None.
	//
	//=================================================================================
	void cPXGWriterBase::writeRawChunk(int iType, const vector<char>& vchData) 
	{
        writeInteger(getSynchronisationWord());
        writeInteger(iType);
        writeInteger(vchData.size());
		if (!vchData.empty()) {
			write(&vchData[0], vchData.size());
		}
    } 


	//=================================================================================
	//	cPXGWriterBase::getChunkName
	//--------------------------------------------------------------------------------
	//	Get the name of the chunk as a string
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	iType		IN		the type ID for the chunk
	//
	//
	//	Return
	//	------
	//	string	the name of the chunk
	//
	//=================================================================================
	string cPXGWriterBase::getChunkName(int iType) 
	{
		char pchName[5];
		*(int*)pchName = iType;
		pchName[4] = 0;
		return string(pchName);
	}

}
