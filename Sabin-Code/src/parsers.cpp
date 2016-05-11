#include <string>
#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include <algorithm>

#include "parsers.h"


void parsers::evaluate_identification(std::string data){
	return;

}

attitude_frame parsers::evaluate_attitude(std::string rx_frame){
	
	std::string temp;
	
	attitude_frame attitude;

	// Step through frame to data

	// extract ang_x

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

	return attitude;
}

void parsers::evaluate_altitude(std::string data){
	return;
}

set_raw_rc_frame parsers::evaluate_raw_rc(std::string rx_frame){

	set_raw_rc_frame rc;

	if (rx_frame.empty())
	{
		return rc;
	}

	// 22 is expected number of bytes
	if (rx_frame.length() < 22)
	{
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