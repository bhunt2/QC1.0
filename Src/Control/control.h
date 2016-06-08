
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

#include <stdint.h>

#include "protocol.h"
#include "msp_frames.h"

#ifndef CONTROL_H
#define CONTROL_H

class control{
	private:

		// Control States to follow and hover
		enum control_state{
			START,
			ARM,
			INCTHROTTLE,
			FIX,
			KEEP,
			HOLD,
			DECTHROTTLE,
			DISARM
		};

		// calls set_rc method in protocol class to set given rc frame 
		void set_flight_controls(raw_rc_frame);

		// Calls set_alt method in protocol class to set given altitude(cm)
		void set_alt(uint32_t);


	public:
		//control();

		//~control();

		// retreives the model give the path
		//------------- path with file name
		model get_model(std::string);

		// This method arms the drone.
		// As soon as the done is arm, the legacy RX takes over
		// and turns ON the motors automatically.
		void arm();

		// This method only disarms the drone
		void disarm();
		
		//This methods takes in distance, speed and height as a parameter from the image processing API. 
		//This method uses these parameters to calculate how much throttle and pitch values are needed to to stay in flight and follow an object. 
		//After throttle and pitch values are calculated, it is passed to the move_forward() method which sends and sets values on Flight Controller 
		//using set_flight_controls() private method.
		//--------- distance, speed, height
		void follow(float, float, float);

		// calls set_flight_controls() private method so that the external calls can directly send RC values to FC
		//---------------- desired RC frame
		void set_rc(raw_rc_frame);

		//This method takes in throttle value as a parameter and sets the given throttle value on 
		//Flight Controller using the set_flight_controls() private method.
		// ---------- throttle
		void throttle(uint16_t);

		// This method takes in throttle and pitch values which is then sent and set on Flight Controller using set_flight_controls private method.
		// -------------- throttle, pitch
		void move_forward(uint16_t, uint16_t);

		// This hover method takes in the target altitude and 
		// constantly queries the barometer on FC to check if target altitude is reached and 
		// activates altitude hold if the target atitude is reached.
		//--------Target Altitude
		void hover(int32_t);

		// This another hover method takes in the target altitude and
		// uses the custom MSP_SET_ALT command on the MultiWii firmware to set the target altitude hold value.
		// After the drone is in the air, the target altitude is set on firmwarer using the command and altitude hold is activated
		//--------------------------Tarrget Altitude
		void hover_with_msp_set_alt(int32_t);

};

#endif