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

Libsad::Libsad() {
	/* TODO */
	this->plugin = new LibsadPlugin();
}

Libsad::~Libsad() {
	/* TODO */
}

const Libsad::objectId Libsad::getObjectId(objectTime time) {
	/* TODO */
	return 1;
}

size_t Libsad::freeStorageSpace() {
	/* TODO */
	return 1;
}

size_t Libsad::availibleStorageSpace() {
	/* TODO */
	return 1;
}

bool Libsad::deleteObject(objectId id) {
	/* TODO */
	return true;
}

LibsadPlugin *Libsad::getPluginHandler(void) {
	return this->plugin;
}

int Libsad::getFilteredList(FilterObjectType o, FilterListType l, const FilterInfoType i, char **entries) {
	return 0;
}

uint_fast8_t Libsad::getActivityCodeFromName(const char *name, size_t len) {
	return 1;
}
