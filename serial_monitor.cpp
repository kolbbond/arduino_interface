// from ai suggestion
#include <iostream>
#include <boost/asio.hpp>
#include "Log.hh"

int main(int argc, char** argv) {
	// add log for color @hey:

    ShLogPr lg = Log::create();
    lg->msg("%sConnect to serial port%s\n",KBLU,KNRM);

	// add tclap for input @hey:


	boost::asio::io_service io;
	boost::asio::serial_port port(io, "/dev/ttyUSB0"); // Replace with your port name

	port.set_option(boost::asio::serial_port_base::baud_rate(115200));

	// keep open and read lines
	while(true) {
		try {
			while(true) {
				char bdata[100];
				std::string data;
				//				boost::asio::read_until(port, boost::asio::dynamic_buffer(data), '\n');
				boost::system::error_code ec;
				size_t bytes_read = port.read_some(boost::asio::buffer(bdata), ec);
				//std::cout << data << std::endl;
				std::cout << std::string(bdata,bytes_read);
			}
		} catch(boost::system::system_error& e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
	return 0;
}
