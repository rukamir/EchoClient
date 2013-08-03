#pragma once

#include <Winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "PacketTypes.h"

#define DEFAULT_PORT "6000"
#define PORT 600
#define DEFAULT_BUFLEN 512
#define IP "96.226.153.76"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class NetClient
{
private:
	PCSTR serverIP;
struct sockaddr_in serv;
	struct addrinfo *result, *ptr, hints;
	WSADATA wsaData;
	int error;
	void *sendbuf;
	int recvbuflen;
	SOCKET		m_sListenSock;
	SOCKET		m_sClientSock;
	WSADATA		m_SocketData;
public:
	char recvbuf[DEFAULT_BUFLEN];
	NetClient();
	NetClient(int argc, char* argv[]);
	~NetClient();

	void ValidateClient();
	void Initialize();
	void Initialize(int argc, char* argv[]);
	void SetServIP(PCSTR ip){serverIP=ip;}
	void Connect();
	void SetBuffer(void*);
	void SendBuffer();
	void CheckIncoming();
	void Shutdown();

	static DWORD WINAPI listen_loop(void* param){
		((NetClient*)param)->CheckIncoming();
		return 1;	// error
	}
};