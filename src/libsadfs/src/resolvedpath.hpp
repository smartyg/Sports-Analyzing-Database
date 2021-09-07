#ifndef _X_RESOLVEDPATH_HPP_
#define _X_RESOLVEDPATH_HPP_

class ResolvedPath;

#include <cstdint>

#include "path.hpp"
#include "pathresolver.hpp"
#include "libsad.hpp"

class ResolvedPath {
public:
	ResolvedPath (const char *, path_type);
	ResolvedPath (const char *, path_type, const bool, const PathResolver::BaseFilter, const Libsad::FilterInfoType);
	ResolvedPath (const char *, const char *, path_type);
	ResolvedPath (const char *, const char *, path_type, const bool, const PathResolver::BaseFilter, const Libsad::FilterInfoType);
	ResolvedPath (const char *, const char *, const char *, path_type);
	ResolvedPath (const char *, const char *, const char *, path_type, const bool, const PathResolver::BaseFilter, const Libsad::FilterInfoType);
	~ResolvedPath (void);

	/* Getters */
	path_type getPathId (void) const;
	const char *getPath (void) const;
	const char *getFilename (void) const;
	PathResolver::BaseFilter getPathFilterBase (void) const;
	const Libsad::FilterInfoType getPathFilterInfo (void) const;
	Libsad::objectId getObjectId (void) const;
	Libsad::objectTime getObjectTime (void) const;

	/* Booleans */
	bool hasFilter (void) const;
	bool isFilterComplete (void) const;
	bool hasObjectId (void) const;
	bool hasObjectTime (void) const;

	/* Checker */
	bool isValid (void) const;

private:
	const char *path;
	uint_least32_t path_id:8, has_filter:1, base_filter:7, filename_offset:16;
	Libsad::FilterInfoType filter_info;
};

#endif /* _X_RESOLVEDPATH_HPP_ */
