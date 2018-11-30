#include <iostream>
#include "TCPClient.h"
#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main() {

	TCPClient *client = new TCPClient; 
	string msg = "a";


	if (client->initWinsock()) {

		client->connectSock();

		client->recvThread = thread([&] {
			client->threadRecv(); 
		});

		while (true) {
			getline(cin, msg); 
			client->sendMsg(msg);
		}

	}

	delete client; 
	cin.get(); 
	return 0;

}