#include <stddef.h>
#include <sys/types.h>
#include <fuse.h>

void *fuse_sadfs_init (struct fuse_conn_info *);
void fuse_sadfs_destroy (void *);
int fuse_sadfs_getattr (const char *, struct stat *);
int fuse_sadfs_readlink (const char *, char *, size_t);
int fuse_sadfs_mknod (const char *, mode_t, dev_t);
int fuse_sadfs_mkdir (const char *, mode_t);
int fuse_sadfs_unlink (const char *);
int fuse_sadfs_rmdir (const char *);
int fuse_sadfs_symlink (const char *, const char *);
int fuse_sadfs_rename (const char *, const char *);
int fuse_sadfs_link (const char *, const char *);
int fuse_sadfs_chmod (const char *, mode_t);
int fuse_sadfs_chown (const char *, uid_t, gid_t);
int fuse_sadfs_truncate (const char *, off_t);
int fuse_sadfs_open (const char *, struct fuse_file_info *);
int fuse_sadfs_read (const char *, char *, size_t s, off_t o, struct fuse_file_info *);
int fuse_sadfs_write (const char *, const char *, size_t s, off_t o, struct fuse_file_info *);
int fuse_sadfs_statfs (const char *, struct statvfs *);
int fuse_sadfs_flush (const char *, struct fuse_file_info *);
int fuse_sadfs_release (const char *, struct fuse_file_info *);
int fuse_sadfs_fsync (const char *, int, struct fuse_file_info *);
int fuse_sadfs_opendir (const char *, struct fuse_file_info *);
int fuse_sadfs_readdir (const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *);
int fuse_sadfs_releasedir (const char *, struct fuse_file_info *);
int fuse_sadfs_fsyncdir (const char *, int, struct fuse_file_info *);
int fuse_sadfs_access (const char *, int);
int fuse_sadfs_create (const char *, mode_t, struct fuse_file_info *);
int fuse_sadfs_ftruncate (const char *, off_t, struct fuse_file_info *);
int fuse_sadfs_fgetattr (const char *, struct stat *, struct fuse_file_info *);
int fuse_sadfs_lock (const char *, struct fuse_file_info *, int, struct flock *);
int fuse_sadfs_utimens (const char *, const struct timespec tv[2]);
int fuse_sadfs_ioctl (const char *, int, void *, struct fuse_file_info *, unsigned int, void *);
int fuse_sadfs_poll (const char *, struct fuse_file_info *, struct fuse_pollhandle *, unsigned *);
int fuse_sadfs_write_buf (const char *, struct fuse_bufvec *, off_t, struct fuse_file_info *);
int fuse_sadfs_read_buf (const char *, struct fuse_bufvec **, size_t, off_t, struct fuse_file_info *);
int fuse_sadfs_flock (const char *, struct fuse_file_info *, int);
int fuse_sadfs_fallocate (const char *, int, off_t, off_t, struct fuse_file_info *);
