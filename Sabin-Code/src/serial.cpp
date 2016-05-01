#include <unistd.h>
#include <iostream>
#include <exception>
#include <bitset>

#include "mraa.hpp"

/* FUNCTION DECLARATION */

void get_response_without_parameters(uint8_t);

void get_response_with_parameters(uint8_t, uint8_t, uint8_t*);

void evaluate_msp_ident(std::string);

void evaluate_msp_attitude(std::string);

void read(uint8_t);

void msp_request(uint8_t);

void msp_command(uint8_t, uint8_t, uint8_t*);

// This is another way of sending the request
// Converts to HEX before the write
void send_cmd(uint8_t, uint8_t*, uint8_t);

void setup();

std::string binary(uint8_t);


/* GLOBAL PROPERTIES */
const char* to_fc_header = "$M<";

const char* from_fc_header = "$M>";

mraa::Uart* dev;

/* MULTIWII SERIAL PROTOCOL COMMANDS */
enum MSP_COMMANDS
{
	MSP_IDENT = 100,
	MSP_ATTITUDE = 108
};

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

struct msp_ident{
	const uint8_t* version; // version of MultiWii
	const uint8_t* multitype; // type of multi: TRI/QUADP,QUADX,BI,GIMBAL,Y6,HEX6,FLYING_WING,Y4,HEX6X,OCTOX8, OCTOFLATP,OCTOFLATX,AIRPLANE,HELI_120,HELI_90,VTAIL4,HEX6H,SINGLECOPTER,DUALCOPTER
	const uint32_t* capability; // A 32 bit variable to indicate capability of FC board. (Currently, BIND button is used on first bit, DYNBAL on second, FLAP on third )
} ident;

struct msp_attitude
{
	signed int ang_x; // 16 bit Range [-1800;1800] (unit: 1/10 degree)
	signed int ang_y; // 16 bit Range [-900;900] (unit: 1/10 degree)
	signed int heading; // 16 bit [-180;180]
} att;

int main(int argc, char* argv[]){
	setup();

	get_response_without_parameters(MSP_IDENT);

	std::cout << "version: " << ident.version  << "multitype: " << ident.multitype << std::endl;

	return EXIT_SUCCESS;
}

void get_response_without_parameters(uint8_t opcode){

	std::cout << "Sending Command code: " << opcode << std::endl;

	msp_request(opcode);

	usleep(500);

	read(opcode);

}

void get_response_with_parameters(uint8_t opcode, uint8_t param_length, uint8_t* params){

	std::cout << "Sending Command : " << opcode << std::endl;

	msp_command(opcode, param_length, params);

	usleep(500);

	read(opcode);

}

void read(uint8_t opcode) {

	while (1)	{
		if (dev->dataAvailable()){
			std::string header = dev->readStr(1);

			if (header == "$") {
				header = header + dev->readStr(2);
				break;
			}
		}
	}

	std::string l = dev->readStr(1);
	std::string c = dev->readStr(1);
	
	const uint8_t *datalength = reinterpret_cast<const uint8_t*>(&l);
	const uint8_t *code = reinterpret_cast<const uint8_t*>(&c);
	std::string data = dev->readStr((int)datalength);

	switch (opcode) {
		case MSP_IDENT:
		  evaluate_msp_ident(data);
			break;
		case MSP_ATTITUDE:
			evaluate_msp_attitude(data);
			break;
	}
}

void evaluate_msp_ident(std::string data){

	ident.version = reinterpret_cast<const uint8_t*>(&data[0]);
	ident.multitype = reinterpret_cast<const uint8_t*>(&data[1]);
	ident.capability = reinterpret_cast<const uint32_t*>(&data[2]);

}

void evaluate_msp_attitude(std::string data){

	att.ang_x = atoi(&data[0]);
	att.ang_y = atoi(&data[1]) ;
	att.heading = atoi(&data[2]);

}

// Only opcode is needed for this request.
// data length is 0 and no parameter needed.
void msp_request(uint8_t opcode){
	uint8_t checksum = 0;
	uint8_t data_length = 0;

	// Send the MSP header
	dev->writeStr(to_fc_header);

  // Send the size
	const char* length = reinterpret_cast<const char*>(&data_length);
	dev->writeStr(length);
	checksum ^= data_length;

	// Send the op-code
	const char* code = reinterpret_cast<const char*>(&opcode);
	dev->writeStr(code);
	checksum ^= opcode;

	// Send the checksum
	const char* cs = reinterpret_cast<const char*>(&checksum);
	dev->writeStr(cs);
}

// Send command with the parameters
void msp_command(uint8_t opcode, uint8_t paramenter_size, uint8_t* parameters){
	uint8_t checksum = 0;

	// Send the MSP header and message length
	dev->writeStr(to_fc_header);

	// Send the parameter size
	const char* length = reinterpret_cast<const char*>(&paramenter_size);
	dev->writeStr(length);
	checksum ^= paramenter_size;

	// Send the op-code
	const char* code = reinterpret_cast<const char*>(&opcode);
	dev->writeStr(code);
	checksum ^= opcode;

	// Send the data bytes
	for(int i = 0; i < paramenter_size/sizeof(uint8_t); i++) {
		const char* data = reinterpret_cast<const char*>(&parameters[i]);
		dev->writeStr(data);
		checksum ^= parameters[i];
	}

	// Send the checksum
	const char* cs = reinterpret_cast<const char*>(&checksum);
	dev->writeStr(cs);
}


// Initial Setup of UART
void setup(){
	try {
		std::cout << "Initializing UART..." << std::endl;
		dev = new mraa::Uart(0);

	}
	catch (std::exception& e) {
		std::cout << e.what() << ", likely invalid platform config" << std::endl;
	}

	if (dev->setBaudRate(uartsetup.baudrate) != mraa::SUCCESS) {
		std::cout << "Error setting parity on UART" << std::endl;
	}
	else{
		std::cout << "Setting Baudrate to " << uartsetup.baudrate << "..." << std::endl;
	}

	if (dev->setMode(uartsetup.mode_byte_size, uartsetup.mode_parity, uartsetup.mode_stop_bits) != mraa::SUCCESS) {
		std::cout << "Error setting parity on UART" << std::endl;
	}
	else{
		std::cout << "Setting Mode to " << uartsetup.mode_byte_size << uartsetup.mode_parity << uartsetup.mode_stop_bits << "..." << std::endl;
	}

	if (dev->setFlowcontrol(uartsetup.xon_xoff, uartsetup.rts_cts) != mraa::SUCCESS) {
		std::cout << "Error setting flow control UART" << std::endl;
	}
	else{
		std::cout << "Setting Flow Control: xonxoff->" << uartsetup.xon_xoff << " rtscts->" << uartsetup.rts_cts << "..." << std::endl;
	}

	std::cout << "Device Path: " << dev->getDevicePath() << std::endl;
}


// Another way of sending the requests
// This function converts to HEX before sending
void send_cmd(uint8_t opcode, uint8_t* data, uint8_t data_length){
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

}

std::string binary(uint8_t value)
{
	std::bitset<8> x(value);
	x.set();
	std::string binary_value = x.to_string<char, std::string::traits_type, std::string::allocator_type>();

	return binary_value;
}