
#include <exception>
#include <iostream>
#include <sstream>
#include <time.h>

#include "protocol.h"
#include "types.h"

/*
	///////////////////////////////////////////////
	CONSTRUCTORS
	///////////////////////////////////////////////
*/

protocol::protocol(){
	setup_uart();
}

protocol::~protocol(){
	delete device;
}

/*
	/////////////////////////////////////
	PUBLIC FUNCTIONS
	////////////////////////////////////
*/

void protocol::arm(){

	uint8_t length = 8;

	std::cout << "\nArming..." << std::endl;

	// Arm sequence
	//--------------------------//roll, pitch, yaw, throttle, aux1, aux2, aux3, aux4
	uint16_t arm_data[length] = {1500, 1500, 2000, 1000, 1000, 1000, 1000, 1000}; 

	set_rc(msp_set_raw_rc, length, arm_data);
}


void protocol::disarm(){

	uint8_t length = 8;

	std::cout << "\n\nDisArming..." << std::endl;
	// Arm sequence	
	//--------------------------//roll, pitch, yaw, throttle, aux1, aux2, aux3, aux4
	uint16_t disarm_data[length] = {1500, 1500, 1000, 1000, 1000, 1000, 1000, 1000};
	
	set_rc(msp_set_raw_rc, length, disarm_data);
}


void protocol::set_flight_controls(uint16_t roll, uint16_t pitch, uint16_t yaw, uint16_t throttle){

	uint8_t length = 8;

	uint16_t disarm_data[length] = {roll, pitch, yaw, throttle, 1000, 1000, 1000, 1000};
	
	set_rc(msp_set_raw_rc, length, disarm_data);

}


void protocol::set_flight_controls(float distance, float speed, float height){

	// Calculate the Throtle and Pitch here with the given paramter here
	// and set the rc values using set_rc(opcode, payloadsize, payload)

}


// Only takes in command code
// and reads the response back
std::string protocol::request_data(uint8_t opcode){

	std::cout << "Sending Command: " << opcode << std::endl;

	msp_request(opcode);

	usleep(500);

	return read();

}

// Sends the parameter to the Flight controller w/ command code
// and reads the response back
std::string protocol::request_data(uint8_t opcode, uint8_t param_length, uint16_t* params){

	std::cout << "Sending Command: " << opcode << std::endl;

	msp_command(opcode, param_length, params);

	usleep(500);

	return read();

}



/*
	///////////////////////////////////////////////
	PRIVATE FUNCTIONS
	///////////////////////////////////////////////
*/

void protocol::setup_uart(){
	try {
		std::cout << "Initializing UART..." << std::endl;
		device = new mraa::Uart(0);

		if (device->setBaudRate(uartsetup.baudrate) != mraa::SUCCESS) {
		std::cout << "Error setting parity on UART" << std::endl;
		}
		else{
			std::cout << "Setting Baudrate to " << uartsetup.baudrate << "..." << std::endl;
		}

		if (device->setMode(uartsetup.mode_byte_size, uartsetup.mode_parity, uartsetup.mode_stop_bits) != mraa::SUCCESS) {
			std::cout << "Error setting parity on UART" << std::endl;
		}
		else{
			std::cout << "Setting Mode to " << uartsetup.mode_byte_size << uartsetup.mode_parity << uartsetup.mode_stop_bits << "..." << std::endl;
		}

		if (device->setFlowcontrol(uartsetup.xon_xoff, uartsetup.rts_cts) != mraa::SUCCESS) {
			std::cout << "Error setting flow control UART" << std::endl;
		}
		else{
			std::cout << "Setting Flow Control: xonxoff->" << uartsetup.xon_xoff << " rtscts->" << uartsetup.rts_cts << "..." << std::endl;
		}

		std::cout << "Device Path: " << device->getDevicePath() << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << ", likely invalid platform config" << std::endl;
	}
}

void protocol::msp_request(uint8_t opcode){

	char msp_string[20];

	uint8_t checksum = 0;

	uint8_t ploadsize = 0;

	msp_string[0]=ploadsize*2 & 0xff;
    
    checksum ^= (ploadsize*2 & 0xFF);
    
    msp_string[1]=opcode & 0xff;
    
    checksum ^= (opcode & 0xFF);
    
    msp_string[ploadsize*2+2]= checksum;

	std::ostringstream buf;

    // Add header to the buffer
    buf << to_fc_header;

    // Add each of byte from string to the buffer
    for(int u=0;u<(ploadsize*2+3);u++){
    	buf << msp_string[u];
    }

    // Send the buffer data to Flight Controller
    device->writeStr(buf.str());

    /*
	std::ostringstream buf;

	uint8_t length = 0;

	uint8_t checksum = 0;

    // Calculate checksum
    checksum = checksum ^ length ^ opcode;
	
	// Pack frame
	buf << to_fc_header << (char)length << (char)opcode << (char)checksum;
	
	// Convert buffer into a string
	std::string frame_string = buf.str();
	
	// Send packed frame
    device->writeStr(frame_string);
	

	if(debug == 1)
	{
		std::cout << "\n\n\t\t" << "Header\t     Size\t      Command\t       CRC\n";
		std::cout << "Sending Frame:\t";
		uint8_t ctr = 1;
		for ( std::string::iterator it = frame_string.begin(); it != frame_string.end(); ++it)
		{
			std::cout << *it;
			if(ctr == 3 || ctr == 4 || ctr == 5)
			{
				std::cout << "\t:\t";
			}
			ctr++;
		}
		
		std::string display_string;
		display_string.assign(string_to_hex(frame_string));
		std::cout << "\n\t\t";
		ctr = 1;
		for ( std::string::iterator it = display_string.begin(); it != display_string.end(); ++it)
		{
			std::cout << *it;
			if(ctr == 6 || ctr == 8 || ctr == 10)
			{
				std::cout << "\t:\t";
			}
			ctr++;
		}
		std::cout << "\n\n\n";
	}
	*/
}

