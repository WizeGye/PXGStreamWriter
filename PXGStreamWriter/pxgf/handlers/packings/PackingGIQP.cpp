// ---------------------------------------------------------------------------
// FILE NAME            : PackingGIQP.cpp
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
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/packings/PackingGIQP.cpp $
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PackingGIQP.h"

namespace pxgf {

    using namespace std;

	//=================================================================================
    //	cPackingGIQP::cPackingGIQP
	//--------------------------------------------------------------------------------
	//	Default constructor of packing object describing the packing order of IQ data
	//
	//	Parameter			Dir		Description
	//	---------			---		-----------
	//	bIsIQPacked			IN		Are samples packed IQ as opposed to QI.
	//
	//
	//	Return
	//	------
	//	None
	//
	//=================================================================================
	cPackingGIQP::cPackingGIQP(bool bIsIQPacked) :
        m_iIncrement(1),
        m_viChannelOffsets(vector<int>(1))
	{
        m_bIsIQPacked = bIsIQPacked;
    }

	//=================================================================================
    //	cPackingGIQP::cPackingGIQP
	//--------------------------------------------------------------------------------
	//	Constructor of packing object describing the packing order of IQ data
	//
	//	Parameter			Dir		Description
	//	---------			---		-----------
	//	bIsIQPacked			IN		Are samples packed IQ as opposed to QI.
	//	iIncrement			IN		The number of samples to increment to read the next
	//								sample for a particular channel. The value of
	//								iIncrement will be 1 or iNumChannels.
	//	viChannelOffsets	IN		The channel offset to the start of each channel
	//								given in samples where a sample is an IQ pair.
	//
	//
	//	Return
	//	------
	//	None
	//
	//=================================================================================
	cPackingGIQP::cPackingGIQP(bool bIsIQPacked, int iIncrement, const vector<int>& viChannelOffsets)
	{
        m_bIsIQPacked = bIsIQPacked;
        m_iIncrement = iIncrement;
        m_viChannelOffsets = viChannelOffsets;
    }

	//=================================================================================
    //	cPackingGIQP::Set
	//--------------------------------------------------------------------------------
	//	Sets the member variables of a packing object describing the packing order of IQ data
	//
	//	Parameter			Dir		Description
	//	---------			---		-----------
	//	bIsIQPacked			IN		Are samples packed IQ as opposed to QI.
	//	iIncrement			IN		The number of samples to increment to read the next
	//								sample for a particular channel. The value of
	//								iIncrement will be 1 or iNumChannels.
	//	viChannelOffsets	IN		The channel offset to the start of each channel
	//								given in samples where a sample is an IQ pair.
	//
	//
	//	Return
	//	------
	//	None
	//
	//=================================================================================
	void cPackingGIQP::set(bool bIsIQPacked, int iIncrement, const std::vector<int>& viChannelOffsets)
	{
        m_bIsIQPacked = bIsIQPacked;
        m_iIncrement = iIncrement;
        m_viChannelOffsets = viChannelOffsets;
	}

	//=================================================================================
    //	cPackingGIQP::equals
	//--------------------------------------------------------------------------------
	//	This method compares two cPackingGIQP objects
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	oOther		IN		cPackingGIQP object
	//
	//	Return
	//	------
	//	TRUE if input equals object
	//
	//=================================================================================
	bool cPackingGIQP::equals(const cPackingGIQP& oOther) const
	{
        return m_bIsIQPacked == oOther.m_bIsIQPacked &&
               m_iIncrement == oOther.m_iIncrement &&
               m_viChannelOffsets == oOther.m_viChannelOffsets;
    }


	//=================================================================================
    //	cPackingGIQP::getNumChannels
	//--------------------------------------------------------------------------------
	//	This method returns the number of channels in a group.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	None
	//
	//	Return
	//	------
	//	int		the number of channels in the group
	//
	//=================================================================================
	int cPackingGIQP::getNumChannels() const
	{
        return m_viChannelOffsets.size();
    }


	//=================================================================================
    //	cPackingGIQP::getIsIQPacked
	//--------------------------------------------------------------------------------
	//	Get whether IQ packing was used.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	None
	//
	//	Return
	//	------
	//	TRUE if IQ packing was specified in the packing object
	//
	//=================================================================================
	bool cPackingGIQP::getIsIQPacked() const
	{
        return m_bIsIQPacked;
    }

	//=================================================================================
    //	cPackingGIQP::getIncrement
	//--------------------------------------------------------------------------------
	//	The number of samples to increment to read the next sample for a particular
	//	channel. The value of iIncrement will be 1 or iNumChannels.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	None
	//
	//	Return
	//	------
	//	int m_iIncrement
	//
	//=================================================================================
	int cPackingGIQP::getIncrement() const
	{
        return m_iIncrement;
    }


	//=================================================================================
    //	cPackingGIQP::getChannelOffsets
	//--------------------------------------------------------------------------------
	//	This method returns m_viChannelOffsets, an array containing the offset to the
	//  start of each channel given in samples where a sample is an IQ pair.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	None
	//
	//	Return
	//	------
	//	const vector<int>& m_aiChannelOffsets
	//
	//=================================================================================
	const vector<int>& cPackingGIQP::getChannelOffsets() const
	{
        return m_viChannelOffsets;
    }

}
