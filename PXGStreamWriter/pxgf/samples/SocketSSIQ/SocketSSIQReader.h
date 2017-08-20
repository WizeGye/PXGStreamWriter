// ---------------------------------------------------------------------------
// FILE NAME            : SocketSSIQReader.h
// ---------------------------------------------------------------------------
// COMPANY              : PERALEX ELECTRONICS (PTY) LTD
// ---------------------------------------------------------------------------
// COPYRIGHT NOTICE :
//
// The copyright, manufacturing and patent rights stemming from this document
// in any form are vested in PERALEX ELECTRONICS (PTY) LTD.
//
// (c) PERALEX ELECTRONICS (PTY) LTD 2006
//
// PERALEX ELECTRONICS (PTY) LTD has ceded these rights to its clients
// where contractually agreed.
// ---------------------------------------------------------------------------
// DESCRIPTION :
//
// Declaration of class that connects to a PXGF SSIQ socket stream and prints 
// out information about the received stream.
//
// ---------------------------------------------------------------------------
// Author:     $LastChangedBy: mark $ 
// Date:       $LastChangedDate: 2007-03-29 11:34:25 +0200 (Thu, 29 Mar 2007) $ 
// Revision:   $LastChangedRevision: 27448 $
// $HeadURL: svn://iserver.ct/raid/SVN/repos/EWDev/trunk/com/peralex/Framework/pxgf/samples/SocketSSIQ/SocketSSIQReader.h $ 
// ---------------------------------------------------------------------------
// SYSTEM INCLUDE FILES

// ---------------------------------------------------------------------------
// LIBRARY INCLUDE FILES

// ---------------------------------------------------------------------------
// LOCAL INCLUDE FILES
#include "../../PXGSocketReader.h"
#include "../../handlers/ChunkHandlerSIQP.h"
#include "../../handlers/ChunkHandlerSSIQ.h"
#include "../../handlers/ChunkHandlerCF__.h"
#include "../../handlers/ChunkHandlerBW__.h"
#include "../../handlers/ChunkHandler_dBFS.h"
#include "../../handlers/ChunkHandler_dBTG.h"
#include "../../handlers/ChunkHandlerIQDC.h"
#include "../../handlers/ChunkHandlerSR__.h"


/// Class that simply provides an example on how to use the cPXGSocketReader class.
/// It reads a SSIQ based PXGF stream and prints out information relating to the data.
class cSocketSSIQReader : 	public pxgf::cChunkHandlerSIQP::cCallbackSIQP,
							public pxgf::cChunkHandlerSSIQ::cCallbackSSIQ,
							public pxgf::cChunkHandlerCF__::cCallbackCF__,
							public pxgf::cChunkHandlerBW__::cCallbackBW__,
							public pxgf::cChunkHandler_dBFS::cCallback_dBFS,
							public pxgf::cChunkHandler_dBTG::cCallback_dBTG,
							public pxgf::cChunkHandlerIQDC::cCallbackIQDC,
							public pxgf::cChunkHandlerSR__::cCallbackSR__
{
public:
	/// Constructor
	/// @param strHost Hostname or IP Address of socket server that is streaming the relevant PXGf stream
	/// @param usPortNo Port number on host to connect to
	cSocketSSIQReader(std::string strHost, unsigned short usPortNo);

	/// Destructor
	virtual ~cSocketSSIQReader();

	//pxgf handler callBacks
	virtual void callbackSIQP(const pxgf::cPackingSIQP& oPacking);
	virtual void callbackSSIQ(int64_t lTimestamp_microSec, const std::vector<short>& vsIqData);
	virtual void callbackCF__(int64_t lCentreFrequency_microHz);
	virtual void callbackBW__(int64_t lBandwidth_microHz);
	virtual void callback_dBFS(float f_dBFullScale_dBm);
	virtual void callback_dBTG(float fTotalGain_dB);
	virtual void callbackIQDC();
	virtual void callbackSR__(int64_t lSampleRate_microHz);

private:
	/// Handles the connection to a socket server
	bool Connect(const std::string &strAddress, unsigned short usPort);

	/// Calculates RMS power of the given IQ data and returns it in dBm.
	float CalculateRMSPower(std::vector<short> vsIQData, int iFullScale_dBm, int iTotalGain_dB);

	///Socket handle to PXGF socket stream
	SOCKET m_hSocket; 

	///pxgf socket reader
	pxgf::cPXGSocketReader	m_PXGReader; 

	//pxgf handlers
	pxgf::cChunkHandlerSIQP		m_PXGHandlerSIQP;
	pxgf::cChunkHandlerSSIQ		m_PXGHandlerSSIQ;
	pxgf::cChunkHandlerCF__		m_PXGHandlerCF__;
	pxgf::cChunkHandlerBW__		m_PXGHandlerBW__;
	pxgf::cChunkHandler_dBFS	m_PXGHandler_dBFS;
	pxgf::cChunkHandler_dBTG	m_PXGHandler_dBTG;
	pxgf::cChunkHandlerIQDC		m_PXGHandlerIQDC;
	pxgf::cChunkHandlerSR__		m_PXGHandlerSR__;

	//local copy of PXGF stream attributes.
	unsigned	m_uCenterFrequency_Hz;
	unsigned	m_uSampleRate_Hz;
	unsigned	m_uBandwidth_Hz;
	float		m_fTotalGain_dB;
	float		m_fFullScale_dBm;
	bool		m_bIsIQPacked;

	///variable used to limit amount of info printed to screen
	unsigned	m_uLastTick;


};
