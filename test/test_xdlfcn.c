/*
 * test_xdlfcn.c
 *
 * Basic cross-platform tests for xdlfcn.h
 *
 * Build examples:
 *
 * POSIX:
 *   cc -std=c11 -Wall -Wextra -pedantic test_xdlfcn.c -ldl -o test_xdlfcn
 *
 * Windows/MSVC:
 *   cl /W4 test_xdlfcn.c
 *
 * Windows/MinGW:
 *   gcc -Wall -Wextra test_xdlfcn.c -o test_xdlfcn.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "xdlfcn.h"

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

static void test_error_api(void)
{
	printf("\n== Error API ==\n");

#ifdef _WIN32
	/*
	 * Force a Windows error.
	 */
	HMODULE h = xdlopen("__xdlfcn_this_module_does_not_exist__.dll",XRTLD_NOW);

	TEST("xdlopen() failure returns NULL", h == NULL);

	char *err = xdlerror();

	TEST("xdlerror() returns an error", err != NULL);
	TEST("xdlerror() returns non-empty string",err != NULL && *err != '\0');

	/*
	 * xdlerror() is documented by this implementation as consuming
	 * the Windows error.
	 */
	TEST("xdlerror() consumes error", xdlerror() == NULL);

#else
	/*
	 * dlopen() should fail for a deliberately nonexistent object.
	 */
	HMODULE h = xdlopen("__xdlfcn_this_module_does_not_exist__.so",XRTLD_NOW);

	TEST("xdlopen() failure returns NULL for xdlerror", h == NULL);

	char *err = xdlerror();

	TEST("xdlerror() returns an error", err != NULL);
	TEST("xdlerror() returns non-empty string",err != NULL && *err != '\0');

	/*
	 * POSIX dlerror() also consumes the pending error.
	 */
	TEST("xdlerror() consumes error", xdlerror() == NULL);

	h = xwdlopen(L"__xdlfcn_this_module_does_not_exist__.so",XRTLD_NOW);

	TEST("xwdlopen() failure returns NULL for xwdlerror", h == NULL);
	wchar_t *werr = xwdlerror();

	TEST("xwdlerror() returns an error", werr != NULL);
	TEST("xwdlerror() returns non-empty string",werr != NULL && *werr != L'\0');

	/*
	 * xwdlerror() consumes the pending loader error.
	 */
	TEST("xwdlerror() consumes error", xwdlerror() == NULL);
#endif
}

static void test_native_library(void)
{
	printf("\n== Native library ==\n");

#ifdef _WIN32

	/*
	 * kernel32.dll is guaranteed to exist on Windows.
	 */
	HMODULE h = xdlopen("kernel32.dll", XRTLD_NOW);

	TEST("xdlopen(kernel32.dll)", h != NULL);

	if (!h) return;

	FARPROC p1 = xdlsym(h, "GetModuleHandleA");

	TEST("xdlsym(GetModuleHandleA)", p1 != NULL);

	FARPROC p2 = GetProcAddressA(h, "GetModuleHandleA");

	TEST("GetProcAddressA(GetModuleHandleA)", p2 != NULL);
	TEST("xdlsym == GetProcAddressA", p1 == p2);

	FARPROC p3 = GetProcAddressW(h, L"GetModuleHandleA");

	TEST("GetProcAddressW(GetModuleHandleA)", p3 != NULL);

	FARPROC p4 = xwdlsym(h, L"GetModuleHandleA");

	TEST("xwdlsym(GetModuleHandleA)", p4 != NULL);
	TEST("xwdlsym == GetProcAddressW", p3 == p4);

	TEST("xdlclose(kernel32.dll)", xdlclose(h) == 0);

#else

	/*
	 * On POSIX, use the process's global symbol namespace.
	 *
	 * dlopen(NULL, ...) returns a handle for the main program.
	 */
	HMODULE h = xdlopen(NULL, XRTLD_NOW);

	TEST("xdlopen(NULL)", h != NULL);

	if (!h) return;

	/*
	 * printf is normally available through the process' global
	 * symbol namespace. RTLD_DEFAULT is also tested separately.
	 */
	FARPROC p1 = xdlsym(h, "printf");

	TEST("xdlsym(printf)", p1 != NULL);

	FARPROC p2 = xdlsym(XRTLD_DEFAULT, "printf");

	TEST("xdlsym(XRTLD_DEFAULT, printf)", p2 != NULL);

	/*
	 * RTLD_NEXT is only meaningful when the lookup occurs from
	 * a shared object. Therefore it is intentionally not tested
	 * as a successful lookup here.
	 */

	/*
	 * printf is normally available through the process' global
	 * symbol namespace. RTLD_DEFAULT is also tested separately.
	 */
	FARPROC wp1 = xwdlsym(h,L"printf");

	TEST("xwdlsym(printf)", p1 != NULL);

	FARPROC wp2 = xwdlsym(XRTLD_DEFAULT,L"printf");

	TEST("xwdlsym(XRTLD_DEFAULT, printf)", p2 != NULL);

	TEST("xdlclose(process handle)", xdlclose(h) == 0);

#endif
}

