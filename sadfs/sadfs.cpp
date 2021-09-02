#include "sadfs.hpp"

#include <errno.h>
#include <sys/stat.h>
#include <fuse.h>

#include "pathresolver.hpp"
#include "resolvedpath.hpp"
#include "path.hpp"

#include "libsad.hpp"

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_SUB 1

#define VERSION_TEXT #VERSION_MAJOR ## . ## #VERSION_MINOR ## . ## #VERSION_SUB

class Sadfs {
public:
	Sadfs(struct sadfs_options *options, fuse_conn_info *conn, fuse_context *fc)
	{
		LibSad *lib = new LibSad();
		PathResolver *pr = new PathResolver(paths);
	}

	/** Get file attributes */
	int getattr (const char *path, struct stat *);

	/** Read the target of a symbolic link */
	int readlink (const char *path, char *, size_t)	{
		return -ENOSYS;
	}

	int mknod (const char *path, mode_t, dev_t)	{
		return -ENOSYS;
	}

	/** Create a directory */
	int mkdir (const char *path, mode_t);

	/** Remove a file */
	int unlink (const char *path);

	/** Remove a directory */
	int rmdir (const char *path);

	/** Create a symbolic link */
	int symlink (const char *path1, const char *path2) {
		return -ENOSYS;
	}

	/** Rename a file */
	int rename (const char *path1, const char *path2);

	/** Create a hard link to a file */
	int link (const char *path1, const char *path2) {
		return -ENOSYS;
	}

	/** Change the permission bits of a file */
	int chmod (const char *path, mode_t) {
		return -ENOSYS;
	}

	/** Change the owner and group of a file */
	int chown (const char *path, uid_t, gid_t) {
		return -ENOSYS;
	}

	/** Change the size of a file */
	int truncate (const char *path, off_t);

	/** File open operation */
	int open (const char *path, struct fuse_file_info *);

	/** Read data from an open file */
	int read (const char *path, char *, size_t, off_t, struct fuse_file_info *);

	/** Write data to an open file */
	int write (const char *path, const char *, size_t, off_t, struct fuse_file_info *);

	/** Get file system statistics */
	int statfs (const char *path, struct statvfs *);

	/** Possibly flush cached data */
	int flush (const char *path, struct fuse_file_info *);

	/** Release an open file */
	int release (const char *path, struct fuse_file_info *);

	/** Synchronize file contents */
	int fsync (const char *path, int, struct fuse_file_info *);

	/** Set extended attributes */
	int setxattr (const char *path, const char *, const char *, size_t, int) {
		return -ENOSYS;
	}

	/** Get extended attributes */
	int getxattr (const char *path, const char *, char *, size_t) {
		return -ENOSYS;
	}

	/** List extended attributes */
	int listxattr (const char *path, char *, size_t) {
		return -ENOSYS;
	}

	/** Remove extended attributes */
	int removexattr (const char *path, const char *) {
		return -ENOSYS;
	}

	/** Open directory */
	int opendir (const char *path, struct fuse_file_info *);

	/** Read directory */
	int readdir (const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *info) {
		(void) offset;
		(void) info;
		ResolvedPath *rp = this->pr->resolve(path);

		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);

		if (this->resolve_path(path) == DIR_ROOT) {
			filler(buf, , NULL, 0);
		}
	}

	/** Release directory */
	int releasedir (const char *path, struct fuse_file_info *);

	/** Synchronize directory contents */
	int fsyncdir (const char *path, int, struct fuse_file_info *);

	/** Check file access permissions */
	int access (const char *path, int);

	/** Create and open a file */
	int create (const char *path, mode_t, struct fuse_file_info *);

	/** Change the size of an open file */
	int ftruncate (const char *path, off_t, struct fuse_file_info *);

	/** Get attributes from an open file */
	int fgetattr (const char *path, struct stat *, struct fuse_file_info *);

	/** Perform POSIX file locking operation */
	int lock (const char *path, struct fuse_file_info *, int cmd, struct flock *);

	/** Change the access and modification times of a file */
	int utimens (const char *path, const struct timespec tv[2]);

	/** Map block index within file to block index within device */
	int bmap (const char *path, size_t blocksize, uint64_t *idx) {
		return -ENOSYS;
	}

	/** Ioctl */
	int ioctl (const char *path, int cmd, void *arg, struct fuse_file_info *, unsigned int flags, void *data);

	/** Poll for IO readiness events */
	int poll (const char *path, struct fuse_file_info *, struct fuse_pollhandle *ph, unsigned *reventsp);

	/** Write contents of buffer to an open file */
	int write_buf (const char *path, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *);

	/** Store data from an open file in a buffer */
	int read_buf (const char *path, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *);

	/** Perform BSD file locking operation */
	int flock (const char *path, struct fuse_file_info *, int op);

	/** Allocates space for an open file */
	int fallocate (const char *path, int, off_t, off_t, struct fuse_file_info *);

private:
	LibSad *lib;
	PathResolver *pr;
};