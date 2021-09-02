#ifndef _X_LIBSAD_HPP_
#define _X_LIBSAD_HPP_

#include <stddef.h>

#include "libsadplugin.hpp"

class Libsad {
public:
	typedef unsigned int objectId;
	typedef unsigned long int objectTime;

	Libsad();
	~Libsad();

	const objectId getObjectId(objectTime time);
	size_t freeStorageSpace(void);
	size_t availibleStorageSpace(void);
	bool deleteObject(objectId id);
	LibsadPlugin *getPluginHandler(void);

private:
	LibsadPlugin *plugin;
};

#endif /* _X_LIBSAD_HPP_ */