#ifndef _X_EXCEPTIONS_HPP_
#define _X_EXCEPTIONS_HPP_

#include <exception>
#include <string>

#include "resolvedpath.hpp"
#include "path.hpp"
#include "libsad.hpp"

using namespace std;

class SadExceptions : public std::exception {};

class InvalidResolvedPath : public SadExceptions
{
public:
	InvalidResolvedPath(const char *file, const char *function, int line, const ResolvedPath *rp);
	InvalidResolvedPath(const char *file, const char *function, int line, const ResolvedPath *rp, path_type);

	const char* what() const noexcept override {
		return this->msg.c_str();
	}

private:
	std::string msg;
};


class InvalidPath : public SadExceptions
{
public:
	InvalidPath(const char *file, const char *function, int line, const char *path);

	const char* what() const noexcept override {
		return this->msg.c_str();
	}

private:
	std::string msg;
};

class InvalidFilter : public SadExceptions
{
public:
	InvalidFilter(const char *file, const char *function, int line, Libsad::FilterInfoType);

	const char* what() const noexcept override {
		return this->msg.c_str();
	}

private:
	std::string msg;
};

#endif /* _X_EXCEPTIONS_HPP_ */
