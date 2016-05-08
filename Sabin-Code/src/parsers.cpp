#include <string>

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

void parsers::evaluate_raw_rc(std::string data){
	return;
}