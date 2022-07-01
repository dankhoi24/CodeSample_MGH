/**
 * @file Source.cpp
 * @author ndkhoi (nguyen.khoi@hitachivantara.com)
 * @brief source client side send and recieve message from client
 * @version 0.1
 * @date 2022-07-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include<iostream>
#include <thread>
#include<string>
#include<fstream>
#include<stdio.h>
#include <stdlib.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"Ws2_32.lib") 

//port
#define PORT 8080
// control text color in console
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
// socket client id
int socket_client;
// socket client addressc
sockaddr_in socket_address;
// socket client address length
int socketAddr_len;


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
 * @brief send message to server from user's input
 * 
 */
void sendMessage() {
	while (1) {
		std::string message_text = "";
		getline(std::cin, message_text);
		SetConsoleTextAttribute(hConsole, 2);
		std::cout << "[ You ] : " + message_text << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		send(socket_client, message_text.c_str(), message_text.length(), 0);
	}
}


/**
 * @brief  recieve message from server
 * 
 */
void receiveMessage() {
	while (true)
	{
		char buffer[1024] = {'\0'};
		recv(socket_client, buffer, 1024, 0);
		if (buffer[0] != '\0')
		{
			std::cout << buffer << std::endl;
		}
	}
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
	if ((socket_client = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		warningError("Create socket failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "[NOTIFY] : Socket created successfully.\n";

	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(PORT);
	socketAddr_len = sizeof(socket_address);
	if (inet_pton(AF_INET, "127.0.0.1", &socket_address.sin_addr) <= 0) {
		warningError("Invalid address");
	}
	// connect to server
	if (connect(socket_client, (struct sockaddr*)&socket_address, sizeof(sockaddr)) < 0) {
		warningError("Connect to server failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "[LOG] : Connection Successfull.\n";


		std::thread read_thread(receiveMessage);
		std::thread send_thread(sendMessage);
		if (read_thread.joinable()) {
			read_thread.join();
		}

		if (send_thread.joinable()) {
			send_thread.join();
		}



	return 0;
}
