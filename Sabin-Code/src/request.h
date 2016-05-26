

#include "msp_frames.h"
#include "parsers.h"

#ifndef REQUEST_H
#define REQUEST_H

class request
{
	
	private:
		parsers parse;
			
	public:
		ident_frame request_ident();
		attitude_frame request_attitude();
		altitude_frame request_altitude();
		raw_rc_frame request_rc_values();

};

#endif







