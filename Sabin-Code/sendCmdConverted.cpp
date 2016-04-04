
	try {
		dev = new mraa::Uart(0);
	} catch (std::exception& e) {
		std::cout << e.what() << ", likely invalid platform config" << std::endl;
	}

	std::string dev_path = dev->getDevicePath();

	std::cout << "dev path: " << dev_path << std::endl;

	/*
	try {
		//dev = new mraa::Uart("/dev/ttyACM0");
		dev = new mraa:: Uart(dev_path);
	} catch (std::exception& e) {
		std::cout << "Error while setting up raw UART, do you have a uart?" << std::endl;
		std::terminate();
	}
	 */

	if (dev->setBaudRate(115200) != mraa::SUCCESS) {
		std::cout << "Error setting parity on UART" << std::endl;
	}
	if (dev->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {
		std::cout << "Error setting parity on UART" << std::endl;
	}
	if (dev->setFlowcontrol(false, false) != mraa::SUCCESS) {
		std::cout << "Error setting flow control UART" << std::endl;
	}
