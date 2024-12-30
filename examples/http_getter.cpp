// from chatgpt example

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include "Log.hh"
#include "HttpHelper.hh"


// string splitter
std::vector<std::string> split_string(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;

	// split
	while(std::getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}

// vector of servers
std::vector<HttpHelper::serverconfig> myservers;

int main() {

	// create log
	ShLogPr lg = Log::create();

	// add servers to list
	myservers.push_back({"10.0.0.89"});
	myservers.push_back({"10.0.0.175"});

	// clock

	while(true) {

		// timer
		auto start = std::chrono::steady_clock::now();


		// walk over servers
		for(int i = 0; i < myservers.size(); i++) {

			// error catch
			try {

				// start http connector
				boost::asio::io_context ioc;
				boost::asio::ip::tcp::resolver resolver(ioc);
				boost::beast::tcp_stream stream(ioc);

				// connect to server
				auto const results = resolver.resolve(myservers[i].name, "80");
				stream.connect(results);

				// start the get request
				boost::beast::http::request<boost::beast::http::string_body> req{
					boost::beast::http::verb::get, "/data", 11};
				req.set(boost::beast::http::field::host, myservers[i].name);
				req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

				boost::beast::http::write(stream, req);

				// get the result
				boost::beast::flat_buffer buffer;
				boost::beast::http::response<boost::beast::http::string_body> res;
				boost::beast::http::read(stream, buffer, res);

				// get the body
				std::string body = res.body();


				//	std::cout << body << std::endl;

				// parse body

				struct {
					float temp;
					float humi;
				} rdata;

				// message now is
				// Temperature [°F]: 75.02  |  Humidity [%]: 37.00
				// so split by | and then :
				std::vector<std::string> mparts = split_string(body, '|');
				assert(mparts.size() == 2);

				// walk string parts
				int cnt = 0;
				for(auto s : mparts) {
					std::vector<std::string> myreading;

					// split again for each sensor
					myreading = split_string(s, ':');
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
				// test output
				std::cout << lg->format("s#(%i)| T: %0.2f, H: %0.2f\n", i, rdata.temp, rdata.humi);


				// error code and exit
				boost::beast::error_code ec;
				stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
			} catch(std::exception& e) {
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}

		// check clock
		auto end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		// Sleep for the remainder of the second
		if(duration < std::chrono::milliseconds(1000)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000) - duration);
		}
	}

	return 0;
}