static void test_wide_api(void)
{
	printf("\n== Wide-character API ==\n");

#ifdef _WIN32

	HMODULE h = xwdlopen(L"kernel32.dll", XRTLD_NOW);

	TEST("xwdlopen(kernel32.dll)", h != NULL);

	if (!h)
		return;

	FARPROC p1 = xwdlsym(h, L"GetModuleHandleA");

	TEST("xwdlsym(GetModuleHandleA)", p1 != NULL);

	FARPROC p2 = GetProcAddressW(h, L"GetModuleHandleA");

	TEST("GetProcAddressW(GetModuleHandleA)", p2 != NULL);
	TEST("xwdlsym == GetProcAddressW", p1 == p2);

	TEST("xdlclose(wide handle)", xdlclose(h) == 0);

#else

	/*
	 * xwdlopen() converts wchar_t -> multibyte before calling dlopen().
	 */
	HMODULE h = xwdlopen(L"libc.so.6", XRTLD_NOW);

	/*
	 * libc.so.6 is not universal across all POSIX systems.
	 * Therefore, if it isn't present, use the process itself.
	 */
	if (!h)
		h = xwdlopen(NULL, XRTLD_NOW);

	TEST("xwdlopen()", h != NULL);

	if (!h)
		return;

	FARPROC p = xwdlsym(h, L"printf");

	TEST("xwdlsym(printf)", p != NULL);

	/*
	 * xwdlerror() should return a wchar_t string after an error.
	 */
	FARPROC invalid = xwdlsym(h, L"__xdlfcn_missing_symbol__");

	TEST("xwdlsym(missing symbol) returns NULL", invalid == NULL);

	wchar_t *err = xwdlerror();

	TEST("xwdlerror() returns an error", err != NULL);
	TEST("xwdlerror() returns non-empty string",err != NULL && *err != L'\0');

	/*
	 * On POSIX, xwdlerror() internally obtains dlerror() and
	 * converts the returned string to wchar_t.
	 *
	 * NOTE: The returned allocation belongs to xmbstowcs().
	 */
	free(err);

	TEST("xdlclose(wide handle)", xdlclose(h) == 0);

#endif
}

static void test_noload(void)
{
	printf("\n== XRTLD_NOLOAD ==\n");

#ifdef _WIN32

	/*
	 * kernel32.dll is already loaded by a normal Windows process.
	 */
	HMODULE h = xdlopen("kernel32.dll", XRTLD_NOLOAD);

	TEST("XRTLD_NOLOAD finds kernel32.dll", h != NULL);

	if (h) {
		/*
		 * GetModuleHandle-style handles must not be freed with
		 * FreeLibrary unless a corresponding LoadLibrary reference
		 * was acquired. So do NOT call xdlclose(h) here.
		 */
	}

#else

	/*
	 * libc is normally already loaded, but its exact soname varies.
	 * Test NOLOAD against the main process instead.
	 */
	HMODULE h = xdlopen(NULL, XRTLD_NOW | XRTLD_NOLOAD);

	TEST("XRTLD_NOLOAD with NULL", h != NULL);

	if (h) xdlclose(h);
#endif
}

