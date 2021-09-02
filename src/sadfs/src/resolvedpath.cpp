#include "config.h"

#include "resolvedpath.hpp"

#include "pathresolver.hpp"
#include "pathfilter.hpp"
#include "path.hpp"

ResolvedPath::ResolvedPath(const char *path, path_type path_id, PathFilter *filter)
	{
		this->path = path;
		this->path_id = path_id;
		this->filter = filter;
	}

ResolvedPath::~ResolvedPath(void)
	{
		delete this->filter;
	}

	const path_type ResolvedPath::getPathId() {
		this->path_id;
	}

	const bool ResolvedPath::hasPathFilter() {
		return ((this->filter != NULL) && (this->filter->getPathType() == this->path_id) && (this->filter->isValid(this->filter->getAvailibleFilters())));
	}

	const PathFilter *ResolvedPath::getPathFilter() {
		this->filter;
	}

	const char *ResolvedPath::getPath() {
		this->path;
	}

	const bool ResolvedPath::isValid() {
		return ((this->path_id > PATH_TYPE_NONE) && (this->path_id < PATH_TYPE_LAST));
	}
