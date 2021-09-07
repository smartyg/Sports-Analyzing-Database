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
#include "logger.hpp"

PathResolver::PathResolver(const Libsad *l, path_def *pd) {
	DEBUG("PathResolver::%s(%p, %p)\n", __func__, lib, pd);
	this->lib = l;
	this->path_definitions = PathResolver::parsePathDefinition(pd);
}

PathResolver::~PathResolver(void) {
	DEBUG("PathResolver::%s()\n", __func__);
	free(const_cast<PathDefinition *>(this->path_definitions));
}

/* Getters */
mode_t PathResolver::getMode(path_type id) const {
	DEBUG("PathResolver::%s(%d)\n", __func__, id);
	const PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p == NULL) return 0;
	return p->mode;
}

mode_t PathResolver::getMode(const ResolvedPath *rp) const {
	DEBUG("PathResolver::%s(%p)\n", __func__, rp);
	const PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	if (p == NULL) return 0;
	return p->mode;
}

const char *PathResolver::getName(path_type id) const {
	DEBUG("PathResolver::%s(%d)\n", __func__, id);
	const PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p == NULL) return NULL;
	return p->name;
}

const char *PathResolver::getName(const ResolvedPath *rp) const {
	DEBUG("PathResolver::%s(%p)\n", __func__, rp);
	const PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	if (p == NULL) return NULL;
	return p->name;
}

path_type PathResolver::getParent(path_type id) const {
	DEBUG("PathResolver::%s(%d)\n", __func__, id);
	const PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p == NULL) return PATH_TYPE_NONE;
	return static_cast<path_type>(p->parent_id);
}

path_type PathResolver::getParent(const ResolvedPath *rp) const {
	DEBUG("PathResolver::%s(%p)\n", __func__, rp);
	const PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	if (p == NULL) return PATH_TYPE_NONE;
	return static_cast<path_type>(p->parent_id);
}

Libsad::FilterObjectType PathResolver::getTargetObjectType(path_type id) const {
	DEBUG("PathResolver::%s(%d)\n", __func__, id);
	const PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p == NULL) return Libsad::OBJECT_NONE;
	return static_cast<Libsad::FilterObjectType>(p->filter_object);
}

Libsad::FilterObjectType PathResolver::getTargetObjectType(const ResolvedPath *rp) const {
	DEBUG("PathResolver::%s(%p)\n", __func__, rp);
	const PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	if (p == NULL) return Libsad::OBJECT_NONE;
	return static_cast<Libsad::FilterObjectType>(p->filter_object);
}

LibsadPlugin::DataFileHandlerType PathResolver::getDataFileHandler(path_type id) const {
	DEBUG("PathResolver::%s(%d)\n", __func__, id);
	const PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p == NULL) return LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE;
	return static_cast<LibsadPlugin::DataFileHandlerType>(p->data_file_handler);
}

LibsadPlugin::DataFileHandlerType PathResolver::getDataFileHandler(const ResolvedPath *rp) const {
	DEBUG("PathResolver::%s(%p)\n", __func__, rp);
	const PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	if (p == NULL) return LibsadPlugin::PLUGIN_TYPE_DATA_FILE_NONE;
	return static_cast<LibsadPlugin::DataFileHandlerType>(p->data_file_handler);
}

/* Booleans */
bool PathResolver::hasParent(path_type id) const {
	DEBUG("PathResolver::%s(%d)\n", __func__, id);
	const PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p == NULL) return false;
	return PathResolver::isValidPathId(static_cast<path_type>(p->parent_id));
}

bool PathResolver::hasParent(const ResolvedPath *rp) const {
	DEBUG("PathResolver::%s(%p)\n", __func__, rp);
	const PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	if (p == NULL) return false;
	return PathResolver::isValidPathId(static_cast<path_type>(p->parent_id));
}

bool PathResolver::isDirectory(path_type id) const {
	DEBUG("PathResolver::%s(%d)\n", __func__, id);
	return S_ISDIR(this->getMode(id));
}

bool PathResolver::isDirectory(const ResolvedPath *rp) const {
	DEBUG("PathResolver::%s(%p)\n", __func__, rp);
	return S_ISDIR(this->getMode(rp));
}

