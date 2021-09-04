## Place generated object files (.o) into the same directory as their source
## files, in order to avoid collisions when non-recursive make is used.
AUTOMAKE_OPTIONS = subdir-objects

## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

AM_CFLAGS =
AM_CXXFLAGS =

## Only use the compiler flags when we build a debug version.
if DEBUG
COMMON_DEBUG_OPTIONS = -Wall -Wextra -W -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align \
	-Wmissing-declarations -Wno-missing-braces -Wno-missing-field-initializers \
	-Wformat=1 -Wswitch-default -Wswitch-enum -Wstrict-overflow=5 -Winline \
	-Wundef -Wunreachable-code -Wlogical-op -Wfloat-equal -Wno-float-equal \
	-Wstrict-aliasing=2 -Wredundant-decls -Werror \
	-fno-omit-frame-pointer -ffloat-store -fno-common -fstrict-aliasing \
	-fvar-tracking -fvar-tracking-assignments -g3 -ggdb -O0
AM_CFLAGS += $(COMMON_DEBUG_OPTIONS) \
	-Wbad-function-cast -Wstrict-prototypes -Wmissing-prototypes -Wnested-externs \
	-Wold-style-definition
AM_CXXFLAGS += $(COMMON_DEBUG_OPTIONS) \
	-Wabi -Wconversion-null  -Wctor-dtor-privacy -Wdelete-non-virtual-dtor \
	-Wnarrowing -Wnoexcept -Wnon-virtual-dtor  -Wreorder -Weffc++ -Wstrict-null-sentinel \
	-Wno-non-template-friend  -Wold-style-cast -Woverloaded-virtual  -Wno-pmf-conversions \
	-Wsign-promo
else
AM_CFLAGS += -flto
AM_CXXFLAGS += -flto
endif

## Install the generated pkg-config file (.pc) into the expected location for
## architecture-dependent package configuration information.  Occasionally,
## pkg-config files are also used for architecture-independent data packages,
## in which case the correct install location would be $(datadir)/pkgconfig.
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libsad.pc

lib_LTLIBRARIES = libsad-@LIBRARY_API_VERSION@.la
libsad_@LIBRARY_API_VERSION@_la_CPPFLAGS = -Isrc/libsad/include -DLOCALEDIR=\"$(localedir)\" -DPKGLIBDIR=\"$(pkglibdir)\"

## Define the source file list for the "libexample-@EXAMPLE_API_VERSION@.la"
## target.  Note that @EXAMPLE_API_VERSION@ is not interpreted by Automake and
## will therefore be treated as if it were literally part of the target name,
## and the variable name derived from that.
## Note that it is not necessary to list header files
## which are already listed elsewhere in a _HEADERS variable assignment.
libsad_@LIBRARY_API_VERSION@_la_SOURCES = src/libsad/src/libsad.cpp \
	src/libsad/src/libsadplugin.cpp

libsad_@LIBRARY_API_VERSION@_la_LIBADD = @DL_LIBS@

## Instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.
libsad_@LIBRARY_API_VERSION@_la_LDFLAGS = -version-info @LIBRARY_CURRENT_VERSION@:@LIBRARY_REVISION_VERSION@:@LIBRARY_AGE_VERSION@

noinst_HEADERS =

## Define the list of public header files and their install location.  The
## nobase_ prefix instructs Automake to not strip the directory part from each
## filename, in order to avoid the need to define separate file lists for each
## installation directory.  This only works if the directory hierarchy in the
## source tree matches the hierarchy at the install location, however.
libsad_@LIBRARY_API_VERSION@_la_includedir = $(includedir)
nobase_libsad_@LIBRARY_API_VERSION@_la_include_HEADERS = src/libsad/include/libsad.hpp \
	src/libsad/include/libsadplugin.hpp

lib_LTLIBRARIES += libsadfs-@LIBRARY_API_VERSION@.la
libsadfs_@LIBRARY_API_VERSION@_la_CPPFLAGS = -Isrc/libsad/include -Isrc/libsadfs/include -DLOCALEDIR=\"$(localedir)\" -DPKGLIBDIR=\"$(pkglibdir)\" @FUSE_CFLAGS@
libsadfs_@LIBRARY_API_VERSION@_la_SOURCES = src/libsadfs/src/path.cpp \
	src/libsadfs/src/pathfilter.cpp \
	src/libsadfs/src/pathresolver.cpp \
	src/libsadfs/src/resolvedpath.cpp \
	src/libsadfs/src/sadfs.cpp
