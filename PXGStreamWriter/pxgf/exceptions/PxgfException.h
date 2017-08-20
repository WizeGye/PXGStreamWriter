#ifndef CPXGFEXCEPTION_H
#define CPXGFEXCEPTION_H
// ---------------------------------------------------------------------------
// FILE NAME            : PxgfException.h
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
//	PxgfException.h defines the base exception class for the PXGF project.  
//	 
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/exceptions/PxgfException.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES
#include <string>
// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES

namespace pxgf {

	/// Base exception class for the PXGF library
	class cPxgfException
    {
    public:
		/// Default Constructor
        cPxgfException()
        {}
    
		/// Constructor with a defined message
        cPxgfException(const std::string& sMessage)
          : m_sMessage(sMessage)
        {}
        
	
		/// Destructor
		virtual ~cPxgfException () {};
	

		/// Get a string representing the exception message
        virtual std::string toString()
        { return std::string("cPxgfException: ") + getMessage(); }
        

    protected:
		/// Get the exception message
        std::string getMessage()
        { return m_sMessage; }
        
    private:
        std::string m_sMessage; ///< The exception message
        
    };

}

#endif
