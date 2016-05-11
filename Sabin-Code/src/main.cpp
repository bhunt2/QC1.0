#include <unistd.h>
#include <iostream>
#include <exception>
#include <endian.h>

#include "msp_frames.h"
#include "types.h"
#include "parsers.h"
#include "protocol.h"


int main(int argc, char* argv[]){

	unsigned int microseconds = 1000000;

	if (argc <= 1)
	{
		std::cout << "Argument must be provided." << std::endl;
		std::cout << "Example: ./drone [msp command number]\n" << std::endl;
		return 0;
	}

	if (argc > 2)
	{
		std::cout << "Only 1 argument is expected." << std::endl;
		std::cout << "Example: ./drone [msp command number]\n" << std::endl;
		return 0;
	}


	protocol msp_protocol(1);

	parsers parse;

	// Read Attitude
	if (strcmp(argv[1], "108") == 0
)	{
		std::string response = msp_protocol.request_data(msp_attitude);
		std::cout << "Data Response: " << response << std::endl;

		attitude_frame att;

		att = parse.evaluate_attitude(response);

		printf("angx: %0.2f \tangy: %0.2f \theading: %0.2f\n", att.ang_x, att.ang_y, att.heading);

		return 0;
	}


	// Read MSP RC
	if (strcmp(argv[1], "105") == 0)
	{

		std::string response = msp_protocol.request_data(msp_read_rc);
		std::cout << "Data Response: " << response << std::endl;

		set_raw_rc_frame rc;

		rc = parse.evaluate_raw_rc(response);

		printf("roll: %u\t pitch: %u\t yaw: %u\t throttle: %u\n", rc.roll, rc.pitch, rc.yaw, rc.throttle);

		return 0;
	}


	// Set MSP RC
	if (strcmp(argv[1], "200") == 0)
	{
		uint8_t length = 4;

		std::cout << "Arming..." << std::endl;
		// Arm sequence
		int arm_data[length] = {1500, 1500, 2000, 1000}; //roll, pitch, yaw, throttle

		msp_protocol.set_data(msp_set_raw_rc, length, arm_data);

		usleep(3*microseconds);

		std::cout << "DisArming..." << std::endl;
		// Arm sequence
		int disarm_data[length] = {1500, 1500, 1000, 1000}; //roll, pitch, yaw, throttle

		msp_protocol.set_data(msp_set_raw_rc, length, disarm_data);

		return 0;
	}


	return 0;
	
}