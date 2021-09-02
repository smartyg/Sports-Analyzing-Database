/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall hello.c `pkg-config fuse --cflags --libs` -o hello
*/

#define FUSE_USE_VERSION 26

#include <atomic>
#include <cstdlib>

#include <fuse.h>
#include <errno.h>

#include "sadfs.hpp"

#include <vector>

#define EXPAND_RETURN_FUSE_OPERATION(name, ...) \
	struct fuse_context *fc; \
	Sadfs *fs; \
	if (init_in_progress) return -EBUSY; \
	if ((fc = fuse_get_context()) == NULL) return -ENOTSUP; \
	if ((fs = (Sadfs *)(fc->private_data)) == NULL) return -ENOTSUP; \
	return fs->name (__VA_ARGS__);

//atomic_bool init_in_progress = true;
std::atomic<bool> init_in_progress(true);

void *fuse_sadfs_init(struct fuse_conn_info *conn) {
	struct fuse_context *fc;
	struct sadfs_options *options;
	Sadfs *fs;
	fc = fuse_get_context();
	if (fc == NULL) return NULL;
	options = (struct sadfs_options *)(fc->private_data);
	fc->private_data = NULL;

	if (options == NULL) return NULL;
	fs = new Sadfs(options, conn, fc);
	free(options);
	init_in_progress = false;
	return (void *)fs;
}

void fuse_sadfs_destroy(void *private_data) {
	delete (Sadfs *)private_data;
}

int fuse_sadfs_getattr(const char *path, struct stat *stat) {
	EXPAND_RETURN_FUSE_OPERATION(getattr, path, stat);
}

int fuse_sadfs_readlink (const char *path, char *buf, size_t s) {
	EXPAND_RETURN_FUSE_OPERATION(readlink, path, buf, s);
}

int fuse_sadfs_mknod (const char *path, mode_t mode, dev_t dev) {
	EXPAND_RETURN_FUSE_OPERATION(mknod, path, mode, dev);
}

int fuse_sadfs_mkdir (const char *path, mode_t mode) {
	EXPAND_RETURN_FUSE_OPERATION(mkdir, path, mode);
}

int fuse_sadfs_unlink (const char *path) {
	EXPAND_RETURN_FUSE_OPERATION(unlink, path);
}

int fuse_sadfs_rmdir (const char *path) {
	EXPAND_RETURN_FUSE_OPERATION(rmdir, path)
}

int fuse_sadfs_symlink (const char *path1, const char *path2) {
	EXPAND_RETURN_FUSE_OPERATION(symlink, path1, path2)
}

int fuse_sadfs_rename (const char *path1, const char *path2) {
	EXPAND_RETURN_FUSE_OPERATION(rename, path1, path2);
}

int fuse_sadfs_link (const char *path1, const char *path2) {
	EXPAND_RETURN_FUSE_OPERATION(link, path1, path2);
}

int fuse_sadfs_chmod (const char *path, mode_t mode) {
	EXPAND_RETURN_FUSE_OPERATION(chmod, path, mode)
}

int fuse_sadfs_chown (const char *path, uid_t uid, gid_t gid) {
	EXPAND_RETURN_FUSE_OPERATION(chown, path, uid, gid);
}

int fuse_sadfs_truncate (const char *path, off_t o) {
	EXPAND_RETURN_FUSE_OPERATION(truncate, path, o);
}

int fuse_sadfs_open (const char *path, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(open, path, info);
}

int fuse_sadfs_read (const char *path, char *buf, size_t s, off_t o, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(read, path, buf, s, o, info);
}

int fuse_sadfs_write (const char *path, const char *buf, size_t s, off_t o, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(write, path, buf, s, o, info);
}

int fuse_sadfs_statfs (const char *path, struct statvfs *stat) {
	EXPAND_RETURN_FUSE_OPERATION(statfs, path, stat);
}

int fuse_sadfs_flush (const char *path, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(flush, path, info);
}

