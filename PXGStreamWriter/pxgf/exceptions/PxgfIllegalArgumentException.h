#ifndef CPXGFILLEGALARGUMENTEXCEPTION_H
#define CPXGFILLEGALARGUMENTEXCEPTION_H
// ---------------------------------------------------------------------------
// FILE NAME            : PxgfIllegalArgumentException.h
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
// Definition of the class cPxgfIllegalArgumentException.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/exceptions/PxgfIllegalArgumentException.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "PxgfException.h"

namespace pxgf {

	/// Illegal argument exception for the PXGF library
	class cPxgfIllegalArgumentException: public cPxgfException
    {
    public:
		/// Default Constructor
        cPxgfIllegalArgumentException()
        {}
        
		/// Constructor with a defined message
        cPxgfIllegalArgumentException(const std::string& sMessage)
          : cPxgfException(sMessage)
        {}
        
		/// Get a string representing the exception message
        virtual std::string toString()
        { return std::string("cPxgfIllegalArgumentException: " + getMessage()); }
        
    };

}

#endif
