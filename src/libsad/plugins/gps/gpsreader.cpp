#include "config.h"

#include "gpsreader.hpp"

#include "libsadplugin.hpp"
#include "gpsplugin.hpp"

GpsReader::GpsReader(GpsPlugin *base, LocationCallback cb_location, StatisticsCallback cb_statistics) {
	this->base_instance = base;
	this->cb_location = cb_location;
	this->cb_statistics = cb_statistics;
}

GpsReader::~GpsReader(void) {
}

bool GpsReader::read(size_t buffer_size, void *buffer) {
}

bool GpsReader::finished() {
}
