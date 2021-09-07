#include "config.h"

#include "gpsreader.hpp"

#include "libsadplugin.hpp"
#include "gpsplugin.hpp"

GpsReader::GpsReader (const GpsPlugin *b, const LocationCallback cb_l, const StatisticsCallback cb_s) {
	this->base_instance = b;
	this->cb_location = cb_l;
	this->cb_statistics = cb_s;
}

GpsReader::~GpsReader (void) {
}

bool GpsReader::read (size_t buffer_size, void *buffer) {
	(void) buffer_size;
	(void) buffer;
	return true;
}

bool GpsReader::finished () {
	return true;
}
