// cpp includes
#include <iostream>
#include <chrono>
#include <thread>

// externals
#include <boost/beast.hpp>
#include <boost/asio.hpp>

// internals
#include "Log.hh"
#include "HttpHelper.hh"

// custom parse data function and struct?
// make a type maybe @todo:
struct data_point {
	float temp;
	float humi;
};

//
data_point parse_body(std::string body) {
	// assume data is structure this way
	// what is a better format?
	// message now is
	// @hey: maybe change to... a json? idk A BETTER FORMAT
	// Temperature [°F]: 75.02  |  Humidity [%]: 37.00
	// so split by | and then :

	//std::cout << "body: " << body << std::endl;

	// split string
	std::vector<std::string> mparts = Log::split_string(body, '|');
	if(mparts.size() != 2) {
		throw("more than two parts");
	} else {

		// allocate
		data_point rdata;

		// walk string parts
		int cnt = 0;
		for(auto s : mparts) {
			std::vector<std::string> myreading;

			// split again for each sensor
			myreading = Log::split_string(s, ':');
			float mynum = std::stof(myreading[1]);

			// set data
			switch(cnt) {
			case 0: {
				rdata.temp = mynum;
				break;
			}
			case 1: {

				rdata.humi = mynum;
				break;
			}
			default: {
				break;
			}
			}
			cnt++;
		}
		return rdata;
	}
}


// main
int main(int argc, char** argv) {

	// create log
	ShLogPr lg = Log::create();

	// load server json
	ShHttpHelperPr hp = HttpHelper::create();
	hp->load_servers("../examples/servers.json", lg);

	// get servers
	std::vector<HttpHelper::serverconfig> myservers = hp->_servers;

	// game loop
	while(true) {

		// timer
		auto start = std::chrono::steady_clock::now();

		// walk over servers
		for(int i = 0; i < myservers.size(); i++) {

			HttpHelper::serverconfig myserver = myservers[i];

			std::string body;
			bool good_req = false;

			// try http request
			try {
				// get the body
				body = HttpHelper::make_request(myserver);
				good_req = true;
			} catch(std::exception& e) {
				lg->msg("%sERROR ERROR%s\n", KRED, KNRM);
				body = "error";
				std::cerr << "Error: " << e.what() << std::endl;
			}

			// if request is good
			if(good_req) {

				// try parsing request
				try {

					// parse the request body
					data_point rdata = parse_body(body);

					// test output
					std::cout << lg->format("s#(%i)| T: %0.2f, H: %0.2f\n", i, rdata.temp, rdata.humi);

				} catch(...) {
					std::cout << "bad parsing: " << i << "\n";
				}

			} else {
				// bad request?
			}
		}

		// check clock
		auto end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		// Sleep for the remainder of the second
		// @question: is sleeping better than running a bunch of cycles...?
		if(duration < std::chrono::milliseconds(1000)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000) - duration);
		}
	}

	return 0;
}

