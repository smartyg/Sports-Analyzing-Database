#include "config.h"

#include "path.hpp"

#include <sys/stat.h>

#include "pathfilter.hpp"

#define ADD_PATH(id, name, base_id, type, mode, suffix, filter) {id, name, base_id, (mode_t)((type) | (mode)), suffix, (filter) }

path_def paths[] = {
	ADD_PATH(PATH_TYPE_DIR_ROOT, "/", PATH_TYPE_NONE, S_IFDIR, S_IRUSR | S_IXUSR, LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE, PathFilter::FILTER_NONE),
	ADD_PATH(PATH_TYPE_FILE_VERSION, ".version", PATH_TYPE_DIR_ROOT, S_IFREG, S_IRUSR, LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE, PathFilter::FILTER_NONE),
	ADD_PATH(PATH_TYPE_DIR_ROUTE_FILTER, "routes", PATH_TYPE_DIR_ROOT, S_IFDIR, S_IRUSR | S_IXUSR, LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE, PathFilter::FILTER_BY_DATE | PathFilter::FILTER_BY_TITLE | PathFilter::FILTER_BY_ACTIVITY),
	ADD_PATH(PATH_TYPE_FILE_ROUTES_OVERVIEW, "overview", PATH_TYPE_DIR_ROUTE_FILTER, S_IFREG, S_IRUSR, LibsadPlugin::PLUGIN_TYPE_DATA_FILE_TABLE, PathFilter::FILTER_NONE),
	ADD_PATH(PATH_TYPE_FILE_ROUTES_STATISTICS, "statistics", PATH_TYPE_DIR_ROUTE_FILTER, S_IFREG, S_IRUSR, LibsadPlugin::PLUGIN_TYPE_DATA_FILE_TABLE, PathFilter::FILTER_NONE),
	ADD_PATH(PATH_TYPE_DIR_ROUTE_DETAIL, NULL, PATH_TYPE_DIR_ROUTE_FILTER, S_IFDIR, S_IRUSR | S_IXUSR, LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE, PathFilter::FILTER_NONE),
	ADD_PATH(PATH_TYPE_DIR_HEATMAP_FILTER, "heatmaps", PATH_TYPE_DIR_ROOT, S_IFDIR, S_IRUSR | S_IXUSR, LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE, PathFilter::FILTER_BY_DATE),
	ADD_PATH(PATH_TYPE_DIR_DATA, ".data", PATH_TYPE_DIR_ROOT, S_IFDIR, S_IRUSR | S_IXUSR, LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE, PathFilter::FILTER_NONE),
	ADD_PATH(PATH_TYPE_DIR_PICTURES, "pictures", PATH_TYPE_DIR_ROOT, S_IFDIR, S_IRUSR | S_IWUSR | S_IXUSR, LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE, PathFilter::FILTER_NONE),
	ADD_PATH(PATH_TYPE_DIR_SEGMENT_FILTER, "segments", PATH_TYPE_DIR_ROOT, S_IFDIR, S_IRUSR | S_IXUSR, LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE, PathFilter::FILTER_BY_TITLE | PathFilter::FILTER_BY_ACTIVITY),
	ADD_PATH(PATH_TYPE_DIR_SEGMENT_DETAIL, NULL, PATH_TYPE_DIR_SEGMENT_FILTER, S_IFDIR, S_IRUSR | S_IXUSR, LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE, PathFilter::FILTER_NONE),
	ADD_PATH(PATH_TYPE_LAST, NULL, PATH_TYPE_NONE, 0, 0, LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE, PathFilter::FILTER_NONE)
};
