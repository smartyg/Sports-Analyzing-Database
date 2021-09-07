#include "config.h"

#include "libsad.hpp"

#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <dlfcn.h>

LibsadPlugin::pluginDataFileEntry *LibsadPlugin::data_file_table = NULL;
LibsadPlugin::pluginAlgorithmEntry *LibsadPlugin::algorithm_table = NULL;
unsigned int LibsadPlugin::n_data_file_table = 0;
unsigned int LibsadPlugin::n_algorithm_table = 0;
int LibsadPlugin::instance_counter = 0;

LibsadPlugin::LibsadPlugin(void) {
	/* TODO */
	// get exclusive lock (mutex)
	this->instance_counter++;
	LibsadPlugin::loadPlugins();
	// release exclusive lock
}

LibsadPlugin::~LibsadPlugin(void) {
	/* TODO */
	// get exclusive lock (mutex)
	this->instance_counter--;
	if(this->instance_counter == 0) {
		free(this->data_file_table);
		free(this->algorithm_table);
		this->data_file_table = NULL;
		this->algorithm_table = NULL;
		this->n_data_file_table = 0;
		this->n_algorithm_table = 0;
	}
	// release exclusive lock
}

bool LibsadPlugin::isExtentionMatch (const char *ext, DataFileHandlerType t) const {
	/* TODO */
	//std::cout << "match_data_file_handler_extention(" << ext << ", " << t << ")" << std::endl;
	if (t == PLUGIN_TYPE_DATA_FILE_TABLE && strcmp(ext, ".csv") == 0) return true;
	else return false;
}

void LibsadPlugin::registerPlugin(const char *code, const pluginType type, const pluginDetails *details, void *data) {
	if (type == PLUGIN_TYPE_DATA_FILE) LibsadPlugin::addDataFileEntry(code, details, static_cast<pluginDataFile *>(data));
	if (type == PLUGIN_TYPE_ALGORITHM) LibsadPlugin::addAlgorithmEntry(code, details, static_cast<pluginAlgorithm *>(data));
	return;
}

void LibsadPlugin::removePlugin(const char *code, const pluginType type) {
	if (type == PLUGIN_TYPE_DATA_FILE) LibsadPlugin::removeDataFileEntry(code);
	if (type == PLUGIN_TYPE_ALGORITHM) LibsadPlugin::removeAlgorithmEntry(code);
	return;
}

int LibsadPlugin::getDataFileExtentions(DataFileHandlerType t, char **buf) {
	int n = 0;
	*buf = NULL;
	for(unsigned int i = 0; i < LibsadPlugin::n_data_file_table; i++) {
		pluginDataFileEntry e = LibsadPlugin::data_file_table[i];
		if ((t == PLUGIN_TYPE_DATA_FILE_READ && e.data.read_class != NULL) ||
			(t == PLUGIN_TYPE_DATA_FILE_WRITE && e.data.write_class != NULL) ||
			(t == PLUGIN_TYPE_DATA_FILE_WAYPOINT && e.data.waypoint_class != NULL) ||
			(t == PLUGIN_TYPE_DATA_FILE_TABLE && e.data.table_class != NULL)) {
			n++;
			*buf = static_cast<char *>(realloc(*buf, n * sizeof(char *)));
			char *ptr = static_cast<char *>(*buf + n);
			*ptr = *(e.code);
		}
	}

	return n;
}

bool LibsadPlugin::addDataFileEntry(const char *code, const pluginDetails *details, pluginDataFile *data) {
	(void) code;
	(void) details;
	(void) data;
	/* TODO */
	return true;
}

bool LibsadPlugin::removeDataFileEntry(const char *code) {
	(void) code;
	/* TODO */
	return true;
}

bool LibsadPlugin::addAlgorithmEntry(const char *code, const pluginDetails *details, pluginAlgorithm *data) {
	(void) code;
	(void) details;
	(void) data;
	/* TODO */
	return true;
}

bool LibsadPlugin::removeAlgorithmEntry(const char *code) {
	(void) code;
	/* TODO */
	return true;
}

void LibsadPlugin::loadPlugins(void)
{
	char plugin_dir[] = PKGLIBDIR;
	struct dirent **entry;
	int j, i;
	if((j = scandir(plugin_dir, &entry, LibsadPlugin::loadPluginFilter, NULL)) != -1)
	{
		for(i = 0; i < j; i++)
		{
			size_t len = strlen(plugin_dir) + strlen(entry[i]->d_name) + 2;
			char *file = static_cast<char *>(malloc(sizeof(char) * len));
			strncpy(file, plugin_dir, strlen(plugin_dir));
			file[strlen(plugin_dir)] = '/';
			strcpy(file + strlen(plugin_dir) + 1, entry[i]->d_name);
			//create_path(plugin_dir, strlen(plugin_dir), entry[i]->d_name, strlen(entry[i]->d_name));
			//debug(_("try to load module: %s"), file);
			void *plugin = dlopen(file, RTLD_NOW);
			if(!plugin)
			{
				//error(_("Cannot load module %s: %s"), file, dlerror());
			}
			free(file);
		}
	}
}

int LibsadPlugin::loadPluginFilter(const struct dirent *file)
{
	if ((file->d_type == DT_REG) && (strcmp(file->d_name + strlen(file->d_name) - 3, ".so") == 0)) return 1;
	return 0;
}
