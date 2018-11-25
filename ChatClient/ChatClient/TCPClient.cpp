#include "TCPClient.h"
#include <iostream>
#include <string>

using namespace std;


TCPClient::TCPClient()
{
}


TCPClient::~TCPClient()
{
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

	return sock;

}

void TCPClient::connectSock() {

	//SOCKET sock = createSocket();
	clientSocket = createSocket();

	hint.sin_family = AF_INET; 
	hint.sin_port = htons(serverPort);
	inet_pton(AF_INET, serverIP.c_str(), &hint.sin_addr);

	int connResult = connect(clientSocket, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cout << "Error: can't connect to server." << endl;
		closesocket(clientSocket);
		WSACleanup();
		return;
	}

}

void TCPClient::sendMsg(string txt) {

	char buf[4096];
	string message = txt;

	//do {

		//cout << "> ";
		//getline(cin, message);

		//if (message.size() > 0) {

			int sendResult = send(clientSocket, message.c_str(), message.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {

				ZeroMemory(buf, 4096);
				int bytesReceived = recv(clientSocket, buf, 4096, 0);

				if (bytesReceived > 0) {

					cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;

				}


			}


		//}

	//} while (message.size() > 0);

	//closesocket(clientSocket);
	//WSACleanup();

}
