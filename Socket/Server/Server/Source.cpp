/**
 * @file Source.cpp
 * @author ndkhoi (nguyen.khoi@hitachivantara.com)
 * @brief source file from server side, recieve and handle multi client
 * @version 0.1
 * @date 2022-07-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include<iostream>
#include<vector>
#include <string>
#include <thread>
#include<fstream>
#include<stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#include <mutex>
#pragma comment(lib,"Ws2_32.lib") 
#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
#endif

//port
#define PORT 8080
// listen socket (active socket)
int socket_listening;
// list socket after listen client (passive socket)
static std::vector<int> socket_passive;

// socket address
sockaddr_in socket_address;
// socket address lenght
int socketAddress_len;

int state;
// lock key handle send message to multi client
std::mutex mutex_sending;

/**
 * @brief format message to [ client ID ] : "message"
 * 
 * @param clientID 
 * @param message 
 * @return std::string 
 */
std::string formatMessage(int clientID, std::string message) {
	return "[ " + std::to_string(clientID) + " ] : " + message;
}

/**
 * @brief format message error to [ERROR - error code ] "message"
 * 
 * @param message 
 */
void warningError(std::string message) {
	int error_code = WSAGetLastError();
	std::cout << "[ERROR - " + std::to_string(error_code) + "] " + message << std::endl;
}

/**
 * @brief send message to all client are connected
 * 
 * @param clientID 
 * @param message 
 */
void sendAllClient(int clientID, std::string message) {
	mutex_sending.lock();
	for (auto socket_client = socket_passive.begin(); socket_client != socket_passive.end(); socket_client++) {
		if (*socket_client != clientID) {
			std::string send_mess = formatMessage(clientID, message);
			send(*socket_client, send_mess.c_str(), send_mess.length() , 0);
		}
	}
	mutex_sending.unlock();
}

/**
 * @brief handle after connected to client
 * 
 * @param socket_client 
 */
void handleClient(int socket_client) {
	send(socket_client, "Hello from Server!", 20, 0);
	while (1) {
		char buffer[1024] = { '\0' };
		recv(socket_client, buffer,sizeof(buffer), 0);
		std::cout <<"[ " << socket_client << " ] : " << std::string(buffer) << std::endl;
		sendAllClient(socket_client,std::string(buffer));
		if (send(socket_client, "", 20, 0) < 0) {
			return;
		}
	}

}


/**
 * @brief extend function for sending file
 * 
 * @param client_socket 
 * @return true 
 * @return false 
 */
bool sendFile(int client_socket) {

	std::cout << "[LOG] : Sending...\n";

	int bytes_sent;//= send(new_socket_descriptor , contents.c_str() , contents.length() , 0 );
	bytes_sent = send(client_socket, "123", 4, 0);

	std::cout << "[LOG] : Transmitted Data Size " << bytes_sent << " Bytes.\n";
	std::cout << WSAENOTCONN << std::endl;
	std::cout << WSAGetLastError() << std::endl;
	std::cout << "[LOG] : Transmitted Data Size " << bytes_sent << " Bytes.\n";

	std::cout << "[LOG] : File Transfer Complete.\n";
	return true;
}


/**
 * @brief Main function
 * 
 * @return int 
 */
int main() {

	//init environment
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) < 0) {
		warningError("Init environment failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "[NOTIFY] : Init environment succeeded\n";

	// create possive socket
	if ((socket_listening = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		warningError("Create socket failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "[NOTIFY] : Socket created successfully.\n";



	socket_address.sin_family = AF_INET;
	socket_address.sin_addr.s_addr = INADDR_ANY;
	socket_address.sin_port = htons(PORT);
	socketAddress_len = sizeof(socket_address);

	// bind address
	if (bind(socket_listening, (struct sockaddr*)&socket_address, sizeof(socket_address)) < 0) {
		warningError("Binding socket failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "[NOTIFY] : Bind Successful.\n";

	// listen client (max = 3)
	if (listen(socket_listening, 3) < 0) {
		warningError("Listning Failed");
		exit(EXIT_FAILURE);
	}
	std::cout<<"Listening . .  (Max spending connection: 3)\n";

	while (1) {
		int socket_client = 0;
		if ((socket_client = accept(socket_listening, (struct sockaddr*)&socket_address, (socklen_t*)&socketAddress_len)) < 0) {
			warningError("Acception failed");
			exit(EXIT_FAILURE);
		}
		socket_passive.push_back(socket_client);
		std::cout << "[NOTIFY] : Connected to Client.\n";
		std::cout << socket_passive.back()<< std::endl;
		std::thread t(handleClient, socket_passive.back());
		if (t.joinable()) {
			t.detach();
		}
	}


	return 0;

}
