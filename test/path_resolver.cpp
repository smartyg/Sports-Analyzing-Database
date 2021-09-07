#include "config.h"

#include "path.hpp"
#include "pathresolver.hpp"
#include "resolvedpath.hpp"
#include "libsad.hpp"

#include <stdio.h>

int main(int argc, char *argv[])
{
	(void) argc;

	Libsad *lib = new Libsad();
	PathResolver *pr = new PathResolver(lib, paths);
	const ResolvedPath *rp = pr->resolve(argv[1]);

	printf("Path resolve results for query: %s\n", argv[1]);
	printf("found matching path: %s\n", rp->getPath());
	printf("path id: %d\n", rp->getPathId());
	printf("mode: %o\n", pr->getMode(rp));
	if (rp->hasFilter()) {
		printf("path has a filter.\n");
		PathResolver::BaseFilter base_filter = rp->getPathFilterBase ();
		Libsad::FilterInfoType filter_info = rp->getPathFilterInfo ();
		printf("base filter: %d\n", base_filter);
		printf("year: %ld\n", filter_info.filter_year);
		printf("month: %ld\n", filter_info.filter_month);
		printf("day: %ld\n", filter_info.filter_day);
		printf("hour: %ld\n", filter_info.filter_hour);
		printf("minute: %ld\n", filter_info.filter_minute);
		printf("second: %ld\n", filter_info.filter_second);
		printf("activity: %ld\n", filter_info.filter_activity);
		printf("first letter: %c\n", static_cast<int>(filter_info.filter_title_first_letter));
		printf("title: %s\n", filter_info.filter_title);
		if (rp->hasObjectId ()) printf("object id: %ld\n", rp->getObjectId ());
		if (rp->hasObjectTime ()) printf("object time: %ld\n", rp->getObjectTime ());
		if (rp->isFilterComplete ()) printf("filter is complete\n");
		else printf("filter is not complete\n");
	}
	else printf("path has no filter.\n");


	if (pr->isDirectory(rp)) {
		const vector<const ResolvedPath *> *entries = pr->getChildEntries(rp);

		printf("child entries of path: %s\n", argv[1]);
		printf("found %ld child entries\n", entries->size ());

		for (const ResolvedPath *entry : *entries) {
			printf("----- entry -----\n");
			printf("matching path: %s\n", entry->getPath());
			printf("path id: %d\n", entry->getPathId());
			printf("mode: %o\n", pr->getMode(entry));
		}
	}
}