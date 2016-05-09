
#include <exception>
#include <iostream>
#include <sstream>

#include "protocol.h"

#include "types.h"


protocol::protocol(int debug){
	DEBUG = debug;
	setup_uart();
}

protocol::~protocol(){
	delete device;
}


std::string protocol::string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

void protocol::setup_uart(){
	try {
		std::cout << "Initializing UART..." << std::endl;
		device = new mraa::Uart(0);

		if (device->setBaudRate(uartsetup.baudrate) != mraa::SUCCESS) {
		std::cout << "Error setting parity on UART" << std::endl;
		}
		else{
			std::cout << "Setting Baudrate to " << uartsetup.baudrate << "..." << std::endl;
		}

		if (device->setMode(uartsetup.mode_byte_size, uartsetup.mode_parity, uartsetup.mode_stop_bits) != mraa::SUCCESS) {
			std::cout << "Error setting parity on UART" << std::endl;
		}
		else{
			std::cout << "Setting Mode to " << uartsetup.mode_byte_size << uartsetup.mode_parity << uartsetup.mode_stop_bits << "..." << std::endl;
		}

		if (device->setFlowcontrol(uartsetup.xon_xoff, uartsetup.rts_cts) != mraa::SUCCESS) {
			std::cout << "Error setting flow control UART" << std::endl;
		}
		else{
			std::cout << "Setting Flow Control: xonxoff->" << uartsetup.xon_xoff << " rtscts->" << uartsetup.rts_cts << "..." << std::endl;
		}

		std::cout << "Device Path: " << device->getDevicePath() << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << ", likely invalid platform config" << std::endl;
	}
}

void protocol::msp_request(uint8_t opcode){

	std::ostringstream buf;

	uint8_t length = 0;

	uint8_t checksum = 0;

    // Calculate checksum
    checksum = checksum ^ length ^ opcode;
	
	// Pack frame
	buf << to_fc_header << (char)length << (char)opcode << (char)checksum;
	
	// Convert buffer into a string
	std::string frame_string = buf.str();
	
	// Send packed frame
    device->writeStr(frame_string);
	

	if(DEBUG == 1)
	{
		std::cout << "\n\n\t\t" << "Header\t     Size\t      Command\t       CRC\n";
		std::cout << "Sending Frame:\t";
		uint8_t ctr = 1;
		for ( std::string::iterator it = frame_string.begin(); it != frame_string.end(); ++it)
		{
			std::cout << *it;
			if(ctr == 3 || ctr == 4 || ctr == 5)
			{
				std::cout << "\t:\t";
			}
			ctr++;
		}
		
		std::string display_string;
		display_string.assign(string_to_hex(frame_string));
		std::cout << "\n\t\t";
		ctr = 1;
		for ( std::string::iterator it = display_string.begin(); it != display_string.end(); ++it)
		{
			std::cout << *it;
			if(ctr == 6 || ctr == 8 || ctr == 10)
			{
				std::cout << "\t:\t";
			}
			ctr++;
		}
		std::cout << "\n\n\n";
	}
}

void protocol::msp_command(uint8_t opcode, uint8_t data_length, int* params){

	std::ostringstream buf;

	uint8_t checksum = 0;

    // Calculate checksum
    checksum = checksum ^ data_length ^ opcode;
	
	// Pack frame

	buf << to_fc_header << (char)data_length << (char)opcode;
	
	// Pack the data parameters
	for (int i = 0; i < data_length; i++)
	{
		buf << (char)params[i];

		checksum = checksum ^ params[i];
	}

	// Append the checksum
	buf << (char)checksum;

	// Convert buffer into a string
	std::string frame_string = buf.str();
	
	// Send packed frame
    device->writeStr(frame_string);

	if(DEBUG == 1)
	{
		std::cout << "\n\n\t\t" << "Header\t     Size\t      Command\t       CRC\n";
		std::cout << "Sending Frame:\t";
		uint8_t ctr = 1;
		for ( std::string::iterator it = frame_string.begin(); it != frame_string.end(); ++it)
		{
			std::cout << *it;
			if(ctr == 3 || ctr == 4 || ctr == 5)
			{
				std::cout << "\t:\t";
			}
			ctr++;
		}
		
		std::string display_string;
		display_string.assign(string_to_hex(frame_string));
		std::cout << "\n\t\t";
		ctr = 1;
		for ( std::string::iterator it = display_string.begin(); it != display_string.end(); ++it)
		{
			std::cout << *it;
			if(ctr == 6 || ctr == 8 || ctr == 10)
			{
				std::cout << "\t:\t";
			}
			ctr++;
		}
		std::cout << "\n\n\n";
	}
	
}

std::string protocol::read(){
	// Establish variables for use in receiving data
	std::string buf;
    int to_counter = 0;
	int bytes_ctr = 0;
	
	// Check for data availability on the UART
	// Do once, then continue unless time has run out 
	do
	{
		if(device->dataAvailable())
		{
			// Get known frame size back for now
			buf.append(device->readStr(1));
			
			if(DEBUG == 1)
			{
				// State data is available only on the first byte
				if(bytes_ctr == 0)
				{
					std::cout << "Data Available" << std::endl;
				}
				
				// Count number of bytes received
				bytes_ctr++;
			}
			
			// Reset Time Out counter
			to_counter = 0;
		}
		else
		{
			to_counter += 1;
			if(to_counter == 1000)
			{
				std::cout << "No Serial Data Available.\n" << std::endl;
				usleep(1000);
			}
			
		}
	}while(to_counter < 1000);
	
	if(DEBUG == 1)
	{
		std::cout << "Number of bytes received: " << bytes_ctr << std::endl;
	}
	
	return buf;
}

std::string protocol::request_data(uint8_t opcode){

	std::cout << "Sending Command: " << opcode << std::endl;

	msp_request(opcode);

	usleep(500);

	return read();

}

std::string protocol::request_data(uint8_t opcode, uint8_t param_length, int* params){

	std::cout << "Sending Command: " << opcode << std::endl;

	msp_command(opcode, param_length, params);

	usleep(500);

	return read();

}

void protocol::set_data(uint8_t opcode, uint8_t param_length, int* params){

	std::cout << "Sending Command: " << opcode << std::endl;

	msp_command(opcode, param_length, params);
}

