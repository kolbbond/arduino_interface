// command line parser
#include <tclap/CmdLine.h>

#include <iostream>
#include <boost/asio.hpp>
#include "Log.hh"


int main(int argc, char** argv) {

	// add tclap for input @hey:
	// create tclap object
	TCLAP::CmdLine cmd("Serial Monitor (serial port, baudrate)", ' ');

	// filename input argument
	TCLAP::UnlabeledValueArg<std::string> input_serialport_argument(
		"sp", "Serial Port", true, "/dev/ttyACM0", "string", cmd);
	TCLAP::UnlabeledValueArg<int> input_baudrate_argument("br", "baudrate", false, 9600, "int", cmd);
	cmd.parse(argc, argv);

	// add log for color @hey:
	const ShLogPr lg = Log::create();
	lg->msg("%sConnect to serial port with baudrate%s\n", KBLU, KNRM);

	lg->msg("%s\n", input_serialport_argument.getValue().c_str());
	lg->msg("%i\n", input_baudrate_argument.getValue());

	// open port
	boost::asio::io_service io;
	boost::asio::serial_port port(io, input_serialport_argument.getValue()); // Replace with your port name
	port.set_option(boost::asio::serial_port_base::baud_rate(input_baudrate_argument.getValue()));

	// keep open and read lines
	lg->msg("%sStart reading lines...%s\n", KRED, KNRM);
	while(true) {
		try {
			while(true) {
				char bdata[100];
				std::string data;
				//				boost::asio::read_until(port, boost::asio::dynamic_buffer(data), '\n');
				boost::system::error_code ec;
				size_t bytes_read = port.read_some(boost::asio::buffer(bdata), ec);
				// check error code
				if(!ec) {
					std::string outstr = std::string(bdata, bytes_read);

					// check for numbers and color @hey:
					for(auto c : outstr) {
						std::string color = KNRM;
						if(c >= '0' && c <= '9') {
							color = KGRN;
						}

						// output character
						lg->msg("%s%c%s", color.c_str(), c, KNRM);
					}

					//std::cout << outstr;
				} else {
					//std::cerr << "Error: " << ec.message() << std::endl;
					break;
				}
			}
		} catch(boost::system::system_error& e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
	return 0;
}
