#pragma once

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "PacketTypes.h"

#include "ThreadController.h"

#define DEFAULT_PORT "6000"
#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")


class EchoServer
{
public:
	struct ServSockInfo{
		SOCKET		*sock;
		EchoServer	*server;
	};
private:
	ServSockInfo		pack;
	void		*ThreadProc;
	WSADATA wsaData;
	int error;
	SOCKET ListenSocket;
	SOCKET ClientSocket;
	vector<SOCKET>		m_vClientSockets;
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen;
	struct addrinfo *result;
	struct addrinfo hints;
public:
	EchoServer();
	~EchoServer();

	void Initialize();
	void CreateSocket();
	void BindSocket();
	void StartListening();
	void AcceptClientSocket();
	void Poll();
	void Shutdown();

	bool EchoIncomingPackets(SOCKET sd);

	// Serever Thread
	static DWORD WINAPI listen_loop(void* param){
		while(true){
			((EchoServer*)param)->StartListening();
			((EchoServer*)param)->AcceptClientSocket();
		}
		//((EchoServer*)param)->Poll();
		return 1;	// error
	}

	static DWORD WINAPI echo_handler(void* param){
		int nRetval = 0;
		SOCKET sd = *((ServSockInfo*)param)->sock;

		if (! ((ServSockInfo*)param)->server->EchoIncomingPackets(sd) ) {
			printf("Echo incoming packets failed");
		}
	
		printf("Shutting connection down...");
	
		shutdown(sd, SD_SEND);
		closesocket(sd);
		return 1;
	}
};