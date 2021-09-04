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

// Local functions
ResolvedPath *storeTable(ResolvedPath **, size_t *, ResolvedPath *);

PathResolver::PathResolver(Libsad *lib, path_def *path_definitions) {
	fprintf(stderr, "PathResolver::%s(%p, %p)\n", __FUNCTION__, lib, path_definitions);
	this->lib = lib;
	this->path_definitions = PathResolver::parsePathDefinition(path_definitions);
}

PathResolver::~PathResolver(void) {
	fprintf(stderr, "PathResolver::%s()\n", __FUNCTION__);
	free(this->path_definitions);
}

ResolvedPath *PathResolver::resolve(const char *path) {
	fprintf(stderr, "PathResolver::%s(%s)\n", __FUNCTION__, path);
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
					res = this->getDefinitionWithParentSingle(p->id);
				}
				if (res == NULL) rp = new ResolvedPath(path, p->id, filter);
				else rp = new ResolvedPath(path, res->id, filter);
				break;
			}
		}
	}

	if (rp != NULL && rp->isValid()) return rp;
	if (rp != NULL) delete rp;
	return NULL;
}

path_type PathResolver::getPathIdWithParentSingle(path_type id) {
	fprintf(stderr, "PathResolver::%s(%d)\n", __FUNCTION__, id);
	PathResolver::PathDefinition *p = this->getDefinitionWithParentSingle(id);
	if (p != NULL) return p->id;
	return PATH_TYPE_NONE;
}

const int PathResolver::getChildEntries(ResolvedPath *rp, ResolvedPath **children) {
	fprintf(stderr, "PathResolver::%s(%p, %p)\n", __FUNCTION__, rp, *children);

	path_type id = rp->getPathId();
	const char *base_path = rp->getPath();
	size_t n = 0;
	*children = NULL;

	for(PathResolver::PathDefinition *p = this->path_definitions; p->id != PATH_TYPE_LAST; p++) {
		if (p->parent_id == id && p->name != NULL)
		{
			fprintf(stderr, "PathResolver::%s: loop; parse id: %d\n", __FUNCTION__, p->id);

			if ((S_ISREG(p->mode)) && (p->data_file != LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE)) {
				// for each file handler that match extention
				char *extentions = NULL;
				int n_extentions = LibsadPlugin::getDataFileExtentions(p->data_file, &extentions);
				for(int i = 0; i < n_extentions; i++) {
					fprintf(stderr, "PathResolver::%s: store entry %d: path: %s; file: %s\n", __FUNCTION__, n, base_path, p->name);
					ResolvedPath *rp_tmp = new ResolvedPath(base_path, p->name, (const char *)(extentions + i), p->id, NULL);
					fprintf(stderr, "PathResolver::%s 1\n", __FUNCTION__);
					if (rp_tmp != NULL) {
						ResolvedPath *tmp = storeTable(children, &n, rp_tmp);
						if (tmp != *children) fprintf(stderr, "PathResolver::%s: error returned pointers do not match\n");
					}
				}
			} else {
				fprintf(stderr, "PathResolver::%s: store entry %d: path: %s; file: %s\n", __FUNCTION__, n, base_path, p->name);
				ResolvedPath *rp_tmp = new ResolvedPath(base_path, p->name, p->id, NULL);
				fprintf(stderr, "PathResolver::%s 2\n", __FUNCTION__);
				if (rp_tmp != NULL) {
					ResolvedPath *tmp = storeTable(children, &n, rp_tmp);
					if (tmp != *children) fprintf(stderr, "PathResolver::%s: error returned pointers do not match\n");
				}
			}
		}
	}
	/* TODO: if rp has an active filter, apply filter as well */
	/*
	if (rp->hasFilter()) ...*/
	return (const int)n;
}

const mode_t PathResolver::getMode(path_type id) {
	fprintf(stderr, "PathResolver::%s(%d)\n", __FUNCTION__, id);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p == NULL) return 0;
	return p->mode;
}

