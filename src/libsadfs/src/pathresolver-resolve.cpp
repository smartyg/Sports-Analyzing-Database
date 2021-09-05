#include "config.h"

#include "pathresolver.hpp"

#include <stddef.h>
#include <cstdlib>
#include <memory>
#include <cstring>

#include "resolvedpath.hpp"
#include "exceptions.hpp"
#include "libsadplugin.hpp"

#define HAS_SINGLE_FILTER_BIT_SET(m) (((m) == FILTER_BY_DATE) || ((m) == FILTER_BY_ACTIVITY) || ((m) == FILTER_BY_TITLE) || ((m) == FILTER_BY_ID))

uint_fast8_t pathToInt (const char *, size_t);
uint_fast32_t pathToTime (const char *, size_t);
uint_fast64_t pathToDate (const char *, size_t);
size_t getNextFilterLabel(const char *, size_t *);
bool isFilterComplete(const Libsad::FilterInfoType);
PathResolver::BaseFilter matchBaseFilter(const char *, size_t);

ResolvedPath *PathResolver::resolve(const char *path) {
	fprintf(stderr, "PathResolver::%s(%s)\n", __func__, path);
	Libsad::FilterInfoType filter_info = {0};
	return this->resolveInternal(path, 0, PATH_TYPE_DIR_ROOT, FILTER_NONE, &filter_info);
}

// Look if given record is a child of parent (or if it is the parent itself).
const bool PathResolver::isFromParent(PathDefinition *p, path_type parent_id) {
	fprintf(stderr, "PathResolver::%s(%p, %d)\n", __func__, p, parent_id);
	if (p == NULL) return false;

	for (PathDefinition *pd = p; pd->parent_id != PATH_TYPE_NONE; pd = this->path_definitions + pd->parent_id) {
		// Check both current id and parent id at once.
		if ((path_type)p->id == parent_id) return true;
		if ((path_type)p->parent_id == parent_id) return true;
	}
	return false;
}

