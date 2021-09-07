#include "config.h"

#include "pathresolver.hpp"

#include <stddef.h>
#include <cstdlib>
#include <memory>
#include <cstring>
#include <vector>

#include "resolvedpath.hpp"
#include "libsadplugin.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

using namespace std;

const vector<const ResolvedPath *> *PathResolver::getChildEntries(const ResolvedPath *rp) const {
	DEBUG("PathResolver::%s(%p)\n", __FUNCTION__, rp);

	path_type id = rp->getPathId();
	const char *base_path = rp->getPath();
	vector<const ResolvedPath *> *children = new vector<const ResolvedPath *>;
	const PathDefinition *pd = this->getPathDefinitionRecord(rp);

	// If for some reason the id's do not match, or there is no record, something went wrong.
	if ((pd == NULL) || (pd->id != rp->getPathId())) throw InvalidResolvedPath(__FILE__, __func__, __LINE__, rp, id);

	// The given *rp should not have a complete filter while still pointing to a entry with filter enabled.
	if (pd->filters != FILTER_NONE && rp->hasFilter() && (rp->hasObjectId() || rp->hasObjectTime())) throw InvalidPath(__FILE__, __func__, __LINE__, base_path);

	// if we (*rp) have a filter attached that is not complete (yet), list the filter results.
	if (rp->hasFilter() && !(rp->hasObjectId() || rp->hasObjectTime())) {
		// Save our base filter localy to make access easier.
		BaseFilter base_filter = rp->getPathFilterBase();

		// Save the pointer to our filter info to make access easier.
		const Libsad::FilterInfoType filter_info = rp->getPathFilterInfo();

		// If we do not have a base filter (yet), list the filter options. NOTE: the path resolver should already set the base filter if only 1 filter option is availible.
		if (base_filter == FILTER_NONE) {
			if (IS_SET(pd->filters, FILTER_BY_DATE)) {
				const ResolvedPath *rp_tmp = new ResolvedPath(base_path, LABEL_FILTER_BY_DATE, id);
				if (rp_tmp != NULL) children->push_back(rp_tmp);
			}
			if (IS_SET(pd->filters, FILTER_BY_ACTIVITY)) {
				const ResolvedPath *rp_tmp = new ResolvedPath(base_path, LABEL_FILTER_BY_ACTIVITY, id);
				if (rp_tmp != NULL) children->push_back(rp_tmp);
			}
			if (IS_SET(pd->filters, FILTER_BY_TITLE)) {
				const ResolvedPath *rp_tmp = new ResolvedPath(base_path, LABEL_FILTER_BY_TITLE, id);
				if (rp_tmp != NULL) children->push_back(rp_tmp);
			}
			if (IS_SET(pd->filters, FILTER_BY_ID)) {
				const ResolvedPath *rp_tmp = new ResolvedPath(base_path, LABEL_FILTER_BY_ID, id);
				if (rp_tmp != NULL) children->push_back(rp_tmp);
			}
		} else {
			// Now we have to select the correct Libsad::FilterListType base on which base filter we are using and which filters are not yet filled.
			Libsad::FilterListType list = Libsad::LIST_TYPE_NONE;
			if (base_filter == FILTER_BY_DATE || base_filter == FILTER_BY_ACTIVITY) {
				if (base_filter == FILTER_BY_ACTIVITY && filter_info.filter_activity == 0) list = Libsad::LIST_TYPE_ACTIVITIES;
				else if (IS_SET(pd->filters, FILTER_BY_DATE) && filter_info.filter_year == 0) list = Libsad::LIST_TYPE_YEARS;
				else if (IS_SET(pd->filters, FILTER_BY_DATE) && filter_info.filter_month == 0) list =Libsad::LIST_TYPE_MONTHS;
				else if (IS_SET(pd->filters, FILTER_BY_DATE) && filter_info.filter_day == 0) list =Libsad::LIST_TYPE_DAYS;
				else list = Libsad::LIST_TYPE_TIME;
			} else if (base_filter == FILTER_BY_TITLE) {
				if (filter_info.filter_title_first_letter == 0) list = Libsad::LIST_TYPE_FIRST_LETTER_TITLES;
				else if (filter_info.filter_title == NULL) list = Libsad::LIST_TYPE_TITLES;
				else list = Libsad::LIST_TYPE_TIME;
			} else if (base_filter == FILTER_BY_ID) list = Libsad::LIST_TYPE_IDS;

			// If we did not find any missing information, the this filter should have been marked complete by the resolver already. Here we can not handle that case anymore.
			if (list == Libsad::LIST_TYPE_NONE) throw InvalidFilter(__FILE__, __func__, __LINE__, filter_info);

			// Now it's time to execute the filter (in the backend) and process the results. The results of the executed filter will be stored in *filters.
			char *filters = NULL;
			size_t n_filters = this->lib->getFilteredList(static_cast<Libsad::FilterObjectType>(pd->filter_object), list, filter_info, &filters);
			for(unsigned int i = 0; i < n_filters; i++) {
				const char *f = filters + i;
				const ResolvedPath *rp_tmp = new ResolvedPath(base_path, f, id);
				// If the constructed ResolvedPath is successfully created, store it.
				if (rp_tmp != NULL) children->push_back(rp_tmp);
				// As we handled this entry, immediately free the data, there is no reson to keep it around.
				free(const_cast<char *>(f));
			}
			// Now also free the table.
			if (filters != NULL) free(filters);
		}
	}
	// First we look for entries that have our (*rp) id as parent id, entries without path name are dynamic (they depend on filters) so we exclude those.
	for(const PathResolver::PathDefinition *p = this->path_definitions; p->id != PATH_TYPE_LAST; p++) {
		if (p->parent_id == id && p->name != NULL)
		{
			DEBUG("loop; parse id: %ld\n", p->id);

			// In case the entry is a regular file and has a data file handler attached.
			if ((S_ISREG(p->mode)) && (p->data_file_handler != LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE)) {
				// for each file handler that match the type of data file handler.

				// List all extentions for all data file handlers that support the requested handler type.
				const vector<const char *> *extentions = LibsadPlugin::getDataFileExtentions(static_cast<LibsadPlugin::DataFileHandlerType>(p->data_file_handler));

				// Loop over the extensions and make create a ResolvedPath entry for each.
				for (const char *e : *extentions) {
					DEBUG("store entry: path: %s; file: %s\n", __FUNCTION__, base_path, p->name);
					const ResolvedPath *rp_tmp = new ResolvedPath (base_path, p->name, e, static_cast<path_type>(p->id));
					// If the constructed ResolvedPath is successfully created, store it.
					if (rp_tmp != NULL) children->push_back(rp_tmp);
				}
				delete extentions;
			} else { // In the other cases, life is simple.
				DEBUG("store entry: path: %s; file: %s\n", __func__, base_path, p->name);
				// Create a ResolvedPath entry.
				const ResolvedPath *rp_tmp = new ResolvedPath(base_path, p->name, static_cast<path_type>(p->id));

				// If the constructed ResolvedPath is successfully created, store it.
				if (rp_tmp != NULL) children->push_back(rp_tmp);
			}
		}
	}

	return children;
}
