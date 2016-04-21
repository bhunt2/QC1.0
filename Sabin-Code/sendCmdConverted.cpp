

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
	uint8_t cmd_ident = 100;
	uint8_t cmd_att = 108;
	
	setup();
	
	get_data(cmd_ident, 0);

	return EXIT_SUCCESS;
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
	printf ("Sending Command: %d \n", cmd);

	send_cmd(cmd, 0, data_length);

	usleep(1000);

	while(1)
	{
		if(dev->dataAvailable())
		{
			//char* buff;
			//dev->read(buff, 3);
		
			std::cout << "Data Response: " << dev->readStr(20) << "\n" << std::endl;
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

	int baudrate = 115200; // 9600;//115200

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


