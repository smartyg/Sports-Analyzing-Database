#include "config.h"

#include "pathresolver.hpp"

#include <stddef.h>
#include <cstdlib>
#include <memory>
#include <cstring>

#include "resolvedpath.hpp"
#include "utils.hpp"
#include "libsad.hpp"
#include "libsadplugin.hpp"

PathResolver::PathResolver(Libsad *lib, path_def *path_definitions) {
	fprintf(stderr, "PathResolver::%s(%p, %p)\n", __FUNCTION__, lib, path_definitions);
	this->lib = lib;
	this->path_definitions = PathResolver::parsePathDefinition(path_definitions);
}

PathResolver::~PathResolver(void) {
	fprintf(stderr, "PathResolver::%s()\n", __FUNCTION__);
	free(this->path_definitions);
}

/* Getters */
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

const char *PathResolver::getName(path_type id) {
	fprintf(stderr, "PathResolver::%s(%d)\n", __FUNCTION__, id);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p == NULL) return NULL;
	return p->name;
}

const char *PathResolver::getName(ResolvedPath *rp) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, rp);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	if (p == NULL) return NULL;
	return p->name;
}

const path_type PathResolver::getParent(path_type id) {
	fprintf(stderr, "PathResolver::%s(%d)\n", __FUNCTION__, id);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p == NULL) return PATH_TYPE_NONE;
	return (path_type)p->parent_id;
}

const path_type PathResolver::getParent(ResolvedPath *rp) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, rp);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	if (p == NULL) return PATH_TYPE_NONE;
	return (path_type)p->parent_id;
}

const Libsad::FilterObjectType PathResolver::getTargetObjectType(path_type id) {
	fprintf(stderr, "PathResolver::%s(%d)\n", __FUNCTION__, id);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p == NULL) return Libsad::OBJECT_NONE;
	return (Libsad::FilterObjectType)p->filter_object;
}

const Libsad::FilterObjectType PathResolver::getTargetObjectType(ResolvedPath *rp) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, rp);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	if (p == NULL) return Libsad::OBJECT_NONE;
	return (Libsad::FilterObjectType)p->filter_object;
}

const LibsadPlugin::DataFileHandlerType PathResolver::getDataFileHandler(path_type id) {
	fprintf(stderr, "PathResolver::%s(%d)\n", __FUNCTION__, id);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p == NULL) return LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE;
	return (LibsadPlugin::DataFileHandlerType)p->data_file_handler;
}

const LibsadPlugin::DataFileHandlerType PathResolver::getDataFileHandler(ResolvedPath *rp) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, rp);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	if (p == NULL) return LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE;
	return (LibsadPlugin::DataFileHandlerType)p->data_file_handler;
}

/* Booleans */
const bool PathResolver::hasParent(path_type id) {
	fprintf(stderr, "PathResolver::%s(%d)\n", __FUNCTION__, id);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p == NULL) return false;
	return PathResolver::isValidPathId((path_type)p->parent_id);
}

const bool PathResolver::hasParent(ResolvedPath *rp) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, rp);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	if (p == NULL) return false;
	return PathResolver::isValidPathId((path_type)p->parent_id);
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

const bool PathResolver::hasDataFileHandler(path_type id) {
	fprintf(stderr, "PathResolver::%s(%d)\n", __FUNCTION__, id);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p == NULL) return false;
	return (p-> data_file_handler != Libsad::OBJECT_NONE);
}

const bool PathResolver::hasDataFileHandler(ResolvedPath *rp) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, rp);
	PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	if (p == NULL) return false;
	return (p-> data_file_handler != Libsad::OBJECT_NONE);
}

/* Checkers */
const bool PathResolver::isValidPathId(path_type id) {
	fprintf(stderr, "PathResolver::%s()\n", __FUNCTION__);
	return ((id > PATH_TYPE_NONE) && (id < PATH_TYPE_LAST));
}

/* Private methods */
PathResolver::PathDefinition *PathResolver::getPathDefinitionRecord (path_type id) {
	fprintf(stderr, "PathResolver::%s(%ld)\n", __FUNCTION__, id);

	if (id <= PATH_TYPE_NONE || id >= PATH_TYPE_LAST) return NULL;
	PathDefinition *p = (PathDefinition *)(this->path_definitions + (size_t)id);

	if (id == p->id) return p;
	return NULL;
}

PathResolver::PathDefinition *PathResolver::getPathDefinitionRecord (ResolvedPath *rp) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, rp);
	if (rp == NULL) return NULL;
	path_type id = rp->getPathId();
	return PathResolver::getPathDefinitionRecord(id);
}

const char *PathResolver::getFullPath (path_type id) {
	fprintf(stderr, "PathResolver::%s(%d)\n", __FUNCTION__, id);
	PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p != NULL) return p->full_path;
	return NULL;
}

const char *PathResolver::getFullPath (PathDefinition *paths, path_type id) {
	fprintf(stderr, "PathResolver::%s(%p, %d)\n", __FUNCTION__, paths, id);

	if (id <= PATH_TYPE_NONE || id >= PATH_TYPE_LAST) return NULL;
	PathDefinition *p = (PathDefinition *)(paths + (size_t)id);
	if (p == NULL) return NULL;
	if (id != p->id) return NULL;
	return p->full_path;
}

PathResolver::PathDefinition *PathResolver::parsePathDefinition (path_def *in) {
	fprintf(stderr, "PathResolver::%s(%p)\n", __FUNCTION__, in);

	size_t alloc_size = (PATH_TYPE_LAST + 1) * sizeof(PathResolver::PathDefinition);
	PathResolver::PathDefinition *full_paths = (PathResolver::PathDefinition *)malloc(alloc_size);
	memset(full_paths, 0, alloc_size);
	fprintf(stderr, "%s: table: %p\n", __FUNCTION__, full_paths);
	fprintf(stderr, "%s: sizeof: %p\n", __FUNCTION__, sizeof(PathResolver::PathDefinition));

	for(path_def *p_in = in; p_in->id != PATH_TYPE_LAST; p_in++) {
		size_t offset = p_in->id;
		PathResolver::PathDefinition *p_out = full_paths + offset;
		fprintf(stderr, "%s: record: %p\n", __FUNCTION__, p_out);
		const char *full_path = Utils::makeFilePath(PathResolver::getFullPath (full_paths, p_in->parent_id), p_in->name, NULL);

		p_out->id = p_in->id;
		p_out->name = p_in->name;
		p_out->full_path = full_path;
		p_out->parent_id = p_in->parent_id;
		p_out->mode = p_in->mode;
		p_out->data_file_handler = p_in->data_file_handler;
		p_out->filters = p_in->filters;
		p_out->filter_object = p_in->filter_object;

		if (p_out->name == NULL) p_out->name_len = 0;
		else p_out->name_len = strlen(p_out->name);

		if (p_out->full_path == NULL) p_out->full_path_len = 0;
		else p_out->full_path_len = strlen(full_path);

		fprintf(stderr, "%s: record out %d: name: %s; path: %s; parent_id: %d; mode %x; file handler: %d; filters: %d; filter object: %d\n", __FUNCTION__, p_out->id, p_out->name, p_out->full_path, p_out->parent_id, p_out->mode, p_out->data_file_handler, p_out->filters, p_out->filter_object);
	}
	((PathResolver::PathDefinition *)(full_paths + PATH_TYPE_LAST))->id = PATH_TYPE_LAST;
	return full_paths;
}
