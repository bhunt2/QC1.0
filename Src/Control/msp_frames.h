
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
#include <string>

#ifndef MSP_FRAMES_H
#define MSP_FRAMES_H

struct model{
	double distance;
	double speed;
	double height;

	model():
	distance(0.0),
	speed(0.0),
	height(0.0)
	{}
};

struct ident_frame {
	uint8_t version; // version of MultiWii
	uint8_t multitype; // type of multi: TRI/QUADP,QUADX,BI,GIMBAL,Y6,HEX6,FLYING_WING,Y4,HEX6X,OCTOX8, OCTOFLATP,OCTOFLATX,AIRPLANE,HELI_120,HELI_90,VTAIL4,HEX6H,SINGLECOPTER,DUALCOPTER
	//const uint32_t capability; // A 32 bit variable to indicate capability of FC board. (Currently, BIND button is used on first bit, DYNBAL on second, FLAP on third )

	ident_frame():
	version(0),
	multitype(0)
	{}
};

struct attitude_frame {
	float ang_x; // 16 bit Range [-1800;1800] (unit: 1/10 degree)
	float ang_y; // 16 bit Range [-900;900] (unit: 1/10 degree)
	float heading; // 16 bit [-180;180]

	attitude_frame():
		ang_x(0.00),
		ang_y(0.00),
		heading(0)
		{}
};

struct altitude_frame {
	int32_t est_alt;
	int16_t vario; 

	altitude_frame() :
	est_alt(0),
	vario(0)
	{}
};

struct raw_rc_frame {
	uint16_t roll; // 16 bit Range [1000;2000]
	uint16_t pitch; // 16 bit Range [1000;2000]
	uint16_t yaw; // 16 bit Range [1000;2000]
	uint16_t throttle; // 16 bit Range [1000;2000]
	uint16_t aux1;

	raw_rc_frame() :
		roll(1500),
		pitch(1500),
		yaw(1000),
		throttle(1000),
		aux1(1000)
		{}
};

struct read_frame{
	std::string header;
	uint8_t payload_size;
	uint8_t opcode;
	std::string payload;
	int total_bytes;

	read_frame() :
		payload_size(0),
		opcode(0),
		total_bytes(0)
	{}
};

#endif