const mode_t PathResolver::getMode(ResolvedPath *rp) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, rp);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	if (p == NULL) return 0;
	return p->mode;
}

const bool PathResolver::isDirectory(path_type id) {
	fprintf(stderr, "PathResolver::%s(%d)\n", __FUNCTION__, id);
	return S_ISDIR(this->getMode(id));
}

const bool PathResolver::isDirectory(ResolvedPath *rp) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, rp);
	return S_ISDIR(this->getMode(rp));
}

const bool PathResolver::isFile(path_type id) {
	fprintf(stderr, "PathResolver::%s(%d)\n", __FUNCTION__, id);
	return S_ISREG(this->getMode(id));
}

const bool PathResolver::isFile(ResolvedPath *rp) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, rp);
	return S_ISREG(this->getMode(rp));
}

const char *PathResolver::getName(path_type id) {
	fprintf(stderr, "PathResolver::%s(%d)\n", __FUNCTION__, id);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	return p->name;
}

const char *PathResolver::getName(ResolvedPath *rp) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, rp);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	return p->name;
}

const Libsad::FilterObjectType PathResolver::getTargetObject(path_type id) {
	fprintf(stderr, "PathResolver::%s(%d)\n", __FUNCTION__, id);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	return p->target_object;
}

const Libsad::FilterObjectType PathResolver::getTargetObject(ResolvedPath *rp) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, rp);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	return p->target_object;
}

PathResolver::PathDefinition *PathResolver::getDefinitionWithParentSingle(path_type id) {
	fprintf(stderr, "PathResolver::%s(%ld)\n", __FUNCTION__, id);

	for(PathResolver::PathDefinition *p = this->path_definitions; p->id != PATH_TYPE_LAST; p++) {
		if (p->parent_id == id)
		{
			return p;
		}
	}
	return NULL;
}

PathResolver::PathDefinition *PathResolver::getPathDefinitionRecord(path_type id) {
	fprintf(stderr, "PathResolver::%s(%ld)\n", __FUNCTION__, id);

	if (id <= PATH_TYPE_NONE || id >= PATH_TYPE_LAST) return NULL;
	PathDefinition *p = (PathDefinition *)(this->path_definitions + (off_t)id);

	if (id == p->id) return p;
	return NULL;
}

PathResolver::PathDefinition *PathResolver::getPathDefinitionRecord(ResolvedPath *rp) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, rp);
	path_type id = rp->getPathId();
	return PathResolver::getPathDefinitionRecord(id);
}

const char *PathResolver::getFullPath(path_type id) {
	fprintf(stderr, "PathResolver::%s(%d)\n", __FUNCTION__, id);
	PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p != NULL) return p->full_path;
	return NULL;
}

const char *PathResolver::getFullPath (PathDefinition *paths, path_type id) {
	fprintf(stderr, "PathResolver::%s(%p, %d)\n", __FUNCTION__, paths, id);

	if (id <= PATH_TYPE_NONE || id >= PATH_TYPE_LAST) return NULL;
	PathDefinition *p = (PathDefinition *)(paths + (off_t)id);
	if (p == NULL) return NULL;
	if (id != p->id) return NULL;
	return p->full_path;
}

