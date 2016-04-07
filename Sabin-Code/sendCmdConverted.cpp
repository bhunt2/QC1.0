

#include <unistd.h>
#include <iostream>
#include <exception>
#include <bitset>

#include "mraa.hpp"

void setup();

std::string binary(uint8_t);

void get_data(uint8_t, uint8_t);

void send_msp(uint8_t, uint8_t*, uint8_t);

void send_cmd(uint8_t, uint8_t*, uint8_t);

mraa::Uart* dev;

int main(int argc, char* argv[])
{
	/*if(argc < 2){
		std:: cerr << "Usage: " << argv[0] << "COMMAND" << std::endl;
		return 1;
	}*/

	setup();
	//get_data(108, 48);
	get_data(108,0);//send_msp(100,0,0);
	//get_data((uint8_t)argv[0], (uint8_t)argv[1]);

	return EXIT_SUCCESS;
}


void send_cmd(uint8_t opcode, uint8_t* data, uint8_t data_length)
{
/*
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

	//for (int i = 0; i < data_length; i++)
	//{
	//	char data_str[sizeof(std::string)];
	//	sprintf(data_str, "%x", data[i]);
	//	dev->writeStr(data_str);
	//	checksum ^= data[i];
	//}

	char checksum_str[sizeof(std::string)];
	sprintf(checksum_str, "%x", checksum);

	dev->writeStr(checksum_str);
	dev->flush();
*/
}

void send_msp(uint8_t opcode, uint8_t * data, uint8_t n_bytes) {
  uint8_t checksum = 0;
   
  // Send the MSP header and message length
  dev->writeStr("$M<");

  const char* length = reinterpret_cast<const char*>(&n_bytes);
  dev->writeStr(length);
  checksum ^= n_bytes;
 
  // Send the op-code
  const char* code = reinterpret_cast<const char*>(&opcode);
  dev->writeStr(code);
  checksum ^= opcode;

  /*
  // Send the data bytes
  for(int i = 0; i < n_bytes; i++) {
    dev->writeStr(data[i]);
    checksum ^= data[i];
  }
   */

  // Send the checksum
  const char* cs = reinterpret_cast<const char*>(&checksum);
  dev->writeStr(cs);
}


void get_data(uint8_t cmd, uint8_t data_length)
{
	//send_cmd(cmd, 0, data_length);
	send_msp(cmd,0,data_length);
	usleep(1000);

	while(1)
	{
		if(dev->dataAvailable())
		{
			//char* buff;
			//dev->read(buff, 3);
		
			std::cout << "Data Response: " << dev->readStr(1) << "\n" << std::endl;
		} /*else{
			std::cout << "Not Serial Data Available.\n" << std::endl;
		}*/
	}
}


void setup()
{
	try {
		std::cout << "Initializing UART..." << std::endl;
		dev = new mraa::Uart(0);

	} catch (std::exception& e) {
		std::cout << e.what() << ", likely invalid platform config" << std::endl;
	}

	/*
	try {
		//dev = new mraa::Uart("/dev/ttyMFD1");
		//dev = new mraa:: Uart(dev_path);
	} catch (std::exception& e) {
		std::cout << "Error while setting up raw UART, do you have a uart?" << std::endl;
		std::terminate();
	}
	 */

	int baudrate = 9600;//115200

	if (dev->setBaudRate(baudrate ) != mraa::SUCCESS) {
		std::cout << "Error setting parity on UART" << std::endl;
	}else{
		std::cout << "Setting Baudrate to " << baudrate << "..." << std::endl;
	}

	if (dev->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {
		std::cout << "Error setting parity on UART" << std::endl;
	}else{
		std::cout << "Setting Mode to 8N1..." << std::endl;
	}

	if (dev->setFlowcontrol(false, false) != mraa::SUCCESS) {
		std::cout << "Error setting flow control UART" << std::endl;
	}else{
		std::cout << "Setting Flow Control..." << std::endl;
	}

	std::cout << "Device Path: " << dev->getDevicePath() << std::endl;
}

std::string binary(uint8_t value)
{
	std::bitset<8> x(value);
	x.set();
	std::string binary_value = x.to_string<char,std::string::traits_type,std::string::allocator_type>();

	return binary_value;
}


