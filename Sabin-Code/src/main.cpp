#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <exception>

#include "msp_frames.h"
#include "types.h"
#include "parsers.h"
#include "protocol.h"
#include "control.h"


int main(int argc, char* argv[]){

	std::cout << "Debug is =>" << debug << std::endl;

	if (argc <= 1)
	{
		std::cout << "Argument must be provided." << std::endl;
		std::cout << "Example: ./drone [msp command number]\n" << std::endl;
		return 0;
	}


	enum CMD{
		ATT,
		ALT,
		RCREAD,
		IDENT,
		ARM,
		DISARM,
		HOVER,
		MODELREAD,
		MODEL,
		THROTTLE,
		UNKNOWN
	};

	CMD SETCMD = UNKNOWN;

	if (strcmp(argv[1], "att") == 0)	
	{
		SETCMD = ATT;

	}
	else if (strcmp(argv[1], "alt") == 0)
	{
		SETCMD = ALT;

	}
	else if (strcmp(argv[1], "rc-read") == 0)
	{
		SETCMD = RCREAD;
		
	}
	else if (strcmp(argv[1], "ident") == 0)
	{
		SETCMD = IDENT;
		
	}
	else if (strcmp(argv[1], "arm") == 0)
	{
		SETCMD = ARM;

	}
	else if (strcmp(argv[1], "disarm") == 0)
	{
		SETCMD = DISARM;
	}
	else if (strcmp(argv[1], "hover") == 0)
	{
		if (argv[2] == NULL || argv[3] == NULL || argv[3] == NULL)
		{
			std::cout << "3 arguments expected! [max_throtte, hover_time, throttle_step]" << std::endl;

			return 0;
		}

		SETCMD = HOVER;

	}
	else if (strcmp(argv[1], "model") == 0)
	{
		SETCMD = MODEL;
	}
	else if(strcmp(argv[1], "throttle") == 0)
	{
		SETCMD = THROTTLE;
	}
	else
	{
		SETCMD = UNKNOWN;
	}


	parsers parse;
	control drone_ctrl;
	protocol msp_protocol(false);

	//sbool in_test = false;

	switch(SETCMD)
	{

		case ATT:
			parse.evaluate_attitude(msp_protocol.request_data(msp_attitude));
			break;

		case ALT:
		{

			int target_alt = 200; // 2 cmd

			while(true){
				altitude_frame alt_f = parse.evaluate_altitude(msp_protocol.request_data(msp_altitude));
				
				printf("target_alt=%d cm\t curr_alt=%d cm\n", target_alt, alt_f.est_alt);
				if (target_alt == alt_f.est_alt)
				{
					break;
				}
				usleep(1*microseconds);
			}
		}
			
			break;

		case RCREAD:
			parse.evaluate_raw_rc(msp_protocol.request_data(msp_read_rc));
			break;
		
		case IDENT:
			parse.evaluate_identification(msp_protocol.request_data(msp_ident));
			break;

		case ARM:
			drone_ctrl.arm();
			break;

		case DISARM:
			drone_ctrl.disarm();
			break;

		case HOVER:
			{
				
				uint16_t throttle = (uint16_t) strtoul(argv[2], NULL, 0);
				int seconds = (int) strtoul(argv[3], NULL, 0);
				int step = (int) strtoul(argv[4], NULL, 0);

				drone_ctrl.hover(throttle, seconds, step);
				break;
			}
			
		case MODEL:
			drone_ctrl.get_model();
			break;

		case THROTTLE:
			//in_test = true;
			
			/*{

				uint16_t throttle = 1000;

				throttle += 95;

				int c;
				system("/bin/stty raw");

				drone_ctrl.arm();
				
				while((c=getchar()) != '.')
				{

					drone_ctrl.throttle(throttle);

					putchar(c);

					if (c == 'w')
					{
						throttle += 1;
						drone_ctrl.throttle(throttle);
					}

					if (c == 's')
					{
						throttle -= 1;
						drone_ctrl.throttle(throttle);
					}

				}

				system("/bin/stty cooked");

			}*/

			break;

		case UNKNOWN:
		default:
			return 0;
			break;
	}


/*
	// Set MSP RC
	if (strcmp(argv[1], "control") == 0)
	{

		if (argv[2] == NULL || argv[3] == NULL || argv[4] == NULL || argv[5] == NULL)
		{
			std::cout << "4 arguments expected! [Roll Pitch Yaw Throttle]" << std::endl;

			return 0;
		}

		set_raw_rc_frame rc;

		rc.roll = (uint16_t) strtoul(argv[2], NULL, 0);
		rc.pitch = (uint16_t) strtoul(argv[3], NULL, 0);
		rc.yaw = (uint16_t) strtoul(argv[4], NULL, 0);
		rc.throttle = (uint16_t) strtoul(argv[5], NULL, 0);

		//drone_ctrl.set_flight_controls(rc.roll, rc.pitch, rc.yaw, rc.throttle);

		//usleep(3*microseconds);

		return 0;
	}

	
*/

	return 0;
	
}