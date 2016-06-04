#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <exception>
#include <termios.h>
#include <fcntl.h>
#include <thread>
//#include <mutex>

#include "msp_frames.h"
#include "types.h"
#include "request.h"
#include "control.h"

void control_test();
void thread_control(raw_rc_frame);
void thread_read();
int kbhit();
void nonblock();

request read_me;
control drone_ctrl;

int main(int argc, char* argv[]){
	

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
		CONTROL,
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
	else if(strcmp(argv[1], "control") == 0)
	{
		SETCMD = CONTROL;
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
				uint16_t throttle = (uint16_t) strtoul(argv[2], NULL, 0);
				int seconds = (int) strtoul(argv[3], NULL, 0);
				int step = (int) strtoul(argv[4], NULL, 0);

				drone_ctrl.hover(throttle, seconds, step);
				break;
			}
			
		case MODEL:
			drone_ctrl.get_model();
			break;

		case CONTROL:

			control_test();
			break;

		case UNKNOWN:
		default:
			return 0;
			break;
	}

	return 0;
	
}

int kbhit()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

void nonblock(int state)
{
    struct termios ttystate;
 
    //get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);
 
    if (state==1)
    {
        //turn off canonical mode
        ttystate.c_lflag &= ~ICANON;
        //minimum of number input read.
        ttystate.c_cc[VMIN] = 1;
    }
    else if (state==0)
    {
        //turn on canonical mode
        ttystate.c_lflag |= ICANON;
    }
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
 
}

void control_test(){

	char c;
    int i=0;
 
    nonblock(1);

    std::thread controlThread; 
    std::thread readThread;

    drone_ctrl.arm();

    raw_rc_frame rc_frame;
	rc_frame.yaw = 1500;
	rc_frame.throttle = 1100;
	
	raw_rc_frame alt_hold;
	alt_hold.yaw = 1500;
	alt_hold.aux1 = 1500;

    while(!i)
    {
        i = kbhit();

        if (i!= 0)
        {
            c = fgetc(stdin);

            switch(c){
        		case 'w':
        		rc_frame.throttle += 1;
        		break;

        		case 's':
        		rc_frame.throttle -= 1;
        		break;

				case 'a':
				rc_frame.pitch -= 1;
        		break;

        		case 'd':
        		rc_frame.pitch += 1;
        		break;

        		case 'h':
        		drone_ctrl.set_rc(alt_hold);
        		break;

        		case 'x':
        		drone_ctrl.disarm();
        		return;
        		break;

        		default:
        			break;
        	}
        	i = 0;
        }else{
        	i = 0;
        	
        	controlThread = std::thread(thread_control, rc_frame); 
        	readThread = std::thread(thread_read);
        	
        } // End if check for input
        
        if (controlThread.joinable())
        {
        	controlThread.join();
        }

        if (readThread.joinable())
        {
        	readThread.join();
        }

		//printf("\n%d\n", rc_frame.throttle);
    } // End while
    
    nonblock(0);

}

void thread_read(){
    //read_me.request_rc_values();

	//printf("roll: %u\t pitch: %u\t yaw: %u\t throttle: %u aux1: %u\n", rc.roll, rc.pitch, rc.yaw, rc.throttle, rc.aux1);
}

void thread_control(raw_rc_frame rc_f){
	drone_ctrl.set_rc(rc_f);

}
