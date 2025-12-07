#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <wchar.h>
#include "xstdio.h"

int main() {
	// ---------- Test 1: fdputs ----------
	int fd = open("test.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	if (fdputs("Hello, xstdio fdputs!\n", fd) != 0) {
		perror("fdputs");
	}
	close(fd);

	// ---------- Test 2: furead ----------
	FILE *fp = fopen("test.txt", "r");
	if (!fp) {
		perror("fopen");
		return 1;
	}

	size_t size;
	char *content = (char*)furead(fp, &size);
	if (content) {
		printf("furead (%zu bytes): %s\n", size, content);
		free(content);
	} else {
		perror("furead");
	}
	fclose(fp);

	// ---------- Test 3: fduread ----------
	fd = open("test.txt", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}
	unsigned char *buf = (unsigned char*)fduread(fd, &size);
	if (buf) {
		printf("fduread (%zu bytes): %s\n", size, buf);
		free(buf);
	} else {
		perror("fduread");
	}
	close(fd);

	// ---------- Test 4: cprintf / vcprintf ----------
	char *s = cprintf("Formatted string: %d + %d = %d", 5, 10, 5 + 10);
	if (s) {
		printf("%s\n", s);
		free(s);
	}

	// ---------- Test 5: wcprintf / vwcprintf ----------
	wchar_t *ws = wcprintf(L"Wide string: %ls %d", L"xstdio", 2025);
	if (ws) {
		wprintf(L"%ls\n", ws);
		free(ws);
	}

	// ---------- Test 6: xremove ----------
	if (xremove("test.txt") == 0) {
		printf("File 'test.txt' removed successfully\n");
	} else {
		perror("xremove");
	}

	// ---------- Test 7: xwremove ----------
	wchar_t filename[] = L"test_w.txt";
	FILE *wfp = wfopen(filename, L"w");
	if (wfp) {
		fputws(L"Test wide file\n", wfp);
		fclose(wfp);
	}

	if (xwremove(filename) == 0) {
		wprintf(L"File '%ls' removed successfully\n", filename);
	} else {
		perror("xwremove");
	}

	// ---------- Test 8: getcurrentdirectory ----------
	char* dc = getcurrentdirectory();
	if (dc) {
		fprintf(stdout, "Current Executable: \"%s\"\n", dc);
		free(dc);
	}

	// ---------- Test 9: wgetcurrentdirectory ----------
	wchar_t* wdc = wgetcurrentdirectory();
	if (wdc) { // Fix: Check if 'wdc' is not NULL
		fwprintf(stdout, L"Current Executable: L\"%ls\"\n", wdc);
		free(wdc);
	}

	return 0;
}