const char *PathResolver::construct_full_name(const char *base, const char *name, const char* ext) {
	fprintf(stderr, "PathResolver::%s(%s, %s, %s)\n", __FUNCTION__, base, name, ext);

	if (name == NULL) return NULL;

	bool add_starting_slash = false;
	bool add_slash = false;
	bool add_dot = false;

	if ((base != NULL) && base[0] != '/') return NULL;

	size_t base_len = (base == NULL) ? 0 : strlen(base);
	size_t name_len = strlen(name);
	size_t ext_len = (ext == NULL) ? 0 : strlen(ext);
	size_t len = base_len + name_len + ext_len + 1; // +1 for \0 byte

	if ((base == NULL) && name[0] != '/') {
		add_starting_slash = true;
		len++;
	}
	if ((base != NULL) && base[base_len - 1] == '/' && name[0] == '/') {
		base_len--;
		len--;
	}
	if ((base != NULL) && base[base_len - 1] != '/' && name[0] != '/') {
		add_slash = true;
		len++;
	}
	if ((ext != NULL) && ext[0] != '.') {
		add_dot = true;
		len++;
	}

	char *dest = (char *)malloc(sizeof(char) * len);
	char *ptr = dest;

	if (add_starting_slash) {
		ptr[0] = '/';
		ptr++;
	}

	if ((base != NULL)) {
		strncpy(ptr, base, base_len);
		ptr += base_len;
	}

	if (add_slash) {
		ptr[0] = '/';
		ptr++;
	}

	strncpy(ptr, name, name_len);
	ptr += name_len;

	if (ext != NULL) {
		if (add_dot) {
			ptr[0] = '/';
			ptr++;
		}

		strncpy(ptr, ext, ext_len);
		ptr += ext_len;
	}
	ptr[0] = '\0';

	return (const char *)dest;
}

PathResolver::PathDefinition *PathResolver::parsePathDefinition(path_def *in) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, in);

	off_t alloc_size = (PATH_TYPE_LAST + 1) * sizeof(PathResolver::PathDefinition);
	PathResolver::PathDefinition *full_paths = (PathResolver::PathDefinition *)malloc(alloc_size);
	memset(full_paths, 0, alloc_size);
	fprintf(stderr, "%s: table: %p\n", __FUNCTION__, full_paths);
	fprintf(stderr, "%s: sizeof: %p\n", __FUNCTION__, sizeof(PathResolver::PathDefinition));

	for(path_def *p_in = in; p_in->id != PATH_TYPE_LAST; p_in++) {
		off_t offset = p_in->id;
		PathResolver::PathDefinition *p_out = full_paths + offset;
		fprintf(stderr, "%s: record: %p\n", __FUNCTION__, p_out);
		const char *full_path = PathResolver::construct_full_name(PathResolver::getFullPath (full_paths, p_in->parent_id), p_in->name, NULL);

		p_out->id = p_in->id;
		p_out->name = p_in->name;
		p_out->full_path = full_path;
		p_out->parent_id = p_in->parent_id;
		p_out->mode = p_in->mode;
		p_out->data_file = p_in->data_file;
		p_out->filter = p_in->filter;

		if (p_out->name == NULL) p_out->name_len = 0;
		else p_out->name_len = strlen(p_out->name);

		if (p_out->full_path == NULL) p_out->full_path_len = 0;
		else p_out->full_path_len = strlen(full_path);

		fprintf(stderr, "%s: record out %d: path: %s; parent_id: %d; mode %x; file handler: %d; filter: %d\n", __FUNCTION__, p_out->id, p_out->full_path, p_out->parent_id, p_out->mode, p_out->data_file, p_out->filter);
	}
	((PathResolver::PathDefinition *)(full_paths + PATH_TYPE_LAST))->id = PATH_TYPE_LAST;
	return full_paths;
}

ResolvedPath *storeTable(ResolvedPath **t, size_t *n, ResolvedPath *e) {
	fprintf(stderr, "%s(%p, %d, %p)\n", __FUNCTION__, *t, *n, e);

	// Calculate new size of table.
	size_t new_size = (*n + 1) * sizeof(ResolvedPath);

	// Reallocate new table with new size.
	*t = (ResolvedPath *)realloc (*t, new_size);

	// Save the element in the entry.
	*((uintptr_t *)(*t + (off_t)(*n))) = (uintptr_t)e;

	fprintf(stderr, "entry %p %p %ld (%p)\n", *t + (off_t)(*n), (uintptr_t *)(*t + (off_t)(*n)), *((uintptr_t *)(*t + (off_t)(*n))), *((uintptr_t *)(*t + (off_t)(*n))));

	// Increase the number of elements.
	(*n)++;

	// Return the (new) location of the table.
	return *t;
}