ResolvedPath *PathResolver::resolveInternal(const char *path, size_t start_pos, path_type parent_id, BaseFilter base_filter, Libsad::FilterInfoType *filter_info) {
	fprintf(stderr, "PathResolver::%s(%s, %d, %d, %d)\n", __FUNCTION__, path, start_pos, parent_id, base_filter);
	ResolvedPath *rp = NULL;

	// Store a matched filtered path in here.
	PathDefinition *fpd = NULL;

	// Store the length of the filtered patch match.
	size_t match_len = 0;

	// First we are looking for a full match. But if we find a partial (filter entry) match, save the one with the longest matching path.
	for(PathResolver::PathDefinition *p = this->path_definitions; p->id != PATH_TYPE_LAST; p++) {
		// Only check if this type sets a path and if it is (a child of) the parent.
		if (p->full_path != NULL && this->isFromParent(p, parent_id)) {
			fprintf(stderr, "PathResolver::%s: check record %d: name: %s; path: %s; parent_id: %d; mode %x; file handler: %d; filters: %d; filter object: %d\n", __FUNCTION__, p->id, p->name, p->full_path, p->parent_id, p->mode, p->data_file_handler, p->filters, p->filter_object);
			if (strcmp(path + start_pos, p->full_path) == 0) {
				// We have a full match with the path.
				// Check if this directory has a filter, if so, initialize the filter.
				//Libsad::FilterInfoType filter_info;
				//BaseFilter base_filter = FILTER_NONE;
				//filter_info
				/* TODO: we need to save the filtered result as we now encoutered a new filter dir. */
				base_filter = FILTER_NONE;
				if (p->filters != FILTER_NONE) {
					if (HAS_SINGLE_FILTER_BIT_SET(p->filters)) base_filter = (BaseFilter)p->filters;
					rp = new ResolvedPath(path, (path_type)p->id, base_filter, *filter_info);
				} else rp = new ResolvedPath(path, (path_type)p->id, base_filter, *filter_info);
				// We are done, path is fullly resolved.
				break;
			} else if ((strncmp(path + start_pos, p->full_path, p->full_path_len) == 0) &&
				       p->data_file_handler != LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE &&
				       (this->lib->getPluginHandler()->isExtentionMatch(path + start_pos + p->full_path_len, (LibsadPlugin::DataFileHandlerType)p->data_file_handler) == true)) {
				// We found a full match on a file with a data file handler set.
				rp = new ResolvedPath(path, (path_type)p->id, base_filter, *filter_info);
				// Path fully resolved.
				break;
			} else if ((strncmp(path + start_pos, p->full_path, p->full_path_len) == 0) && p->filters != FILTER_NONE) {
				// The first part of the path is a match with a filter entry. Check if this is the longest partial (filter entry) match so far.
				if (p->full_path_len > match_len) {
					// Longest partial match so far, save it.
					fpd = p;
					match_len = p->full_path_len;
				}
			}
		}
	}

	// Check if ResolvedPath is really set and good.
	if (rp != NULL && rp->isValid()) return rp;

	// We did not find a full match, check if we have a partial match.
	if (match_len > 0 && fpd != NULL) {
		// If this directory has only 1 filter, already set it.
		if (HAS_SINGLE_FILTER_BIT_SET(fpd->filters)) base_filter = (BaseFilter)fpd->filters;

		size_t n = start_pos + fpd->full_path_len;
		size_t len;

		// Loop over all the remaining parts of the path, see how much we can match.
		while ((len = getNextFilterLabel(path, &n)) > 0 || isFilterComplete(*filter_info)) {

			if (base_filter == FILTER_NONE) {
				base_filter = matchBaseFilter(path + n, len);

				// matchBaseFilter did not match, so we can not resolve this path.
				if (base_filter == FILTER_NONE) throw InvalidPath(__FILE__, __func__, __LINE__, path);
			} else if (base_filter == FILTER_BY_ACTIVITY || base_filter == FILTER_BY_DATE || base_filter == FILTER_BY_TITLE) {
				if (base_filter == FILTER_BY_ACTIVITY && filter_info->filter_activity == 0) {
					if ((filter_info->filter_activity = this->lib->getActivityCodeFromName(path + n, len - 1)) == 0) throw InvalidPath(__FILE__, __func__, __LINE__, path); // We want to filter by activity, but next part does not contain an activity.
				} else if ((base_filter == FILTER_BY_ACTIVITY && filter_info->filter_activity != 0 && IS_SET(fpd->filters, FILTER_BY_DATE)) || base_filter == FILTER_BY_DATE) {
					if (filter_info->filter_year == 0 && (path + n)[0] == '.') {
						if ((filter_info->object = pathToDate(path + n, len)) == 0) throw InvalidPath(__FILE__, __func__, __LINE__, path);
						filter_info->object_type = TYPE_OBJECT_TIME;
					} else if (filter_info->filter_year == 0) {
						if ((filter_info->filter_year = pathToInt(path + n, len)) == 0) throw InvalidPath(__FILE__, __func__, __LINE__, path);
					} else if (filter_info->filter_month == 0) {
						if ((filter_info->filter_month = pathToInt(path + n, len)) == 0) throw InvalidPath(__FILE__, __func__, __LINE__, path);
					} else if (filter_info->filter_day == 0) {
						if ((filter_info->filter_day = pathToInt(path + n, len)) == 0) throw InvalidPath(__FILE__, __func__, __LINE__, path);
					} else if (filter_info->filter_time == 0) {
						if ((filter_info->filter_time = pathToTime(path + n, len)) == 0) throw InvalidPath(__FILE__, __func__, __LINE__, path);
					} else throw InvalidFilter(__FILE__, __func__, __LINE__); // We say there is no complete filter, but all fields are already set, something is wrong.
				} else if ((base_filter == FILTER_BY_ACTIVITY && filter_info->filter_activity != 0 && IS_SET(fpd->filters, FILTER_BY_TITLE)) || base_filter == FILTER_BY_TITLE) {
					if (filter_info->filter_title_first_letter == 0 && len == 1) filter_info->filter_title_first_letter = (path + n)[0];
					else if (filter_info->filter_title_first_letter == 0 && (path + n)[0] == '.') filter_info->filter_title = strndup(path + n + 1, len - 1);
					else if (filter_info->filter_title_first_letter == 0 && filter_info->filter_title == NULL) throw InvalidPath (__FILE__, __func__, __LINE__, path); // We want to filter on title, but the next entry is neither a single character or a dot (full activity title) directory. Something went wrong.
					else if (filter_info->filter_title_first_letter != 0 && filter_info->filter_title == NULL) filter_info->filter_title = strndup(path + n, len);
					else if (filter_info->filter_title != NULL) {
						if ((filter_info->object = pathToDate(path + n, len)) == 0) throw InvalidPath(__FILE__, __func__, __LINE__, path); // We filter by title, there is a title set and no complete filter, so the next part must contain a date, if not something is wrong.
						filter_info->object_type = TYPE_OBJECT_TIME;
					}
				}
			} else if (base_filter == FILTER_BY_ID) {
				if ((filter_info->object = pathToInt(path + n, len)) == 0) throw InvalidPath(__FILE__, __func__, __LINE__, path); // We want to filter by id, but the next part is not a id. Something is wrong.
			} else throw InvalidFilter(__FILE__, __func__, __LINE__); // Base filter has a value that we do not know about.

			// Increase n so next run it will look for a path starting at the end of this part.
			n += len;
		}

		if (isFilterComplete(*filter_info)) {
			// Filter is complete, look for the child which defines the result.
			for(PathResolver::PathDefinition *p = this->path_definitions; p->id != PATH_TYPE_LAST; p++) {
				// Only check dynamic entries, as we look for and entry to hold the complete filter result.
				if (p->name == NULL && p->parent_id == fpd->id) {
					// place the found match in *fpd.
					fpd = p;
					break;
				}
			}
		}

		if (n == strlen(path)) {
			// We consumed the whole path, so construct the ResolvedPath and return.
			rp = new ResolvedPath(path, (path_type)fpd->id, base_filter, *filter_info);
			return rp;
		} else if (isFilterComplete(*filter_info)) {
			// Filter is complete, but there is still path left, do another loop and return its result.
			return this->resolveInternal(path, n, (path_type)fpd->id, base_filter, filter_info);
		} else throw InvalidPath(__FILE__, __func__, __LINE__, path); // We are not suppose to be here, there is still path left and not a complete filter (this should have been tackled earlier in code.
	}

	// Something is not right, check if we have a ResolvedPath.
	if (rp != NULL) {
		throw InvalidResolvedPath(__FILE__, __func__, __LINE__, rp);
		delete rp;
	}
	throw InvalidPath(__FILE__, __func__, __LINE__, path);
	return NULL;
}

