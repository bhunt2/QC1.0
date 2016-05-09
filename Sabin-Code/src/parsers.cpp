#include <string>
#include <stdint.h>
//#include <endian.h>
#include <iostream>
#include <stdio.h>
#include <algorithm>

#include "parsers.h"


void parsers::evaluate_identification(std::string data){
	return;

}

attitude_frame parsers::evaluate_attitude(std::string data){
	
	std::string rx_frame;
	
	// Get the data back for that command
	rx_frame.append(data);

	const char * msp_data = rx_frame.data();
	
	attitude_frame attitude;

	// Step through frame to data
	msp_data += 5;
	
	attitude.ang_x = (float)*msp_data / 10;
	msp_data += 2;
	attitude.ang_y = (float)*msp_data / 10;
	msp_data += 2;
	attitude.heading = (float)*msp_data;

	return attitude;
}

void parsers::evaluate_altitude(std::string data){
	return;
}

set_raw_rc_frame parsers::evaluate_raw_rc(std::string data){
		

	std::string rx_frame;
		
	rx_frame.append(data);

	const char * msp_data = rx_frame.data();
		
	set_raw_rc_frame rc;

	msp_data += 5;

	rc.roll = (uint16_t) *msp_data;
	msp_data += 2;
	rc.pitch = (unsigned int)*msp_data;
	msp_data += 2;
	rc.yaw = (unsigned int)*msp_data;
	msp_data += 2;
	rc.throttle = (unsigned int)*msp_data;
	
	return rc;
}

std::string parsers::reverse(std::string str){
	std::reverse(str.begin(),str.end());
	return str;
}