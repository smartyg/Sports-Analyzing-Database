#include "config.h"

#include "resolvedpath.hpp"

#include <stdio.h>
#include <cstdlib>
#include <cstring>

#include "pathresolver.hpp"
#include "path.hpp"
#include "utils.hpp"

ResolvedPath::ResolvedPath(const char *base, const char *filename, const char *extention, path_type id, const bool f, const PathResolver::BaseFilter bf, const Libsad::FilterInfoType fi) {
	fprintf(stderr, "new ResolvedPath::%s(%s, %s, %s, %d, %d, %d, %p) at %p\n", __func__, base, filename, extention, id, f, bf, &fi, this);
	this->path = Utils::makeFilePath(base, filename, extention);
	this->path_id = id;
	this->has_filter = f;
	this->base_filter = bf;
	this->filter_info = fi;

	this->filename_offset = static_cast<uint_fast16_t>((strrchr(this->path, '/') - this->path) + 1);
}

ResolvedPath::ResolvedPath(const char *base, const char *filename, path_type id, const bool f, const PathResolver::BaseFilter bf, const Libsad::FilterInfoType fi): ResolvedPath(base, filename, NULL, id, f, bf,  fi) {}
ResolvedPath::ResolvedPath(const char *name, path_type id, const bool f, const PathResolver::BaseFilter bf, const Libsad::FilterInfoType fi): ResolvedPath(NULL, name, NULL, id, f, bf, fi) {}

ResolvedPath::ResolvedPath(const char *base, const char *filename, const char *extention, path_type id): ResolvedPath(base, filename, extention, id, false, PathResolver::FILTER_NONE, {0}) {}
ResolvedPath::ResolvedPath(const char *base, const char *filename, path_type id): ResolvedPath(base, filename, NULL, id, false, PathResolver::FILTER_NONE, {0}) {}
ResolvedPath::ResolvedPath(const char *name, path_type id): ResolvedPath(NULL, name, NULL, id, false, PathResolver::FILTER_NONE, {0}) {}

ResolvedPath::~ResolvedPath(void)
{
	fprintf(stderr, "ResolvedPath::%s(%s) at %p\n", __func__, this->path, this);
	if (this->path != NULL) free(const_cast<char *>(this->path));
	if (this->filter_info.filter_title != NULL) free(const_cast<char *>(this->filter_info.filter_title));
}

/* Getters */
path_type ResolvedPath::getPathId (void) const {
	fprintf(stderr, "ResolvedPath::%s()\n", __func__);
	return static_cast<path_type>(this->path_id);
}

const char *ResolvedPath::getPath (void) const {
	fprintf(stderr, "ResolvedPath::%s()\n", __func__);
	return this->path;
}

const char *ResolvedPath::getFilename(void) const {
	fprintf(stderr, "ResolvedPath::%s()\n", __func__);
	return static_cast<const char *>(this->path + this->filename_offset);
}

PathResolver::BaseFilter ResolvedPath::getPathFilterBase(void) const {
	fprintf(stderr, "ResolvedPath::%s()\n", __func__);
	return static_cast<PathResolver::BaseFilter>(this->base_filter);
}

const Libsad::FilterInfoType ResolvedPath::getPathFilterInfo(void) const {
	fprintf(stderr, "ResolvedPath::%s()\n", __func__);
	return this->filter_info;
}

Libsad::objectId ResolvedPath::getObjectId(void) const {
	fprintf(stderr, "ResolvedPath::%s()\n", __func__);
	if (this->filter_info.object_type != TYPE_OBJECT_ID) return 0;
	return this->filter_info.object;
}

Libsad::objectTime ResolvedPath::getObjectTime(void) const {
	fprintf(stderr, "ResolvedPath::%s()\n", __func__);
	if (this->filter_info.object_type != TYPE_OBJECT_TIME) return 0;
	return this->filter_info.object;
}

/* Booleans */
//const bool ResolvedPath::hasPathFilterInfo(void) {
bool ResolvedPath::hasFilter(void) const {
	fprintf(stderr, "ResolvedPath::%s()\n", __func__);
	return static_cast<bool>(this->has_filter);
}

bool ResolvedPath::isFilterComplete(void) const {
	fprintf(stderr, "ResolvedPath::%s()\n", __func__);
	return (this->has_filter && ((this->filter_info.object_type != TYPE_OBJECT_EMPTY && this->filter_info.object != 0) || (this->filter_info.filter_year != 0 && this->filter_info.filter_month != 0 && this->filter_info.filter_day != 0 && (this->filter_info.filter_hour != 0 || this->filter_info.filter_minute != 0 || this->filter_info.filter_second != 0))));
}

bool ResolvedPath::hasObjectId(void) const {
	fprintf(stderr, "ResolvedPath::%s()\n", __func__);
	return (this->filter_info.object_type == TYPE_OBJECT_ID && this->filter_info.object != 0);
}

bool ResolvedPath::hasObjectTime(void) const {
	fprintf(stderr, "ResolvedPath::%s()\n", __func__);
	return (this->filter_info.object_type == TYPE_OBJECT_TIME && this->filter_info.object != 0);
}

/* Checker */
bool ResolvedPath::isValid(void) const {
	fprintf(stderr, "ResolvedPath::%s()\n", __func__);
	return ((this->path_id > PATH_TYPE_NONE) && (this->path_id < PATH_TYPE_LAST) && this->path != NULL);
}
