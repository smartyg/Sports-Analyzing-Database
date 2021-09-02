#ifndef _X_PATHRESOLVER_HPP_
#define _X_PATHRESOLVER_HPP_

#include <stddef.h>

#include "resolvedpath.hpp"
#include "path.hpp"
#include "libsad.hpp"
#include "libsadplugin.hpp"

class PathResolver {
public:
	PathResolver(Libsad *lib, path_def *path_definitions);
	~PathResolver(void);
	ResolvedPath *resolve(const char *path);
	path_type getPathIdWithParentSingle(path_type id);
	path_type *getPathIdWithParentMulti(path_type id);

	const mode_t getMode(path_type id);
	const bool isDirectory(path_type id);
	const bool isFile(path_type id);

private:
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
		LibsadPlugin::DataFileHandlerType data_file;
		unsigned int filter:4;
	} PathDefinition;

	Libsad *lib;
	PathResolver::PathDefinition *path_definitions;

	PathResolver::PathDefinition *getDefinitionWithParentSingle(path_type id);
	static const char *construct_base_path(path_type id, PathResolver::PathDefinition *paths);
	static const char *construct_full_name(const char *base, const char *name);
	static PathResolver::PathDefinition *parsePathDefinition(path_def *in);
};

#endif /* _X_PATHRESOLVER_HPP_ */
