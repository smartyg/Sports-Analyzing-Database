#ifndef _X_LIBSADPLUGIN_HPP_
#define _X_LIBSADPLUGIN_HPP_

#include <stddef.h>
#include <cstdint>
#include <vector>

using namespace std;

typedef void (*LocationCallback) (void);
typedef void (*StatisticsCallback) (void);
typedef void (*WriteCallback) (void);

// Base class
class Plugin {
public:
};

// Base class
class DataFilePlugin: public Plugin {
public:
};

// Base class
class AlgorithmPlugin: public Plugin {
public:
};

class DataFilePluginReader {
public:
	virtual ~DataFilePluginReader() = default;
	virtual bool read(size_t, void *);
	virtual bool finished();
};

class DataFilePluginWriter {
public:
	virtual ~DataFilePluginWriter() = default;
	virtual bool addLocation(const struct GpsLocation*);
	virtual bool addStatistics(const struct GpsStatistics*);
	virtual bool finished();
};

class DataFilePluginWaypoint {
public:
	virtual ~DataFilePluginWaypoint() = default;
	virtual bool addLocation(const struct GpsLocation*);
	virtual bool finished();
};

class DataFilePluginTable {
public:
	virtual ~DataFilePluginTable() = default;
	virtual bool setColumns(const int);
	virtual bool setHeaders(const size_t, const void *);
	virtual bool addEntry(const size_t, const void *);
	virtual bool finished();
};

class LibsadPlugin {
public:
	typedef enum : uint_fast8_t {
		PLUGIN_TYPE_NONE = 0,
		PLUGIN_TYPE_DATA_FILE,
		PLUGIN_TYPE_ALGORITHM,
		PLUGIN_TYPE_LAST
	} pluginType;

	typedef enum : uint_fast8_t {
		PLUGIN_TYPE_DATA_FILE_NONE = 0x0,
		PLUGIN_TYPE_DATA_FILE_READ = 0x1,
		PLUGIN_TYPE_DATA_FILE_WRITE = 0x2,
		PLUGIN_TYPE_DATA_FILE_WAYPOINT = 0x4,
		PLUGIN_TYPE_DATA_FILE_TABLE = 0x8,
	} DataFileHandlerType;

	typedef struct {
		const char *name;
		const char *author;
		const char *license;
		uint_least8_t major_version:8;
		uint_least8_t minor_version:8;
	} pluginDetails;

	typedef struct {
		DataFilePlugin *instance;
		DataFilePluginReader *(*read_class) (const DataFilePlugin*, const LocationCallback, const StatisticsCallback);
		DataFilePluginWriter *(*write_class) (const DataFilePlugin*, const WriteCallback);
		DataFilePluginWaypoint *(*waypoint_class) (const DataFilePlugin*, const WriteCallback);
		DataFilePluginTable *(*table_class) (const DataFilePlugin*, const WriteCallback);
	} pluginDataFile;

	typedef struct {
		int i;
	} pluginAlgorithm;

	LibsadPlugin (void);
	~LibsadPlugin (void);

	bool isExtentionMatch (const char *, DataFileHandlerType) const;

	static void registerPlugin (const char *, const pluginType, const pluginDetails *, void *);
	static void removePlugin (const char *, const pluginType);

	static const vector<const char *> *getDataFileExtentions(DataFileHandlerType t);

private:
	typedef struct {
		const char *code;
		DataFileHandlerType handlers;
		pluginDetails details;
		pluginDataFile data;
	} pluginDataFileEntry;

	typedef struct {
		const char *code;
		//const pluginType type;
		pluginDetails details;
		pluginAlgorithm data;
	} pluginAlgorithmEntry;

	static vector<pluginDataFileEntry> *data_file_table;
	static vector<pluginAlgorithmEntry> *algorithm_table;
	static int instance_counter;

	static void loadPlugins (void);
	static int loadPluginFilter (const struct dirent *);
	static bool addDataFileEntry (const char *, const pluginDetails *, const pluginDataFile *);
	static bool removeDataFileEntry (const char *);
	static bool addAlgorithmEntry (const char *, const pluginDetails *, const pluginAlgorithm *);
	static bool removeAlgorithmEntry (const char *);
};

#endif /* _X_LIBSADPLUGIN_HPP_ */
