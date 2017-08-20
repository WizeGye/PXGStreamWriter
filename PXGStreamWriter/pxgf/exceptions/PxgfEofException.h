#ifndef CPXGFEOFEXCEPTION_H
#define CPXGFEOFEXCEPTION_H
// ---------------------------------------------------------------------------
// FILE NAME            : PxgfEofException.h
// ---------------------------------------------------------------------------
// COMPANY              : PERALEX ELECTRONICS (PTY) LTD
// ---------------------------------------------------------------------------
// COPYRIGHT NOTICE :
//
// The copyright, manufacturing and patent rights stemming from this document
// in any form are vested in PERALEX ELECTRONICS (PTY) LTD.
//
// (c) PERALEX ELECTRONICS (PTY) LTD 2008
//
// PERALEX ELECTRONICS (PTY) LTD has ceded these rights to its clients
// where contractually agreed.
// ---------------------------------------------------------------------------
// DESCRIPTION :
//
//	PxgfEofException.h defines the End of File exception for the PXGF project.  
//	 
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mmcgrath $ 
// Date:       $LastChangedDate: 2008-05-22 16:19:59 +0200 (Thu, 22 May 2008) $ 
// Revision:   $LastChangedRevision: 31724 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/exceptions/PxgfEofException.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES
// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PxgfException.h"

namespace pxgf {

	/// Eof exception class for the PXGF library
	class cPxgfEofException : public cPxgfException
    {
    public:
		/// Default Constructor
        cPxgfEofException()
        {}
    
		/// Constructor with a defined message
        cPxgfEofException(const std::string& sMessage)
          :cPxgfException(sMessage)
        {}
        				
		/// Get a string representing the exception message
        virtual std::string toString()
		{ return std::string("PxgfEofException: ") + getMessage(); }               
    };

}

#endif
