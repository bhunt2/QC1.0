
#include "parsers.h"

void evaluate_identification(std::string data){
	return;

}

msp_attitude evaluate_attitude(std::string data){
	std::string rx_frame;
	
	// Get the data back for that command
	rx_frame.append(data);
	
	const char * msp_data = rx_frame.data();
	
	msp_attitude attitude;

	// Step through frame to data
	msp_data += 5;
	
	attitude.ang_x = (float)*msp_data / 10;
	msp_data += 2;
	attitude.ang_y = (float)*msp_data / 10;
	msp_data += 2;
	attitude.heading = (float)*msp_data;

	return attitude
}

void evaluate_altitude(std::string data){
	return;
}

void evaluate_raw_rc(std::string data){
	return;
}