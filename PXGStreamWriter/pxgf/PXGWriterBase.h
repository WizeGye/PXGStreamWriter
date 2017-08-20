#ifndef CPXGWRITERBASE_H
#define CPXGWRITERBASE_H
// ---------------------------------------------------------------------------
// FILE NAME            : PXGWriterBase.h
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
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/PXGWriterBase.h $
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#include <vector>
#include <ostream>
#include <string>
#include <stdint.h>

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "ChunkHandler.h"


namespace pxgf {

	/// Base class for writing PXGF streams
	//
	/// This class is used for writing PXG streams. It contains helper functions
	/// which take almost all the responsibility for dealing with different endian
	/// orderings on different platforms.
	/// Do not use this class directly, but rather make use of one the derived classes.
    class cPXGWriterBase
	{
    public:
		/// Constructor
		//
		/// Constructor specifies output stream to write to and endian byte
		/// ordering to use in recording.
		/// @param bBigEndian - Write stream in big endian?
		cPXGWriterBase(const bool bBigEndian = false);

		/// Get whether the native environment is big endian or not
        static bool getIsLocalBigEndian();

		/// Get whether the output is big endian or not
		/// @return is the output stream to be recorded using big endian?
        bool getIsBigEndian() const;

		/// Writes a chunk header
        /// @param handler - Handler from which the type is obtained
        /// @param iLength - The length of the chunk in bytes
        void writeChunkHeader(const cChunkHandler& handler, int iLength);

		/// Write an integer value using the desired endian
        /// @param iValue - value to write
        void writeInteger(int iValue);

		/// Write a long value using the desired endian
        /// @param lValue - value to write
        void writeInt64(int64_t lValue);

		/// Write a float value using the desired endian
        /// @param fValue - value to write
        void writeFloat(float fValue);

		/// Write a short array of values using the desired endian
        /// @param vsData - short values to write
        void writeShortArray(const std::vector<short>& vsData);

		/// Write an integer array of values using the desired endian
        /// @param viData - integer values to write
        void writeIntegerArray(const std::vector<int>& viData);

		/// Write a char array to the output stream
		/// @param vchData - char values to write
		void writeCharArray(const std::vector<char>& vchData);

		/// Write a string to the output stream
		/// @param strMessage - string to write
		void writeString(const std::string strMessage);

		/// Write a raw chunk to the output stream.
		//
		/// Note that this method should only be used by cPXGStreamReader to
        /// facilitate in copying streams. Note that care should be taken when
        /// calling this method as the byte ordering of data in vchData must be
        /// correct for the output endian type otherwise you will have BIG problems!
        /// @param iType - the chunk type
        /// @param vchData - the binary data in the chunk
        void writeRawChunk(int iType, const std::vector<char>& vchData);

		/// Get the special synchronisation word that each chunk starts with
        /// @return the synchronisation word
        inline static int getSynchronisationWord()
		{
			return m_iSynchronisationWord;
		}

		/// Get the maximum length that chunks are allowed to be.
        /// @return the maximum legal chunk length
        inline static int getMaxChunkLength()
		{
			return m_iMaxChunkLength;
		}

		/// Get the name of the chunk as a string
		/// @param iType - the type ID for the chunk
		/// @return the name of the chunk
		static std::string getChunkName(int iType);

	protected:
		virtual void write(const char* pchData, std::streamsize iLength) = 0;

    private:
        static const int m_iSynchronisationWord;
        //: This is the synchronisation word which is stored at the start of each chunk.

        static const int m_iMaxChunkLength;
        //: Represents the maximum length chunk which may be written

        bool m_bBigEndian;
        //: Should the output stream be written using big endian byte ordering?

		bool m_bSwapEndian;
		//: Is the output endian different from the native endian?

		int32_t m_piChunkHeader[3];
		//: Array used to hold the chunk header to be written to disk

    };

}

#endif
