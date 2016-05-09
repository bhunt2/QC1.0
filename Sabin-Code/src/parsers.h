
#ifndef PARSERS_H
#define PARSERS_H

#include "msp_frames.h"

class parsers
{
	
	private:
		
		
	public:
		//parsers();
		
		//~parsers();

		attitude_frame evaluate_attitude(std::string);

		void evaluate_altitude(std::string);

		void evaluate_identification(std::string);

		set_raw_rc_frame evaluate_raw_rc(std::string);

};

#endif







