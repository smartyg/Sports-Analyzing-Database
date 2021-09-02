#include "config.h"

#include "libsadplugin.hpp"
#include "gpsplugin.hpp"
#include "gpsreader.hpp"
#include "gpswriter.hpp"

#define DATA_FILE_TYPE "gps"

void __attribute__((constructor)) register_handler();
void __attribute__((destructor)) remove_handler();

DataFilePluginReader* create_read_class(DataFilePlugin *base, LocationCallback cb_location, StatisticsCallback cb_statistics)
{
	GpsReader *reader = new GpsReader((GpsPlugin *)base, cb_location, cb_statistics);
	return reader;
}

DataFilePluginWriter* create_write_class(DataFilePlugin *base, WriteCallback cb_write)
{
	GpsWriter *writer = new GpsWriter((GpsPlugin *)base, cb_write);
	return writer;
}

void register_handler()
{
	LibsadPlugin::pluginDetails* details = LibsadPlugin::getEmptyPluginDetails();
	LibsadPlugin::pluginDataFile* data = LibsadPlugin::getEmptyPluginDataFile();

	details->name = "gps datafile handler";
	details->author = "Martijn Goedhart";
	details->license = "GPLv2.0+";
	details->major_version = 0;
	details->minor_version = 1;

	data->instance = new GpsPlugin();
	data->read_class = create_read_class;
	data->write_class = create_write_class;

	LibsadPlugin::registerPlugin(DATA_FILE_TYPE, LibsadPlugin::PLUGIN_TYPE_DATA_FILE, details, (void *)data);
}

void remove_handler()
{
	LibsadPlugin::removePlugin(DATA_FILE_TYPE, LibsadPlugin::PLUGIN_TYPE_DATA_FILE);
}
