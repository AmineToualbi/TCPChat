#include <iostream>
#include "TCPClient.h"
#include <string>
#include <sstream>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main() {

	TCPClient *client = new TCPClient; 
	string msg = "a";
	string usernameEntered; 


	cout << "Enter your username." << endl;
	cin >> usernameEntered;
	client->username = usernameEntered; 

	if (client->initWinsock()) {

		client->connectSock();

		client->recvThread = thread([&] {
			client->threadRecv(); 
		});

		while (true) {
			getline(cin, msg); 
			std::string messageToSend; 
			if (client->joinChat == false) {
				std::ostringstream ss;
				ss << client->username << ": " << msg;
				messageToSend = ss.str();
			}
			else if (client->joinChat == true) {
				std::ostringstream ss; 
				ss << client->username << " joined the chat!"; 
				messageToSend = ss.str(); 
				client->joinChat = false; 
			}
			client->sendMsg(messageToSend);
		}

	}


	delete client; 
	cin.get(); 
	return 0;

}