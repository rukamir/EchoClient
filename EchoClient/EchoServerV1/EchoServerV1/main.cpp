#include "EchoClient.h"
#include "EchoServer.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "ThreadController.h"

using namespace std;

#pragma comment(lib, "Ws2_32.lib")

void main(int argc, char* argv[])
{
/*// server main
	EchoServer server;
	server.Initialize();
	server.CreateSocket();

	server.BindSocket();
	server.StartListening();

	//ThrdCon->CreateNewThread(EchoServer::listen_loop, ((void*)&server), false);

	while(true)
		server.AcceptClientSocket();
	//server.AcceptClientSocket();

	int count;
	server.Poll();
	//while(true){
	//	//count++;
	//	//--count;
	//}

	server.Shutdown();
//*/

const int BUFFER_SIZE = 512;
	NetClient client(argc, argv);
	client.SetServIP("192.168.1.3");
	client.Initialize(argc, argv);
	client.Connect();
	//client.SetBuffer("MESSAGEBLAH");
	//client.SendBuffer();
	char *textIn = new char[BUFFER_SIZE];
	string x="0", z="0", id = "0";
	//std::stoi( str );
	//memset(textIn, 0, 999);
	//char textIn[BUFFER_SIZE];
	//std::string text;
	Packet* item = new Packet(stoi(id), stoi(x), stoi(z));

	cout << "Enter X: ";
	cin >> x;
	cout << "Enter Z: ";
	cin >> z;
	//Packet* item = new Packet(stoi(id), stoi(x), stoi(z));



	//Packet* item = new Packet();
	//Packet

	ThrdCon->CreateNewThread(NetClient::listen_loop, ((void*)&client), false);


	do{
	//std::cout << "\nSend: ";
	//std::cin.getline(textIn, BUFFER_SIZE);
	cout << "Enter X: ";
	cin >> x;
	cout << "Enter Z: ";
	cin >> z;
	item->x = stoi(x);
	item->y = stoi(z);

	//textIn = text.c_str();
	//textIn = "l\0";
	//client.SetBuffer(textIn);
	client.SetBuffer((char*)item);
	cout << ((Packet*)client.recvbuf)->x << endl;
	cout << ((Packet*)client.recvbuf)->y << endl;

	client.SendBuffer();
	if (textIn[0] == 'z')
		break;
	} while (true);
	//client.CheckIncoming();

	client.Shutdown();
	delete textIn;

//*/
}