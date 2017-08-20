#ifndef CPACKINGGIQP_H
#define CPACKINGGIQP_H
// ---------------------------------------------------------------------------
// FILE NAME            : PackingGIQP.h
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
// Declaration of class for storing packing information about data sent in
// Group Short IQ time data - GSIQ chunks
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: ryan $
// Date:       $LastChangedDate: 2011-01-14 11:01:31 +0200 (Fri, 14 Jan 2011) $
// Revision:   $LastChangedRevision: 40836 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/packings/PackingGIQP.h $
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#include <vector>
// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES

namespace pxgf {

	/// Class for storing packing information about data sent in
	/// Group Short IQ time data - GSIQ chunks
	class cPackingGIQP
    {
    public:
		/// Constructor
		/// @param bIsIQPacked - Are the samples IQ packed? Otherwise they are QI packed
        cPackingGIQP(bool bIsIQPacked = true);

		/// Constructor
		/// @param bIsIQPacked - Are the samples IQ packed? Otherwise they are QI packed
		/// @param iIncrement - The number of samples to skip in the data block to advance to the
		///						next sample for a particular channel
		/// @param viChannelOffsets - the offset (in samples) to the first sample for each channel
        cPackingGIQP(bool bIsIQPacked, int iIncrement, const std::vector<int>& viChannelOffsets);

		/// Sets the cPackingGIQP members
		/// @param bIsIQPacked - Are the samples IQ packed? Otherwise they are QI packed
		/// @param iIncrement - The number of samples to skip in the data block to advance to the
		///						next sample for a particular channel
		/// @param viChannelOffsets - the offset (in samples) to the first sample for each channel
		void set(bool bIsIQPacked, int iIncrement, const std::vector<int>& viChannelOffsets);

		/// Test for equality with another object
		/// @param oOther - object to test against
		/// @returns true if equal
		bool equals(const cPackingGIQP& oOther) const;

		/// Get the number of channels in the group
        int getNumChannels() const;

		/// Test if the packing is IQ?
		bool getIsIQPacked() const;

		/// Get the number of samples to advance in the GSIQ data block in order to retrieve the
		/// next sample for a particluar channel
        int getIncrement() const;

        /// Get the offsets to the first sample of each channel
		const std::vector<int>& getChannelOffsets() const;


    private:
        bool m_bIsIQPacked;
        int m_iIncrement;
        std::vector<int> m_viChannelOffsets;
    };

}

#endif
