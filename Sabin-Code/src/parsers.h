#include <string>

#include "msp_frames.h"

class parsers
{
	
	private:
		msp_ident identification;
		msp_attitude attitude;
		msp_altitude altitude;
		msp_set_raw_rc raw_rc;
		
	public:
		parsers();
		
		~parsers();

		void evaluate_attitude();

		void evaluate_altitude();

		void evaluate_identification();

		void evaluate_raw_rc();

};

