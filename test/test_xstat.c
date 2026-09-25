#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>

#include "xstat.h"

#ifdef _WIN32
# include <direct.h>
# define GETCWD _getcwd
# define GETWCWD _wgetcwd
# define PATH_SEP '\\'
#else
# include <unistd.h>
# define GETCWD getcwd
# define GETWCWD getcwd
# define PATH_SEP '/'
#endif


static int tests_run = 0;
static int tests_failed = 0;


#define TEST(name, expr) \
do { \
	tests_run++; \
	if (expr) { \
		printf("[ PASS ] %s\n", name); \
	} else { \
		printf("[ FAIL ] %s (%s:%d)\n", name, __FILE__, __LINE__); \
		tests_failed++; \
	} \
} while (0)


static int same_path(const char *a, const char *b)
{
	if (!a || !b)
		return 0;

#ifdef _WIN32
	return _stricmp(a, b) == 0;
#else
	return strcmp(a, b) == 0;
#endif
}


static void print_current_directory(void)
{
	char buffer[4096];

	if (GETCWD(buffer, sizeof(buffer)) != NULL)
		printf("Current directory: %s\n", buffer);
	else
		printf("Current directory: <error>\n");
}


/*
 * Test wchdir() directly.
 *
 * We change to "." because it is guaranteed to exist and does not
 * disturb the test process' working directory.
 */
static void test_wchdir(void)
{
	printf("\n== wchdir() ==\n");

	char before[4096];
	char after[4096];

	TEST("get initial working directory",
	     GETCWD(before, sizeof(before)) != NULL);

	if (!GETCWD(before, sizeof(before)))
		return;

	int ret = wchdir(L".");

	TEST("wchdir(L\".\") succeeds", ret == 0);

	TEST("working directory remains valid",
	     GETCWD(after, sizeof(after)) != NULL);

	if (GETCWD(after, sizeof(after))) {
		TEST("working directory unchanged",
		     same_path(before, after));
	}
}


/*
 * Test wrmdir() without actually deleting anything.
 *
 * A nonexistent directory is used so the operation should fail.
 */
static void test_wrmdir_failure(void)
{
	printf("\n== wrmdir() ==\n");

	int ret = wrmdir(L"__xdlfcn_xstat_nonexistent_directory__");

	TEST("wrmdir(nonexistent) fails", ret != 0);
}


/*
 * Test xchdir() formatting.
 */
static void test_xchdir(void)
{
	printf("\n== xchdir() ==\n");

	char before[4096];
	char after[4096];

	TEST("get initial directory",
	     GETCWD(before, sizeof(before)) != NULL);

	if (!GETCWD(before, sizeof(before)))
		return;

	int ret = xchdir("%s", ".");

	TEST("xchdir(\"%s\", \".\") succeeds", ret == 0);

	TEST("get directory after xchdir",
	     GETCWD(after, sizeof(after)) != NULL);

	if (GETCWD(after, sizeof(after))) {
		TEST("xchdir(\".\") preserves directory",
		     same_path(before, after));
	}
}


/*
 * Test xwchdir() formatting.
 */
static void test_xwchdir(void)
{
	printf("\n== xwchdir() ==\n");

	char before[4096];
	char after[4096];

	TEST("get initial directory",
	     GETCWD(before, sizeof(before)) != NULL);

	if (!GETCWD(before, sizeof(before)))
		return;

	int ret = xwchdir(L"%ls", L".");

	TEST("xwchdir(L\"%ls\", L\".\") succeeds", ret == 0);

	TEST("get directory after xwchdir",
	     GETCWD(after, sizeof(after)) != NULL);

	if (GETCWD(after, sizeof(after))) {
		TEST("xwchdir(L\".\") preserves directory",
		     same_path(before, after));
	}
}


/*
 * Test xrmdir() failure.
 */
static void test_xrmdir_failure(void)
{
	printf("\n== xrmdir() ==\n");

	int ret = xrmdir("%s", "__xstat_nonexistent_directory__");

	TEST("xrmdir(nonexistent) fails", ret != 0);
}


/*
 * Test xwrmdir() failure.
 */
