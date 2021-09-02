#ifndef _X_LIBSAD_HPP_
#define _X_LIBSAD_HPP_

typedef enum {
	DATA_FILE_SUFFIX_NONE = 0,
	DATA_FILE_SUFFIX_WRITE,
	DATA_FILE_SUFFIX_READ,
	DATA_FILE_SUFFIX_WAYPOINT,
	DATA_FILE_SUFFIX_TABLE
} data_file_suffix;

class Libsad {
public:
	Libsad();
};

#endif /* _X_LIBSAD_HPP_ */