#include <stdint.h>

bool debug = true;

unsigned int microseconds = 1000000;

uint8_t msp_ident = 100;

uint8_t msp_attitude = 108;

uint8_t msp_altitude = 109;

uint8_t msp_read_rc = 105;

uint8_t msp_set_raw_rc = 200;

uint8_t msp_set_alt = 216;

const char* to_fc_header = "$M<";

const char* from_fc_header = "$M>";