#include <string>
#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "parsers.h"
#include "types.h"


/*
	///////////////////////////////////////////////
	CONSTRUCTORS
	///////////////////////////////////////////////
*/

	//parsers::parsers(){	debug = false;}


	//parsers::~parsers(){}


/*
	///////////////////////////////////////////////
	PUBLIC FUNCTIONS
	///////////////////////////////////////////////
*/	


ident_frame parsers::evaluate_identification(std::string data){
	ident_frame ident;

	ident.version = (uint8_t)(strtoul(string_to_hex(data.substr(5,1)).c_str(), NULL, 16));

	if(debug){

		std::cout << "Response: " << data << "\n" << std::endl;
		printf("version: %d\n", ident.version);
	}

	return ident;

}

attitude_frame parsers::evaluate_attitude(std::string rx_frame){
	
	attitude_frame attitude;

	// Step through frame to data
	/*

	std::string temp;

	temp.append(string_to_hex(rx_frame.substr(6,1)));
	temp.append(string_to_hex(rx_frame.substr(5,1)));

	attitude.ang_x = ((float)(strtol(temp.c_str(), NULL, 16))/10);

	temp.clear();

	// extract ang_y
	temp.append(string_to_hex(rx_frame.substr(8,1)));
	temp.append(string_to_hex(rx_frame.substr(7,1)));

	attitude.ang_y = ((float)(strtol(temp.c_str(), NULL, 16))/10);

	temp.clear();

	// extract heading
	temp.append(string_to_hex(rx_frame.substr(10,1)));
	temp.append(string_to_hex(rx_frame.substr(9,1)));

	attitude.heading = ((float)(strtol(temp.c_str(), NULL, 16)));
	*/

	const char * data = rx_frame.data();
	
	// Step through frame to data
	data += 5;
	
	attitude.ang_x = (float)*data / 10;
	data += 2;
	attitude.ang_y = (float)*data / 10;
	data += 2;
	attitude.heading = (float)*data;

	if(debug){

		std::cout << "Response: " << rx_frame << "\n" << std::endl;

		printf("angx: %0.2f \tangy: %0.2f \theading: %0.2f\n", attitude.ang_x, attitude.ang_y, attitude.heading);
	}

	return attitude;
}

altitude_frame parsers::evaluate_altitude(std::string rx_frame){

	std::string temp;

	altitude_frame alt_frame;


	if (rx_frame.length() == 0)
	{
		if (debug)
		{
			std::cout << "No Altitude Available!\n" << std::endl;
		}

		return alt_frame;
	}

	// 4 bytes for the estimated altitude value
	temp.append(string_to_hex(rx_frame.substr(8,1)));
	temp.append(string_to_hex(rx_frame.substr(7,1)));
	temp.append(string_to_hex(rx_frame.substr(6,1)));
	temp.append(string_to_hex(rx_frame.substr(5,1)));


	alt_frame.est_alt = ((uint32_t)(strtoul(temp.c_str(), NULL, 16)));

	temp.clear();

	// 2 bytes for the vario value
	temp.append(string_to_hex(rx_frame.substr(10,1)));
	temp.append(string_to_hex(rx_frame.substr(9,1)));

	alt_frame.vario = ((uint16_t)(strtoul(temp.c_str(), NULL, 16)));

	temp.clear();


	if(debug){ 

		std::cout << "Response: " << rx_frame << "\n" << std::endl;

		printf("est altitude: %0.2f m \tVario: %0.2f m/s\n", (float)(alt_frame.est_alt/100), (float)(alt_frame.vario/100));
	}

	return alt_frame;
}

raw_rc_frame parsers::evaluate_raw_rc(std::string rx_frame){

	raw_rc_frame rc;


	if (rx_frame.empty())
	{
		std::cout << "Empty frame from Flight Controller." << std::endl;
		return rc;
	}

	// 22 is expected number of bytes
	if (rx_frame.length() < 22)
	{
		std::cout << "Less than 22 bytes received." << std::endl;
		return rc;
	}


	
	std::string temp;

	// Take a LSB byte and convert it to hex.
	// Then convert to hex to get value

	// Parse YAW
	temp.append(string_to_hex(rx_frame.substr(6,1)));
	temp.append(string_to_hex(rx_frame.substr(5,1)));

	rc.roll = strtoul(temp.c_str(), NULL, 16);

	temp.clear();

	// Parse PITCH
	temp.append(string_to_hex(rx_frame.substr(8,1)));
	temp.append(string_to_hex(rx_frame.substr(7,1)));

	rc.pitch = strtoul(temp.c_str(), NULL, 16);

	temp.clear();

	// Parse YAW
	temp.append(string_to_hex(rx_frame.substr(10,1)));
	temp.append(string_to_hex(rx_frame.substr(9,1)));

	rc.yaw = strtoul(temp.c_str(), NULL, 16);

	temp.clear();

	// Parse THROTTLE
	temp.append(string_to_hex(rx_frame.substr(12,1)));
	temp.append(string_to_hex(rx_frame.substr(11,1)));

	rc.throttle = strtoul(temp.c_str(), NULL, 16);

	if(debug){
		printf("roll: %u\t pitch: %u\t yaw: %u\t throttle: %u\n", rc.roll, rc.pitch, rc.yaw, rc.throttle);
	}

	return rc;
}

std::string parsers::string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}