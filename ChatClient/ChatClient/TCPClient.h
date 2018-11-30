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
	bool receiveMsg(MessageReceivedHandler handler);
	std::thread recvThread;
	void threadRecv();

private:
	SOCKET createSocket();
	std::string serverIP = "127.0.0.1";
	int serverPort = 54010;
	sockaddr_in hint;
	SOCKET serverSocket;		//This is the socket we will connect to. 
	bool recvThreadRunning; 


};

