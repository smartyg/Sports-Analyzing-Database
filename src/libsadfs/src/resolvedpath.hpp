#ifndef _X_RESOLVEDPATH_HPP_
#define _X_RESOLVEDPATH_HPP_

class ResolvedPath;

#include "pathfilter.hpp"
#include "path.hpp"
#include "libsad.hpp"

class ResolvedPath {
public:
	ResolvedPath(const char *path, path_type id, PathFilter *filter);
	ResolvedPath(const char *base_path, const char *filename, path_type id, PathFilter *filter);
	ResolvedPath(const char *base_path, const char *filename, const char *extention, path_type id, PathFilter *filter);
	~ResolvedPath(void);

	const path_type getPathId(void);
	const bool hasPathFilter(void);
	PathFilter *getPathFilter(void);
	const char *getPath(void);
	const char *getFilename(void);
	const bool isValid(void);
	const bool hasObjectId(void);
	const bool hasObjectTime(void);
	const Libsad::objectId getObjectId(void);
	const Libsad::objectTime getObjectTime(void);

private:
	const char *path;
	off_t filename_offset;
	path_type path_id;
	PathFilter *filter;
};

#endif /* _X_RESOLVEDPATH_HPP_ */
