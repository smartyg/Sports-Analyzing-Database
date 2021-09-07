#include "config.h"

#include "pathresolver.hpp"

#include <stddef.h>
#include <cstdlib>
#include <memory>
#include <cstring>

#include "resolvedpath.hpp"
#include "exceptions.hpp"
#include "libsadplugin.hpp"
#include "logger.hpp"

#define HAS_SINGLE_FILTER_BIT_SET(m) (((m) == FILTER_BY_DATE) || ((m) == FILTER_BY_ACTIVITY) || ((m) == FILTER_BY_TITLE) || ((m) == FILTER_BY_ID))

inline bool correctToUTC (struct tm *);
inline bool pathToTime (const char *, size_t, Libsad::FilterInfoType *);
inline bool pathToDate (const char *, size_t, Libsad::FilterInfoType *);
inline uint_fast32_t pathToInt (const char *, size_t);
size_t getNextFilterLabel(const char *, size_t *);
inline bool isFilterComplete(const Libsad::FilterInfoType);
PathResolver::BaseFilter matchBaseFilter(const char *, size_t);

const ResolvedPath *PathResolver::resolve(const char *path) const {
	DEBUG("PathResolver::%s(%s)\n", __func__, path);
	Libsad::FilterInfoType filter_info = {0};
	return this->resolveInternal(path, 0, PATH_TYPE_DIR_ROOT, FILTER_NONE, &filter_info);
}

// Look if given record is a child of parent (or if it is the parent itself).
bool PathResolver::isFromParent(const PathDefinition *p, path_type parent_id) const {
	DEBUG("PathResolver::%s(%p, %d)\n", __func__, p, parent_id);
	if (p == NULL) return false;

	for (const PathDefinition *pd = p; pd->id != PATH_TYPE_NONE; pd = const_cast<PathDefinition *>(this->path_definitions) + pd->parent_id) {
		// Check both current id and parent id at once.
		if (static_cast<path_type>(p->id) == parent_id) return true;
		if (static_cast<path_type>(p->parent_id) == parent_id) return true;
	}
	return false;
}

