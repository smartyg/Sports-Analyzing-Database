#include "config.h"

#include "resolvedpath.hpp"

#include <stdio.h>
#include <cstdlib>
#include <cstring>

#include "pathresolver.hpp"
#include "path.hpp"
#include "utils.hpp"

ResolvedPath::ResolvedPath(const char *path, const char *filename, const char *extention, path_type id, const PathResolver::BaseFilter base_filter, const Libsad::FilterInfoType filter_info) {
	fprintf(stderr, "new ResolvedPath::%s(%s, %s, %s, %d, %d, %p) at %p\n", __FUNCTION__, path, filename, extention, id, base_filter, filter_info, this);
	this->path = Utils::makeFilePath(path, filename, extention);
	this->path_id = id;
	this->base_filter = base_filter;
	this->filter_info = filter_info;
/*
	if (filter_info != NULL) {
		this->filter_info.filter_year               = filter_info.filter_year;
		this->filter_info.filter_month              = filter_info.filter_month;
		this->filter_info.filter_day                = filter_info.filter_day;
		this->filter_info.filter_time               = filter_info.filter_time;
		this->filter_info.filter_activity           = filter_info.filter_activity;
		this->filter_info.filter_title_first_letter = filter_info.filter_title_first_letter;
		this->filter_info.filter_title              = strdup(filter_info.filter_title);
		this->filter_info.object_type               = filter_info.object_type;
		this->filter_info.object                    = filter_info.object;
	}
*/
	this->filename_offset = (uint_fast16_t)(((char *)strrchr(this->path, '/') - (char *)(this->path)) + 1);
	fprintf(stderr, "done\n");
}

ResolvedPath::ResolvedPath(const char *path, const char *filename, path_type id, const PathResolver::BaseFilter base_filter, const Libsad::FilterInfoType filter_info): ResolvedPath(path, filename, NULL, id, base_filter,  filter_info) {}
ResolvedPath::ResolvedPath(const char *name, path_type id, const PathResolver::BaseFilter base_filter, const Libsad::FilterInfoType filter_info): ResolvedPath(NULL, name, NULL, id, base_filter, filter_info) {}

ResolvedPath::ResolvedPath(const char *path, const char *filename, const char *extention, path_type id): ResolvedPath(path, filename, extention, id, PathResolver::FILTER_NONE, {0}) {}
ResolvedPath::ResolvedPath(const char *path, const char *filename, path_type id): ResolvedPath(path, filename, NULL, id, PathResolver::FILTER_NONE, {0}) {}
ResolvedPath::ResolvedPath(const char *name, path_type id): ResolvedPath(NULL, name, NULL, id, PathResolver::FILTER_NONE, {0}) {}

ResolvedPath::~ResolvedPath(void)
{
	fprintf(stderr, "ResolvedPath::%s(%s) at %p\n", __FUNCTION__, this->path, this);
	if (this->path != NULL) free((void *)(this->path));
	if (this->filter_info.filter_title != NULL) free((void *)(this->filter_info.filter_title));
}

/* Getters */
const path_type ResolvedPath::getPathId(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return (path_type)this->path_id;
}

const char *ResolvedPath::getPath(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return this->path;
}

const char *ResolvedPath::getFilename(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return (const char *)(this->path + this->filename_offset);
}

const PathResolver::BaseFilter ResolvedPath::getPathFilterBase(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return (PathResolver::BaseFilter)this->base_filter;
}

const Libsad::FilterInfoType ResolvedPath::getPathFilterInfo(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return this->filter_info;
}

const Libsad::objectId ResolvedPath::getObjectId(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	if (this->filter_info.object_type != TYPE_OBJECT_ID) return 0;
	return this->filter_info.object;
}

const Libsad::objectTime ResolvedPath::getObjectTime(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	if (this->filter_info.object_type != TYPE_OBJECT_TIME) return 0;
	return this->filter_info.object;
}

/* Booleans */
const bool ResolvedPath::hasPathFilterInfo(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return (this->filter_info.object_type != Libsad::OBJECT_NONE || this->base_filter != PathResolver::FILTER_NONE);
}

const bool ResolvedPath::hasObjectId(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return (this->filter_info.object_type == TYPE_OBJECT_ID && this->filter_info.object != 0);
}

const bool ResolvedPath::hasObjectTime(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return (this->filter_info.object_type == TYPE_OBJECT_TIME && this->filter_info.object != 0);
}

/* Checker */
const bool ResolvedPath::isValid(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return ((this->path_id > PATH_TYPE_NONE) && (this->path_id < PATH_TYPE_LAST) && this->path != NULL);
}
