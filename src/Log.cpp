// include header file
// mostly lifted from Van Nugteren
#include "Log.hh"

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// constructor
Log::Log(const LogoType logo) {}

// factory
ShLogPr Log::create(const LogoType logo) {
	return std::make_shared<Log>(logo);
}

// force set increment
void Log::set_num_indent(const int num_indent) {
	num_indent_ = num_indent;
}

// only change indent
void Log::msg(const int incr) {
	// lock for thread safety
	mtx_.lock();

	// increment indentation
	assert(static_cast<int>(num_indent_) >= -incr);
	if(static_cast<int>(num_indent_ >= -incr)) num_indent_ += incr;

	// unlock
	mtx_.unlock();
}

// new line
void Log::newl() {
	// lock for thread safety
	mtx_.lock();

	// enter
	std::printf(" \n");

	// unlock
	mtx_.unlock();
}

// access to indentation
int Log::get_num_indent() {
	return num_indent_;
}

// horizontal line
void Log::hline(const int width, const char ch, const std::string& str1, const std::string& str2) {
	if(width <= 0) return;
	msg("%s%c", str1.c_str(), ch);
	for(int i = 0; i < width - 1; i++) {
		msg(0, "%c", ch);
	}
	msg(0, "%s\n", str2.c_str());
}

std::string Log::format(const char* fmt, ...) {
	char* result = 0;
	va_list ap;
	va_start(ap, fmt);
	if(vasprintf(&result, fmt, ap) == -1) throw std::bad_alloc();
	va_end(ap);
	std::string str_result(result);
	free(result);
	return str_result;
}

std::vector<std::string> Log::split_string(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;

	// split
	while(std::getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}