void protocol::msp_command(uint8_t opcode, uint8_t data_length, uint16_t* params){

	std::ostringstream buf;

	uint8_t checksum = 0;

    // Calculate checksum
    checksum = checksum ^ data_length ^ opcode;
	
	// Pack frame

	buf << to_fc_header << (char)data_length << (char)opcode;
	
	// Pack the data parameters
	for (int i = 0; i < 8; i++)
	{
		checksum = checksum ^ params[i];
		std::ostringstream temp;
		temp << std::hex << params[i];
		buf << params[i];
		//buf << itoa(params[i], 16);		
	}

	// Append the checksum
	buf << (char)checksum;

	// Convert buffer into a string
	std::string frame_string = buf.str();
	
	// Send packed frame
    device->writeStr(frame_string);

	if(debug == 1)
	{
		std::cout << "\n\n\t\t" << "Header\t     Size\t      Command\t       CRC\n";
		std::cout << "Sending Frame:\t";
		uint8_t ctr = 1;
		for ( std::string::iterator it = frame_string.begin(); it != frame_string.end(); ++it)
		{
			std::cout << *it;
			if(ctr == 3 || ctr == 4 || ctr == 5)
			{
				std::cout << "\t:\t";
			}
			ctr++;
		}
		
		std::string display_string;
		display_string.assign(string_to_hex(frame_string));
		std::cout << "\n\t\t";
		ctr = 1;
		for ( std::string::iterator it = display_string.begin(); it != display_string.end(); ++it)
		{
			std::cout << *it;
			if(ctr == 6 || ctr == 8 || ctr == 10)
			{
				std::cout << "\t:\t";
			}
			ctr++;
		}
		std::cout << "\n\n\n";
	}
	
}


void protocol::set_rc(uint8_t opcode, uint8_t ploadsize, uint16_t pload_in[12]){
    
    time_t timer = 0;

    time_t start = difftime(time(NULL),0);

    unsigned int microseconds = 1000000;

    while (timer < 0.5){

	    char msp_string[20];
	    char aggregate[20];
	    char checksum=0;
	  
	    uint8_t msb_pl=0;
	    uint8_t lsb_pl=1;

	    // Aggregate data
	    for(int i=0;i<ploadsize;i++){
	      aggregate[msb_pl]=pload_in[i] & 0xff;
	      msb_pl=msb_pl+2;

	      aggregate[lsb_pl]=pload_in[i] >> 8;
	      lsb_pl=lsb_pl+2;
	    }

	    // Add Size
	    msp_string[0]=ploadsize*2 & 0xff;
	    checksum ^= (ploadsize*2 & 0xFF);

	    // Add MSP Command Code
	    msp_string[1]=opcode & 0xff;
	    checksum ^= (opcode & 0xFF);

	    // Calculate Checksum
	    for(int j=0;j<(ploadsize*2);j++){
	      msp_string[2+j]=aggregate[j] & 0xff;
	      checksum ^= (aggregate[j] & 0xff);
	    }

	    // Add Checksum  
	    msp_string[ploadsize*2+2]= checksum;
	    
	    std::ostringstream buf;

	    // Add header to the buffer
	    buf << to_fc_header;

	    // Add each of byte from string to the buffer
	    for(int u=0;u<(ploadsize*2+3);u++){
	    	buf << msp_string[u];
	    }

	    // Send the buffer data to Flight Controller
	    device->writeStr(buf.str());

        usleep(microseconds*0.05);

        timer = timer + (difftime(time(NULL),start));

        start =  time(NULL);
    }

}

std::string protocol::read(){
	// Establish variables for use in receiving data
	std::string buf;
    int to_counter = 0;
	int bytes_ctr = 0;
	/*
	typedef enum state
	{
		none,
		header
	} curr_state;

	curr_state = none;

	while(true){
		if (device->dataAvailable())
		{
			
		}
		header = self.ser.read()
            if header == '$':
                header = header+self.ser.read(2)
                break

	}
*/








	// Check for data availability on the UART
	// Do once, then continue unless time has run out 
	do
	{
		if(device->dataAvailable())
		{
			// Get known frame size back for now
			buf.append(device->readStr(1));
			
			if(debug == 1)
			{
				// State data is available only on the first byte
				if(bytes_ctr == 0)
				{
					std::cout << "Data Available" << std::endl;
				}
				
				// Count number of bytes received
				bytes_ctr++;
			}
			
			// Reset Time Out counter
			to_counter = 0;
		}
		else
		{
			to_counter += 1;
			if(to_counter == 1000)
			{
				std::cout << "No Serial Data Available.\n" << std::endl;
				usleep(1000);
			}
			
		}
	}while(to_counter < 1000);
	
	if(debug == 1)
	{
		std::cout << "Number of bytes received: " << bytes_ctr << std::endl;
	}
	
	return buf;
}


std::string protocol::string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}
