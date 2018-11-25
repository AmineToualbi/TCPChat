#include <iostream>
#include <string>
#include "TCPListener.h"

using namespace std; 

void Listener_MessageReceived(TCPListener* listener, int client, string msg);
string serverIP = "127.0.0.1";

int main() {
	
	TCPListener server(serverIP, 54010, Listener_MessageReceived);

	if (server.initWinsock()) {

		server.run();

	}


}

void Listener_MessageReceived(TCPListener* listener, int client, string msg) {

	//Echo message back to the client. 
	listener->sendMsg(client, msg);

}