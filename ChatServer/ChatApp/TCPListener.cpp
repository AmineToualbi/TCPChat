#include "TCPListener.h"
#include <iostream>
#include <string>
#include <sstream>

const int MAX_BUFFER_SIZE = 4096;

TCPListener::TCPListener() { }

TCPListener::TCPListener(std::string ipAddress, int port, MessageReceivedHandler handler)
	: listenerIPAddress(ipAddress), listenerPort(port), messageReceived(handler) {


}
TCPListener::~TCPListener() {
	cleanupWinsock();
	std::cout << "Server is shutting down. Goodbye." << std::endl;
}

void TCPListener::sendMsg(int clientSocket, std::string msg) {

	send(clientSocket, msg.c_str(), msg.size() + 1, 0);

}
bool TCPListener::initWinsock() {

	WSADATA data; 
	WORD ver = MAKEWORD(2, 2); 

	int wsInit = WSAStartup(ver, &data); 

	//TODO: Error handling. 
	
	return wsInit == 0;

}
void TCPListener::run() {

	char buf[MAX_BUFFER_SIZE];

	while (true) {
		
		//1. Create a listening socket. 
		SOCKET listeningSocket = createSocket(); 
		if (listeningSocket == INVALID_SOCKET) {
			break; 
		}

		fd_set master;				//File descriptor storing all the connections. 	
		FD_ZERO(&master);			//Empty file file descriptor. 

		FD_SET(listeningSocket, &master);		//Add listening socket to file descriptor. 

		while (true) {

			fd_set copy = master;	//Create new file descriptor bc the file descriptor gets destroyed every time. 
			int socketCount = select(0, &copy, nullptr, nullptr, nullptr);				//Select() determines status of sockets. 

			for (int i = 0; i < socketCount; i++) {				//Server can only accept connection & receive msg from client. 

				SOCKET sock = copy.fd_array[i]; 

				if (sock == listeningSocket) {				//Case 1: accept new connection.
					
					SOCKET client = accept(listeningSocket, nullptr, nullptr);	
					FD_SET(client, &master);		//Add new connection to list of connected clients. 
					std::string welcomeMsg = "Welcome to Amine's Chat.\n"; 
					//send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
					std::cout << "New user joined the chat." << std::endl;

					
				}
				else {										//Case 2: receive a msg. 
					
					char buf[MAX_BUFFER_SIZE]; 
					ZeroMemory(buf, MAX_BUFFER_SIZE);
					int bytesReceived = recv(sock, buf, MAX_BUFFER_SIZE, 0);

					if (bytesReceived <= 0) {	//No msg = drop client. 
						closesocket(sock); 
						FD_CLR(sock, &master);	//Remove connection from file director.
					}
					else {						//Send msg to other clients & not listening socket. 
						
						for (int i = 0; i < master.fd_count; i++) {
							SOCKET outSock = master.fd_array[i]; 
//That line for multi clients	//if (outSock != listeningSocket && outSock != sock) {
							if(outSock!=listeningSocket && outSock != sock){
								std::ostringstream ss; 
								ss << "SOCKET #" << sock << ": " << buf << "\n";
								std::string strOut = ss.str();
								send(outSock, strOut.c_str(), strOut.size() + 1, 0);
								
							}
						}

						std::cout << std::string(buf, 0, bytesReceived) << std::endl;

					}

				}
			}
		}

		
	}

}
void TCPListener::cleanupWinsock() {

	WSACleanup();

}

SOCKET TCPListener::createSocket() {

	SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);	//AF_INET = IPv4. 

	if (listeningSocket != INVALID_SOCKET) {

		sockaddr_in hint;		//Structure used to bind IP address & port to specific socket. 
		hint.sin_family = AF_INET; 
		hint.sin_port = htons(listenerPort); 
		//Converts IP string to bytes & pass it to our hint. hint.sin_addr is the buffer. 
		inet_pton(AF_INET, listenerIPAddress.c_str(), &hint.sin_addr);	

		int bindCheck = bind(listeningSocket, (sockaddr*) &hint, sizeof(hint));
		if (bindCheck != SOCKET_ERROR) {

			int listenCheck = listen(listeningSocket, SOMAXCONN);
			if (listenCheck == SOCKET_ERROR) {
				return -1;
			}
		}
		else {
			return -1;

		}

		return listeningSocket; 
																							
	}

}

SOCKET TCPListener::waitForConnection(SOCKET listeningSocket) {

	SOCKET client = accept(listeningSocket, NULL, NULL); 
	return client; 

}


	//Structure used to identify client on server side to print IP. 
		/*sockaddr_in client;
		int clientSize = sizeof(client);

		//2. Wait for connection. 
		SOCKET clientSocket = waitForConnection(listeningSocket);
		if (clientSocket != INVALID_SOCKET) {

			char host[NI_MAXHOST];		//Client's remote name. 
			char service[NI_MAXSERV];	//Service (i.e. port) the client is connected on.

			ZeroMemory(host, NI_MAXHOST);
			ZeroMemory(service, NI_MAXSERV);

			if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
				std::cout << host << " connected on port " << service << std::endl;
			}
			else {
				inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
				std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
			}

			closesocket(listeningSocket);	//Close listening socket so no one else can connect. 

			int bytesReceived = 0;		//Data received by socket. 
			std::string welcomeMsg = "Welcome to Amine's chat."; 

			send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);

			

			
			//3. Receive & send the msg.
			do {		//In example, it has do {} while (>0);
				ZeroMemory(buf, MAX_BUFFER_SIZE);		//Clear memory of buffer.
				bytesReceived = recv(clientSocket, buf, MAX_BUFFER_SIZE, 0);
				
				if (bytesReceived > 0) {
					if (messageReceived != NULL) {
						//Copy content of buffer from 0 -> bytesReceived into messageReived.
						messageReceived(this, clientSocket, std::string(buf, 0, bytesReceived));	
						
					}
				}
			}
			while (bytesReceived > 0);
			*/
			//closesocket(clientSocket); 