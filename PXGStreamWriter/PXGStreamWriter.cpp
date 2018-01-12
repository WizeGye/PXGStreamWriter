// PXGStreamWriter.cpp : Defines the entry point for the console application.
//

#ifndef __unix__
#include <WinSock2.h>
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <inttypes.h>
#endif

#ifndef __unix__

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>

#else

#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif //unix

#include <iostream>
#include <boost\asio.hpp>
#include <memory>
#include <fstream>
#include <vector>

#include "stdafx.h"
#include "pxgf\PXGReaderBase.h"
#include "pxgf\PXGSocketReader.h"
#include "pxgf\exceptions\PxgfException.h"

/* some shit later*/

using namespace std;
using namespace boost::asio::ip;

void OpenSocket(string Address);
void CloseSocket(void);
void EndianSwap(int numSwap, char* data);

boost::asio::io_service m_oIOService;
boost::asio::ip::tcp::socket m_oPXGFSocket(m_oIOService);
//string strSocketAddress = "jetson1.cr.uct.ac.za";
#ifndef __unix__
	SOCKET m_hSocket;
	//: Handle to the system socket
#else //unix
	int m_hSocket;
	//: Handle to the system socket
#endif 

/*	Input args:
 *		IP Address
 *		File length in chunks (no of data chunks to record)
 *		File name & path
 */
