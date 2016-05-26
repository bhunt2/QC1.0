
#include <stdint.h>

#include "protocol.h"
#include "msp_frames.h"

#ifndef CONTROL_H
#define CONTROL_H

class control{
	private:
		enum control_state{
			START,
			ARM,
			INCTHROTTLE,
			KEEP,
			HOLD,
			DECTHROTTLE,
			DISARM
		};

		void set_flight_controls(raw_rc_frame);


	public:
		//control();

		//~control();

		model get_model();

		void arm();

		void disarm();
		
		//--------------------- distance, speed, height
		void follow(float, float, float);

		// ---------- throttle
		void throttle(uint16_t);

		// -------------- throttle, pitch
		void move_forward(uint16_t, uint16_t);

		//--------max_throttle, time_to_hover_in_seconds, step value to increase throttle
		void hover(uint16_t, int, int);

};

#endif