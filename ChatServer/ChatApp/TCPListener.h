#pragma once

#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

class TCPListener; 

//Callback fct = fct with fct as parameter.
typedef void(*MessageReceivedHandler)(TCPListener* listener, int socketID, std::string msg);

class TCPListener {
public:
	TCPListener(); 
	TCPListener(std::string ipAddress, int port, MessageReceivedHandler handler);
	~TCPListener(); 

	void sendMsg(int clientSocket, std::string msg);
	bool initWinsock(); 
	void run();
	void cleanupWinsock(); 
	

private: 
	SOCKET createSocket(); 
	SOCKET waitForConnection(SOCKET listeningSocket);
	std::string listenerIPAddress; 
	int listenerPort; 
	MessageReceivedHandler messageReceived; 
};