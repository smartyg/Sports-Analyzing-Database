#include "config.h"

#include "exceptions.hpp"

#include <exception>
#include <string>

#include "resolvedpath.hpp"
#include "path.hpp"
#include "libsad.hpp"

using namespace std;

InvalidResolvedPath::InvalidResolvedPath(const char *file, const char *function, int line, const ResolvedPath *) {
	this->msg = std::string(__func__) + " exception at: " + std::string(file) + ":" + std::to_string(line) + " in function `" + std::string(function) + "`.\n";
}

InvalidResolvedPath::InvalidResolvedPath(const char *file, const char *function, int line, const ResolvedPath *, path_type) {
	this->msg = std::string(__func__) + " exception at: " + std::string(file) + ":" + std::to_string(line) + " in function `" + std::string(function) + "`.\n";
}

InvalidPath::InvalidPath(const char *file, const char *function, int line, const char *path) {
	this->msg = std::string(__func__) + " exception at: " + std::string(file) + ":" + std::to_string(line) + " in function `" + std::string(function) + "`: Path `" + std::string(path) + "` is not valid.\n";
}

InvalidFilter::InvalidFilter(const char *file, const char *function, int line, Libsad::FilterInfoType fi) {
	this->msg = std::string(__func__) + " exception at: " + std::string(file) + ":" + std::to_string(line) + " in function `" + std::string(function) + "`.\n";
	this->msg += "Filter so far:\nyear: " + std::to_string(fi.filter_year) + "\nmonth: " + std::to_string(fi.filter_month) + "\nday: " + std::to_string(fi.filter_day) + "\n";
	this->msg += "hour: " + std::to_string(fi.filter_hour) + "\nminute: " + std::to_string(fi.filter_minute) + "\nsecond: " + std::to_string(fi.filter_second) + "\n";
	this->msg += "activity: " + std::to_string(fi.filter_activity) + "\nfirst letter: " + std::to_string(fi.filter_title_first_letter) + "\ntitle: " + std::string(fi.filter_title) + "\n";
	this->msg += "object: " + std::to_string(fi.object) + "\n";
}
