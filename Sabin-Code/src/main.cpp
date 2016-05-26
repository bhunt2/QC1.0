#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <exception>
#include <termios.h>
#include <fcntl.h>

#include "msp_frames.h"
#include "types.h"
#include "request.h"
#include "control.h"

void throttle_test();

request read_me;
control drone_ctrl;

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

	switch(SETCMD)
	{
		
		case IDENT:
			read_me.request_ident();

			break;

		case ATT:
			read_me.request_attitude();
			break;

		case ALT:
		{

			//int target_alt = 200; // 2 cmd

			while(true){
				read_me.request_altitude();
				/*
				altitude_frame alt_f = 
				printf("target_alt=%d cm\t curr_alt=%d cm\n", target_alt, alt_f.est_alt);
				if (target_alt == alt_f.est_alt)
				{
					break;
				}*/
				usleep(1*microseconds);
			}
		}
			
			break;

		case RCREAD:
			read_me.request_rc_values();
			break;

		case ARM:
			drone_ctrl.arm();
			break;

		case DISARM:
			drone_ctrl.disarm();
			break;

		case HOVER:
			{
				control drone_ctrl;

				uint16_t throttle = (uint16_t) strtoul(argv[2], NULL, 0);
				int seconds = (int) strtoul(argv[3], NULL, 0);
				int step = (int) strtoul(argv[4], NULL, 0);

				drone_ctrl.hover(throttle, seconds, step);
				break;
			}
			
		case MODEL:
			control drone_ctrl;
			drone_ctrl.get_model();
			break;

		case THROTTLE:

			throttle_test();
			break;

		case UNKNOWN:
		default:
			return 0;
			break;
	}

	return 0;
	
}

void throttle_test(){

	control drone_ctrl;

	uint16_t throttle = 1100;
	
	bool armed = false;

	struct termios oldSettings, newSettings;

    tcgetattr( fileno( stdin ), &oldSettings );
    newSettings = oldSettings;
    newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr( fileno( stdin ), TCSANOW, &newSettings );    
	char c;
    while (1)
    {
    	if (!armed)
    	{
    		drone_ctrl.arm();
    		armed = true;
    	}else{
    		printf("\n\n****throttle: %d****\n\n", throttle);
			drone_ctrl.throttle(throttle);
    	}
    	
        fd_set set;
        struct timeval tv;

        tv.tv_sec = 0;
        tv.tv_usec = 0;

        FD_ZERO( &set );
        FD_SET( fileno( stdin ), &set );

        int res = select( fileno( stdin )+1, &set, NULL, NULL, &tv );

        if( res > 0 )
        {
            
            read( fileno( stdin ), &c, 1 );

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
			if (c == 'x')
			{
				drone_ctrl.disarm();
				break;
			}
        }
        else if( res < 0 )
        {
            perror( "select error" );
            break;
        } 
    }

    tcsetattr( fileno( stdin ), TCSANOW, &oldSettings );
}