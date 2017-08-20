#ifndef RESETHANDLER_H
#define RESETHANDLER_H
// ---------------------------------------------------------------------------
// FILE NAME            : ResetHandler.h
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
// ResetHandler.h defines an interface to be implemented in order to provide 
// a mechanism for resetting state information in the application when 
// cPXGStreamReader resynchronises with the data stream.
// 
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/ResetHandler.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES


namespace pxgf {

    /// Interface to be implemented in order to provide a mechanism for resetting
    /// state in the application when cPXGStreamReader resynchronises with the data
    /// stream.
    class cResetHandler 
	{
    public:
		/// Callback method used for resetting application state
		//
		/// This function should be implemented in the application and take 
		/// responsibility for resetting the stream state. It will be called 
		/// whenever the engine resynchronised with the input stream.
        virtual void resetState() =0;
     
    };

}

#endif
