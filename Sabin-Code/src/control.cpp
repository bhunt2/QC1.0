
#include <fstream>
#include <iostream>
#include <sstream>

#include "time.h"

#include "control.h"
#include "types.h"
#include "parsers.h"

/*
	///////////////////////////////////////////////
	CONSTRUCTORS
	///////////////////////////////////////////////
*/

//control::control(){}

//control::~control(){}


/*
	///////////////////////////////////////////////
	PRIVATE FUNCTIONS
	///////////////////////////////////////////////
*/

void control::set_flight_controls(set_raw_rc_frame frame){

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
	
	protocol p(false);
	
	p.set_rc(msp_set_raw_rc, payload_size, payload);

}


/*
	///////////////////////////////////////////////
	PUBLIC FUNCTIONS
	///////////////////////////////////////////////
*/

model control::get_model(){

	std::ifstream infile("model.txt");

	model m;

	if (!infile.good())
	{
		if (debug)
		{
			std::cout << "File model.txt is invalid!" << std::endl;
		}

		return m;
	}

	std::string line;

	while(std::getline(infile, line)){

		std::istringstream iss(line);

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

	set_raw_rc_frame arm_frame;

	arm_frame.yaw = 2000;

	set_flight_controls(arm_frame);
	
}


void control::disarm(){

	std::cout << "\n\nDisArming..." << std::endl;
	
	set_raw_rc_frame disarm_frame;

	set_flight_controls(disarm_frame);
}

void control::follow(float distance, float speed, float height){

	// Calculate the Throtle and Pitch here with the given paramter here
	// and set the rc values using set_rc(opcode, payloadsize, payload)

	//move_forward(throttle_val, pitch_val);

}



void control::throttle(uint16_t throttle_set_val){
	set_raw_rc_frame a_frame;

	a_frame.throttle = throttle_set_val;

	set_flight_controls(a_frame);
}

void control::move_forward(uint16_t throttle_set_val, uint16_t pitch_set_val){
	set_raw_rc_frame move_frame;

	move_frame.throttle = throttle_set_val;
	move_frame.pitch = pitch_set_val;

	set_flight_controls(move_frame);
}


void control::hover(uint16_t max_throttle, int hover_time, int step){

	set_raw_rc_frame hover_frame;
	set_raw_rc_frame alt_hold;
	hover_frame.yaw = 1500;
	hover_frame.throttle += 110;

	control_state ctrl_state = ARM;

	bool hovering = true;

	uint16_t start_throttle = hover_frame.throttle; // min throttle

	time_t startTime = 0,
			elapsedTime = 0;

	//parsers parse;
	//protocol p(false);
 
	while(hovering){

		switch(ctrl_state){

			case ARM:

				arm();

				ctrl_state = INCTHROTTLE;

				break;

			case INCTHROTTLE:

				hover_frame.throttle += step;
				if (hover_frame.throttle < max_throttle)
				{
					
					set_flight_controls(hover_frame);

					ctrl_state = INCTHROTTLE;
				}else{
					ctrl_state = HOLD;
				}

				break;

			case HOLD:
				alt_hold.yaw = 1500;
				alt_hold.aux1 = 1500;
				set_flight_controls(alt_hold);
				ctrl_state = HOLD;
				break;

			case KEEP:
				
				if (startTime == 0){ startTime = time(NULL);}

				std::cout<<"Hovering for " << hover_time << " seconds" << std::endl; 

				elapsedTime = time(NULL) - startTime; 
				
				if(elapsedTime < hover_time){ 

					ctrl_state = KEEP;

					set_flight_controls(hover_frame);

				} else{
					ctrl_state = DECTHROTTLE;
				}

				break;

			case DECTHROTTLE:

				{
					hover_frame.throttle -= step;

					if (hover_frame.throttle > start_throttle)
					{
						set_flight_controls(hover_frame);

						ctrl_state = DECTHROTTLE;
					}else{
						ctrl_state = DISARM;
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
		//parse.evaluate_raw_rc(p.request_data(msp_altitude));
		//usleep(300);
	} // end while
}

