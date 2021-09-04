#include "config.h"

#include "pathfilter.hpp"

#include <cstdlib>
#include <cstring>

#include "libsad.hpp"
#include "pathresolver.hpp"
#include "path.hpp"

#define IS_SET(v, m) (((v) & (m)) == (m))

#define LABEL_FILTER_BY_DATE "by-date"
#define LABEL_FILTER_BY_TITLE "by-title"
#define LABEL_FILTER_BY_ACTIVITY "by-activity"
#define LABEL_FILTER_BY_ID ".by-id"

PathFilter::PathFilter(const char *path, const char *filter_part, path_type path_id, unsigned int filter_options) {
	this->path = path;
	this->filter_part = filter_part;
	this->path_id = path_id;
	this->filters_availible = filter_options;

	//this->applyFilters();
}

size_t PathFilter::getNextFilterLabel(const char *path, off_t *n) {
	size_t len = strlen(path);
	while ((path[*n] == '/') && ((*n) < len)) (*n)++;
	const char *ptr = strchr(path + (*n), '/');
	if (ptr == NULL) return (len - *n);
	else return (size_t)(ptr - (path + *n));
	return 1;
}

bool PathFilter::applyFilters() {
	// If only one filter is active, use that as base filter, do not offer the option to select (that one) base filter.
	if ((FILTER_BY_DATE == this->filters_availible) ||
		(FILTER_BY_ACTIVITY == this->filters_availible) ||
		(FILTER_BY_TITLE == this->filters_availible)) this->base_filter = (BaseFilter)this->filters_availible;

	off_t n = 0;
	if (this->base_filter == 0) {
		size_t len = getNextFilterLabel(this->filter_part, &n);
		if(len == strlen(LABEL_FILTER_BY_DATE) && (strncmp(this->filter_part + n, LABEL_FILTER_BY_DATE, len) == 0)) this->base_filter = FILTER_BY_DATE;
		else if(len == strlen(LABEL_FILTER_BY_TITLE) && (strncmp(this->filter_part + n, LABEL_FILTER_BY_TITLE, len) == 0)) this->base_filter = FILTER_BY_TITLE;
		else if(len == strlen(LABEL_FILTER_BY_ACTIVITY) && (strncmp(this->filter_part + n, LABEL_FILTER_BY_ACTIVITY, len) == 0)) this->base_filter = FILTER_BY_ACTIVITY;
		n += len;
	}
/*
	if (this->base_filter == FILTER_BY_ACTIVITY) {

	} else if  (this->base_filter == FILTER_BY_DATE) {
		while (this->filter_day == 0 || strlen(this->filter_part + n) == 0 || this->filtered_start_time != 0) {
			size_t len = getNextFilterLabel(this->filter_part, &n);
			char *ptr;
			if (this->filter_part[n] == '.') {
				char *date = strndup(this->filter_part + n + 1, len);
				struct tm t;
				char *ptr = strptime (date, "%Y-%m-%d %H:%M:%S UTC%z", &t);
				if (ptr == NULL || ((off_t)(ptr - this->filter_part) - n) != len) ...
			} else {
				long int i = strtol (this->filter_part + n, &ptr, 10);
				if (len == ((off_t)(ptr - this->filter_part) - n)) {
					if (this->filter_year == 0) this->filter_year = i;
					if (this->filter_month == 0) this->filter_month = i;
					if (this->filter_day == 0) this->filter_day = i;
				}
			}
			n += len;
		}

	}*/


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

int PathFilter::getNextFilters(Libsad *lib, PathResolver *pr, char **filters) {
	if (this->isComplete()) return 0;

	*filters = NULL;
	if (this->base_filter == FILTER_NONE) {
		int n = 0;
		if (IS_SET(this->filters_availible, FILTER_BY_DATE)) n++;
		if (IS_SET(this->filters_availible, FILTER_BY_ACTIVITY)) n++;
		if (IS_SET(this->filters_availible, FILTER_BY_TITLE)) n++;
		*filters = (char *)malloc (n * sizeof(char *));
		int ret = n;
		if (IS_SET(this->filters_availible, FILTER_BY_DATE)) filters[--n] = strdup(LABEL_FILTER_BY_DATE);
		if (IS_SET(this->filters_availible, FILTER_BY_ACTIVITY)) filters[--n] = strdup(LABEL_FILTER_BY_ACTIVITY);
		if (IS_SET(this->filters_availible, FILTER_BY_TITLE)) filters[--n] = strdup(LABEL_FILTER_BY_TITLE);
		return ret;
	}
	Libsad::FilterInfoType info;
	Libsad::FilterObjectType object = pr->getTargetObject(this->path_id);



	if (this->base_filter == FILTER_BY_DATE || this->base_filter == FILTER_BY_ACTIVITY) {
		if (this->base_filter == FILTER_BY_ACTIVITY && this->filter_activity == 0)  return lib->getFilteredList(object, Libsad::LIST_TYPE_ACTIVITIES, &info, filters);
		else if (IS_SET(this->filters_availible, FILTER_BY_DATE) && this->filter_year == 0) return lib->getFilteredList(object, Libsad::LIST_TYPE_YEARS, &info, filters);
		else if (IS_SET(this->filters_availible, FILTER_BY_DATE) && this->filter_month == 0) return lib->getFilteredList(object, Libsad::LIST_TYPE_MONTHS, &info, filters);
		else if (IS_SET(this->filters_availible, FILTER_BY_DATE) && this->filter_day == 0) return lib->getFilteredList(object, Libsad::LIST_TYPE_DAYS, &info, filters);
		else return lib->getFilteredList(object, Libsad::LIST_TYPE_TIME, &info, filters);
	} else if (this->base_filter == FILTER_BY_TITLE) {
		if (this->filter_title_first_letter == 0) return lib->getFilteredList(object, Libsad::LIST_TYPE_FIRST_LETTER_TITLES, &info, filters);
		else if (this->filter_title == NULL) return lib->getFilteredList(object, Libsad::LIST_TYPE_TITLES, &info, filters);
		else return lib->getFilteredList(object, Libsad::LIST_TYPE_TIME, &info, filters);
	}

	return 0;
}

#if 0
int PathFilter::getFilterResult() {
	/* TODO: convert this->filtered_start_time to an route_id
	 * return this->lib->getRouteId(this->filtered_start_time)
	 */
	return 0;
}
#endif

const bool PathFilter::isValid(unsigned int filter) {
	return ((filter > FILTER_NONE) && (filter < FILTER_LAST));
}
