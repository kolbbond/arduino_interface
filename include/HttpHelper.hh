//

// HttpHelper.hh

#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <fstream>

#include <boost/beast.hpp>
#include <boost/asio.hpp>

#include <json/json.h>

#include "Log.hh"

typedef std::shared_ptr<class HttpHelper> ShHttpHelperPr;

class HttpHelper {

	// properties
protected:
public:
	// need server names
	struct serverconfig {
		std::string name; // "name of server"
		std::string ip; // "10.0.0.175" for example
		int port; // probably 80
		std::string page; // after the name /data for example
		std::string location; // where it is, kinda useless
        std::string sensor;
        std::string tag;
		bool connected = false; // if we are connected
	};

	Json::Value _root;

	std::vector<serverconfig> _servers;

	// methods
public:
	// constructor
	HttpHelper() {};

	// destructor
	~HttpHelper() {};

	// factory
	static ShHttpHelperPr create() {
		return std::make_shared<HttpHelper>();
	}

	// load json server file
	int load_json(const std::string& fname, const ShLogPr& lg = NullLog::create()) {

		// Open and parse JSON file
		std::ifstream file("../examples/servers.json", std::ifstream::binary);
		if(!file.is_open()) {
			lg->msg("%sFile not open%s\n", KRED, KNRM);
			return 1;
		}

		//Json::Value root;
		file >> _root;
		return 0; // success
	}

	// overload
	void load_servers(std::string fname, const ShLogPr& lg = NullLog::create()) {
		load_json(fname, lg);
		load_servers();
	}

	// load servers from json server file
	void load_servers() {
		// check json root is loaded
		//assert(_root != NULL);

		// Access servers array
		const Json::Value servers = _root["servers"];
		for(const auto& server : servers) {
			// new server config
			HttpHelper::serverconfig myserver;
			std::string name = server["name"].asString();
			std::string ip = server["ip"].asString();
			int port = server["port"].asInt();
			std::string page = server["page"].asString();
			std::string location = server["location"].asString();
			std::string sensor = server["sensor"].asString();
			std::string tag = server["tag"].asString();

			// log
			std::cout << "--- new server --- \n";
			std::cout << "name: " << name << "\n";
			std::cout << "ip: " << ip << "\n";
			std::cout << "port: " << port << "\n";
			std::cout << "page: " << page << "\n";
			std::cout << "location: " << location << "\n";
			std::cout << "sensor: " << sensor << "\n";
			std::cout << "tag: " << tag << "\n";

			// add struct to vector
			myserver.name = name;
			myserver.ip = ip;
			myserver.port = port;
			myserver.page = page;
			myserver.location = location;
			myserver.sensor = sensor;
			myserver.tag = tag;

			// add to our servers
			_servers.push_back(myserver);
		}
	}

	// statics

	static std::string make_request(serverconfig myserver) {
		return make_request(myserver.ip, myserver.port, myserver.page);
	}

	static std::string make_request(std::string ip, int port, std::string page) {


		// check ip is valid
		if(!is_valid_ip(ip)) {
			throw("bad ip");
		}

		// output string
		std::string body;

		// start http connector
		boost::asio::io_context ioc;
		boost::asio::ip::tcp::resolver resolver(ioc);
		boost::beast::tcp_stream stream(ioc);

		// connect to server
		auto const results = resolver.resolve(ip, std::to_string(port));
		stream.connect(results);

		// start the get request
		boost::beast::http::request<boost::beast::http::string_body> req{boost::beast::http::verb::get, "/" + page, 11};
		req.set(boost::beast::http::field::host, ip);
		req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		boost::beast::http::write(stream, req);

		// get the result
		boost::beast::flat_buffer buffer;
		boost::beast::http::response<boost::beast::http::string_body> res;
		boost::beast::http::read(stream, buffer, res);

		// debug
//		std::cout << "req: " << req << std::endl;
//		std::cout << "res: " << res << std::endl;

		// get result body
		body = res.body();

		// error code and exit
		boost::beast::error_code ec;
		boost::beast::error_code ec2;
		ec = stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec2);

		// handle errors
		if(ec.failed()) {
			std::cerr << "Error: " << ec.message() << " (code: " << ec.value() << ")" << std::endl;
			// Handle the error here
		} else {
			// no error so we are good!
			//std::cout << "Operation succeeded." << std::endl;
		}

		return body;
	}

	// check ip address is valid
	// must have 3 dots
	static bool is_valid_ip(std::string ip) {
		int dcnt = 0;
		for(auto c : ip) {
			if(c == '.') dcnt++;
		}

		return (dcnt == 3) ? true : false;
	}
};
