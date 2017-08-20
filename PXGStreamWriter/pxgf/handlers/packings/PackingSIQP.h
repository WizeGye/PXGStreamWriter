#ifndef CPACKINGSIQP_H
#define CPACKINGSIQP_H
// ---------------------------------------------------------------------------
// FILE NAME            : PackingSIQP.h
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
// Declaration for the class cPackingSIQP used to decribe the packing of IQ
// data in SSIQ chunks.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: ryan $
// Date:       $LastChangedDate: 2011-01-14 11:01:31 +0200 (Fri, 14 Jan 2011) $
// Revision:   $LastChangedRevision: 40836 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/packings/PackingSIQP.h $
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES

namespace pxgf {

	/// Class used to decribe the packing of IQ data. This information is needed
	/// to parse SSIQ data chunks.
	class cPackingSIQP
    {
    public:
		/// Cconstructor
		/// @param bIsIQPacked - Is SSIQ data IQ packed? (Otherwise it is QI packed)
        cPackingSIQP(bool bIsIQPacked = true);

		/// Test for equality with another object
        bool equals(const cPackingSIQP& oOther) const;

		/// Test if the packing is IQ?
        bool getIsIQPacked() const;

    private:
        bool m_bIsIQPacked;
    };

}

#endif
