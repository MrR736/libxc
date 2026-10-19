/*
 * test_xmemory.c: Tests for xmemory.h
 *
 * Copyright (C) 2026 MrR736
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "xmemory.h"

#define TEST(name, expr) \
do { \
	if (!(expr)) { \
		fprintf(stderr, "FAIL: %s\n", name); \
		return EXIT_FAILURE; \
	} \
	printf("PASS: %s\n", name); \
} while (0)

int main(void)
{
	char src[] = "Hello, xmemory!";
	char dest[32];
	char buffer[32];
	void *p;
	int result;

	/*
	 * xmalloc / xfree
	 */
	p = xmalloc(32);
	TEST("xmalloc", p != NULL);

	memset(p, 0xA5, 32);
	xfree(p);

	/*
	 * xcalloc
	 */
	p = xcalloc(8, sizeof(int));
	TEST("xcalloc", p != NULL);

	{
		int *array = (int *)p;
		size_t i;

		for (i = 0; i < 8; ++i)
			TEST("xcalloc zero initialization", array[i] == 0);
	}

	xfree(p);

	/*
	 * xrealloc
	 */
	p = xmalloc(8);
	TEST("xrealloc initial allocation", p != NULL);

	memcpy(p, "test", 5);

	p = xrealloc(p, 32);
	TEST("xrealloc", p != NULL);
	TEST("xrealloc preserves data", strcmp((char *)p, "test") == 0);

	xfree(p);

	/*
	 * xmemset
	 */
	memset(buffer, 0, sizeof(buffer));
	TEST("xmemset",
		 xmemset(buffer, 'A', 5) == buffer &&
		 memcmp(buffer, "AAAAA", 5) == 0);

	/*
	 * xmemcpy
	 */
	memset(dest, 0, sizeof(dest));

	TEST("xmemcpy",
		 xmemcpy(dest, src, sizeof(src)) == dest &&
		 strcmp(dest, src) == 0);

	/*
	 * xmemccpy
	 *
	 * Copy through '!' and verify the returned pointer.
	 */
	memset(dest, 0, sizeof(dest));

	p = xmemccpy(dest, src, '!', sizeof(src));

	TEST("xmemccpy return value",
		 p == dest + strlen(src));

	TEST("xmemccpy contents",
		 strcmp(dest, src) == 0);

	/*
	 * xmemmove
	 *
	 * Deliberately use overlapping regions.
	 */
	strcpy(buffer, "abcdef");

	TEST("xmemmove",
		 xmemmove(buffer + 2, buffer, 4) == buffer + 2 &&
		 memcmp(buffer, "ababcd", 6) == 0);

	/*
	 * xmemcmp
	 */
	TEST("xmemcmp equal",
		 xmemcmp("abc", "abc", 3) == 0);

	result = xmemcmp("abc", "abd", 3);
	TEST("xmemcmp different", result < 0);

	result = xmemcmp("abd", "abc", 3);
	TEST("xmemcmp reverse different", result > 0);

	/*
	 * xmempcpy
	 */
	memset(dest, 0, sizeof(dest));

	p = xmempcpy(dest, src, strlen(src) + 1);

	TEST("xmempcpy return value",
		 p == dest + strlen(src) + 1);

	TEST("xmempcpy contents",
		 strcmp(dest, src) == 0);

	/*
	 * xmemchr
	 */
	p = xmemchr(src, 'x', strlen(src));

	TEST("xmemchr found",
		 p == src + 7);

	p = xmemchr(src, 'Z', strlen(src));

	TEST("xmemchr not found",
		 p == NULL);

	/*
	 * Final result.
	 */
	printf("\nAll xmemory tests passed.\n");

	return EXIT_SUCCESS;
}