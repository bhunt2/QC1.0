void send_cmd(uint8_t code, uint8_t data, uint8_t data_length)
{
	uint8_t checksum = 0;

	char header[] = "$M<";
	
	dev->write(header, sizeof(header));
	
	dev->write((char *)data_length, sizeof(data_length));
	checksum ^= data_length;

	dev->write((char *)code, sizeof(code));
	checksum ^= code;

	for (int i = 0; i < data_length; i++)
	{
		dev->write((char *)data[i], sizeof(data[i]));
		checksum ^= (char *)data[i];
	}

	dev->write((char *) checksum, sizeof(checksum));
}
