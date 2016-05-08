#include <string>

#include "msp_frames.h"

class parsers
{
	
	private:
		
		
	public:
		parsers();
		
		~parsers();

		msp_attitude evaluate_attitude(std::string);

		void evaluate_altitude();

		void evaluate_identification();

		void evaluate_raw_rc();

};

