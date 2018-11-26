#include <iostream>
#include "TCPClient.h"
#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

void messageReceived(string msgReceived) {
	cout << "SERVER: " << msgReceived << endl;
}


int main() {

	TCPClient *client = new TCPClient; 
	string msg = "a";


	if (client->initWinsock()) {

		client->connectSock();

		client->listenRecvThread(messageReceived); 
		while (true) {
			getline(cin, msg); 
			client->sendMsg(msg);
		}

	}

	/* (true) {
		//client.sendMsg();
		getline(cin, msg);
		//cout << msg << endl;
		//client.sendMsg(msg);
		client.sendMsg(msg);
	}*/



}