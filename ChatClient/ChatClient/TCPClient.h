#pragma once
#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

class TCPClient
{
public:

	TCPClient();
	~TCPClient();
	bool initWinsock(); 
	void connectSock();
	void sendMsg(std::string txt);

private:
	SOCKET createSocket();
	std::string serverIP = "127.0.0.1";
	int serverPort = 54010;
	sockaddr_in hint;
	SOCKET clientSocket; 


};

