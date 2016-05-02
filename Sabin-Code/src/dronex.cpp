/* This is to test basic communications between the Intel Edison
 *  and Flip 1.5 Flight Controller using MSP. Once communication
 *  is verified, a more robust system will be developed.
 */


#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <exception>
#include <bitset>
#include <string>
#include <sstream>
#include "mraa.hpp"


#define DEBUG 1

// Function Prototypes
void setup();
std::string get_data();
void send_msp();
std::string string_to_hex(const std::string& input);

mraa::Uart* dev;
mraa::Gpio* gpio_tx;
mraa::Gpio* gpio_rx;

int main(int argc, char* argv[])
{
	std::string rx_frame;

	// Configure UART
	setup(); 

	// Send a simple command for now
	send_msp();
	
	// Get the data back for that command
	rx_frame.append(get_data());
	
	// Print data received
	uint8_t ctr = 1;
	float angx = 0.0;
	float angy = 0.0;
	float heading = 0.0;
	std::cout << "Received Frame: " << rx_frame << std::endl;
	const char * data = rx_frame.data();
	
	// Step through frame to data
	data += 5;
	
	angx = (float)*data / 10;
	data += 2;
	angy = (float)*data / 10;
	data += 2;
	heading = (float)*data;
	
	
	printf("X: %0.2f \t Y: %0.2f \t Heading: %0.2f\n", angx, angy, heading);
	
	// Release the device
	delete dev;

	return EXIT_SUCCESS;
}


// This function sets up the UART for communications
void setup()
{
	// Initialze/Wakeup UART
	try {
		std::cout << "Initializing UART..." << std::endl;
		dev = new mraa::Uart(0);
	} catch (std::exception& e) {
		std::cout << e.what() << ", likely invalid platform config" << std::endl;
	}

	// Set baudrate
	int baudrate = 115200;
	if (dev->setBaudRate(baudrate) != mraa::SUCCESS) {
		std::cout << "Error setting parity on UART" << std::endl;
	}else{
		std::cout << "Setting Baudrate to " << baudrate << "..." << std::endl;
	}

	// Configure UART to 8-bit, No parity with one stop bit
	if (dev->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {
		std::cout << "Error setting parity on UART" << std::endl;
	}else{
		std::cout << "Setting Mode to 8N1..." << std::endl;
	}

	// Set flow control
	if (dev->setFlowcontrol(false, false) != mraa::SUCCESS) {
		std::cout << "Error setting flow control UART" << std::endl;
	}else{
		std::cout << "Setting Flow Control..." << std::endl;
	}

	// Print the device path for the UART in the OS
	std::cout << "Device Path: " << dev->getDevicePath() << std::endl;
}


// This function is for sending MSP commands
void send_msp() {
	std::ostringstream buf;

	uint8_t cmd_ident = 100;
	uint8_t cmd_att = 108;
	uint8_t length = 0;
	uint8_t checksum = 0;

    // Calculate checksum
    checksum = checksum ^ length ^ cmd_att;
	
	// Pack frame
	buf << "$M<" << (char)length << (char)cmd_att << (char)checksum;
	
	// Convert buffer into a string
	std::string frame_string = buf.str();
	
	// Send packed frame
    dev->writeStr(frame_string);
	
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

// This function is for getting data back from Flip after an MSP command has been sent
std::string get_data()
{
	// Get response
	usleep(1000); // Wait a bit for Flip to prepare response
	
	// Establish variables for use in receiving data
	std::string buf;
    int to_counter = 0;
	int bytes_ctr = 0;
	
	// Check for data availability on the UART
	// Do once, then continue unless time has run out 
	do
	{
		if(dev->dataAvailable())
		{
			// Get known frame size back for now
			buf.append(dev->readStr(1));
			
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


std::string string_to_hex(const std::string& input)
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