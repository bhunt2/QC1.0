
#include "mraa.hpp"

class protocol
{

private:
	// UART Device object
	mraa::Uart* device;

	// UART Properties
	struct uart_setup
	{
		unsigned int baudrate;
		int mode_byte_size;
		mraa::UartParity mode_parity;
		int mode_stop_bits;
		bool xon_xoff;
		bool rts_cts;

		uart_setup() :
			baudrate(115200),
			mode_byte_size(8),
			mode_parity(mraa::UART_PARITY_NONE),
			mode_stop_bits(1),
			xon_xoff(false),
			rts_cts(false)
			{}
	} uartsetup;

	// Setup the UART with 
	void setup_uart(int);

	void msp_request(uint8_t);

	void msp_command(uint8_t, uint8_t, uint8_t*);

public:
	protocol(mraa::Uart*);

	~protocol();

	void get_response_without_parameters(uint8_t);

	void get_response_with_parameters(uint8_t, uint8_t, uint8_t*);

	void read(uint8_t);
};