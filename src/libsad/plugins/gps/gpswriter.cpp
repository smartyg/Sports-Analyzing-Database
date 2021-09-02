#include "config.h"

#include "gpswriter.hpp"

#include "libsadplugin.hpp"
#include "gpsplugin.hpp"

GpsWriter::GpsWriter(GpsPlugin *base, WriteCallback cb_write) {
	this->base_instance = base;
	this->cb_write = cb_write;
}

GpsWriter::~GpsWriter(void) {
}

bool GpsWriter::addLocation(struct GpsLocation *location) {
}

bool GpsWriter::addStatistics(struct GpsStatistics *statistics) {
}

bool GpsWriter::finished() {
}
