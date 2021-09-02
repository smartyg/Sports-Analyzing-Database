#ifndef _X_GPSWRITER_HPP_
#define _X_GPSWRITER_HPP_

#include "../../include/libsadplugin.hpp"

#include "gpsplugin.hpp"

class GpsWriter: public DataFilePluginWriter {
public:
	GpsWriter(GpsPlugin *base, WriteCallback cb_write);
	~GpsWriter(void);
	bool addLocation(struct GpsLocation *location);
	bool addStatistics(struct GpsStatistics *statistics);
	bool finished(void);

private:
	GpsPlugin *base_instance;
	WriteCallback cb_write;
};

#endif /* _X_GPSWRITER_HPP_ */
