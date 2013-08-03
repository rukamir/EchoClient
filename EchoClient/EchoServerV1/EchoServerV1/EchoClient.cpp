#include "EchoClient.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

NetClient::NetClient(){
    SOCKET ConnectSocket = INVALID_SOCKET;
    result = NULL;
    ptr = NULL;
    sendbuf = "this is a test";
    //recvbuf = char[DEFAULT_BUFLEN];
    recvbuflen = DEFAULT_BUFLEN;
}

NetClient::NetClient(int argc, char* argv[])
{
	
    SOCKET ConnectSocket = INVALID_SOCKET;
    result = NULL;
    ptr = NULL;
    sendbuf = "this is a test";
    //recvbuf = char[DEFAULT_BUFLEN];
    recvbuflen = DEFAULT_BUFLEN;

	// Sets ip to local host
	// re assign using SetServIP()
	serverIP = argv[1];

}

NetClient::~NetClient()
{
	closesocket(m_sClientSock);
    WSACleanup();
}

void NetClient::ValidateClient()
{
	//// Validate the parameters
 //   if (argc != 2) {
 //       printf("usage: %s server-name\n", argv[0]);
 //       return;
 //   }
}

void NetClient::Initialize(){
	// Initialize Winsock
    error = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (error != 0) {
        printf("WSAStartup failed with error: %d\n", error);
        return;
    }

	ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    //error = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	//one below works
  // error = getaddrinfo("192.168.1.11", DEFAULT_PORT, &hints, &result);
   error = getaddrinfo(serverIP, DEFAULT_PORT, &hints, &result);
    if ( error != 0 ) {
        printf("getaddrinfo failed with error: %d\n", error);
        WSACleanup();
        return;
    }
}

void NetClient::Initialize(int argc, char* argv[])
{
    if (argc != 2)
        printf("usage: %s server-name\n", argv[0]);

	// Initialize Winsock
    error = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (error != 0) {
        printf("WSAStartup failed with error: %d\n", error);
        return;
    }

	ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    //error = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	//one below works
  // error = getaddrinfo("192.168.1.11", DEFAULT_PORT, &hints, &result);
   error = getaddrinfo(serverIP, DEFAULT_PORT, &hints, &result);
    if ( error != 0 ) {
        printf("getaddrinfo failed with error: %d\n", error);
        WSACleanup();
        return;
    }

}

void NetClient::Connect()
{
 // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        m_sClientSock = socket(ptr->ai_family, ptr->ai_socktype, 

            ptr->ai_protocol);
//u_long iMode=1;
//ioctlsocket(m_sClientSock,FIONBIO,&iMode);

        if (m_sClientSock == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return;
        }

        // Connect to server.
        //error = connect( m_sClientSock, ptr->ai_addr, (int)ptr->ai_addrlen);
		error = connect( m_sClientSock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (error == SOCKET_ERROR) {
            closesocket(m_sClientSock);
            m_sClientSock = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (m_sClientSock == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return;
    }
}

void NetClient::SetBuffer(void* msg)
{
	sendbuf = msg;
}

void NetClient::SendBuffer()
{
	// Send an initial buffer
    error = send( m_sClientSock, (char*)sendbuf, sizeof(Packet), 0 );//(int)strlen((char*)sendbuf), 0 );
    if (error == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(m_sClientSock);
        WSACleanup();
        return;
    }

    printf("Bytes Sent: %ld\n", error);
	//printf((char*)recvbuf, error);

}

void NetClient::CheckIncoming()
{
	// Receive until the peer closes the connection
    do {

        error = recv(m_sClientSock, recvbuf, recvbuflen, 0);
        if ( error > 0 )
		{
            printf("Bytes received: %d\n", error);
			printf((char*)recvbuf, error);
		}
        else if ( error == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( error > 0 );
}

void NetClient::Shutdown()
{
	 // shutdown the connection since no more data will be sent
    error = shutdown(m_sClientSock, SD_SEND);
    if (error == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(m_sClientSock);
        WSACleanup();
        return;
    }
}