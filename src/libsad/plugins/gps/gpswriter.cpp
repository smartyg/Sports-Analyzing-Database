#include "config.h"

#include "gpswriter.hpp"

#include "libsadplugin.hpp"
#include "gpsplugin.hpp"

GpsWriter::GpsWriter(const GpsPlugin *b, const WriteCallback cb_w) {
	this->base_instance = b;
	this->cb_write = cb_w;
}

GpsWriter::~GpsWriter (void) {
}

bool GpsWriter::addLocation (const struct GpsLocation *location) {
	(void) location;
	return true;
}

bool GpsWriter::addStatistics (const struct GpsStatistics *statistics) {
	(void) statistics;
	return true;
}

bool GpsWriter::finished (void) {
	return true;
}
