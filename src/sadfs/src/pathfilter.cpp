#include "config.h"

#include "pathfilter.hpp"

#include "libsad.hpp"
#include "path.hpp"

PathFilter::PathFilter(const char *path, const char *filter_part, path_type path_id, unsigned int filter_options) {
		this->path = path;
		this->filter_part = filter_part;
		this->path_id = path_id;
		this->filters_availible = filter_options;
	}

	const PathFilter::BaseFilter PathFilter::getBaseFilter() {
		return this->base_filter;
	}
	const char *PathFilter::getPath() {
		return this->path;
	}

	const unsigned int PathFilter::getAvailibleFilters() {
		this->filters_availible;
	}

	const path_type PathFilter::getPathType() {
		return this->path_id;
	}

	const bool PathFilter::isComplete() {
		if ((this->base_filter == FILTER_BY_DATE) && (this->filtered_start_time > -1)) return true;
		if ((this->base_filter == FILTER_BY_TITLE) && (this->filter_title != NULL) && (this->filtered_start_time > -1)) return true;
		if ((this->base_filter == FILTER_BY_ACTIVITY) && (this->filter_activity > 0) && (this->filtered_start_time > -1)) return true;
		return false;
	}

	int PathFilter::fillReaddir(void *buf, fuse_fill_dir_t filler) {
		if (this->isComplete()) return -2;

		//filler(buf, entry_name, NULL, 0);
		return 0;
	}

	int PathFilter::getFilterResult() {
		/* TODO: convert this->filtered_start_time to an route_id
		 * return this->lib->getRouteId(this->filtered_start_time)
		 */
		return 0;
	}

	const bool PathFilter::isValid(unsigned int filter) {
		return ((filter > FILTER_NONE) && (filter < FILTER_LAST));
	}
