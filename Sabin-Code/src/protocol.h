#include <string>

#include "mraa.hpp"
#include "msp_frames.h"

#ifndef PROTOCOL_H
#define PROTOCOL_H


class protocol
{

private:
	
	bool internal_debug;

	// Read Parse State
	enum state
	{
		start,
		header,
		payloadsize,
		opcode,
		payload
	};

	state curr_state;

	// UART Device object
	mraa::Uart* device;

	// UART Properties
	struct uart_setup
	{
		unsigned int baudrate;
		int mode_byte_size;
		mraa::UartParity mode_parity;
		int mode_stop_bits;
		bool xon_xoff;
		bool rts_cts;

		uart_setup() :
			baudrate(115200),
			mode_byte_size(8),
			mode_parity(mraa::UART_PARITY_NONE),
			mode_stop_bits(1),
			xon_xoff(false),
			rts_cts(false)
			{}
	} uartsetup;

	
	// This private method is called from the constructor during the initialization of the _protocol_ class. 
	//The device is setup using the members from defined structure uart_setup. 
	void setup_uart();

	//This private method only takes in MultiWii Serial Protocol's Code and returns the values associated with that code. 
	//This is just a request to only read a value from Flight Controller
	void msp_request(uint8_t);

	//The private methods takes in the payload to be sent to the flight controller with its size and expects it to return back another payload associated with the opcode. 
	//This is a request to send command with payload and expect a return value after Flight Controller process the request
	void msp_command(uint8_t, uint8_t, uint16_t*);

	//Reads the values returned from the Flight Controller after sending a request and a command methods. 
	//This method returns the string of values with header, payload size, opcode and payload. These are decoded using the _parser_ class.
	std::string read();

	//read_frame read_new();

	std::string string_to_hex(const std::string&);

	uint8_t read8();

public:
	protocol(bool);

	~protocol();

	// Does not currently work. Uses read_new and returns a data frame
	// params: opcode
	//read_frame request_data_frame(uint8_t);

	// This method is used to send and set the Raw RC values using MSP to the Flight Controller. 
	//------------opcode, payload size, payload
	void set_rc(uint8_t, uint8_t, uint16_t[12]);

	//This method takes in MSP code to request values associated with the code from Flight Controller
	// --------------------- opcode
	std::string request_data(uint8_t);

	//This method sends the payload to flight controller and expects response payload in return associated with the opcodes
	//---------------------- opcoode, payloadsize, payload
	std::string request_data(uint8_t, uint8_t, uint16_t*);
	 
};

#endif