int main(int argc, char *argv[])
{
	string InputAddress;
	string FileName;
	char data[5];
	int count = 0;
	int numRead = 0;
	unsigned char sync[] = { 161,178,195,212 };
	unsigned char dataByte;
	unsigned int size = 0;
	char dataType[5];
	int syncNum = 0;
	bool synced = false;

	char dBTG[5] = "dBTG";
	char dBFS[5] = "dBFS";
	char GRG_[5] = "GRG_";
	char GIQP[5] = "GIQP";
	char GCF_[5] = "GCF_";
	char GCBW[5] = "GCBW";
	char RS__[5] = "RS__";
	char GSIQ[5] = "GSIQ";

	int descriptor = 0;
	int datachunks = 0;
	int discount = 0;
	unsigned int temp;
	int64_t rate;
	int64_t timestamp;
	int dcount;
	int MaxChunks = 1565 * 100;
	FILE* OutputFile;
	int qwe;
	time_t  timev;

	cout << "Initialising program" << endl;// << "Please enter address:";
	//cin >> InputAddress;
	if (argc != 4)
	{
		cout << "There must be three args! argc = " << argc-1 << endl;
		cout << "PXGStreamWriter.exe ";
		cout << "[IP Address] ";
		cout << "[Output file name] ";
		cout << "[Total number of data chunks to record]" << endl;
		//cout << "argv[0] = " << argv[0] << endl;
		return 0;
	}

//	char timeDate = time(&timev);

	InputAddress = string(argv[1]);
	FileName = string(argv[2]);
	sscanf(argv[3], "%d", &MaxChunks);

//	cout << "argv[1] = " << InputAddress << ", argv[2] = " << FileName << ", argv[3] = " << MaxChunks << endl;
	OpenSocket(InputAddress);

	cout << "Reading from socket:" << endl;

	cout << "Opening file" << endl;
	OutputFile = fopen(FileName.c_str(), "wb");
	//cout << "Wrote data to file" << endl;
	//ofstream myfile;
	//myfile.open("test1.txt", ios::binary);
	//myfile << data[0] << data[1] << data[2] << data[3] << endl;
	//myfile.close();
	
	
	while (datachunks < MaxChunks)//for (count = 0; count < 10000; count++)
	{
	/*	if (datachunks % 1000 == 0)
		{
			cout << "datachunks = " << datachunks << ",    descriptor chunks = " << descriptor << endl;
		} */
		if (synced)
		{
			//cout << "Sync acheived!" << endl;
			numRead = recv(m_hSocket, dataType, 4, 0);
			fwrite(dataType, 1, 4, OutputFile);
			//myfile << dataType[0] << dataType[1] << dataType[2] << dataType[3];
			dataType[4] = '\0';
			//cout << dataType;
			
			if (dataType[0] == GSIQ[3] && dataType[1] == GSIQ[2] && dataType[2] == GSIQ[1] && dataType[3] == GSIQ[0])
			{
				datachunks++;
				numRead = recv(m_hSocket, (char*)(&size), 4, 0);
				fwrite(&size, 1, 4, OutputFile);
				//myfile << size;
				numRead = recv(m_hSocket, (char*)(&timestamp), 8, 0);
				fwrite(&timestamp, 1, 8, OutputFile);
				//myfile << timestamp;
				//cout << "GSIQ data size = " << size << endl;
				//cout << "Timestamp = " << timestamp << endl;
				//cout << "GSIQ" << endl;
				for (dcount = 0; dcount < (size - 8) / 4; dcount++)
				{
					numRead = recv(m_hSocket, data, 4, 0);		//read data
					/*qwe = data[0];
					cout << qwe << " ";
					qwe = data[1];
					cout << qwe << " , ";
					qwe = data[2];
					cout << qwe << " ";
					qwe = data[3];
					cout << qwe << endl;*/
					fwrite(data, 1, 4, OutputFile);
					//myfile << data;
					//cout << "Reading data from GSIQ chunk" << endl;
				}
				numRead = recv(m_hSocket, data, 4, 0);			//read next sync word
				fwrite(data, 1, 4, OutputFile);
				//myfile << data;
				//numRead = recv(m_hSocket, dataType, 4, 0);		//read next type
			}
			else if (dataType[0] == dBTG[3] && dataType[1] == dBTG[2] && dataType[2] == dBTG[1] && dataType[3] == dBTG[0])
			{
				descriptor++;
				//cout << "New descriptor chunk. Datachunks = " << datachunks << endl;
				//cout << "Discriptor type = ";
				for (discount = 0; discount < 7; discount++)
				{
					//cout << dataType << " ";
					numRead = recv(m_hSocket, (char*)(&size), 4, 0);
					fwrite(&size, 1, 4, OutputFile);
					//myfile << size;
					for (dcount = 0; dcount < (size / 4); dcount++)
					{
						//put into file...
						numRead = recv(m_hSocket, data, 4, 0);		//read data
						fwrite(data, 1, 4, OutputFile);
						//myfile << data;

						/*if (discount == 6)							//double-checking if sample rate is correct
						{
							((char *)(&rate))[0] = data[0];
							((char *)(&rate))[1] = data[1];
							((char *)(&rate))[2] = data[2];
							((char *)(&rate))[3] = data[3];
							numRead = recv(m_hSocket, data, 4, 0);		//read data
							((char *)(&rate))[4] = data[0];
							((char *)(&rate))[5] = data[1];
							((char *)(&rate))[6] = data[2];
							((char *)(&rate))[7] = data[3];
							EndianSwap(8, (char*)(&rate));
							EndianSwap(8, (char*)(&rate));
							cout << "   Sampling Rate = " << rate;
							break;
						}*/
					}
					numRead = recv(m_hSocket, data, 4, 0);			//read next sync word
					fwrite(data, 1, 4, OutputFile);
					//myfile << data;
					if (discount < 6)
					{
						numRead = recv(m_hSocket, dataType, 4, 0);		//read next type
						fwrite(dataType, 1, 4, OutputFile);
						//myfile << dataType[0] << dataType[1] << dataType[2] << dataType[3];
					}

					/*numRead = recv(m_hSocket, dataType, 4, 0);
					dataType[4] = '\0';
					cout << "Next four bytes: " << dataType << endl;*/
				}
				//cout << endl << "Number of discriptor sections = " << descriptor << endl;
			}
			//synced = false;
		}
		else
		{
			numRead = recv(m_hSocket, (char*)(&dataByte), 1, 0);
			if (numRead == 1)
			{
				//			cout << "Scanning for sync Byte" << endl;
				if (dataByte == sync[3] && syncNum == 0)
				{
					syncNum = 1;
				}
				else if (dataByte == sync[2] && syncNum == 1)
				{
					syncNum = 2;
				}
				else if (dataByte == sync[1] && syncNum == 2)
				{
					syncNum = 3;
				}
				else if (dataByte == sync[0] && syncNum == 3)
				{
					synced = true;
					syncNum = 4;
					fwrite(sync + 3, 1, 1, OutputFile);
					fwrite(sync + 2, 1, 1, OutputFile);
					fwrite(sync + 1, 1, 1, OutputFile);
					fwrite(sync + 0, 1, 1, OutputFile);
					//myfile << sync[3] << sync[2] << sync[1] << sync[0];
				}
				else
				{
					syncNum = 0;
				}
			}

			else
			{
				cout << "Fatal error - cant read stream, closing" << endl;
				CloseSocket();
				break;
			}
		}
	}
	/*for (count = 0; count < 100000; count++)
	{
		numRead = recv(m_hSocket, (char*)(&dataByte), 1, 0);
		//cout << "Number of bytes read: " << numRead << endl;
		//cout << data[0] << data[1] << data[2] << data[3];
		qwe = dataByte;
		cout << qwe << " ";
	}*/

	cout << "Closing socket.." << endl;
	CloseSocket();
	cout << "Socket closed" << endl;
	cout << "Closing file..." << endl;
	//myfile.close();
	fclose(OutputFile);
	cout << "File closed" << endl;

	return 0;
}

