#ifndef _X_RESOLVEDPATH_HPP_
#define _X_RESOLVEDPATH_HPP_

#include "pathfilter.hpp"
#include "path.hpp"
#include "libsad.hpp"

class ResolvedPath {
public:
	ResolvedPath(const char *path, path_type id, PathFilter *filter);
	~ResolvedPath(void);
	const path_type getPathId();
	const bool hasPathFilter();
	const PathFilter *getPathFilter();
	const char *getPath();
	const bool isValid();
	const bool hasObjectId();
	const bool hasObjectTime();
	const Libsad::objectId getObjectId();
	const Libsad::objectTime getObjectTime();

private:
	const char *path;
	path_type path_id;
	PathFilter *filter;
};

#endif /* _X_RESOLVEDPATH_HPP_ */