int fuse_sadfs_release (const char *path, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(release, path, info);
}

int fuse_sadfs_fsync (const char *path, int user_data, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(fsync, path, user_data, info);
}

int fuse_sadfs_opendir (const char *path, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(opendir, path, info);
}

int fuse_sadfs_readdir (const char *path, void *buf, fuse_fill_dir_t filler, off_t off, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(readdir, path, buf, filler, off, info);
}

int fuse_sadfs_releasedir (const char *path, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(releasedir, path, info);
}

int fuse_sadfs_fsyncdir (const char *path, int user_data, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(fsyncdir, path, user_data, info);
}

int fuse_sadfs_access (const char *path, int i) {
	EXPAND_RETURN_FUSE_OPERATION(access, path, i);
}

int fuse_sadfs_create (const char *path, mode_t mode, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(create, path, mode, info);
}

int fuse_sadfs_ftruncate (const char *path, off_t off, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(ftruncate, path, off, info);
}

int fuse_sadfs_fgetattr (const char *path, struct stat *stat, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(fgetattr, path, stat, info);
}

int fuse_sadfs_lock (const char *path, struct fuse_file_info *info, int cmd, struct flock *lock) {
	EXPAND_RETURN_FUSE_OPERATION(lock, path, info, cmd, lock);
}

int fuse_sadfs_utimens (const char *path, const struct timespec tv[2]) {
	EXPAND_RETURN_FUSE_OPERATION(utimens, path, tv);
}

int fuse_sadfs_ioctl (const char *path, int cmd, void *arg, struct fuse_file_info *info, unsigned int flags, void *data) {
	EXPAND_RETURN_FUSE_OPERATION(ioctl, path, cmd, arg, info, flags, data);
}

int fuse_sadfs_poll (const char *path, struct fuse_file_info *info, struct fuse_pollhandle *ph, unsigned *reventsp) {
	EXPAND_RETURN_FUSE_OPERATION(poll, path, info, ph, reventsp);
}

int fuse_sadfs_write_buf (const char *path, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(write_buf, path, buf, off, info);
}

int fuse_sadfs_read_buf (const char *path, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(read_buf, path, bufp, size, off, info);
}

int fuse_sadfs_flock (const char *path, struct fuse_file_info *info, int op) {
	EXPAND_RETURN_FUSE_OPERATION(flock, path, info, op);
}

int fuse_sadfs_fallocate (const char *path, int i, off_t o1, off_t o2, struct fuse_file_info *info) {
	EXPAND_RETURN_FUSE_OPERATION(fallocate, path, i, o1, o2, info);
}

struct fuse_operations sadfs_operations;/* = {
	.getattr	= fuse_sadfs_getattr,
	.readlink	= fuse_sadfs_readlink,
	.mknod		= fuse_sadfs_mknod,
	.mkdir		= fuse_sadfs_mkdir,
	.unlink		= fuse_sadfs_unlink,
	.rmdir		= fuse_sadfs_rmdir,
	.symlink	= fuse_sadfs_symlink,
	.rename		= fuse_sadfs_rename,
	.link		= fuse_sadfs_link,
	.chmod		= fuse_sadfs_chmod,
	.chown		= fuse_sadfs_chown,
	.truncate	= fuse_sadfs_truncate,
	.open		= fuse_sadfs_open,
	.read		= fuse_sadfs_read,
	.write		= fuse_sadfs_write,
	.statfs		= fuse_sadfs_statfs,
	.flush		= fuse_sadfs_flush,
	.release	= fuse_sadfs_release,
	.fsync		= fuse_sadfs_fsync,
	.opendir	= fuse_sadfs_opendir,
	.readdir	= fuse_sadfs_readdir,
	.releasedir	= fuse_sadfs_releasedir,
	.fsyncdir	= fuse_sadfs_fsyncdir,
	.init		= fuse_sadfs_init,
	.destroy	= fuse_sadfs_destroy,
	.access		= fuse_sadfs_access,
	.create		= fuse_sadfs_create,
	.ftruncate	= fuse_sadfs_ftruncate,
	.fgetattr	= fuse_sadfs_fgetattr,
	.lock		= fuse_sadfs_lock,
	.utimens	= fuse_sadfs_utimens,
	.ioctl		= fuse_sadfs_ioctl,
	.poll		= fuse_sadfs_poll,
	.write_buf	= fuse_sadfs_write_buf,
	.read_buf	= fuse_sadfs_read_buf,
	.flock		= fuse_sadfs_flock,
	.fallocate	= fuse_sadfs_fallocate,
};*/

