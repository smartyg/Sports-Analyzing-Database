#include "config.h"

#include "resolvedpath.hpp"

#include <stdio.h>
#include <cstdlib>
#include <cstring>

#include "pathresolver.hpp"
#include "pathfilter.hpp"
#include "path.hpp"
#include "utils.hpp"

ResolvedPath::ResolvedPath(const char *path, path_type path_id, PathFilter *filter)
{
	fprintf(stderr, "new ResolvedPath::%s(%s, %d, %p) at %p\n", __FUNCTION__, path, path_id, filter, this);
	this->path = strdup(path);
	this->path_id = path_id;
	this->filter = filter;

	// Find filename part
	this->filename_offset = ((char *)strrchr(this->path, '/') - (char *)(this->path)) + 1;
}

ResolvedPath::ResolvedPath(const char *base_path, const char *name, path_type path_id, PathFilter *filter)
{
	fprintf(stderr, "new ResolvedPath::%s(%s, %s, %d, %p) at %p\n", __FUNCTION__, base_path, name, path_id, filter, this);
	this->path = Utils::makeFilePath(base_path, name, NULL);
	this->path_id = path_id;
	this->filter = filter;

	this->filename_offset = ((char *)strrchr(this->path, '/') - (char *)(this->path)) + 1;
}

ResolvedPath::ResolvedPath(const char *base_path, const char *name, const char *ext, path_type path_id, PathFilter *filter)
{
	fprintf(stderr, "new ResolvedPath::%s(%s, %s, %s, %d, %p) at %p\n", __FUNCTION__, base_path, name, ext, path_id, filter, this);
	this->path = Utils::makeFilePath(base_path, name, ext);
	this->path_id = path_id;
	this->filter = filter;

	this->filename_offset = ((char *)strrchr(this->path, '/') - (char *)(this->path)) + 1;
}

ResolvedPath::~ResolvedPath(void)
{
	fprintf(stderr, "ResolvedPath::%s(%s) at %p\n", __FUNCTION__, this->path, this);
	if (this->path != NULL) free((void *)(this->path));
	if (this->filter != NULL) delete this->filter;
}

const path_type ResolvedPath::getPathId(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return this->path_id;
}

const bool ResolvedPath::hasPathFilter(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return ((this->filter != NULL) && (this->filter->getPathType() == this->path_id) && (this->filter->isValid(this->filter->getAvailibleFilters())));
}

PathFilter *ResolvedPath::getPathFilter(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return this->filter;
}

const char *ResolvedPath::getPath(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return this->path;
}

const char *ResolvedPath::getFilename(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return (const char *)(this->path + this->filename_offset);
}

const bool ResolvedPath::isValid(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	return ((this->path_id > PATH_TYPE_NONE) && (this->path_id < PATH_TYPE_LAST));
}

const bool ResolvedPath::hasObjectId(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	/* TODO */
	return true;
}

const bool ResolvedPath::hasObjectTime(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	/* TODO */
	return true;
}

const Libsad::objectId ResolvedPath::getObjectId(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	/* TODO */
	return 1;
}

const Libsad::objectTime ResolvedPath::getObjectTime(void) {
	fprintf(stderr, "ResolvedPath::%s()\n", __FUNCTION__);
	/* TODO */
	return 1;
}
