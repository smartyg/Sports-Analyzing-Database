#ifndef _X_RESOLVEDPATH_HPP_
// This is to make sure the header file are loaded in the correct order, first resolvedpath.hpp than this one.
#include "resolvedpath.hpp"
#else
#ifndef _X_PATHRESOLVER_HPP_
#define _X_PATHRESOLVER_HPP_

#include <stddef.h>

class PathResolver;

#include <cstdint>

#include "resolvedpath.hpp"
#include "path.hpp"
#include "libsad.hpp"
#include "libsadplugin.hpp"

#define IS_SET(v, m) (((v) & (m)) == (m))

#define LABEL_FILTER_BY_DATE "by-date"
#define LABEL_FILTER_BY_TITLE "by-title"
#define LABEL_FILTER_BY_ACTIVITY "by-activity"
#define LABEL_FILTER_BY_ID ".by-id"

class PathResolver {
public:
	typedef enum : uint_fast8_t {
		FILTER_NONE = 0x0,
		FILTER_BY_DATE = 0x1,
		FILTER_BY_TITLE = 0x2,
		FILTER_BY_ACTIVITY = 0x4,
		FILTER_BY_ID = 0x8,
		FILTER_LAST = 0xff
	} BaseFilter;

	PathResolver(const Libsad *lib, path_def *path_definitions);
	~PathResolver(void);

	const ResolvedPath *resolve(const char *) const;
	size_t getChildEntries(const ResolvedPath *, ResolvedPath **) const;

	/* Getters */
	mode_t getMode(path_type) const;
	mode_t getMode(const ResolvedPath *) const;
	const char *getName(path_type) const;
	const char *getName(const ResolvedPath *) const;
	path_type getParent(path_type) const;
	path_type getParent(const ResolvedPath *) const;
	Libsad::FilterObjectType getTargetObjectType(path_type) const;
	Libsad::FilterObjectType getTargetObjectType(const ResolvedPath *) const;
	LibsadPlugin::DataFileHandlerType getDataFileHandler(path_type) const;
	LibsadPlugin::DataFileHandlerType getDataFileHandler(const ResolvedPath *) const;

	/* Booleans */
	bool hasParent(path_type) const;
	bool hasParent(const ResolvedPath *) const;
	bool isDirectory(path_type) const;
	bool isDirectory(const ResolvedPath *) const;
	bool isFile(path_type) const;
	bool isFile(const ResolvedPath *) const;
	bool hasDataFileHandler(path_type) const;
	bool hasDataFileHandler(const ResolvedPath *) const;

	/* Checkers */
	static bool isValidPathId(path_type);

private:
	typedef struct {
		// path_type id (unsigned 8 bits)
		// path_type parent_id (unsigned 8 bits)
		// unused (4 bits)
		// size_t name_len (unsigned 12 bits)
		// mode_t mode (unsigned 16 bits)
		// LibsadPlugin::DataFileHandlerType data_file_handler (unsigned 8 bits)
		// PathResolver::BaseFilter filters (unsigned 8 bits)

		// unused (8 bits)
		// Libsad::FilterObjectType filter_object (unsigned 8 bits)
		// size_t full_path_len (unsigned 16 bits)

		uint_fast64_t id:8, parent_id:8, :4, name_len:12, mode:16, data_file_handler:8, filters:8;
		uint_fast32_t :8, filter_object:8, full_path_len:16;
		const char *name;
		const char *full_path;
	} PathDefinition;

	const Libsad *lib;
	const PathResolver::PathDefinition *path_definitions;

	const PathResolver::PathDefinition *getPathDefinitionRecord(path_type id) const;
	const PathResolver::PathDefinition *getPathDefinitionRecord(const ResolvedPath *) const;
	const char *getFullPath(path_type id) const;
	static const char *getFullPath(const PathDefinition *full_paths, path_type id);
	static const PathResolver::PathDefinition *parsePathDefinition(path_def *in);

	/* From pathresolver-resolve.cpp */
	const ResolvedPath *resolveInternal(const char *, size_t, path_type, BaseFilter, Libsad::FilterInfoType *) const;
	bool isFromParent(const PathDefinition *, path_type) const;

};

#endif /* _X_PATHRESOLVER_HPP_ */
#endif
