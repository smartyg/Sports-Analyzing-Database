#ifndef _X_GPSREADER_HPP_
#define _X_GPSREADER_HPP_

#include "../../include/libsadplugin.hpp"

#include "gpsplugin.hpp"


class GpsReader: public DataFilePluginReader {
public:
	GpsReader(GpsPlugin *base, LocationCallback cb_location, StatisticsCallback cb_statistics);
	~GpsReader(void);
	bool read(size_t buffer_size, void *buffer);
	bool finished(void);

private:
	GpsPlugin *base_instance;
	LocationCallback cb_location;
	StatisticsCallback cb_statistics;
};

#endif /* _X_GPSREADER_HPP_ */