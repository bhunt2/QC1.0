/*
 Name:		Sketch1.ino
 Created:	2/14/2016 4:36:23 PM
 Author:	sabin
*/


// Send a message using the MultiWii serial protocol.
void send_msp(uint8_t opcode, uint8_t * data, uint8_t n_bytes) {
	uint8_t checksum = 0;

	// Send the MSP header and message length
	Serial.write((byte *)"$M<", 3);
	Serial.write(n_bytes);
	checksum ^= n_bytes;

	// Send the op-code
	Serial.write(opcode);
	checksum ^= opcode;

	// Send the data bytes
	for (int i = 0; i < n_bytes; i++) {
		Serial.write(data[i]);
		checksum ^= data[i];
	}

	// Send the checksum
	Serial.write(checksum);
}

// Send a message using the MultiWii serial protocol.
void receive_msp(uint8_t opcode, uint8_t * data, uint8_t n_bytes) {
	uint8_t checksum = 0;

	// Send the MSP header and message length
	Serial.write((byte *)"$M>", 3);
	Serial.write(n_bytes);
	checksum ^= n_bytes;

	// Send the op-code
	Serial.write(opcode);
	checksum ^= opcode;

	// Send the data bytes
	/**/for (int i = 0; i < n_bytes; i++) {
		Serial.write(data[i]);
		checksum ^= data[i];
	}

	// Send the checksum
	Serial.write(checksum);
}

void request_msp(uint8_t opcode, uint8_t n_bytes) {
	uint8_t checksum = 0;

	// Send the MSP header and message length
	Serial.write((byte *)"$M<", 3);
	Serial.write(n_bytes);
	checksum ^= n_bytes;

	// Send the op-code
	Serial.write(opcode);
	checksum ^= opcode;

	// Send the checksum
	Serial.write(checksum);
}

int Byte;
char data[ ] = "VERSION";
// the setup function runs once when you press reset or power the board

void setup() {
	Serial.begin(115200); // Serial Monitor
	Serial1.begin(115200);
}

// the loop function runs over and over again until power down or reset
void loop() 
{

	/* Data request */
	/**/
	request_msp(100, 0);
	//receive_msp(102, 0, sizeof(uint8_t));
	//send_msp(100, 0, sizeof(Byte));
	receive_msp(100, (uint8_t *)data, sizeof(Byte));

	/* Read Data from Mwii */
	while (Serial.available()) {
		Byte = Serial.read();
		Serial.println(Byte); // Print data from Multiwii
		Serial1.println(Serial1.read());
	}
	
	delay(10);
}
