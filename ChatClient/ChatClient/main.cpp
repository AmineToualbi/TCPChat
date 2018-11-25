#include <iostream>
#include "TCPClient.h"
#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main() {

	TCPClient client; 

	if (client.initWinsock()) {
			client.connectSock();

	}
	string msg = "a";

	while (true) {
		//client.sendMsg();
		getline(cin, msg);
		//cout << msg << endl;
		client.sendMsg(msg);
	}

}