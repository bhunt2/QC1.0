
#include "request.h"
#include "protocol.h"
#include "types.h"

ident_frame request::request_ident(){

	protocol msp_protocol(false);

	return parse.evaluate_identification(msp_protocol.request_data(msp_ident));
}

attitude_frame request::request_attitude(){

	protocol msp_protocol(false);

	return parse.evaluate_attitude(msp_protocol.request_data(msp_attitude));
}

altitude_frame request::request_altitude(){

	protocol msp_protocol(false);

	return parse.evaluate_altitude(msp_protocol.request_data(msp_altitude));

}

raw_rc_frame request::request_rc_values(){

	protocol msp_protocol(false);

	return parse.evaluate_raw_rc(msp_protocol.request_data(msp_read_rc));

}