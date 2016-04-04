

#include <unistd.h>
#include <iostream>
#include <exception>
#include <bitset>

#include "mraa.hpp"

void setup();

std::string binary(uint8_t);

void get_data(uint8_t);

void send_cmd(uint8_t, uint8_t*, uint8_t);

mraa::Uart* dev;

int main()
{
	setup();

	get_data(100);

	return EXIT_SUCCESS;
}

void send_cmd(uint8_t opcode, uint8_t* data, uint8_t data_length)
{
	uint8_t checksum = 0;

	dev->writeStr("$M<");

	char length_str[sizeof(std::string)];
	sprintf(length_str, "%x", data_length);
	dev->writeStr(length_str);
	checksum ^= data_length;

	char opcode_str[sizeof(std::string)];
	sprintf(opcode_str, "%x", opcode);
	dev->writeStr(opcode_str);
	checksum ^= opcode;

	for (int i = 0; i < data_length; i++)
	{
		char data_str[sizeof(std::string)];
		sprintf(data_str, "%x", data[i]);
		dev->writeStr(data_str);
		checksum ^= data[i];
	}

	char checksum_str[sizeof(std::string)];
	sprintf(checksum_str, "%x", checksum);

	dev->writeStr(checksum_str);
}


void get_data(uint8_t cmd)
{
	send_cmd(cmd, 0, 0);

	if(dev->dataAvailable())
	{
		std::cout << dev->readStr(0) << std::endl;
	}
}


void setup()
{
	try {
		dev = new mraa::Uart(0);
	} catch (std::exception& e) {
		std::cout << e.what() << ", likely invalid platform config" << std::endl;
	}

	std::string dev_path = dev->getDevicePath();

	std::cout << "dev path: " << dev_path << std::endl;

	/*
	try {
		//dev = new mraa::Uart("/dev/ttyACM0");
		dev = new mraa:: Uart(dev_path);
	} catch (std::exception& e) {
		std::cout << "Error while setting up raw UART, do you have a uart?" << std::endl;
		std::terminate();
	}
	 */

	if (dev->setBaudRate(115200) != mraa::SUCCESS) {
		std::cout << "Error setting parity on UART" << std::endl;
	}
	if (dev->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {
		std::cout << "Error setting parity on UART" << std::endl;
	}
	if (dev->setFlowcontrol(false, false) != mraa::SUCCESS) {
		std::cout << "Error setting flow control UART" << std::endl;
	}
}

std::string binary(uint8_t value)
{
	std::bitset<8> x(value);
	x.set();
	std::string binary_value = x.to_string<char,std::string::traits_type,std::string::allocator_type>();

	return binary_value;
}

