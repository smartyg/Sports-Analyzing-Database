#include "config.h"

#include "libsad.hpp"

#include <iostream>
#include <cstring>

#include "libsadplugin.hpp"

// bool match_data_file_handler_extention(const char *ext, data_file_suffix t)
// {
// 	std::cout << "match_data_file_handler_extention(" << ext << ", " << t << ")" << std::endl;
// 	if (t == DATA_FILE_SUFFIX_TABLE && strcmp(ext, ".csv") == 0) return true;
// 	else return false;
// }

Libsad::Libsad (void) {
	/* TODO */
	this->plugin = new LibsadPlugin();
}

Libsad::~Libsad (void) {
	/* TODO */
}

Libsad::objectId Libsad::getObjectId (const objectTime time) const {
	(void) time;
	/* TODO */
	return 1;
}

size_t Libsad::freeStorageSpace (void) const {
	/* TODO */
	return 1;
}

size_t Libsad::availibleStorageSpace (void) const {
	/* TODO */
	return 1;
}

bool Libsad::deleteObject (objectId id) const {
	/* TODO */
	(void) id;
	return true;
}

const LibsadPlugin *Libsad::getPluginHandler (void) const {
	return this->plugin;
}

size_t Libsad::getFilteredList (FilterObjectType o, FilterListType l, const FilterInfoType i, char **entries) const {
	(void) o;
	(void) l;
	(void) i;
	(void) entries;
	return 0;
}

uint_fast8_t Libsad::getActivityCodeFromName (const char *name, size_t len) const {
	(void) name;
	(void) len;
	return 1;
}
