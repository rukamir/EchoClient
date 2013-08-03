#include "EchoServer.h"

EchoServer::EchoServer()
{
    ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;

    result = NULL;
    recvbuflen = DEFAULT_BUFLEN;

	//DWORD maxLoops;
	//CreateThread(NULL, 
	//	0, 
	//	(LPTHREAD_START_ROUTINE) this, 
	//	&maxLoops, 
	//	0, 
	//	NULL);

}

EchoServer::~EchoServer()
{
	// cleanup
    closesocket(ClientSocket);
    WSACleanup();
}

void EchoServer::Initialize()
{
// Initialize Winsock
    error = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (error != 0) {
        printf("WSAStartup failed with error: %d\n", error);
        return;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    error = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( error != 0 ) {
        printf("getaddrinfo failed with error: %d\n", error);
        WSACleanup();
        return;
    }
}

void EchoServer::CreateSocket()
{
	// Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }
}

void EchoServer::BindSocket()
{
	// Setup the TCP listening socket
    error = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (error == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }
}

void EchoServer::StartListening()
{
    //freeaddrinfo(result);

    error = listen(ListenSocket, SOMAXCONN);
    if (error == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }
}

void EchoServer::AcceptClientSocket()
{
	while(1){
		// Accept a client socket
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
		//	closesocket(ListenSocket);
		//	WSACleanup();
			return;
		} else {
			pack.server = this;
			pack.sock = &ClientSocket;
			printf("Client connected!\n");
			ThrdCon->CreateNewThread(EchoServer::echo_handler, (void*)&pack, false);

		}
		m_vClientSockets.push_back(ClientSocket);
	}
}

void EchoServer::Poll()
{
	// Receive until the peer shuts down the connection
    do {

        //error = recv(ClientSocket, recvbuf, recvbuflen, 0);
		for(vector<SOCKET>::iterator clSock = m_vClientSockets.begin();  clSock != m_vClientSockets.end(); ++clSock){
			error = recv(*clSock, recvbuf, recvbuflen, 0);
			if (error > 0) {
				printf("Bytes received: %d\n", error);
				printf((char*)recvbuf, error);

				// Echo the buffer back to the sender
				iSendResult = send( *clSock, recvbuf, error, 0 );
				if (iSendResult == SOCKET_ERROR) {
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(*clSock);
					WSACleanup();
					return;
				}
				printf("Bytes sent: %d\n", iSendResult);
			}
			else if (error == 0)
				printf("Connection closing...\n");
			else if(error == WSAENOTCONN){
				m_vClientSockets.erase(clSock);

			} else {
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(*clSock);
				WSACleanup();
				return;
			}

			if (((char*)recvbuf)[0] == 'z'){
				break;
			}
		}
	} while (true);// (error > 0);
}

void EchoServer::Shutdown()
{
	closesocket(ListenSocket);

	// Close all client sockets
	for(const auto &clSock : m_vClientSockets){
		closesocket(clSock);
	}

	// shutdown the connection since we're done
    error = shutdown(ClientSocket, SD_SEND);
    if (error == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return;
    }
}

bool EchoServer::EchoIncomingPackets(SOCKET sd)
{
	const int kBufferSize = 50;
    // Read data from client
    char acReadBuffer[kBufferSize];
    int nReadBytes;
    do {
		for(int i = 0; i <kBufferSize; i++)
			acReadBuffer[i] = '\0';

        nReadBytes = recv(sd, acReadBuffer, kBufferSize, 0);
        if (nReadBytes > 0) {

	// Print data here for testing
           //printf("Received "); printf((char*)nReadBytes); printf(" bytes from client.");
			//printf(acReadBuffer);printf(acReadBuffer);
			//printf(acReadBuffer);
			printf("id: %i \nX: %f\nY: %f\n", ((Packet*)acReadBuffer)->id, 
				((Packet*)acReadBuffer)->x, ((Packet*)acReadBuffer)->y);
		vector<SOCKET>::iterator i = m_vClientSockets.begin();
		for (; i < m_vClientSockets.end(); i++)
		{
			int nSentBytes = 0;
			while (nSentBytes < nReadBytes) {
				int nTemp = send(*i, acReadBuffer + nSentBytes, nReadBytes - nSentBytes, 0);
				if (nTemp > 0) {
					/*printf(acReadBuffer);*/
						//printf("Sent "); printf((CHAR*)nTemp);
						//printf(" bytes back to client./n");
						nSentBytes += nTemp;
				}
				else if (nTemp == SOCKET_ERROR)
				{}	
				else
				{
					printf("Peer unexpectedly dropped connection!");
					continue;
				}
			}
		}
        }
        else if (nReadBytes == SOCKET_ERROR) {
			//socket lost
			//m_vClientSockets.erase(clSock);
			vector<SOCKET>::iterator i = m_vClientSockets.begin();
			for (; i < m_vClientSockets.end(); i++)
			{
				if (sd == *i)
				{
					m_vClientSockets.erase(i);
					break;
				}
			}

           // return false;
        }
    } while (nReadBytes != 0);

    printf("Connection closed by peer.");
    return true;
}