static void test_xwrmdir_failure(void)
{
	printf("\n== xwrmdir() ==\n");

	int ret = xwrmdir(
		L"%ls",
		L"__xstat_nonexistent_directory__"
	);

	TEST("xwrmdir(nonexistent) fails", ret != 0);
}


/*
 * Test the NULL-path handling of the formatting wrappers.
 */
static void test_null_arguments(void)
{
	printf("\n== NULL arguments ==\n");

	errno = 0;
	TEST("xchdir(NULL) fails",
	     xchdir(NULL) != 0);

	TEST("xchdir(NULL) sets EINVAL",
	     errno == EINVAL);


	errno = 0;
	TEST("xwchdir(NULL) fails",
	     xwchdir(NULL) != 0);

	TEST("xwchdir(NULL) sets EINVAL",
	     errno == EINVAL);


	errno = 0;
	TEST("xrmdir(NULL) fails",
	     xrmdir(NULL) != 0);

	TEST("xrmdir(NULL) sets EINVAL",
	     errno == EINVAL);


	errno = 0;
	TEST("xwrmdir(NULL) fails",
	     xwrmdir(NULL) != 0);

	TEST("xwrmdir(NULL) sets EINVAL",
	     errno == EINVAL);
}


/*
 * Test formatting with actual arguments rather than simply "%s".
 *
 * This catches implementations where the variadic formatting layer
 * does not correctly forward arguments.
 */
static void test_formatted_paths(void)
{
	printf("\n== Formatted paths ==\n");

	const char *dot = ".";

	int ret = xchdir("%s", dot);

	TEST("xchdir formatted narrow path", ret == 0);


	const wchar_t *wdot = L".";

	ret = xwchdir(L"%ls", wdot);

	TEST("xwchdir formatted wide path", ret == 0);
}


/*
 * Test paths containing several formatting arguments.
 *
 * The resulting path is "./." which is equivalent to ".".
 */
static void test_multiple_arguments(void)
{
	printf("\n== Multiple formatting arguments ==\n");

	int ret = xchdir("%s%c%s", ".", PATH_SEP, ".");

	TEST("xchdir multiple arguments", ret == 0);

	ret = xwchdir(L"%ls%c%ls", L".", (wchar_t)PATH_SEP, L".");

	TEST("xwchdir multiple arguments", ret == 0);
}


/*
 * Test that formatted rmdir paths are actually passed through the
 * formatting machinery.
 */
static void test_formatted_rmdir(void)
{
	printf("\n== Formatted rmdir ==\n");

	const char *prefix = "__xstat_";
	const char *suffix = "_does_not_exist__";

	int ret = xrmdir("%s%s", prefix, suffix);

	TEST("xrmdir formatted path fails correctly", ret != 0);


	ret = xwrmdir(
		L"%ls%ls",
		L"__xstat_",
		L"_does_not_exist__"
	);

	TEST("xwrmdir formatted path fails correctly", ret != 0);
}


/*
 * Verify that xchdir() and xwchdir() can be used repeatedly.
 */
static void test_repeated_operations(void)
{
	printf("\n== Repeated operations ==\n");

	int success = 1;

	for (int i = 0; i < 10; ++i) {
		if (xchdir("%s", ".") != 0) {
			success = 0;
			break;
		}

		if (xwchdir(L"%ls", L".") != 0) {
			success = 0;
			break;
		}
	}

	TEST("repeated xchdir/xwchdir operations", success);
}


int main(void)
{
	printf("========================================\n");
	printf(" xstat test suite\n");
	printf("========================================\n");

#ifdef _WIN32
	printf("Platform: Windows\n");
#else
	printf("Platform: POSIX\n");
#endif

	print_current_directory();

	test_wchdir();
	test_wrmdir_failure();

	test_xchdir();
	test_xwchdir();

	test_xrmdir_failure();
	test_xwrmdir_failure();

	test_null_arguments();

	test_formatted_paths();
	test_multiple_arguments();
	test_formatted_rmdir();

	test_repeated_operations();

	printf("\n========================================\n");
	printf(" Tests run:    %d\n", tests_run);
	printf(" Tests failed: %d\n", tests_failed);
	printf("========================================\n");

	return tests_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}