int main(int argc, char *argv[])
{
	sadfs_operations.getattr	= fuse_sadfs_getattr;
	sadfs_operations.readlink	= fuse_sadfs_readlink;
	sadfs_operations.mknod		= fuse_sadfs_mknod;
	sadfs_operations.mkdir		= fuse_sadfs_mkdir;
	sadfs_operations.unlink		= fuse_sadfs_unlink;
	sadfs_operations.rmdir		= fuse_sadfs_rmdir;
	sadfs_operations.symlink	= fuse_sadfs_symlink;
	sadfs_operations.rename		= fuse_sadfs_rename;
	sadfs_operations.link		= fuse_sadfs_link;
	sadfs_operations.chmod		= fuse_sadfs_chmod;
	sadfs_operations.chown		= fuse_sadfs_chown;
	sadfs_operations.truncate	= fuse_sadfs_truncate;
	sadfs_operations.open		= fuse_sadfs_open;
	sadfs_operations.read		= fuse_sadfs_read;
	sadfs_operations.write		= fuse_sadfs_write;
	sadfs_operations.statfs		= fuse_sadfs_statfs;
	sadfs_operations.flush		= fuse_sadfs_flush;
	sadfs_operations.release	= fuse_sadfs_release;
	sadfs_operations.fsync		= fuse_sadfs_fsync;
	sadfs_operations.opendir	= fuse_sadfs_opendir;
	sadfs_operations.readdir	= fuse_sadfs_readdir;
	sadfs_operations.releasedir	= fuse_sadfs_releasedir;
	sadfs_operations.fsyncdir	= fuse_sadfs_fsyncdir;
	sadfs_operations.init		= fuse_sadfs_init;
	sadfs_operations.destroy	= fuse_sadfs_destroy;
	sadfs_operations.access		= fuse_sadfs_access;
	sadfs_operations.create		= fuse_sadfs_create;
	sadfs_operations.ftruncate	= fuse_sadfs_ftruncate;
	sadfs_operations.fgetattr	= fuse_sadfs_fgetattr;
	sadfs_operations.lock		= fuse_sadfs_lock;
	sadfs_operations.utimens	= fuse_sadfs_utimens;
	sadfs_operations.ioctl		= fuse_sadfs_ioctl;
	sadfs_operations.poll		= fuse_sadfs_poll;
	sadfs_operations.write_buf	= fuse_sadfs_write_buf;
	sadfs_operations.read_buf	= fuse_sadfs_read_buf;
	sadfs_operations.flock		= fuse_sadfs_flock;
	sadfs_operations.fallocate	= fuse_sadfs_fallocate;


	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	struct fuse_chan *ch;
	char *mountpoint;
	int err = -1;

	if (fuse_parse_cmdline(&args, &mountpoint, NULL, NULL) != -1 && (ch = fuse_mount(mountpoint, &args)) != NULL) {
		struct fuse *f;

		f = fuse_new(ch, &args, &sadfs_operations, sizeof(sadfs_operations), NULL);

		if (f != NULL) {
			err = fuse_loop_mt(f);
		}
		fuse_unmount(mountpoint, ch);
	}
	fuse_opt_free_args(&args);

	return err ? 1 : 0;
}