bool isFilterComplete(const Libsad::FilterInfoType fi) {
	if (fi.object_type != TYPE_OBJECT_EMPTY && fi.object > 0) return true;
	if (fi.filter_year > 0 && fi.filter_month > 0 && fi.filter_day > 0 && fi.filter_time > 0) return true;
	else return false;
}

uint_fast32_t pathToTime (const char *path, size_t len) {
	struct tm t;
	char *ptr = strptime (path, "%H:%M:%S UTC%z", &t);
	if (ptr != path + len) return 0;
	uint_fast32_t i = (mktime(&t) - t.tm_gmtoff) * 1000;
}

uint_fast64_t pathToDate (const char *path, size_t len) {
	struct tm t;
	char *ptr = strptime (path, "%Y-%m-%d %H:%M:%S UTC%z", &t);
	if (ptr != path + len) return 0;
	uint_fast64_t i = (mktime(&t) - t.tm_gmtoff) * 1000;
}

uint_fast8_t pathToInt (const char *path, size_t len) {
	char *ptr = NULL;
	long int i = strtol (path, &ptr, 10);
	if (ptr != path + len) return 0;
	return (uint_fast8_t)i;
}


PathResolver::BaseFilter matchBaseFilter(const char *path, size_t len) {
	fprintf(stderr, "%s(%s, %d)\n", __func__, path, len);
	if ((len == strlen(LABEL_FILTER_BY_DATE)) && (strncmp(path, LABEL_FILTER_BY_DATE, len) == 0)) return PathResolver::FILTER_BY_DATE;
	else if ((len == strlen(LABEL_FILTER_BY_TITLE)) && (strncmp(path, LABEL_FILTER_BY_TITLE, len) == 0)) return PathResolver::FILTER_BY_TITLE;
	else if ((len == strlen(LABEL_FILTER_BY_ACTIVITY)) && (strncmp(path, LABEL_FILTER_BY_ACTIVITY, len) == 0)) return PathResolver::FILTER_BY_ACTIVITY;
	else if ((len == strlen(LABEL_FILTER_BY_ID)) && (strncmp(path, LABEL_FILTER_BY_ID, len) == 0)) return PathResolver::FILTER_BY_ID;
	return PathResolver::FILTER_NONE;
}

size_t getNextFilterLabel(const char *path, size_t *n) {
	size_t len = strlen(path);
	while ((path[*n] == '/') && ((*n) < len)) (*n)++;
	const char *ptr = strchr(path + (*n), '/');
	if (ptr == NULL) return (len - *n);
	else return (size_t)(ptr - (path + *n));
	return 1;
}