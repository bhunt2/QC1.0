
#include <exception>

#include "protocol.h"
#include "types.h"
#include "msp_frames.h"

protocol::protocol(){
	uart_setup();
}

void protocol::uart_setup(){
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

}

void protocol::msp_command(uint8_t opcode, uint8_t param_length, uint8_t* params){

}

void protocol::read(uint8_t opcode){
	
}

void protocol::get_response_without_parameters(uint8_t opcode){

	std::cout << "Sending Command code: " << opcode << std::endl;

	msp_request(opcode);

	usleep(500);

	read(opcode);

}

void protocol::get_response_with_parameters(uint8_t opcode, uint8_t param_length, uint8_t* params){

	std::cout << "Sending Command : " << opcode << std::endl;

	msp_command(opcode, param_length, params);

	usleep(500);

	read(opcode);

}
