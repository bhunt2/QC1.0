
#include <stdint.h>

#include "protocol.h"


#ifndef CONTROL_H
#define CONTROL_H

class control{
	private:

		enum control_state{
			START,
			ARM,
			INCTHROTTLE,
			KEEP,
			DECTHROTTLE,
			DISARM
		};

		protocol msp_protocol;

		void arm();

		void disarm();

		//----------------------- roll, pitch, yaw, throttle
		void set_flight_controls(uint16_t,uint16_t,uint16_t,uint16_t);

		//---------------------- distance, speed, height
		void set_flight_controls(float, float, float);

	public:
		//control();

		//~control();

		//--------max_throttle, time_to_hover_in_seconds
		void hover(uint16_t, int);

};

#endif