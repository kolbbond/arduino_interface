// from chatgpt example

// cpp includes
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

// externals
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <json/json.h>

// internals
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

int main(int argc, char** argv) {

	// create log
	ShLogPr lg = Log::create();

	// Open and parse JSON file
	std::ifstream file("../examples/servers.json", std::ifstream::binary);
	if(!file.is_open()) {
		lg->msg("%sFile not open%s\n", KRED, KNRM);
		return 1;
	}

	Json::Value root;
	file >> root;

	// Access servers array
	const Json::Value servers = root["servers"];
	for(const auto& server : servers) {
		// new server config
		HttpHelper::serverconfig myserver;
		std::string name = server["name"].asString();
		std::string ip = server["ip"].asString();
		int port = server["port"].asInt();
		std::string page = server["page"].asString();
		std::string location = server["location"].asString();

		// log
		std::cout << "--- new server --- \n";
		std::cout << "name: " << name << "\n";
		std::cout << "ip: " << ip << "\n";
		std::cout << "port: " << port << "\n";
		std::cout << "page: " << page << "\n";
		std::cout << "location: " << location << "\n";

		// add struct to vector
		myserver.name = name;
		myserver.ip = ip;
		myserver.port = port;
		myserver.page = page;
		myserver.location = location;

		myservers.push_back(myserver);
	}

	// clock

	while(true) {

		// timer
		auto start = std::chrono::steady_clock::now();


		// walk over servers
		for(int i = 0; i < myservers.size(); i++) {

			HttpHelper::serverconfig myserver = myservers[i];
			// error catch
			try {

				// start http connector
				boost::asio::io_context ioc;
				boost::asio::ip::tcp::resolver resolver(ioc);
				boost::beast::tcp_stream stream(ioc);

				// connect to server
				auto const results = resolver.resolve(myserver.ip, std::to_string(myserver.port));
				stream.connect(results);

				// start the get request
				boost::beast::http::request<boost::beast::http::string_body> req{
					boost::beast::http::verb::get, "/" + myserver.page, 11};
				req.set(boost::beast::http::field::host, myserver.ip);
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
				// @hey: maybe change to... a json? idk A BETTER FORMAT
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
				lg->msg("%sERROR ERROR%s\n", KRED, KNRM);
				std::cerr << "Error: " << e.what() << std::endl;
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