static void test_invalid_arguments(void)
{
	printf("\n== Invalid arguments ==\n");

#ifdef _WIN32

	TEST("xdlsym(NULL, NULL) returns NULL",
	     xdlsym(NULL, NULL) == NULL);

	TEST("xwdlsym(NULL, NULL) returns NULL",
	     xwdlsym(NULL, NULL) == NULL);

	TEST("xdlclose(NULL) fails",
	     xdlclose(NULL) != 0);

#else

	TEST("xdlsym(NULL, NULL) returns NULL",
	     xdlsym(NULL, NULL) == NULL);

	TEST("xwdlsym(NULL, NULL) returns NULL",
	     xwdlsym(NULL, NULL) == NULL);

#endif
}

static void test_proc_address_wrappers(void)
{
	printf("\n== GetProcAddress wrappers ==\n");

#ifdef _WIN32

	HMODULE h = LoadLibraryA("kernel32.dll");

	TEST("LoadLibraryA(kernel32.dll)", h != NULL);

	if (!h)
		return;

	FARPROC a = GetProcAddressA(h, "GetModuleHandleA");
	FARPROC b = GetProcAddressW(h, L"GetModuleHandleA");

	TEST("GetProcAddressA()", a != NULL);
	TEST("GetProcAddressW()", b != NULL);
	TEST("A/W addresses match", a == b);

	TEST("Free library", xdlclose(h) == 0);

#else

	/*
	 * These wrappers emulate the Windows API on POSIX.
	 */
	HMODULE h = LoadLibraryA(NULL);

	TEST("LoadLibraryA(NULL)", h != NULL);

	if (!h)
		return;

	FARPROC p = GetProcAddress(h, "printf");

	TEST("GetProcAddress(printf)", p != NULL);

	TEST("Free process handle", xdlclose(h) == 0);

#endif
}

static void test_xdllibpath(void) {
	printf("\n== xdllibpath ==\n");

	char *path;

	path = xdllibpath("build", "libxc");
#ifdef _WIN32
	TEST("xdllibpath(\"build\", \"libxc\")", path != NULL);
	TEST("xdllibpath() produces .dll",
		 path != NULL && strstr(path, ".dll") != NULL);
#else
#if defined(__APPLE__)
	TEST("xdllibpath() produces .dylib",
		 path != NULL && strstr(path, ".dylib") != NULL);
#else
	TEST("xdllibpath() produces .so",
		 path != NULL && strstr(path, ".so") != NULL);
#endif
#endif

	if (path) {
		printf("Library path: %s\n", path);
		free(path);
	}

	path = xdllibpath("test", "libxc");
	TEST("xdllibpath(\"test\", \"libxc\")", path != NULL);

	if (path) {
		printf("Library path: %s\n", path);
		free(path);
	}

	path = xdllibpath(NULL, NULL);
	TEST("xdllibpath(NULL, NULL) fails", path == NULL);
}


void test_xdladdr(void) {
	printf("\n== xdladdr ==\n");
	XDl_info info;
	memset(&info, 0, sizeof(info));

	int result = xdladdr((const void *)&printf, &info);
	TEST("xdladdr() succeeds", result != 0);
	TEST("xdladdr() returns filename",result != 0 && info.dli_fname != NULL);
	TEST("xdladdr() returns module base",result != 0 && info.dli_fbase != NULL);

	if (result) {
		printf(" Filename : %s\n", info.dli_fname ? info.dli_fname : "(null)");
		printf(" Base     : %p\n", info.dli_fbase);
		printf(" Symbol   : %s\n", info.dli_sname ? info.dli_sname : "(null)");
		printf(" Address  : %p\n", info.dli_saddr);
	}

	TEST("xdladdr(NULL, &info) fails",xdladdr(NULL, &info) == 0);
	TEST("xdladdr(&printf, NULL) fails",xdladdr((const void *)&printf, NULL) == 0);
}

int main(void) {
	printf("========================================\n");
	printf(" xdlfcn test suite\n");
	printf("========================================\n");

#ifdef _WIN32
	printf("Platform: Windows\n");
#else
	printf("Platform: POSIX\n");
#endif

	test_error_api();
	test_native_library();
	test_wide_api();
	test_noload();
	test_invalid_arguments();
	test_proc_address_wrappers();
	test_xdllibpath();
	test_xdladdr();

	printf("\n========================================\n");
	printf(" Tests: %d\n", tests_run);
	printf(" Failed: %d\n", tests_failed);
	printf("========================================\n");

	return tests_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}