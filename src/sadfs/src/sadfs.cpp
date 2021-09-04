#include "config.h"

#include "sadfs.hpp"

#include <errno.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <fuse.h>

#include "pathresolver.hpp"
#include "resolvedpath.hpp"
#include "path.hpp"
#include "libsad.hpp"

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_SUB 1

#define VERSION_TEXT #VERSION_MAJOR ## . ## #VERSION_MINOR ## . ## #VERSION_SUB

Sadfs::Sadfs(sadfsOptions *options, struct fuse_conn_info *conn, struct fuse_context *fc)
{
	this->lib = new Libsad();
	this->pr = new PathResolver(this->lib, paths);
	this->fuse_context = fc;
	this->fuse_conn = conn;
	this->options = options;

	if (this->options->debug) fprintf(stderr, "Sadfs constructor\n");
}

Sadfs::~Sadfs(void)
{
	if (this->options->debug) fprintf(stderr, "Sadfs destructor\n");
	delete this->lib;
	delete this->pr;
	free(this->options);
}

/** Get file attributes */
int Sadfs::getattr (const char *path, struct stat *stat) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);

	ResolvedPath *rp = this->pr->resolve(path);
	if (rp == NULL) return -ENOENT;
	else if (!rp->isValid()) {
		delete rp;
		return -ENOENT;
	}

	this->getPathStat(rp, stat);

	delete rp;
	return 0;
}


/** Read the target of a symbolic link */
int Sadfs::readlink (const char *path, char *, size_t) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	return -ENOSYS;
}

int Sadfs::mknod (const char *path, mode_t, dev_t) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	return -ENOSYS;
}

/** Create a directory */
int Sadfs::mkdir (const char *path, mode_t) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	return -EPERM;
}

/** Remove a file */
int Sadfs::unlink (const char *path) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Remove a directory */
int Sadfs::rmdir (const char *path) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	ResolvedPath *rp = this->pr->resolve(path);
	if (rp == NULL) return -ENOENT;
	else if (!rp->isValid()) {
		delete rp;
		return -ENOENT;
	}

	path_type path_id = rp->getPathId();

	if (!this->pr->isDirectory(rp)) {
		delete rp;
		return -ENOTDIR;
	}

	if (path_id == PATH_TYPE_DIR_ROUTE_DETAIL || path_id == PATH_TYPE_DIR_SEGMENT_DETAIL) {
		Libsad::objectId object_id = 0;
		if (rp->hasObjectId()) object_id = rp->getObjectId();
		if (rp->hasObjectTime()) object_id = this->lib->getObjectId(rp->getObjectTime());
		if (object_id != 0) this->lib->deleteObject(object_id);
		else return -EINVAL;
	} else -EPERM;

	delete rp;
	return 0;
}

/** Create a symbolic link */
int Sadfs::symlink (const char *path1, const char *path2) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path1);
	return -ENOSYS;
}

/** Rename a file */
int Sadfs::rename (const char *path1, const char *path2) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path1);
	return -EPERM;
}

/** Create a hard link to a file */
int Sadfs::link (const char *path1, const char *path2) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path1);
	return -ENOSYS;
}

/** Change the permission bits of a file */
int Sadfs::chmod (const char *path, mode_t) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	return -ENOSYS;
}

/** Change the owner and group of a file */
int Sadfs::chown (const char *path, uid_t, gid_t) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	return -ENOSYS;
}

/** Change the size of a file */
int Sadfs::truncate (const char *path, off_t) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	return -EPERM;
}

