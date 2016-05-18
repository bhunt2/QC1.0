
//#include <stdio.h>
#include <iostream>

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

	uint16_t payload[payload_size] = {frame.roll, frame.pitch, frame.yaw, frame.throttle, 0, 0, 0, 0};
	
	msp_protocol.set_rc(msp_set_raw_rc, payload_size, payload);

}


/*
	///////////////////////////////////////////////
	PUBLIC FUNCTIONS
	///////////////////////////////////////////////
*/


void control::arm(){

	std::cout << "\nArming..." << std::endl;

	set_raw_rc_frame arm_frame;

	arm_frame.roll = 1500;
	arm_frame.pitch = 1500;
	arm_frame.yaw = 2000;
	arm_frame.throttle = 1000;

	set_flight_controls(arm_frame);
	
}


void control::disarm(){

	std::cout << "\n\nDisArming..." << std::endl;
	
	set_raw_rc_frame disarm_frame;

	disarm_frame.roll = 1500;
	disarm_frame.pitch = 1500;

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


void control::hover(uint16_t max_throttle, int hover_time){

	set_raw_rc_frame hover_frame;

	control_state ctrl_state = ARM;

	bool hovering = true;

	uint16_t start_throttle = hover_frame.throttle; // min throttle

	time_t startTime,
			endTime,
			currentTime, 
			elapsedTime;

	elapsedTime = 0;

	int step = 5;
	//parsers parse;

	while(hovering){

		printf("roll: %u\t pitch: %u\t yaw: %u\t throttle: %u\n", 
			hover_frame.roll, hover_frame.pitch, hover_frame.yaw, hover_frame.throttle);

		//parse.evaluate_raw_rc(msp_protocol.request_data(msp_altitude));

		switch(ctrl_state){

			case ARM:

				arm();
	
				usleep(1*microseconds);

				ctrl_state = INCTHROTTLE;

				break;

			case INCTHROTTLE:

				hover_frame.throttle += step;

				if (hover_frame.throttle < max_throttle)
				{
					set_flight_controls(hover_frame);

					ctrl_state = INCTHROTTLE;
				}else{
					ctrl_state = DECTHROTTLE;
				}

				break;

			case KEEP:

				for(startTime = time(NULL),endTime = startTime+hover_time; (currentTime = time(NULL)) < endTime;){ 

					if((currentTime-startTime ) > elapsedTime){ 

						set_flight_controls(hover_frame);

						elapsedTime = currentTime - startTime; 
						
						std::cout<<"Decreasing Throttle In: " << hover_time - elapsedTime << " seconds" << std::endl; 
					} 
				} 

				ctrl_state = DECTHROTTLE;

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

	} // end while
}

