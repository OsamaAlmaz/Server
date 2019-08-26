#include <iostream>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")


using namespace std;

void main()
{

	//Initialize Winsock. 
	WSADATA WSData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &WSData);

	if (wsOk != 0) {
		cerr << "Cant Initialize WinSock!! Quiting" << endl;

	}

	//create a socket. 
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	/*
		AF_INET represent the IPv4 internet protocol while the AF_INET6 represent
		the IPv6.
		SOCK_STREAM: represents the type of reading the message
		it will represent the TCP rather than the datagram represents
		the UDP.
		represents the message read in a continous stream rather
		than chunks in datagrams.

	*/
	if (listening == INVALID_SOCKET) {
		cerr << "Cant create a socket! Quiting" << endl;
		return;
	}


	//bind the socket to an ip address and port 
	/*
	The sockaddr_in specifies the transport address and the port.
	for the socket that was made.
	for the AF_INET.
	*/
	/*
	#include <netinet/in.h>

	struct sockaddr_in {
		short            sin_family;   // e.g. AF_INET
		unsigned short   sin_port;     // e.g. htons(3490)
		struct in_addr   sin_addr;     // see struct in_addr, below
		char             sin_zero[8];  // zero this if you want to
	};

struct in_addr {
	unsigned long s_addr;  // load with inet_aton()
};
	*/

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	/*
	htons converts the value to TCP/IP network order.
	*/
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	//could also use inet_pton; 
	/*
	The bind function associates a local address with a socket.

	*/

	bind(listening, (sockaddr*)& hint, sizeof(hint));


	//tell winsock the socket for listening. 
	/*
	This tells the server to listen to as much SOMAXCONN
	at the same time.
	*/
	listen(listening, SOMAXCONN);

	//wait for connection. 

	sockaddr_in client;
	int clientSize = sizeof(client);
	/*
	accept: used by the server to accept connection request from a
	client.
	*/

	SOCKET clientSocket = accept(listening, (sockaddr*)& client, &clientSize);

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);


	if (getnameinfo((sockaddr*)& client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {

		cout << host << "connected on port" << service << endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected on port" << ntohs(client.sin_port);
	}


	//close listening sockets

	closesocket(listening);

	//while loop: accept the echo message back to client. 

	char buff[4096];
	while (true) {
		ZeroMemory(buff, 4096);
		//wait for the client to send data. 
		int bytesRecieved = recv(clientSocket, buff, 4096, 0);
		if (bytesRecieved == SOCKET_ERROR) {
			cerr << "Error in recv().Quitting" << endl;
		}
		if (bytesRecieved == 0) {
			cout << "Client is disconnected" << endl;

			break;
		}

		//Echo message back to the client. 
		send(clientSocket, buff, bytesRecieved + 1, 0);


	}
	//close the socket. 
	closesocket(clientSocket);
	//Shutdown winSock. 
	WSACleanup();

}