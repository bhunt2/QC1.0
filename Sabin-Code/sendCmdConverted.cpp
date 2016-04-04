void send_cmd(uint8_t opcode, uint8_t* data, uint8_t data_length)
{
	uint8_t checksum = 0;

	dev->writeStr("$M<");

	char length_str[sizeof(std::string)];
	sprintf(length_str, "%x", data_length);
	dev->writeStr(length_str);
	checksum ^= data_length;

	char opcode_str[sizeof(std::string)];
	sprintf(opcode_str, "%x", opcode);
	dev->writeStr(opcode_str);
	checksum ^= opcode;

	for (int i = 0; i < data_length; i++)
	{
		char data_str[sizeof(std::string)];
		sprintf(data_str, "%x", data[i]);
		dev->writeStr(data_str);
		checksum ^= data[i];
	}

	char checksum_str[sizeof(std::string)];
	sprintf(checksum_str, "%x", checksum);

	dev->writeStr(checksum_str);
}
