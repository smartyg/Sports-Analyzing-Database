#ifndef _X_PATH_H_
#define _X_PATH_H_

#include <sys/stat.h>

#include "libsad.hpp"
#include "libsadplugin.hpp"

typedef enum : uint_fast8_t {
	PATH_TYPE_NONE = 0,
	PATH_TYPE_DIR_ROOT,
	PATH_TYPE_FILE_ROUTES_OVERVIEW,
	PATH_TYPE_FILE_ROUTES_STATISTICS,
	PATH_TYPE_DIR_ROUTE_FILTER,
	PATH_TYPE_DIR_ROUTE_DETAIL,
	PATH_TYPE_DIR_HEATMAP_FILTER,
	PATH_TYPE_FILE_VERSION,
	PATH_TYPE_DIR_DATA,
	PATH_TYPE_DIR_PICTURES,
	PATH_TYPE_DIR_SEGMENT_FILTER,
	PATH_TYPE_DIR_SEGMENT_DETAIL,
	PATH_TYPE_LAST
} path_type;

typedef struct {
	path_type id;
	const char *name;
	path_type parent_id;
	mode_t mode;
	LibsadPlugin::DataFileHandlerType data_file_handler;
	unsigned int filters:8;
	Libsad::FilterObjectType filter_object;
} path_def;

extern path_def paths[];

#endif /* _X_PATH_H_ */