bool PathResolver::isFile(path_type id) const {
	DEBUG("PathResolver::%s(%d)\n", __func__, id);
	return S_ISREG(this->getMode(id));
}

bool PathResolver::isFile(const ResolvedPath *rp) const {
	DEBUG("PathResolver::%s(%p)\n", __func__, rp);
	return S_ISREG(this->getMode(rp));
}

bool PathResolver::hasDataFileHandler(path_type id) const {
	DEBUG("PathResolver::%s(%d)\n", __func__, id);
	const PathResolver::PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p == NULL) return false;
	return (p-> data_file_handler != Libsad::OBJECT_NONE);
}

bool PathResolver::hasDataFileHandler(const ResolvedPath *rp) const {
	DEBUG("PathResolver::%s(%p)\n", __func__, rp);
	const PathResolver::PathDefinition *p = this->getPathDefinitionRecord(rp);
	if (p == NULL) return false;
	return (p-> data_file_handler != Libsad::OBJECT_NONE);
}

/* Checkers */
bool PathResolver::isValidPathId(path_type id) {
	DEBUG("PathResolver::%s()\n", __func__);
	return ((id > PATH_TYPE_NONE) && (id < PATH_TYPE_LAST));
}

/* Private methods */
const PathResolver::PathDefinition *PathResolver::getPathDefinitionRecord (path_type id) const {
	DEBUG("PathResolver::%s(%d)\n", __func__, id);

	if (id <= PATH_TYPE_NONE || id >= PATH_TYPE_LAST) return NULL;
	const PathDefinition *p = this->path_definitions + static_cast<size_t>(id);

	if (id == p->id) return p;
	return NULL;
}

const PathResolver::PathDefinition *PathResolver::getPathDefinitionRecord (const ResolvedPath *rp) const {
	DEBUG("PathResolver::%s(%p)\n", __func__, rp);
	if (rp == NULL) return NULL;
	path_type id = rp->getPathId();
	return PathResolver::getPathDefinitionRecord(id);
}

const char *PathResolver::getFullPath (path_type id) const {
	DEBUG("PathResolver::%s(%d)\n", __func__, id);
	const PathDefinition *p = this->getPathDefinitionRecord(id);
	if (p != NULL) return p->full_path;
	return NULL;
}

const char *PathResolver::getFullPath (const PathDefinition *pd, path_type id) {
	DEBUG("PathResolver::%s(%p, %d)\n", __func__, pd, id);

	if (id <= PATH_TYPE_NONE || id >= PATH_TYPE_LAST) return NULL;
	const PathDefinition *p = static_cast<const PathDefinition *>(pd + static_cast<size_t>(id));
	if (p == NULL) return NULL;
	if (id != p->id) return NULL;
	return p->full_path;
}

const PathResolver::PathDefinition *PathResolver::parsePathDefinition (path_def *in) {
	DEBUG("PathResolver::%s(%p)\n", __func__, in);

	size_t alloc_size = (PATH_TYPE_LAST + 1) * sizeof(PathResolver::PathDefinition);
	PathResolver::PathDefinition *full_paths = static_cast<PathResolver::PathDefinition *>(malloc (alloc_size));
	memset(full_paths, 0, alloc_size);
	DEBUG("table: %p\n", full_paths);
	DEBUG("sizeof: %ld\n", sizeof(PathResolver::PathDefinition));

	for(path_def *p_in = in; p_in->id != PATH_TYPE_LAST; p_in++) {
		size_t offset = p_in->id;
		PathResolver::PathDefinition *p_out = full_paths + offset;
		DEBUG("record: %p\n", p_out);
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

		DEBUG("record out %ld: name: %s; path: %s; parent_id: %ld; mode %lx; file handler: %ld; filters: %ld; filter object: %ld\n", p_out->id, p_out->name, p_out->full_path, p_out->parent_id, p_out->mode, p_out->data_file_handler, p_out->filters, p_out->filter_object);
	}
	(static_cast<PathResolver::PathDefinition *>(full_paths + PATH_TYPE_LAST))->id = PATH_TYPE_LAST;
	return full_paths;
}
