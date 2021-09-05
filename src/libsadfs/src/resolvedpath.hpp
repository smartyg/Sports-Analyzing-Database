#ifndef _X_RESOLVEDPATH_HPP_
#define _X_RESOLVEDPATH_HPP_

class ResolvedPath;

#include <cstdint>

#include "path.hpp"
#include "pathresolver.hpp"
#include "libsad.hpp"

class ResolvedPath {
public:
	ResolvedPath(const char *path, path_type id);
	ResolvedPath(const char *path, path_type id, const PathResolver::BaseFilter base_filter, const Libsad::FilterInfoType filter_info);
	ResolvedPath(const char *base_path, const char *filename, path_type id);
	ResolvedPath(const char *base_path, const char *filename, path_type id, const PathResolver::BaseFilter base_filter, const Libsad::FilterInfoType filter_info);
	ResolvedPath(const char *base_path, const char *filename, const char *extention, path_type id);
	ResolvedPath(const char *base_path, const char *filename, const char *extention, path_type id, const PathResolver::BaseFilter base_filter, const Libsad::FilterInfoType filter_info);
	~ResolvedPath(void);

	/* Getters */
	const path_type getPathId(void);
	const char *getPath(void);
	const char *getFilename(void);
	const PathResolver::BaseFilter getPathFilterBase(void);
	const Libsad::FilterInfoType getPathFilterInfo(void);
	const Libsad::objectId getObjectId(void);
	const Libsad::objectTime getObjectTime(void);

	/* Booleans */
	const bool hasPathFilterInfo(void);
	const bool hasObjectId(void);
	const bool hasObjectTime(void);

	/* Checker */
	const bool isValid(void);

private:
	const char *path;
	uint_least32_t path_id:8, base_filter:8, filename_offset:16;
	Libsad::FilterInfoType filter_info;
};

#endif /* _X_RESOLVEDPATH_HPP_ */
