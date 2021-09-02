#include "config.h"

#include "pathresolver.hpp"

#include <stddef.h>
#include <cstdlib>
#include <memory>
#include <cstring>

#include "pathfilter.hpp"
#include "resolvedpath.hpp"
#include "libsad.hpp"
#include "libsadplugin.hpp"

	PathResolver::PathResolver(Libsad *lib, path_def *path_definitions)
	{
		this->lib = lib;
		this->path_definitions = PathResolver::parsePathDefinition(path_definitions);
	}

	PathResolver::~PathResolver(void) {
		free(this->path_definitions);
	}

	ResolvedPath *PathResolver::resolve(const char *path)
	{
		ResolvedPath *rp = NULL;

		for(PathResolver::PathDefinition *p = this->path_definitions; p->id != PATH_TYPE_LAST; p++) {
			if (p->full_path != NULL) {
				if (strcmp(path, p->full_path) == 0) {
					rp = new ResolvedPath(path, p->id, NULL);
					break;
				} else if ((strncmp(path, p->full_path, p->full_path_len) == 0) && p->data_file != LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE && (this->lib->getPluginHandler()->isExtentionMatch(path + p->full_path_len, p->data_file) == true)) {
					rp = new ResolvedPath(path, p->id, NULL);
					break;
				} else if ((strncmp(path, p->full_path, p->full_path_len) == 0) && PathFilter::isValid(p->filter)) {
					PathFilter *filter = new PathFilter(path, path + p->full_path_len, p->id, p->filter);
					PathResolver::PathDefinition *res = NULL;// = p;
					if (filter->isComplete()) {
						res = getDefinitionWithParentSingle(p->id);
					}
					if (res == NULL) rp = new ResolvedPath(path, p->id, filter);
					else rp = new ResolvedPath(path, res->id, filter);
					break;
				}
			}
		}

		if (rp != NULL && rp->isValid()) return rp;
		delete rp;
		return NULL;
	}

	path_type PathResolver::getPathIdWithParentSingle(path_type id) {
		PathResolver::PathDefinition *p = getDefinitionWithParentSingle(id);
		if (p != NULL) return p->id;
		return PATH_TYPE_NONE;
	}

	path_type *PathResolver::getPathIdWithParentMulti(path_type id) {
		// TODO
		return NULL;
	}

	const mode_t PathResolver::getMode(path_type id) {
		return this->path_definitions[id].mode;
	}

	const bool PathResolver::isDirectory(path_type id) {
		return S_ISDIR(this->getMode(id));
	}

	const bool PathResolver::isFile(path_type id) {
		return S_ISREG(this->getMode(id));
	}

	PathResolver::PathDefinition *PathResolver::getDefinitionWithParentSingle(path_type id)
	{
		for(PathResolver::PathDefinition *p = this->path_definitions; p->id != PATH_TYPE_LAST; p++) {
			if (p->parent_id == id)
			{
				return p;
			}
		}
		return NULL;
	}

	const char *PathResolver::construct_base_path(path_type id, PathResolver::PathDefinition *paths)
	{
		off_t offset = id;
		PathResolver::PathDefinition *p = paths + offset;

		if (p->full_path == NULL) return NULL;
		if (p->full_path[0] != '/') return NULL;
		char *dest = (char *)malloc(sizeof(char) * (p->full_path_len + 1));
		return (const char *)strcpy(dest, p->full_path);
	}

	const char *PathResolver::construct_full_name(const char *base, const char *name)
	{
		bool add_slash = false;
		if (name == NULL) return NULL;
		if (base == NULL && name[0] == '/') {
			char *dest = (char *)malloc(sizeof(char) * (strlen(name) + 1));
			strcpy(dest, name);
			return (const char *)dest;
		} else if (base == NULL) return NULL;

		size_t base_len = strlen(base);
		size_t name_len = strlen(name);
		size_t total_len = base_len + name_len;
		if (base[base_len - 1] != '/') {
			add_slash = true;
			base_len++;
			total_len++;
		}
		char *dest = (char *)malloc(sizeof(char) * (total_len + 1));
		strcpy(dest, base);
		if (add_slash) dest[base_len - 1] = '/';
		strcpy(dest + base_len, name);
		return (const char *)dest;
	}

	PathResolver::PathDefinition *PathResolver::parsePathDefinition(path_def *in) {
		off_t alloc_size = (PATH_TYPE_LAST + 1) * sizeof(PathResolver::PathDefinition);
		PathResolver::PathDefinition *full_paths = (PathResolver::PathDefinition *)malloc(alloc_size);
		memset(full_paths, 0, alloc_size);

		for(path_def *p_in = in; p_in->id != PATH_TYPE_LAST; p_in++) {
			off_t offset = p_in->id;
			PathResolver::PathDefinition *p_out = full_paths + offset;

			const char *base_path = construct_base_path(p_in->parent_id, full_paths);
			const char *full_path = construct_full_name(base_path, p_in->name);

			p_out->id = p_in->id;
			p_out->name = p_in->name;
			p_out->base_path = base_path;
			p_out->full_path = full_path;
			p_out->parent_id = p_in->parent_id;
			p_out->mode = p_in->mode;
			p_out->data_file = p_in->data_file;
			p_out->filter = p_in->filter;

			if (p_out->name == NULL) p_out->name_len = 0;
			else p_out->name_len = strlen(p_out->name);

			if (p_out->base_path == NULL) p_out->base_path_len = 0;
			else p_out->base_path_len = strlen(base_path);

			if (p_out->full_path == NULL) p_out->full_path_len = 0;
			else p_out->full_path_len = strlen(full_path);
		}
		((PathResolver::PathDefinition *)(full_paths + PATH_TYPE_LAST))->id = PATH_TYPE_LAST;
		return full_paths;
	}