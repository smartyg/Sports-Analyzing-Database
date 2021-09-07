#include "config.h"

#include "libsad.hpp"

#include <cstdlib>
#include <cstring>
#include <vector>
#include <dirent.h>
#include <dlfcn.h>

#ifndef IS_SET
#define IS_SET(v, m) (((v) & (m)) == (m))
#endif

using namespace std;

vector<LibsadPlugin::pluginDataFileEntry> *LibsadPlugin::data_file_table = new vector<LibsadPlugin::pluginDataFileEntry>;
vector<LibsadPlugin::pluginAlgorithmEntry> *LibsadPlugin::algorithm_table = new vector<LibsadPlugin::pluginAlgorithmEntry>;

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
		LibsadPlugin::data_file_table->clear ();
		LibsadPlugin::algorithm_table->clear ();

	}
	// release exclusive lock
}

bool LibsadPlugin::isExtentionMatch (const char *ext, DataFileHandlerType t) const {
	char *c = const_cast<char *>(ext);

	// If *ext starts with a '.', increase the pointer to exclusive the dot from the match.
	while (c[0] == '.') c++;

	// Loop over all data hile handlers and check the type and code, if a match is found, return true.
	for (pluginDataFileEntry e : *(LibsadPlugin::data_file_table)) {
		if (strcmp(e.code, c) == 0 && IS_SET(e.handlers, t)) return true;
	}

	// No match was found.
	return false;
}

void LibsadPlugin::registerPlugin(const char *code, const pluginType type, const pluginDetails *details, void *data) {
	if (type == PLUGIN_TYPE_DATA_FILE) LibsadPlugin::addDataFileEntry(code, details, static_cast<const pluginDataFile *>(data));
	if (type == PLUGIN_TYPE_ALGORITHM) LibsadPlugin::addAlgorithmEntry(code, details, static_cast<const pluginAlgorithm *>(data));
	return;
}

void LibsadPlugin::removePlugin(const char *code, const pluginType type) {
	if (type == PLUGIN_TYPE_DATA_FILE) LibsadPlugin::removeDataFileEntry(code);
	if (type == PLUGIN_TYPE_ALGORITHM) LibsadPlugin::removeAlgorithmEntry(code);
	return;
}

const vector<const char *> *LibsadPlugin::getDataFileExtentions(DataFileHandlerType t) {
	vector<const char *> *v = new vector<const char *>;
	v->reserve (LibsadPlugin::data_file_table->size ());

	for (const pluginDataFileEntry e : *(LibsadPlugin::data_file_table)) {
		if ((t == PLUGIN_TYPE_DATA_FILE_READ && e.data.read_class != NULL) ||
			(t == PLUGIN_TYPE_DATA_FILE_WRITE && e.data.write_class != NULL) ||
			(t == PLUGIN_TYPE_DATA_FILE_WAYPOINT && e.data.waypoint_class != NULL) ||
			(t == PLUGIN_TYPE_DATA_FILE_TABLE && e.data.table_class != NULL)) {
			v->push_back(e.code);
		}
	}

	v->shrink_to_fit ();

	return v;
}

bool LibsadPlugin::addDataFileEntry(const char *c, const pluginDetails *d, const pluginDataFile *f) {
	for (const pluginDataFileEntry e : *(LibsadPlugin::data_file_table)) {
		if (strcmp(e.code, c) == 0) return false;
	}

	unsigned int h = PLUGIN_TYPE_DATA_FILE_NONE;

	if (f->read_class != NULL) h |= PLUGIN_TYPE_DATA_FILE_READ;
	if (f->write_class != NULL) h |= PLUGIN_TYPE_DATA_FILE_WRITE;
	if (f->waypoint_class != NULL) h |= PLUGIN_TYPE_DATA_FILE_WAYPOINT;
	if (f->table_class != NULL) h |= PLUGIN_TYPE_DATA_FILE_TABLE;

	const LibsadPlugin::pluginDataFileEntry e = {c, static_cast<DataFileHandlerType>(h), *d, *f};

	LibsadPlugin::data_file_table->push_back (e);

	return true;
}

bool LibsadPlugin::removeDataFileEntry(const char *code) {
	if (code == NULL) return false;
	for (unsigned int i = 0; i < LibsadPlugin::data_file_table->size (); i++) {
		pluginDataFileEntry e = LibsadPlugin::data_file_table->at(i);
		if (strcmp(e.code, code) == 0) {
			LibsadPlugin::data_file_table->erase (LibsadPlugin::data_file_table->begin() + i);
			return true;
		}
	}

	return false;
}

bool LibsadPlugin::addAlgorithmEntry(const char *code, const pluginDetails *details, const pluginAlgorithm *data) {
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