const ResolvedPath *PathResolver::resolveInternal(const char *path, size_t start_pos, path_type parent_id, BaseFilter base_filter, Libsad::FilterInfoType *filter_info) const {
	DEBUG("PathResolver::%s(%s, %ld, %d, %d)\n", __func__, path, start_pos, parent_id, base_filter);
	ResolvedPath *rp = NULL;

	// Store a matched filtered path in here.
	PathDefinition *fpd = NULL;

	// Store the length of the filtered patch match.
	size_t match_len = 0;

	// First we are looking for a full match. But if we find a partial (filter entry) match, save the one with the longest matching path.
	for(const PathResolver::PathDefinition *p = this->path_definitions; p->id != PATH_TYPE_LAST; p++) {
		// Only check if this type sets a path and if it is (a child of) the parent.
		if (p->full_path != NULL && this->isFromParent (p, parent_id)) {
			DEBUG("check record %ld: name: %s; path: %s; parent_id: %ld; mode %lx; file handler: %ld; filters: %ld; filter object: %ld\n", p->id, p->name, p->full_path, p->parent_id, p->mode, p->data_file_handler, p->filters, p->filter_object);
			if (strcmp(path + start_pos, p->full_path) == 0) {
				// We have a full match with the path.
				// Check if this directory has a filter, if so, initialize the filter.
				/* TODO: we need to save the filtered result as we now encoutered a new filter dir. */
				base_filter = FILTER_NONE;
				if (p->filters != FILTER_NONE) {
					if (HAS_SINGLE_FILTER_BIT_SET(p->filters)) base_filter = static_cast<BaseFilter>(p->filters);
					rp = new ResolvedPath (path, static_cast<path_type>(p->id), true, base_filter, *filter_info);
				} else rp = new ResolvedPath (path, static_cast<path_type>(p->id), false, base_filter, *filter_info);
				// We are done, path is fullly resolved.
				break;
			} else if ((strncmp(path + start_pos, p->full_path, p->full_path_len) == 0) &&
				       p->data_file_handler != LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE &&
				       (this->lib->getPluginHandler()->isExtentionMatch(path + start_pos + p->full_path_len, static_cast<LibsadPlugin::DataFileHandlerType>(p->data_file_handler)) == true)) {
				// We found a full match on a file with a data file handler set.
				rp = new ResolvedPath(path, static_cast<path_type>(p->id), false, base_filter, *filter_info);
				// Path fully resolved.
				break;
			} else if ((strncmp(path + start_pos, p->full_path, p->full_path_len) == 0) && p->filters != FILTER_NONE) {
				// The first part of the path is a match with a filter entry. Check if this is the longest partial (filter entry) match so far.
				if (p->full_path_len > match_len) {
					// Longest partial match so far, save it.
					fpd = const_cast<PathDefinition *>(p);
					match_len = p->full_path_len;
				}
			}
		}
	}

	// Check if ResolvedPath is really set and good.
	if (rp != NULL && rp->isValid()) return rp;

	// We did not find a full match, check if we have a partial match.
	if (match_len > 0 && fpd != NULL) {
		DEBUG("This is a filter directory path, try to resolve the filter. Longest match: %ld\n", match_len);
		// If this directory has only 1 filter, already set it.
		if (HAS_SINGLE_FILTER_BIT_SET(fpd->filters)) base_filter = static_cast<BaseFilter>(fpd->filters);

		size_t n = start_pos + fpd->full_path_len;
		size_t len;

		// Loop over all the remaining parts of the path, see how much we can match.
		while ((len = getNextFilterLabel(path, &n)) > 0 && !isFilterComplete(*filter_info)) {
			DEBUG("getNextFilterLabel returned: len: %ld; n: %ld\n", len, n);
			DEBUG("resolving path: %.*s\n", static_cast<int>(len), path + n);
			if (base_filter == FILTER_NONE) {
				base_filter = matchBaseFilter(path + n, len);

				// matchBaseFilter did not match, so we can not resolve this path.
				if (base_filter == FILTER_NONE) throw InvalidPath(__FILE__, __func__, __LINE__, path);
			} else if (base_filter == FILTER_BY_ACTIVITY || base_filter == FILTER_BY_DATE || base_filter == FILTER_BY_TITLE) {
				if (base_filter == FILTER_BY_ACTIVITY && filter_info->filter_activity == 0) {
					if ((filter_info->filter_activity = this->lib->getActivityCodeFromName(path + n, len - 1)) == 0) throw InvalidPath(__FILE__, __func__, __LINE__, path); // We want to filter by activity, but next part does not contain an activity.
				} else if ((base_filter == FILTER_BY_ACTIVITY && filter_info->filter_activity != 0 && IS_SET(fpd->filters, FILTER_BY_DATE)) || base_filter == FILTER_BY_DATE) {
					if (filter_info->filter_year == 0 && (path + n)[0] == '.') {
						if (!pathToDate(path + n, len, filter_info)) throw InvalidPath(__FILE__, __func__, __LINE__, path);
						filter_info->object_type = TYPE_OBJECT_TIME;
					} else if (filter_info->filter_year == 0) {
						unsigned int year;
						if ((year = pathToInt(path + n, len)) == 0) throw InvalidPath(__FILE__, __func__, __LINE__, path);
						filter_info->filter_year = static_cast<uint_fast8_t>(static_cast<unsigned int>(year) - static_cast<unsigned int>(1970));
					} else if (filter_info->filter_month == 0) {
						if ((filter_info->filter_month = pathToInt(path + n, len)) == 0) throw InvalidPath(__FILE__, __func__, __LINE__, path);
					} else if (filter_info->filter_day == 0) {
						if ((filter_info->filter_day = pathToInt(path + n, len)) == 0) throw InvalidPath(__FILE__, __func__, __LINE__, path);
					} else if (filter_info->filter_hour == 0 && filter_info->filter_minute == 0 && filter_info->filter_second == 0) {
						if (!pathToTime(path + n, len, filter_info)) throw InvalidPath(__FILE__, __func__, __LINE__, path);
					} else throw InvalidFilter(__FILE__, __func__, __LINE__, *filter_info); // We say there is no complete filter, but all fields are already set, something is wrong.
				} else if ((base_filter == FILTER_BY_ACTIVITY && filter_info->filter_activity != 0 && IS_SET(fpd->filters, FILTER_BY_TITLE)) || base_filter == FILTER_BY_TITLE) {
					if (filter_info->filter_title_first_letter == 0 && len == 1) filter_info->filter_title_first_letter = (path + n)[0];
					else if (filter_info->filter_title_first_letter == 0 && (path + n)[0] == '.') filter_info->filter_title = strndup(path + n + 1, len - 1);
					else if (filter_info->filter_title_first_letter == 0 && filter_info->filter_title == NULL) throw InvalidPath (__FILE__, __func__, __LINE__, path); // We want to filter on title, but the next entry is neither a single character or a dot (full activity title) directory. Something went wrong.
					else if (filter_info->filter_title_first_letter != 0 && filter_info->filter_title == NULL) filter_info->filter_title = strndup(path + n, len);
					else if (filter_info->filter_title != NULL) {
						if (!pathToDate(path + n, len, filter_info)) throw InvalidPath(__FILE__, __func__, __LINE__, path); // We filter by title, there is a title set and no complete filter, so the next part must contain a date, if not something is wrong.
						filter_info->object_type = TYPE_OBJECT_TIME;
					}
				}
			} else if (base_filter == FILTER_BY_ID) {
				if (filter_info->object_type == TYPE_OBJECT_EMPTY) {
					if ((filter_info->object = pathToInt(path + n, len)) == 0) throw InvalidPath(__FILE__, __func__, __LINE__, path); // We want to filter by id, but the next part is not a id. Something is wrong.
					filter_info->object_type = TYPE_OBJECT_ID;
				}
			} else throw InvalidFilter(__FILE__, __func__, __LINE__, *filter_info); // Base filter has a value that we do not know about.

			// Increase n so next run it will look for a path starting at the end of this part.
			n += len;
		}

		if (isFilterComplete(*filter_info)) {
			// There is a posibility that the final object id is not calculated yet, do that now.
			if (filter_info->object_type == TYPE_OBJECT_EMPTY) {
				struct tm t = {0};
				t.tm_sec  = filter_info->filter_second;
				t.tm_min  = filter_info->filter_minute;
				t.tm_hour = filter_info->filter_hour;
				t.tm_mday = filter_info->filter_day;
				t.tm_mon  = filter_info->filter_month;
				t.tm_year = filter_info->filter_year;
				time_t epoch = timegm(&t);
				if (epoch > 0) {
					filter_info->object = static_cast<uint_fast32_t>(epoch);
					filter_info->object_type = TYPE_OBJECT_TIME;
				}
			}
			// Filter is complete, look for the child which defines the result.
			for(const PathDefinition *p = this->path_definitions; p->id != PATH_TYPE_LAST; p++) {
				// Only check dynamic entries, as we look for and entry to hold the complete filter result.
				if (p->name == NULL && p->parent_id == fpd->id) {
					// place the found match in *fpd.
					fpd = const_cast<PathDefinition *>(p);
					break;
				}
			}
		}

		if (n == strlen(path)) {
			// We consumed the whole path, so construct the ResolvedPath and return.
			rp = new ResolvedPath(path, static_cast<path_type>(fpd->id), true, base_filter, *filter_info);
			return rp;
		} else if (isFilterComplete(*filter_info)) {
			// Filter is complete, but there is still path left, do another loop and return its result.
			return this->resolveInternal(path, n, static_cast<path_type>(fpd->id), base_filter, filter_info);
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

inline bool isFilterComplete(const Libsad::FilterInfoType fi) {
	if (fi.object_type != TYPE_OBJECT_EMPTY && fi.object > 0) return true;
	if (fi.filter_year > 0 && fi.filter_month > 0 && fi.filter_day > 0 && (fi.filter_hour > 0 || fi.filter_minute > 0 || fi.filter_second > 0)) return true;
	else return false;
}

inline bool correctToUTC (struct tm *t) {
	(void) t;
	/* TODO: correct for timezone */
	return true;
}

inline bool pathToTime (const char *path, size_t start, Libsad::FilterInfoType *fi) {
	struct tm t;
	const char *ptr = strptime (path, "%H:%M:%S UTC%z", &t);
	if (ptr != path + start) return false;
	correctToUTC (&t);
	fi->filter_hour   = t.tm_hour;
	fi->filter_minute = t.tm_min;
	fi->filter_second = t.tm_sec;
	return true;
}

inline bool pathToDate (const char *path, size_t start, Libsad::FilterInfoType *fi) {
	struct tm t;
	const char *ptr = strptime (path, "%Y-%m-%d %H:%M:%S UTC%z", &t);
	if (ptr != path + start) return false;
	correctToUTC (&t);
	fi->filter_year   = t.tm_year;
	fi->filter_month  = t.tm_mon;
	fi->filter_day    = t.tm_mday;
	fi->filter_hour   = t.tm_hour;
	fi->filter_minute = t.tm_min;
	fi->filter_second = t.tm_sec;
	return true;
}

inline uint_fast32_t pathToInt (const char *path, size_t start) {
	//fprintf(stderr, "%s(%s, %d)\n", __func__, path, start);
	char *ptr = NULL;
	long int i = strtol (path, &ptr, 10);
	if (ptr != path + start) return 0;
	return static_cast<uint_fast32_t>(i);
}


PathResolver::BaseFilter matchBaseFilter(const char *path, size_t path_len) {
	DEBUG("%s(%s, %ld)\n", __func__, path, path_len);
	if ((path_len == strlen(LABEL_FILTER_BY_DATE)) && (strncmp(path, LABEL_FILTER_BY_DATE, path_len) == 0)) return PathResolver::FILTER_BY_DATE;
	else if ((path_len == strlen(LABEL_FILTER_BY_TITLE)) && (strncmp(path, LABEL_FILTER_BY_TITLE, path_len) == 0)) return PathResolver::FILTER_BY_TITLE;
	else if ((path_len == strlen(LABEL_FILTER_BY_ACTIVITY)) && (strncmp(path, LABEL_FILTER_BY_ACTIVITY, path_len) == 0)) return PathResolver::FILTER_BY_ACTIVITY;
	else if ((path_len == strlen(LABEL_FILTER_BY_ID)) && (strncmp(path, LABEL_FILTER_BY_ID, path_len) == 0)) return PathResolver::FILTER_BY_ID;
	return PathResolver::FILTER_NONE;
}

size_t getNextFilterLabel(const char *path, size_t *n) {
	DEBUG("%s(%s, %ld)\n", __func__, path, *n);
	size_t len = strlen(path);
	while ((path[*n] == '/') && ((*n) < len)) (*n)++;
	const char *ptr = strchr(path + (*n), '/');
	if (ptr == NULL) return (len - *n);
	else return static_cast<size_t>(ptr - (path + *n));
	return 1;
}