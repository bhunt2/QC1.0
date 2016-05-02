#include <string>

#include "parsers.h"

void evaluate_identification(std::string data){
	return;

}

void evaluate_attitude(std::string data){
	std::string rx_frame;
	
	// Get the data back for that command
	rx_frame.append(get_data());
	
	const char * data = rx_frame.data();
	
	// Step through frame to data
	data += 5;
	
	attitude.ang_x = (float)*data / 10;
	data += 2;
	attitude.ang_y = (float)*data / 10;
	data += 2;
	attitude.heading = (float)*data;
}

void evaluate_altitude(std::string data){
	return;
}

void evaluate_raw_rc(std::string data){
	return;
}