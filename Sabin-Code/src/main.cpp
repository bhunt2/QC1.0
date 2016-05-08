#include <unistd.h>
#include <iostream>
#include <exception>

#include "msp_frames.h"
#include "types.h"
#include "parsers.h"
#include "protocol.h"


int main(int argc, char* argv[]){
	
	protocol msp_protocol;

	parsers parse;

	std::string response = msp_protocol.request_data(msp_attitude);

	attitude_frame att;

	att = parse.evaluate_attitude(response);

	std::cout << "angx: " << att.ang_x << "\tangy: " << att.ang_y << "\theading: " << att.heading << std::endl;

	return 0;
	
}