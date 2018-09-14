#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

static int compare_dirent(const void*, const void*);

int main(void) {
	DIR* dir;
	struct dirent *dirent;
	size_t cap = 8;
	size_t len = 0;
	struct dirent *buf = malloc(cap * sizeof(struct dirent));
	if(!buf) {
		perror("OOM");
		return 2;
	}

	dir = opendir(".");
	if(!dir) {
		perror("opendir failed");
		free(buf);
		return 1;
	}

	while((dirent = readdir(dir))) {
		if(len == cap) {
			cap *= 2;
			buf = realloc(buf, cap * sizeof(struct dirent));
			if(!buf) {
				perror("OOM");
				return 2;
			}
		}
		buf[len++] = *dirent;
	}

	qsort(buf, len, sizeof(struct dirent), compare_dirent);

	for(size_t i = 0; i < len; i++) {
		puts(buf[i].d_name);
	}

	free(buf);
	return 0;
}

static int compare_dirent(const void* lv, const void* rv) {
	const struct dirent* l = (const struct dirent*) lv;
	const struct dirent* r = (const struct dirent*) rv;
	return strcmp(l->d_name, r->d_name);
}