/** File open operation */
int Sadfs::open (const char *path, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Read data from an open file */
int Sadfs::read (const char *path, char *, size_t, off_t, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Write data to an open file */
int Sadfs::write (const char *path, const char *, size_t, off_t, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Get file system statistics */
int Sadfs::statfs (const char *path, struct statvfs *vfs) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	size_t bsize = 512;
	size_t blocks_free = this->lib->freeStorageSpace() / bsize;
	size_t blocks_avail = this->lib->availibleStorageSpace() / bsize;
	vfs->f_bsize = bsize;          /* Filesystem block size */
	vfs->f_blocks = blocks_avail;  /* Size of fs in f_frsize units */
	vfs->f_bfree = blocks_free;    /* Number of free blocks */
	vfs->f_bavail = blocks_free;   /* Number of free blocks for unprivileged users */
	vfs->f_files = blocks_avail;   /* Number of inodes */
	vfs->f_ffree = blocks_free;    /* Number of free inodes */
	vfs->f_favail = blocks_free;   /* Number of free inodes for unprivileged users */
	vfs->f_namemax = 1024;         /* Maximum filename length */

	return 0;
}

/** Possibly flush cached data */
int Sadfs::flush (const char *path, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Release an open file */
int Sadfs::release (const char *path, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Synchronize file contents */
int Sadfs::fsync (const char *path, int, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Set extended attributes */
int Sadfs::setxattr (const char *path, const char *, const char *, size_t, int) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	return -ENOSYS;
}

/** Get extended attributes */
int Sadfs::getxattr (const char *path, const char *, char *, size_t) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	return -ENOSYS;
}

/** List extended attributes */
int Sadfs::listxattr (const char *path, char *, size_t) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	return -ENOSYS;
}

/** Remove extended attributes */
int Sadfs::removexattr (const char *path, const char *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	return -ENOSYS;
}

/** Open directory */
int Sadfs::opendir (const char *path, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);

	ResolvedPath *rp = this->pr->resolve(path);
	if (rp == NULL) return -ENOENT;
	else if (!rp->isValid()) {
		delete rp;
		return -ENOENT;
	}

	if (!this->pr->isDirectory(rp)) {
		delete rp;
		return -ENOTDIR;
	}

	delete rp;
	return 0;
}

/** Read directory */
int Sadfs::readdir (const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *info) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);

	(void) offset;
	(void) info;

	ResolvedPath *rp = this->pr->resolve(path);
	if (rp == NULL) return -ENOENT;
	else if (!rp->isValid()) {
		delete rp;
		return -ENOENT;
	}

	if (!this->pr->isDirectory(rp)) {
		delete rp;
		return -ENOTDIR;
	}

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);

	ResolvedPath *children = NULL;
	int n_children = this->pr->getChildEntries(rp, &children);

	for (int i = 0; i < n_children; i++) {
		ResolvedPath *child = (ResolvedPath *)*((uintptr_t *)(children + i));
		fprintf(stderr, "Sadfs::%s: record %d (%p): %s\n", __FUNCTION__, i, child, child->getFilename());
		struct stat *st = (struct stat *)malloc(sizeof(struct stat));
		memset(st, 0, sizeof(struct stat));
		this->getPathStat(child, st);
		filler(buf, child->getFilename(), st, 0);
		free(st);
		delete child;
	}

	delete rp;
	if (children != NULL) free(children);
	return 0;
}

/** Release directory */
int Sadfs::releasedir (const char *path, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Synchronize directory contents */
int Sadfs::fsyncdir (const char *path, int, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Check file access permissions */
int Sadfs::access (const char *path, int) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Create and open a file */
int Sadfs::create (const char *path, mode_t, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Change the size of an open file */
int Sadfs::ftruncate (const char *path, off_t, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Get attributes from an open file */
int Sadfs::fgetattr (const char *path, struct stat *, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Perform POSIX file locking operation */
int Sadfs::lock (const char *path, struct fuse_file_info *, int cmd, struct flock *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Change the access and modification times of a file */
int Sadfs::utimens (const char *path, const struct timespec tv[2]) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Map block index within file to block index within device */
int Sadfs::bmap (const char *path, size_t blocksize, uint64_t *idx) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	return -ENOSYS;
}

/** Ioctl */
int Sadfs::ioctl (const char *path, int cmd, void *arg, struct fuse_file_info *, unsigned int flags, void *data) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Poll for IO readiness events */
int Sadfs::poll (const char *path, struct fuse_file_info *, struct fuse_pollhandle *ph, unsigned *reventsp) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Write contents of buffer to an open file */
int Sadfs::write_buf (const char *path, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Store data from an open file in a buffer */
int Sadfs::read_buf (const char *path, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Perform BSD file locking operation */
int Sadfs::flock (const char *path, struct fuse_file_info *, int op) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

/** Allocates space for an open file */
int Sadfs::fallocate (const char *path, int, off_t, off_t, struct fuse_file_info *) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%s)\n", __FUNCTION__, path);
	/* TODO */
	return -ENOSYS;
}

const int Sadfs::getPathStat(ResolvedPath *rp, struct stat *st) {
	if (this->options->debug) fprintf(stderr, "Sadfs::%s(%p, %p)\n", __FUNCTION__, rp, st);
	st->st_mode = this->pr->getMode(rp);        /* File type and mode */
	if (this->pr->isDirectory(rp)) {
		st->st_nlink = 2;       /* Number of hard links */
	} else {
		st->st_nlink = 1;       /* Number of hard links */
	}

	st->st_uid = this->fuse_context->uid; /* User ID of owner */
	st->st_gid = this->fuse_context->gid; /* Group ID of owner */
	st->st_size = 0;                      /* Total size, in bytes */
	st->st_blocks = 0;                    /* Number of 512B blocks allocated */

	struct timespec ts;
	clock_gettime (CLOCK_REALTIME, &ts);
	st->st_atim = ts;  /* Time of last access */
	st->st_mtim = ts;  /* Time of last modification */
	st->st_ctim = ts;  /* Time of last status change */

	return 0;
}