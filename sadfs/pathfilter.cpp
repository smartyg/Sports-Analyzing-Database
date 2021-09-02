#include "libsad.hpp"
#include "pathresolver.hpp"

class PathFilter {
public:
	PathFilter(Libsad *lib, const char *path, const char *filter_part, PathResolver::PathDefinition *path_definition) {
		this->lib = lib;
		this->path = path;
		this->filter_part = filter_part;
		this->pd = path_definition;
		this->filters_availible = path_definition->filter;
	}

	const PathFilter::BaseFilter getBaseFilter() {
		return this->base_filter;
	}
	const char *getPath() {
		return this->path;
	}

	const unsigned int getAvailibleFilters() {
		this->filters_availible;
	}

	bool isComplete() {
		if ((this->base_filter == FILTER_BY_DATE) && (this->filtered_start_time > -1)) return true;
		if ((this->base_filter == FILTER_BY_TITLE) && (this->filter_title != NULL) && (this->filtered_start_time > -1)) return true;
		if ((this->base_filter == FILTER_BY_ACTIVITY) && (this->filter_activity > 0) && (this->filtered_start_time > -1)) return true;
		return false;
	}

	int fillReaddir(void *buf, fuse_fill_dir_t filler) {
		if (this->isComplete()) return -2;

		filler(buf, entry_name, NULL, 0);
		return 0;
	}

	int getFilterResult() {
		/* TODO: convert this->filtered_start_time to an route_id
		 * return this->lib->getRouteId(this->filtered_start_time)
		 */
		return 0;
	}

	static bool isValid(unsigned int filter) {
		return ((filter > FILTER_NONE) && (filter < FILTER_LAST));
	}

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
	PathResolver::PathDefinition *pd;
	Libsad *lib;
};