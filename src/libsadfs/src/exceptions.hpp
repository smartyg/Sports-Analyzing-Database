#ifndef _X_EXCEPTIONS_HPP_
#define _X_EXCEPTIONS_HPP_

#include <exception>
#include <string>

#include "resolvedpath.hpp"
#include "path.hpp"

using namespace std;

class InvalidResolvedPath : public std::exception
{
public:
	InvalidResolvedPath(const char *file, const char *function, int line, ResolvedPath *rp);
	InvalidResolvedPath(const char *file, const char *function, int line, ResolvedPath *rp, path_type);

	const char* what() const noexcept override {
		return this->msg.c_str();
	}

private:
	std::string msg;
};


class InvalidPath : public std::exception
{
public:
	InvalidPath(const char *file, const char *function, int line, const char *path);

	const char* what() const noexcept override {
		return this->msg.c_str();
	}

private:
	std::string msg;
};

class InvalidFilter : public std::exception
{
public:
	InvalidFilter(const char *file, const char *function, int line);

	const char* what() const noexcept override {
		return this->msg.c_str();
	}

private:
	std::string msg;
};

#endif /* _X_EXCEPTIONS_HPP_ */
