#ifndef _X_PATHFILTER_HPP_
#define _X_PATHFILTER_HPP_

#include "libsad.hpp"
#include "pathresolver.hpp"
#include "fuse.h"

class PathFilter {
public:
	typedef enum {
		FILTER_NONE = 0x0,
		FILTER_BY_DATE = 0x1,
		FILTER_BY_TITLE = 0x2,
		FILTER_BY_ACTIVITY = 0x4,
		FILTER_LAST = 0xf
	} BaseFilter;

	PathFilter(Libsad *lib, const char *path, const char *filter_part, PathResolver::PathDefinition *path_definition);
	const PathFilter::BaseFilter getBaseFilter();
	const char *getPath();
	const unsigned int getAvailibleFilters();
	bool isComplete();
	int fillReaddir(void *buf, fuse_fill_dir_t filler);
	int getFilterResult();
	static bool isValid(unsigned int filter);
};

#endif /* _X_PATHFILTER_HPP_ */