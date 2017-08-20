// ---------------------------------------------------------------------------
// FILE NAME            : PackingSIQP.cpp
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
// Definiation for the class cPackingSIQP used to decribe the packing of IQ 
// data in SSIQ chunks. 	
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/handlers/packings/PackingSIQP.cpp $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PackingSIQP.h"

namespace pxgf {

	//=================================================================================
	//	cPackingSIQP::cPackingSIQP
	//--------------------------------------------------------------------------------
	//	Constructor for the cPackingSIQP class
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	bIsIQPacked	IN		Is the data IQ packed or is it QI packed
	//
	//	Return
	//	------
	//	None
	//
	//=================================================================================
	cPackingSIQP::cPackingSIQP(bool bIsIQPacked) 
	{
        m_bIsIQPacked = bIsIQPacked;
    }
    

	//=================================================================================
    //	cPackingSIQP::equals
	//--------------------------------------------------------------------------------
	//	Compares two objects of the class cPackingSIQP.
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	oOther		IN		cPackingSIQP
	//
	//	Return
	//	------
	//	TRUE if objects equate.
	//
	//=================================================================================
	bool cPackingSIQP::equals(const cPackingSIQP& oOther) const 
	{
        return m_bIsIQPacked == oOther.m_bIsIQPacked;
    }


	//=================================================================================
    //	cPackingSIQP::getIsIQPacked
	//--------------------------------------------------------------------------------
	//	Get whether data is packed IQ or is packed QI
	//
	//	Parameter	Dir		Description
	//	---------	---		-----------
	//	None
	//
	//	Return
	//	------
	//	TRUE if data is packed IQ
	//
	//=================================================================================
	bool cPackingSIQP::getIsIQPacked() const 
	{
        return m_bIsIQPacked;
    }

}
