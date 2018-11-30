/*
	Steps to set up a Winsock client (from https://docs.microsoft.com/en-us/windows/desktop/winsock/winsock-client-application):
		- Creating a socket. 
		- Connect that socket to the IP address & port of the server. 
			-> IP address & port of the server are stored in the sockaddr_in variable, hint. 
		- Sending & receiving the data on the client. 
			-> For the receiving part, we use an external thread to continuously receive data. 
			-> For the sending part, we continuously check for input & send the message when the user presses enter. 
				-> this part is done in the main.cpp file. 
		- Disconnect client. 


*/

#include "TCPClient.h"
#include <iostream>
#include <string>
#include <thread>

using namespace std;


//When we create the client, we do not want the thread to run & try to receive data from the server until 
TCPClient::TCPClient()
{
	recvThreadRunning = false; 
}


TCPClient::~TCPClient()
{
	closesocket(serverSocket); 
	WSACleanup();
	if (recvThreadRunning) {
		recvThreadRunning = false;
		recvThread.join();	//Destroy safely to thread. 
	}
}


bool TCPClient::initWinsock() {
	
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		cout << "Error: can't start Winsock." << endl;
		return false;
	}
	return true;
}

SOCKET TCPClient::createSocket() {

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cout << "Error: can't create socket." << endl;
		WSACleanup();
		return -1;
	}

	//Specify data for hint structure. 
	hint.sin_family = AF_INET;
	hint.sin_port = htons(serverPort);
	inet_pton(AF_INET, serverIP.c_str(), &hint.sin_addr);

	return sock;

}

void TCPClient::threadRecv() {

	recvThreadRunning = true;
	while (recvThreadRunning) {

		char buf[4096];
		ZeroMemory(buf, 4096);

		int bytesReceived = recv(serverSocket, buf, 4096, 0);	
		if (bytesReceived > 0) {			//If client disconnects, bytesReceived = 0; if error, bytesReceived = -1;
				
			std::cout << string(buf, 0, bytesReceived) << std::endl;

		}

	}
}

void TCPClient::connectSock() {

	//If !initWinsock -> return false. 

	serverSocket = createSocket();

	int connResult = connect(serverSocket, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cout << "Error: can't connect to server." << endl;
		closesocket(serverSocket);
		WSACleanup();
		return;
	}

}

void TCPClient::sendMsg(string txt) {

	if (!txt.empty() && serverSocket != INVALID_SOCKET) {

		send(serverSocket, txt.c_str(), txt.size() + 1, 0);

		//It wouldn't work with the previous version bc while we were constantly listening for received msgs, we would keep caling this fct. 
		//This fct would send the message & try to handle the receiving too. It would get stuck while waiting for a received msg. 
	}

}
