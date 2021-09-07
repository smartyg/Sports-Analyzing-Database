#ifndef _X_GPSWRITER_HPP_
#define _X_GPSWRITER_HPP_

#include "../../include/libsadplugin.hpp"

#include "gpsplugin.hpp"

class GpsWriter: public DataFilePluginWriter {
public:
	GpsWriter (const GpsPlugin *, const WriteCallback);
	~GpsWriter (void);
	bool addLocation (const struct GpsLocation *);
	bool addStatistics (const struct GpsStatistics *);
	bool finished (void);

private:
	const GpsPlugin *base_instance;
	WriteCallback cb_write;
};

#endif /* _X_GPSWRITER_HPP_ */
