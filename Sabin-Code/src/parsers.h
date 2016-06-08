

/****************************************************************
	Drone Control and Automation Code
	--------------------------------------------
	Capstone Project: Object Following Drone
	Sponsor: Benjamin Huntsman
	Advisor: Dr. McNames, James
	Written By: Sabin Maharjan
	Date: May 2, 2016
	Version: 1.0
	Github: https://github.com/bhunt2/QC1.0/tree/master/Src/Control
*******************************************************************/

#include "msp_frames.h"

#ifndef PARSERS_H
#define PARSERS_H

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
		raw_rc_frame evaluate_raw_rc(std::string);
};

#endif







