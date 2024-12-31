//

// HttpHelper.hh

#pragma once

#include <memory>
#include <string>

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
	bool connected = false; // if we are connected
};
    
	// methods
public:
	// constructor
	HttpHelper();

	// destructor
	~HttpHelper();

	// factory
	static ShHttpHelperPr create();


};
