#include "config.h"

#include "exceptions.hpp"

#include <exception>
//#include <iostream>
#include <cstring>
#include <string>

#include "resolvedpath.hpp"
#include "path.hpp"

using namespace std;

InvalidResolvedPath::InvalidResolvedPath(const char *file, const char *function, int line, ResolvedPath *rp) {
	this->msg = std::string(__func__) + " exception at: " + std::string(file) + ":" + std::to_string(line) + " in function `" + std::string(function) + "`.";
}

InvalidResolvedPath::InvalidResolvedPath(const char *file, const char *function, int line, ResolvedPath *rp, path_type) {
	this->msg = std::string(__func__) + " exception at: " + std::string(file) + ":" + std::to_string(line) + " in function `" + std::string(function) + "`.";
}

InvalidPath::InvalidPath(const char *file, const char *function, int line, const char *path) {
	this->msg = std::string(__func__) + " exception at: " + std::string(file) + ":" + std::to_string(line) + " in function `" + std::string(function) + "`: Path `" + std::string(path) + "` is not valid.";
}

InvalidFilter::InvalidFilter(const char *file, const char *function, int line) {
	this->msg = std::string(__func__) + " exception at: " + std::string(file) + ":" + std::to_string(line) + " in function `" + std::string(function) + "`.";
}
