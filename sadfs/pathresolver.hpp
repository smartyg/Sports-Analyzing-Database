#ifndef _X_PATHRESOLVER_HPP_
#define _X_PATHRESOLVER_HPP_

#include <stddef.h>

#include "resolvedpath.hpp"
#include "libsad.hpp"
#include "path.hpp"

class PathResolver {
public:
	typedef struct {
		path_type id;
		const char *name;
		size_t name_len;
		const char *base_path;
		size_t base_path_len;
		const char *full_path;
		size_t full_path_len;
		path_type parent_id;
		mode_t mode;
		data_file_suffix data_file;
		unsigned int filter:4;
	} PathDefinition;

	PathResolver(path_def *path_definitions);
	ResolvedPath *Resolve(const char *path);
	path_type getPathIdWithParentSingle(path_type id);
	path_type *getPathIdWithParentMulti(path_type id);
};

#endif /* _X_PATHRESOLVER_HPP_ */
