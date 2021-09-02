#ifndef _X_PATHFILTER_HPP_
#define _X_PATHFILTER_HPP_

#include "path.hpp"
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
	const PathFilter::BaseFilter getBaseFilter();
	const char *getPath();
	const unsigned int getAvailibleFilters();
	const path_type getPathType();
	const bool isComplete();
	int fillReaddir(void *buf, fuse_fill_dir_t filler);
	int getFilterResult();
	static const bool isValid(unsigned int filter);

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
};

#endif /* _X_PATHFILTER_HPP_ */