libsadfs_@LIBRARY_API_VERSION@_la_LIBADD = libsad-@LIBRARY_API_VERSION@.la
libsadfs_@LIBRARY_API_VERSION@_la_LDFLAGS = -version-info @LIBRARY_CURRENT_VERSION@:@LIBRARY_REVISION_VERSION@:@LIBRARY_AGE_VERSION@
libsadfs_@LIBRARY_API_VERSION@_la_includedir = $(includedir)
nobase_libsadfs_@LIBRARY_API_VERSION@_la_include_HEADERS = src/libsadfs/include/libsadfs.hpp
noinst_HEADERS += src/libsadfs/src/pathfilter.hpp \
	src/libsadfs/src/path.hpp \
	src/libsadfs/src/pathresolver.hpp \
	src/libsadfs/src/resolvedpath.hpp


pkglib_LTLIBRARIES = gps.la
gps_la_CPPFLAGS = -Isrc/libsad/include -DLOCALEDIR=\"$(localedir)\"
gps_la_LDFLAGS = -module -avoid-version -shared
gps_la_LIBADD = libsad-@LIBRARY_API_VERSION@.la
gps_la_SOURCES = src/libsad/plugins/gps/plugin.cpp \
	src/libsad/plugins/gps/gpsplugin.cpp \
	src/libsad/plugins/gps/gpsreader.cpp \
	src/libsad/plugins/gps/gpswriter.cpp
noinst_HEADERS += src/libsad/plugins/gps/gpsplugin.hpp \
	src/libsad/plugins/gps/gpsreader.hpp \
	src/libsad/plugins/gps/gpswriter.hpp

# pkglib_LTLIBRARIES += gpx.la
# gpx_la_LDFLAGS = -module -avoid-version -shared
# gpx_la_LIBADD = libsad.la
# gpx_la_SOURCES = libsad/plugins/gpx/GpxDataFilePlugin.cpp
# noinst_HEADERS += libsad/plugins/gpx/GpxDataFilePlugin.hpp
#
# pkglib_LTLIBRARIES += log.la
# log_la_LDFLAGS = -module -avoid-version -shared
# log_la_LIBADD = libsad.la
# log_la_SOURCES = libsad/plugins/log/LogDataFilePlugin.cpp
# noinst_HEADERS += libsad/plugins/log/LogDataFilePlugin.hpp

bin_PROGRAMS = sadfs
sadfs_LDADD = @FUSE_LIBS@ libsadfs-@LIBRARY_API_VERSION@.la
sadfs_LDFLAGS = -Wl,-rpath=$(pkglibdir),--enable-new-dtags
sadfs_CPPFLAGS = -Isrc/libsad/include -Isrc/libsadfs/include -DLOCALEDIR=\"$(localedir)\" -DPKGLIBDIR=\"$(pkglibdir)\" @FUSE_CFLAGS@
sadfs_SOURCES = src/sadfs/src/fuse_sadfs.cpp

noinst_HEADERS += config.h

bin_PROGRAMS += path_resolver
path_resolver_LDADD = libsadfs-@LIBRARY_API_VERSION@.la
path_resolver_LDFLAGS = -Wl,-rpath=$(pkglibdir),--enable-new-dtags
path_resolver_CPPFLAGS = -Isrc/libsad/include -Isrc/libsadfs/include -Isrc/libsadfs/src -Isrc/libsadfs/src -DLOCALEDIR=\"$(localedir)\" -DPKGLIBDIR=\"$(pkglibdir)\" @FUSE_CFLAGS@
path_resolver_SOURCES = test/path_resolver.cpp

EXTRA_DIST = VERSION @PROGRAM_NAME@.spec

## Only try to update the ChangeLog file when git is availible to report all the changes.
if HAVE_GIT
## Force to recreate the ChangeLog file every time
.PHONY: ChangeLog

## Dynamicly create the changelog file when a distribution tarball is created.
ChangeLog:
	-$(GIT) log --decorate > ChangeLog

VERSION:
	-$(GIT) describe --tags --match=v[0-9]* --long --dirty | $(SED) 's/^v//' > VERSION

distclean-local:
	@rm -f ChangeLog VERSION
endif

SUBDIRS = src/sadfs/po src/libsad/po docs