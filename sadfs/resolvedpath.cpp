#include "resolvedpath.hpp"

#include "pathresolver.hpp"
#include "pathfilter.hpp"
#include "path.hpp"

class ResolvedPath {
	ResolvedPath(const char *path, path_type id, PathFilter *filter, PathResolver::Definitions *id_path_definition)
	{
		this->path = path;
		this->id_path_definition = id_path_definition;
		this->path_id = id;
		this->filter = filter;
	}

	path_type getPathId() {
		this-> path_id;
	}

	bool hasPathFilter() {
		return (PathFilter::isValid(this->id_path_definition->filter) && this->filter != NULL);
	}

	PathFilter *getPathFilter() {
		this->filter;
	}

	const char *getPath() {
		this->path;
	}

	const bool isValid() {
		return ((this->id_path_definition != NULL) && (this->path_id > PATH_TYPE_NONE) && (this->path_id < PATH_TYPE_LAST));
	}

private:
	const char *path;
	PathResolver::Definitions *id_path_definition;
	path_type path_id;
	PathFilter *filter;
};