void OpenSocket(string Address)
{
	/*
	//Set the initial values of member variables
	initialiseValues();

	//Discard data if we can't process as fast as the socket delivers it to prevent the TCP backing up too much.
	m_bDiscardData = true;*/

	//Extract port not from socket address. <- fixed spelling of iSeparator
	int iSeparator = Address.find(":");
	string strHostName = Address.substr(0, iSeparator);
	unsigned short usPortNo;
	string strPortNo;
	cout << "\033[22;34mPXGFStreamInput: Attempting to connect to " << strHostName << endl;
	
	if (iSeparator == -1)
	{
		usPortNo = 4444;
		cout << ". No port specified, defaulting to " << usPortNo << ".\033[22;37m" << endl;
	}
	else
	{
		strPortNo = Address.substr(iSeparator + 1);
		usPortNo = atoi(strPortNo.c_str());
		cout << " on TCP port " << strPortNo << ".\033[22;37m" << endl;
	}

	/*if (m_oPXGFSocket.is_open())
	{
		cout << "This motherfucker is OPEN!" << endl;
	}
	else
	{
		cout << "The socket is closed" << endl;
	}*/

	try
	{
		tcp::resolver oResolver(m_oIOService);
		tcp::resolver::query oQuery(strHostName, "");
		tcp::endpoint oEndPoint = oResolver.resolve(oQuery)->endpoint(); //This assumes the first returned IP is correct.
		oEndPoint.port(usPortNo);
		m_oPXGFSocket.connect(oEndPoint);
	}
	catch (boost::system::system_error const &oSystemError)
	{
		cout << "\033[22;34mPXGFStreamInput: Error: Unable to connect to host " << strHostName << " on TCP port " << strPortNo << ".\033[22;37m" << endl;
		cout << "\033[22;34mError was: " << oSystemError.what() << ".\033[22;37m" << endl;
		m_oPXGFSocket.shutdown(tcp::socket::shutdown_both);
		m_oPXGFSocket.close();
		return;
	}

	cout << "\033[22;34mPXGFStreamInput: Connected to " << strHostName << " on TCP port " << strPortNo << ".\033[22;37m" << endl;
	m_hSocket = m_oPXGFSocket.native_handle();
	/*
	//Set the function pointer for handling GSIQ to the initial handler.
	handleGSIQChunk_funcPtr = &cPXGFStreamInput::handleGSIQChunkInitial;

	//Get start sample offset etc.
	updateLocalParameters();
	//At this point the no of samples to skip in the stream is the StartSample number
	m_u64RemainingStartSampleOffset = m_u64StartSample;

	try
	{
		m_PXGSocketReader.processStream(m_oPXGFSocket.native_handle(), pxgf::cPXGReaderBase::Endian_Auto);
	}
	catch (pxgf::cPxgfException &e)
	{
		if (e.toString().compare("cPxgfException: Packed required no. of samples") == 0)
		{
			cout << "\033[22;34mPXGFStreamInput: Processed " << m_u64TotalSamplesProcessed << " samples per channel from source as required.\033[22;37m" << endl;
		}

		else
		{
			cout << "\033[22;34mPXGFStreamInput: Error: Unhandled PXGF exception:" << endl;
			cout << e.toString() << ".\033[22;37m" << endl;
		}
		m_oPXGFSocket.shutdown(tcp::socket::shutdown_both);
		m_oPXGFSocket.close();
	}*/
}

void CloseSocket(void)
{
	m_oPXGFSocket.shutdown(tcp::socket::shutdown_both);
	m_oPXGFSocket.close();
	return;
}

// swap endianess for up to 64 bytes
void EndianSwap(int numSwap, char* data)
{
	char swap[63];
	int x;

	for (x = 0; x < numSwap; x++)
	{
		swap[x] = data[x];
	}
	for (x = 0; x < numSwap; x++)
	{
		data[x] = swap[numSwap - 1 - x];
	}
}
