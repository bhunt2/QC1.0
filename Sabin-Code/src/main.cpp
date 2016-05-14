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


	protocol msp_protocol;

	parsers parse;

	// Read Attitude
	if (strcmp(argv[1], "att") == 0)	
	{
		std::string response = msp_protocol.request_data(msp_attitude);

		parse.evaluate_attitude(response);

		return 0;
	}


	// Read MSP RC
	if (strcmp(argv[1], "rc-read") == 0)
	{

		std::string response = msp_protocol.request_data(msp_read_rc);
		
		parse.evaluate_raw_rc(response);

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

		if (argv[2] == NULL || argv[3] == NULL || argv[4] == NULL || argv[5] == NULL)
		{
			std::cout << "4 arguments expected! [Roll Pitch Yaw Throttle]" << std::endl;

			return 0;
		}

		set_raw_rc_frame rc;

		rc.roll = (uint16_t) strtoul(argv[2], NULL, 0);
		rc.pitch = (uint16_t) strtoul(argv[3], NULL, 0);
		rc.yaw = (uint16_t) strtoul(argv[4], NULL, 0);
		rc.throttle = (uint16_t) strtoul(argv[5], NULL, 0);

		msp_protocol.set_flight_controls(rc.roll, rc.pitch, rc.yaw, rc.throttle);

		//usleep(3*microseconds);

		return 0;
	}


	return 0;
	
}