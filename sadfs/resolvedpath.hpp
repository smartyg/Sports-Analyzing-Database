#ifndef _X_RESOLVEDPATH_HPP_
#define _X_RESOLVEDPATH_HPP_

#include "pathresolver.hpp"
#include "pathfilter.hpp"
#include "path.hpp"

class ResolvedPath {
public:
	ResolvedPath(const char *path, path_type id, PathFilter *filter, PathResolver::PathDefinition *id_path_definition);
	path_type getPathId();
	bool hasPathFilter();
	PathFilter *getPathFilter();
	const char *getPath();
	const bool isValid();
};

#endif /* _X_RESOLVEDPATH_HPP_ */
