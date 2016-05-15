
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


control::control(){

}

control::~control(){

}
*/



/*
	///////////////////////////////////////////////
	PRIVATE FUNCTIONS
	///////////////////////////////////////////////
*/

void control::arm(){

	std::cout << "\nArming..." << std::endl;

	msp_protocol.set_flight_controls(1500, 1500, 2000, 1000);
	
}


void control::disarm(){

	std::cout << "\n\nDisArming..." << std::endl;
	
	msp_protocol.set_flight_controls(1500, 1500, 1000, 1000);
}


void control::set_flight_controls(uint16_t roll, uint16_t pitch, uint16_t yaw, uint16_t throttle){

	msp_protocol.set_flight_controls(roll, pitch, yaw, throttle);

}


void control::set_flight_controls(float distance, float speed, float height){

	// Calculate the Throtle and Pitch here with the given paramter here
	// and set the rc values using set_rc(opcode, payloadsize, payload)

}


/*
	///////////////////////////////////////////////
	PUBLIC FUNCTIONS
	///////////////////////////////////////////////
*/


void control::hover(uint16_t max_throttle, int hover_time){

	uint16_t start_throttle = 1504;

	uint16_t roll = 1529;
	uint16_t pitch = 1526;
	uint16_t yaw = 1518;
	uint16_t throttle = start_throttle;

	control_state ctrl_state = ARM;

	bool hovering = true;

	time_t startTime,
			endTime,
			currentTime, 
			elapsedTime;

	elapsedTime = 0;

	//parsers parse;

	while(hovering){

		printf("roll: %u\t pitch: %u\t yaw: %u\t throttle: %u\n", roll, pitch, yaw, throttle);

		//parse.evaluate_raw_rc(msp_protocol.request_data(msp_altitude));

		switch(ctrl_state){

			case ARM:

				arm();
				
				//set_flight_controls(1500,1500,2000,1000);

				usleep(1*microseconds);

				ctrl_state = INCTHROTTLE;

				break;

			case INCTHROTTLE:

				throttle += 1;

				if (throttle < max_throttle)
				{
					set_flight_controls(roll, pitch, yaw, throttle);

					usleep(0.5*microseconds);

					ctrl_state = INCTHROTTLE;
				}else{
					ctrl_state = DECTHROTTLE;
				}

				break;

			case KEEP:

				for(startTime = time(NULL),endTime = startTime+hover_time; (currentTime = time(NULL)) < endTime;){ 

					if((currentTime-startTime ) > elapsedTime){ 

						set_flight_controls(roll, pitch, yaw, throttle);

						elapsedTime = currentTime - startTime; 
						
						std::cout<<"Decreasing Throttle In: " << hover_time - elapsedTime << " seconds" << std::endl; 
					} 
				} 

				ctrl_state = DECTHROTTLE;

				break;

			case DECTHROTTLE:

				{
					throttle -= 1;

					if (throttle > start_throttle)
					{
						set_flight_controls(roll, pitch, yaw, throttle);

						usleep(0.5*microseconds);

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

