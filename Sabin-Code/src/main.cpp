#include <unistd.h>
#include <iostream>
#include <exception>

#include "msp_frames.h"
#include "types.h"
#include "parsers.h"
#include "protocol.h"


int main(int argc, char* argv[]){

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
	if (strcmp(argv[1], "att") == 0)	
	{
		std::string response = msp_protocol.request_data(msp_attitude);
		std::cout << "Data Response: " << response << std::endl;

		attitude_frame att;

		att = parse.evaluate_attitude(response);

		printf("angx: %0.2f \tangy: %0.2f \theading: %0.2f\n", att.ang_x, att.ang_y, att.heading);

		return 0;
	}


	// Read MSP RC
	if (strcmp(argv[1], "rc-read") == 0)
	{

		std::string response = msp_protocol.request_data(msp_read_rc);
		std::cout << "Data Response: " << response << std::endl;

		set_raw_rc_frame rc;

		rc = parse.evaluate_raw_rc(response);

		printf("roll: %u\t pitch: %u\t yaw: %u\t throttle: %u\n", rc.roll, rc.pitch, rc.yaw, rc.throttle);

		return 0;
	}


	// Set MSP RC
	if (strcmp(argv[1], "arm") == 0)
	{
		msp_protocol.arm();

		//usleep(3*microseconds);

		return 0;
	}



	// Set MSP RC
	if (strcmp(argv[1], "disarm") == 0)
	{
		msp_protocol.disarm();

		//usleep(3*microseconds);

		return 0;
	}

	// Set MSP RC
	if (strcmp(argv[1], "control") == 0)
	{

		set_raw_rc_frame rc;

		rc.roll = 1500;
		rc.pitch = 1500;
		rc.yaw = 1500;
		rc.throttle = 1500;

		msp_protocol.set_flight_controls(rc.roll, rc.pitch, rc.yaw, rc.throttle);

		//usleep(3*microseconds);

		return 0;
	}


	return 0;
	
}