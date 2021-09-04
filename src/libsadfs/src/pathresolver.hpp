#ifndef _X_PATHRESOLVER_HPP_
#define _X_PATHRESOLVER_HPP_

#include <stddef.h>

class PathResolver;

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

	const int getChildEntries(ResolvedPath *, ResolvedPath **);

	const mode_t getMode(path_type id);
	const mode_t getMode(ResolvedPath *);
	const bool isDirectory(path_type id);
	const bool isDirectory(ResolvedPath *);
	const bool isFile(path_type id);
	const bool isFile(ResolvedPath *);
	const char *getName(path_type id);
	const char *getName(ResolvedPath *);
	const Libsad::FilterObjectType getTargetObject(path_type id);
	const Libsad::FilterObjectType getTargetObject(ResolvedPath *);

	static const char *construct_full_name(const char *base, const char *name, const char *ext);


private:
	typedef struct {
		path_type id;
		const char *name;
		size_t name_len;
		//const char *base_path;
		//size_t base_path_len;
		const char *full_path;
		size_t full_path_len;
		path_type parent_id;
		mode_t mode;
		LibsadPlugin::DataFileHandlerType data_file;
		unsigned int filter:4;
		Libsad::FilterObjectType target_object;
	} PathDefinition;

	Libsad *lib;
	PathResolver::PathDefinition *path_definitions;

	PathResolver::PathDefinition *getDefinitionWithParentSingle(path_type id);
	PathResolver::PathDefinition *getPathDefinitionRecord(path_type id);
	PathResolver::PathDefinition *getPathDefinitionRecord(ResolvedPath *);
	const char *getFullPath(path_type id);
	static const char *getFullPath(PathDefinition *full_paths, path_type id);
	//static const char *construct_base_path(path_type id, PathResolver::PathDefinition *paths);
	static PathResolver::PathDefinition *parsePathDefinition(path_def *in);
};

#endif /* _X_PATHRESOLVER_HPP_ */
