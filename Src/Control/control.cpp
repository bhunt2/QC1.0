
#include <fstream>
#include <iostream>
#include <sstream>

#include "time.h"

#include "control.h"
#include "types.h"
#include "request.h"

/*
	///////////////////////////////////////////////
	CONSTRUCTORS
	///////////////////////////////////////////////
*/

//control::control(){debug = false;}

//control::~control(){}


/*
	///////////////////////////////////////////////
	PRIVATE FUNCTIONS
	///////////////////////////////////////////////
*/

void control::set_flight_controls(raw_rc_frame frame){

	// limit to [1000:2000] range
	if (frame.roll > 2000){frame.roll = 2000;}
	if (frame.roll < 1000){frame.roll = 1000;}
	if (frame.pitch > 2000){frame.pitch = 2000;}
	if (frame.pitch < 1000){frame.pitch = 1000;}
	if (frame.yaw > 2000){frame.yaw = 2000;}
	if (frame.yaw < 1000){frame.yaw = 1000;}
	if (frame.throttle > 2000){frame.throttle = 2000;}
	if (frame.throttle < 1000){frame.throttle = 1000;}

	uint8_t payload_size = 8;

	uint16_t payload[payload_size] = {frame.roll, frame.pitch, frame.yaw, frame.throttle, frame.aux1, 0, 0, 0};
	
	protocol msp_protocol;

	// Send the RC values to be set on Flight Controller
	msp_protocol.set_rc(msp_set_raw_rc, payload_size, payload);
}

// Sends the target alittide bo be held once the drone is in the air
void control::set_alt(uint32_t alt){
	protocol msp_protocol;

	msp_protocol.set_alt(alt);
}


/*
	///////////////////////////////////////////////
	PUBLIC FUNCTIONS
	///////////////////////////////////////////////
*/

model control::get_model(std::string path){

	// set the file to be read
	std::ifstream infile(path.c_str());

	model m;

	if (!infile.good())
	{
		if (debug)
		{
			std::cout << "File model.txt is invalid!" << std::endl;
		}

		return m;
	}

	// File is good and start reading line
	std::string line;

	while(std::getline(infile, line)){

		std::istringstream iss(line);

		// Space delimated with distance speed and height in order
	    if (!(iss >> m.distance >> m.speed >> m.height)) { break; } // error

	    if (debug)
	    {
	    	printf("distance:%fmm speed:%fm/s height:%fmm\n", m.distance, m.speed, m.height);
	    }
	}

	return m;
}

void control::arm(){

	std::cout << "\nArming..." << std::endl;

	raw_rc_frame arm_frame;

	// the sequence value of yaw to be armed
	arm_frame.yaw = 2000;

	set_flight_controls(arm_frame);	
}


void control::disarm(){

	std::cout << "\n\nDisArming..." << std::endl;
	
	raw_rc_frame disarm_frame;

	// default raw rc frame is is set to disarm
	set_flight_controls(disarm_frame);
}

void control::set_rc(raw_rc_frame rc_frame){
	set_flight_controls(rc_frame);
}

void control::follow(float distance, float speed, float height){

	// Calculate the Throtle and Pitch here with the given paramter here
	// and set the rc values using set_rc(opcode, payloadsize, payload)

	//move_forward(throttle_val, pitch_val);

}



void control::throttle(uint16_t throttle_set_val){
	raw_rc_frame a_frame;

	a_frame.yaw = 1500;
	a_frame.throttle = throttle_set_val;

	set_flight_controls(a_frame);
}

void control::move_forward(uint16_t throttle_set_val, uint16_t pitch_set_val){
	raw_rc_frame move_frame;

	move_frame.throttle = throttle_set_val;
	move_frame.pitch = pitch_set_val;

	set_flight_controls(move_frame);
}

void control::hover_with_msp_set_alt(int32_t target_alt){

	control_state ctrl_state = ARM;

	raw_rc_frame throttle_frame;
	throttle_frame.throttle = 1500;

	raw_rc_frame hover_frame;
	hover_frame.aux1 = 1500;

	switch(ctrl_state){
		case ARM:
			arm();
			ctrl_state = INCTHROTTLE;
			break;
		case INCTHROTTLE:
			set_flight_controls(throttle_frame);
			ctrl_state = HOLD;
			break;
		case HOLD:
			set_alt(target_alt);
			ctrl_state = KEEP;
			break;
		case KEEP:
			set_flight_controls(hover_frame);
			break;
		default:
			break;
	}
}


void control::hover(int32_t target_alt){


	raw_rc_frame hover_frame;hover_frame.throttle += 110;
	raw_rc_frame alt_hold;	hover_frame.yaw = 1500;
	
	control_state ctrl_state = ARM;

	bool hovering = true;

	request req;

	altitude_frame alt_frame = req.request_altitude();

	int32_t start_alt = alt_frame.est_alt;
 
	while(hovering){

		switch(ctrl_state){

			case ARM:

				arm();

				ctrl_state = INCTHROTTLE;

				break;

			case INCTHROTTLE:
				{

					alt_frame = req.request_altitude();

					// Checks if target_alt is reached 
					if (alt_frame.est_alt == target_alt)
					{
						// Changge state to HOLD
						ctrl_state = HOLD;

					}else{
						// Increase throttle 
						hover_frame.throttle += 1;
						set_flight_controls(hover_frame);
						ctrl_state = INCTHROTTLE;
					}					
				}

				break;

			case HOLD:
				std::cout << "Holding Altitude..... " << std::endl;

				alt_hold.yaw = 1500;
				alt_hold.aux1 = 1500;
				set_flight_controls(alt_hold);

				ctrl_state = KEEP;
				break;

			case KEEP:
				// keeps hovering until Some external logic to change state to decthrottle
				break;

			case FIX:

				alt_frame = req.request_altitude();

				// This needs to be experiments and tested
				// We don't know how fast we want drone to descend
				// 10 cm/s for the place holder
				// If exceeds 10 then increase the throttle
				if (alt_frame.vario > 10) //10cm/s 
				{
					hover_frame.throttle += 1;
					set_flight_controls(hover_frame);
				}
				
				ctrl_state = DECTHROTTLE;
				break;

			case DECTHROTTLE:

				{
					alt_frame = req.request_altitude();

					// Check if the start altitude is reached
					if (alt_frame.est_alt == start_alt)
					{
						ctrl_state = DISARM;
					}else{

						hover_frame.throttle -= 1;
						set_flight_controls(hover_frame);

						// Fix throttle if descends too fast
						ctrl_state = FIX;
					}		
				}

				break;

			case DISARM:

				disarm();

				hovering = false;

				break;

			default:
				break;

		} // end switch
	} // end while
}

