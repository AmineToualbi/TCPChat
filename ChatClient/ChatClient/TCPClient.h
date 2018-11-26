#pragma once
#include <string>
#include <thread>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

class TCPClient; 

typedef void(*MessageReceivedHandler)(std::string msg); 

class TCPClient
{
public:

	TCPClient();
	~TCPClient();
	bool initWinsock(); 
	void connectSock();
	void sendMsg(std::string txt);
	void listenRecvThread(MessageReceivedHandler handler);

private:
	SOCKET createSocket();
	std::string serverIP = "127.0.0.1";
	int serverPort = 54010;
	sockaddr_in hint;
	SOCKET clientSocket; 
	bool recvThreadRunning; 
	std::thread recvThread;
	void threadRecv(); 
	//std::string messageSent; 
	MessageReceivedHandler messageReceived; 


};

