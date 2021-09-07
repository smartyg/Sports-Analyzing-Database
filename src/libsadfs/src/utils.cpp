#include "config.h"

#include "utils.hpp"

#include <cstdlib>
#include <cstring>
#include <memory>

const char *Utils::makeFilePath(const char *base, const char *name, const char* ext) {
	fprintf(stderr, "Utils::%s(%s, %s, %s)\n", __FUNCTION__, base, name, ext);

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

	const char *dest = static_cast<const char *>(malloc(sizeof(char) * len));
	char *ptr = const_cast<char *>(dest);

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

	//return (const char *)dest;
	return dest;
}
