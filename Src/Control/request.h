
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
#include "protocol.h"
#include "parsers.h"

#ifndef REQUEST_H
#define REQUEST_H

class request
{
	
	private:

		protocol msp_protocol;

		parsers parse;

	public:

		//request();

		ident_frame request_ident();
		attitude_frame request_attitude();
		altitude_frame request_altitude();
		raw_rc_frame request_rc_values();

};

#endif







