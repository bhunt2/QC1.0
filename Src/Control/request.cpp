
#include "request.h"
#include "types.h"

/*
request::request(){
	debug = false;
}
*/

ident_frame request::request_ident(){

	return parse.evaluate_identification(msp_protocol.request_data(msp_ident));
}

attitude_frame request::request_attitude(){

	return parse.evaluate_attitude(msp_protocol.request_data(msp_attitude));
}

altitude_frame request::request_altitude(){

	return parse.evaluate_altitude(msp_protocol.request_data(msp_altitude));

}

raw_rc_frame request::request_rc_values(){

	return parse.evaluate_raw_rc(msp_protocol.request_data(msp_read_rc));

}