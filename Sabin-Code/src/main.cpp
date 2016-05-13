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
		uint8_t length = 8;

		uint8_t data_length = 16;
		std::cout << "Arming..." << std::endl;
		// Arm sequence
		uint16_t arm_data[length] = {1500, 1500, 2000, 1000, 1000, 1000, 1000, 1000}; //roll, pitch, yaw, throttle, aux1, aux2, aux3, aux4

		// Send data
		std::string response_arm = msp_protocol.request_data(msp_set_raw_rc, data_length, arm_data);

		std::cout << "Data Response Arm: " << response_arm << std::endl;

		set_raw_rc_frame rc_arm;

		rc_arm = parse.evaluate_raw_rc(response_arm);

		printf("roll: %u\t pitch: %u\t yaw: %u\t throttle: %u\n", rc_arm.roll, rc_arm.pitch, rc_arm.yaw, rc_arm.throttle);




/*
		// wait three seconds
		usleep(3*microseconds);


		std::cout << "\n\nDisArming..." << std::endl;
		// Arm sequence
		uint16_t disarm_data[length] = {1500, 1500, 1000, 1000, 1000, 1000, 1000, 1000}; //roll, pitch, yaw, throttle, aux1, aux2, aux3, aux4

		msp_protocol.set_data(msp_set_raw_rc, data_length, disarm_data);


		std::string response_disarm = msp_protocol.request_data(msp_read_rc);

		std::cout << "Data Response Disarm: " << response_arm << std::endl;

		set_raw_rc_frame rc_disarm;

		rc_disarm = parse.evaluate_raw_rc(response_disarm);

		printf("roll: %u\t pitch: %u\t yaw: %u\t throttle: %u\n", rc_disarm.roll, rc_disarm.pitch, rc_disarm.yaw, rc_disarm.throttle);
		
*/
		return 0;
	}


	return 0;
	
}