/*
Name: Tisha, Sirazum Munira
Project: PA-1a (Regular Project Name) Networked Pingpong game
Instructor: Feng Chen
Class: cs7103-au17
LogonID: cs710307
*/
#pragma comment(lib,"ws2_32.lib")
#include <ws2tcpip.h>
#include <string>
#include <iostream>

SOCKET Connections[100];
int TotalConnections = 0;

void ClientHandlerThread(int index) //index = the index in the SOCKET Connections array
{
	int bufferlength; //Holds the length of the message a client sent
	while (true)
	{
		recv(Connections[index], (char*)&bufferlength, sizeof(int), NULL); //get buffer length
		char * buffer = new char[bufferlength]; //Allocate buffer
		recv(Connections[index], buffer, bufferlength, NULL); //get buffer message from client

		if (index == 0 && (buffer[0] != 'i' && buffer[0] != 'k')) { //client 1 can not press keys that are alotted for client2
			for (int i = 0; i < TotalConnections; i++) //For each client connection
			{	//sending message to other client and client itself
				send(Connections[i], (char*)&bufferlength, sizeof(int), NULL);//send the buffer length to client at index i
				send(Connections[i], buffer, bufferlength, NULL);//send the chat message to client at index i
			}
		}
		if (index == 1 && (buffer[0] != 'w' && buffer[0] != 's')) {  //client 2 can not press keys that are alotted for client1
			for (int i = 0; i < TotalConnections; i++) //For each client connection
			{	//sending message to other client and client itself
				send(Connections[i], (char*)&bufferlength, sizeof(int), NULL);//send the buffer length to client at index i
				send(Connections[i], buffer, bufferlength, NULL);//send the chat message to client at index i
			}
		}
		delete[] buffer; //Deallocate buffer to stop from leaking memory
	}
}

int main()
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr); //length of the address (required for accept call)
	addr.sin_addr.s_addr = INADDR_ANY; //broadcast to any client
	addr.sin_port = htons(1111); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listen for new connections
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //Bind the address to the socket
	listen(sListen, SOMAXCONN); //Places sListen socket in a state in which it is listening for an incoming connection. Note:SOMAXCONN = Socket Oustanding Max Connections

	SOCKET newConnection; //Socket to hold the client's connection
	int ConnectionCounter = 0; //# of client connections
	for (int i = 0; i < 2; i++) //upto 2 clients colud send message to each other not others
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection

		if (newConnection == 0) //If accepting the client connection failed
		{
			std::cout << "Failed to accept the client's connection." << std::endl;
		}
		else //If client connection properly accepted
		{
			TotalConnections += 1; //Incremenent total # of clients that have connected
			std::cout << "Client " << TotalConnections << " Connected!" << std::endl;
			Connections[i] = newConnection; //Set socket in array to be the newest connection before creating the thread to handle this client's socket.
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).

			if (i == 0) { //sending messageto client 1
				std::string buftest = "Hi Player1, Press any key to start playing, Press 'w'=up & 's'=down!!";
				int size = buftest.size(); //Get size of message in bytes and store it in int size
				send(Connections[i], (char*)&size, sizeof(int), NULL); //send Size of message
				send(Connections[i], buftest.c_str(), buftest.size(), NULL); //send Message
			}

			if (i == 1) {//sending message to client 2
				std::string buftest = "Hi Player2, Press any key to start playing Press 'i'=up & 'k'=down !!";
				int size = buftest.size(); //Get size of message in bytes and store it in int size
				send(Connections[i], (char*)&size, sizeof(int), NULL); //send Size of message
				send(Connections[i], buftest.c_str(), buftest.size(), NULL); //send Message
			}
		}
	}

	system("pause");
	return 0;
}