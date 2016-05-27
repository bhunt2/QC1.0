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
  for(int i = 0; i < n_bytes; i++) {
    Serial.write(data[i]);
    checksum ^= data[i];
  }
   
  // Send the checksum
  Serial.write(checksum);
}
