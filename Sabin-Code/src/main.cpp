#include <unistd.h>
#include <iostream>
#include <exception>

#include "protocol.h"
#include "types.h"
#include "parsers.h"

int main(int argc, char* argv[]){
	
	protocol msp_protocol;

	parsers parse;

	std::string response = msp_protocol.request(msp_attitude);

	msp_attitude att = parse.evaluate_attitude(response);

	cout << "angx: " << msp_attitude.ang_x;
	cout << "angy: " << msp_attitude.ang_y;
	cout << "heading: " << msp_attitude.heading;

	return 0;
	
}