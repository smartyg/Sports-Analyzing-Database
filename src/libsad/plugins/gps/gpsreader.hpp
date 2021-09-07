#ifndef _X_GPSREADER_HPP_
#define _X_GPSREADER_HPP_

#include "../../include/libsadplugin.hpp"

#include "gpsplugin.hpp"


class GpsReader: public DataFilePluginReader {
public:
	GpsReader (const GpsPlugin *, const LocationCallback, const StatisticsCallback);
	~GpsReader (void);
	bool read (size_t, void *);
	bool finished (void);

private:
	const GpsPlugin *base_instance;
	LocationCallback cb_location;
	StatisticsCallback cb_statistics;
};

#endif /* _X_GPSREADER_HPP_ */