#ifndef _X_PATHFILTER_HPP_
#define _X_PATHFILTER_HPP_

class PathFilter;

#include "path.hpp"
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

	PathFilter(const char *path, const char *filter_part, path_type path_id, unsigned int filter_options);
	const PathFilter::BaseFilter getBaseFilter(void);
	const char *getPath(void);
	const unsigned int getAvailibleFilters(void);
	const path_type getPathType(void);
	const bool isComplete(void);
	static const bool isValid(unsigned int filter);
	int getNextFilters(Libsad *lib, PathResolver *pr, char **filters);

private:
	BaseFilter base_filter;
	unsigned int filter_year:12;
	unsigned int filter_month:4;
	unsigned int filter_day:5;
	unsigned int filter_time:18;
	unsigned int filter_activity:8;
	unsigned int filter_title_first_letter:8;
	unsigned long int filtered_start_time:42;
	const char *filter_title;

	unsigned int filters_availible;
	const char *path;
	const char *filter_part;
	path_type path_id;

	bool applyFilters(void);
	size_t getNextFilterLabel(const char *, off_t *n);
};

#endif /* _X_PATHFILTER_HPP_ */