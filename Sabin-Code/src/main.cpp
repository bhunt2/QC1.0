#include <unistd.h>
#include <iostream>
#include <exception>
#include <endian.h>

#include "msp_frames.h"
#include "types.h"
#include "parsers.h"
#include "protocol.h"


int main(int argc, char* argv[]){
	
	protocol msp_protocol;

	parsers parse;

	//uint8_t length = 4;
	//int data[length] = {1500, 1500, 2000, 1000};
	//uint8_t msp_rc_read_cmd = 105;
	//std::string response = msp_protocol.request_data(msp_rc_read_cmd);

	//set_raw_rc_frame rc;

	//rc = parse.evaluate_raw_rc(response);

	//printf("yaw: %d\t pitch: %d\t roll: %d\t throttle: %d\n", rc.yaw, rc.pitch, rc.roll, rc.throttle);

	std::string response = msp_protocol.request_data(msp_attitude);

	attitude_frame att;
	
	att = parse.evaluate_attitude(response);

	std::cout << "angx: " << att.ang_x << "\tangy: " << att.ang_y << "\theading: " << att.heading << std::endl;

	return 0;
	
}