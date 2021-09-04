#ifndef _X_LIBSAD_HPP_
#define _X_LIBSAD_HPP_

#include <stddef.h>

#include "libsadplugin.hpp"

class Libsad {
public:
	typedef unsigned int objectId;
	typedef unsigned long int objectTime;

	typedef struct {
		unsigned int filter_year:12;
		unsigned int filter_month:4;
		unsigned int filter_day:5;
		unsigned int filter_time:18;
		unsigned int filter_activity:8;
		unsigned int filter_title_first_letter:8;
		unsigned long int filtered_start_time:42;
		const char *filter_title;
	} FilterInfoType;

	typedef enum {
		LIST_TYPE_NONE = 0,
		LIST_TYPE_ACTIVITIES,
		LIST_TYPE_YEARS,
		LIST_TYPE_MONTHS,
		LIST_TYPE_DAYS,
		LIST_TYPE_TIME,
		LIST_TYPE_FIRST_LETTER_TITLES,
		LIST_TYPE_TITLES,
		LIST_TYPE_IDS,
		LIST_TYPE_LAST
	} FilterListType;

	typedef enum {
		OBJECT_NONE = 0,
		OBJECT_ROUTES,
		OBJECT_SEGMENTS,
		OBJECT_PHOTOS,
		OBJECT_LAST
	} FilterObjectType;

	Libsad();
	~Libsad();

	const objectId getObjectId(objectTime time);
	size_t freeStorageSpace(void);
	size_t availibleStorageSpace(void);
	bool deleteObject(objectId id);
	LibsadPlugin *getPluginHandler(void);

	int getFilteredList(FilterObjectType o, FilterListType l, FilterInfoType *i, char **entries);

private:
	LibsadPlugin *plugin;
};

#endif /* _X_LIBSAD_HPP_ */