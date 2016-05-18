
#ifndef PARSERS_H
#define PARSERS_H

#include "msp_frames.h"

class parsers
{
	
	private:
		
		std::string string_to_hex(const std::string&);
		
	public:
		//parsers();
		
		//~parsers();

		//This method takes in the raw data returned from the flight controller and parses it to give the version of the multiwii.
		attitude_frame evaluate_attitude(std::string);

		//This method takes in the raw data returned from the flight controller and parses it to give the Eular angles - angx, angy and heading value
		altitude_frame evaluate_altitude(std::string);

		//This method takes in the raw data returned from the flight controller and parses it to give the altitude related values
		ident_frame evaluate_identification(std::string);

		//This method takes in the raw data returned from the flight controller and parses it to give the rc values roll, pitch, yaw and throttle
		set_raw_rc_frame evaluate_raw_rc(std::string);
};

#endif







