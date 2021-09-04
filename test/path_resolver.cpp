#include "config.h"

#include "path.hpp"
#include "pathresolver.hpp"
#include "pathfilter.hpp"
#include "resolvedpath.hpp"
#include "libsad.hpp"

#include <stdio.h>

int main(int argc, char *argv[])
{
	Libsad *lib = new Libsad();
	PathResolver *pr = new PathResolver(lib, paths);
	ResolvedPath *rp = pr->resolve(argv[1]);

	printf("Path resolve results for query: %s\n", argv[1]);
	printf("found matching path: %s\n", rp->getPath());
	printf("path id: %d\n", rp->getPathId());
	printf("mode: %o\n", pr->getMode(rp));
	if (rp->hasPathFilter()) {
		printf("path has a filter.\n");
	}
	else printf("path has no filter.\n");


	if (pr->isDirectory(rp)) {
		ResolvedPath *entries = NULL;
		int n_entries = pr->getChildEntries(rp, &entries);

		printf("child entries of path: %s\n", argv[1]);
		printf("found %d child entries\n", n_entries);

		for (int i = 0; i < n_entries; i++) {
			ResolvedPath *entry = (ResolvedPath *)*((uintptr_t *)(entries + i));

			printf("----- entry %d -----\n", (i + 1));
			printf("matching path: %s\n", entry->getPath());
			printf("path id: %d\n", entry->getPathId());
			printf("mode: %o\n", pr->getMode(entry));
		}
	}
}