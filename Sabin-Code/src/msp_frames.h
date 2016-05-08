#include <stdint.h>

#ifndef MSP_FRAMES_H
#define MSP_FRAMES_H

struct ident_frame {
	const uint8_t* version; // version of MultiWii
	const uint8_t* multitype; // type of multi: TRI/QUADP,QUADX,BI,GIMBAL,Y6,HEX6,FLYING_WING,Y4,HEX6X,OCTOX8, OCTOFLATP,OCTOFLATX,AIRPLANE,HELI_120,HELI_90,VTAIL4,HEX6H,SINGLECOPTER,DUALCOPTER
	const uint32_t* capability; // A 32 bit variable to indicate capability of FC board. (Currently, BIND button is used on first bit, DYNBAL on second, FLAP on third )
};

struct attitude_frame {
	signed int ang_x; // 16 bit Range [-1800;1800] (unit: 1/10 degree)
	signed int ang_y; // 16 bit Range [-900;900] (unit: 1/10 degree)
	signed int heading; // 16 bit [-180;180]
};

struct altitude_frame {
	int32_t est_alt;
	int16_t vario; 
};

struct set_raw_rc_frame {
	uint16_t roll;
	uint16_t pitch;
	uint16_t yaw;
	uint16_t throttle;
};

#endif



