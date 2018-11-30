#include <iostream>
#include <string>
#include "TCPServer.h"

using namespace std; 

string serverIP = "127.0.0.1";

int main() {
	
	TCPServer server(serverIP, 54010);

	if (server.initWinsock()) {

		server.run();

	}


}

