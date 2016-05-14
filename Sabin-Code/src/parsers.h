
#ifndef PARSERS_H
#define PARSERS_H

#include "msp_frames.h"

class parsers
{
	
	private:
		std::string string_to_hex(const std::string&);
		
	public:
		parsers();
		
		~parsers();

		attitude_frame evaluate_attitude(std::string);

		void evaluate_altitude(std::string);

		void evaluate_identification(std::string);

		set_raw_rc_frame evaluate_raw_rc(std::string);

		std::string reverse(std::string);
};

#endif







