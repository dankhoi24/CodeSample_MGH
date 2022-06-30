/**
 * @file Source.cpp
 * @author ndkhoi (nguyen.khoi@hitachivantara.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include <string>
#include <thread>
#include <windows.h>


// string store input data from user
static std::string str = "1";

/**
 * @brief get input from user
 * 
 */
void inputter() {

	std:: string sth;
	while (1) {
		getline(std::cin, sth);
		str = sth;
	}
}

/**
 * @brief print string on console
 * 
 */
void outputter() {
	while (1) {
		std::cout <<"__" << str << std::endl;
		Sleep(2000);
	}
}


/**
 * @brief Main function
 * 
 * @return int 
 */
int main() {
	#pragma region [Init thread]
	std::thread t1(&inputter); // input thread
	std::thread t2(&outputter);// output thread
	#pragma endregion

	#pragma region [Wait thread]
	if (t1.joinable()) { // wait input thread
		t1.join();
	}
	if (t2.joinable()) { // wait output thread
		t2.join();
	} 
	#pragma endregion

}



