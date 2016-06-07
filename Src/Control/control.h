
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

		void set_alt(uint32_t);


	public:
		//control();

		//~control();

		model get_model();

		void arm();

		void disarm();
		
		//--------------------- distance, speed, height
		void follow(float, float, float);

		//----------------
		void set_rc(raw_rc_frame);

		// ---------- throttle
		void throttle(uint16_t);

		// -------------- throttle, pitch
		void move_forward(uint16_t, uint16_t);

		//--------Target Altitude
		void hover(int);

		void hover_with_msp_set_alt(uint32_t);

};

#endif