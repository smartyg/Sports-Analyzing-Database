#ifndef _X_LIBSAD_HPP_
#define _X_LIBSAD_HPP_

#include <stddef.h>
#include <cstdint>

#include "libsadplugin.hpp"

#define TYPE_OBJECT_EMPTY (0x0)
#define TYPE_OBJECT_ID (0x1)
#define TYPE_OBJECT_TIME (0x2)

class Libsad {
public:
	typedef uint_fast32_t objectId;
	typedef uint_fast32_t objectTime;
/*
	typedef struct {
		uint_least64_t :25, filter_month:4, filter_time:27, filter_activity:8;
		uint_least64_t filter_year:7, filter_day:5, filter_title_first_letter:8, object_type:2, object:42;
		const char *filter_title;
	} FilterInfoType;
*/
	typedef struct {
		uint_least64_t :13, filter_year:7, filter_month:4, filter_day:5, filter_hour:5, filter_minute:6, filter_second:6, filter_title_first_letter:8, filter_activity:8, object_type:2;
		uint_least32_t object;
		const char *filter_title;
	} FilterInfoType;

	typedef enum : uint_fast8_t {
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

	typedef enum : uint_fast8_t {
		OBJECT_NONE = 0,
		OBJECT_ROUTES,
		OBJECT_SEGMENTS,
		OBJECT_PHOTOS,
		OBJECT_LAST
	} FilterObjectType;

	Libsad();
	~Libsad();

	objectId getObjectId (const objectTime time) const;
	size_t freeStorageSpace (void) const;
	size_t availibleStorageSpace (void) const;
	bool deleteObject (objectId id) const;
	const LibsadPlugin *getPluginHandler (void) const;

	size_t getFilteredList (FilterObjectType, FilterListType, const FilterInfoType, char **) const;
	uint_fast8_t getActivityCodeFromName (const char *, size_t) const;

private:
	const LibsadPlugin *plugin;
};

#endif /* _X_LIBSAD_HPP_ */