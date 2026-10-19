#include <stdio.h>
#include <string.h>
#include "xxlocale.h"

static int test_xgetlocale(void) {
	int failed = 0;

	printf("=== xgetlocale tests ===\n");

	size_t n;
	char **locales = xgetlocale(&n);

	printf("xgetlocale locale: %s, size of locales: %zu\n",locales[5],n);

	printf("xgetlocale: %s\n\n", failed ? "FAIL" : "PASS");
	return failed;
}

static int test_xwgetlocale(void) {
	int failed = 0;

	printf("=== xwgetlocale tests ===\n");

	size_t n;
	wchar_t **locales = xwgetlocale(&n);

	printf("xwgetlocale locale: %ls, size of locales: %zu\n",locales[5],n);

	printf("xwgetlocale: %s\n\n", failed ? "FAIL" : "PASS");
	return failed;
}


static int test_xsetlocale(void) {
	int failed = 0;

	printf("=== xsetlocale tests ===\n");

	const char *tests[] = {
		"UTF-8",
		"C.UTF-8",
		".UTF8",
		"en_US.UTF-8",
		"invalid-locale-name",
		"",
		NULL
	};

	for (int i = 0; tests[i] != NULL; ++i) {
		const char *req = tests[i];
		char *ret = xsetlocale(XLC_ALL, req);

		printf("xsetlocale(XLC_ALL, \"%s\") -> %s\n",
			   req, ret != NULL ? ret : "(null)");

		if (strcmp(req, "invalid-locale-name") == 0) {
			/* An invalid locale must be rejected. */
			if (ret == NULL) {
				printf("  PASS: invalid locale rejected\n");
			} else {
				printf("  FAIL: invalid locale accepted: %s\n", ret);
				failed = 1;
			}
		} else {
			/* All other inputs are expected to succeed. */
			if (ret != NULL) {
				printf("  PASS: locale accepted\n");
			} else {
				printf("  FAIL: expected locale to succeed: %s\n", req);
				failed = 1;
			}
		}
	}

	printf("xsetlocale: %s\n\n", failed ? "FAIL" : "PASS");
	return failed;
}


static int test_xwsetlocale(void) {
	int failed = 0;

	printf("=== xwsetlocale tests ===\n");

	const wchar_t *tests[] = {
		L"UTF-8",
		L"C.UTF-8",
		L".UTF8",
		L"en_US.UTF-8",
		L"invalid-locale-name",
		L"",
		NULL
	};

	for (int i = 0; tests[i] != NULL; ++i) {
		const wchar_t *req = tests[i];
		wchar_t *ret = xwsetlocale(XLC_ALL, req);

		printf("xwsetlocale(XLC_ALL, \"%ls\") -> %ls\n",
			   req, ret != NULL ? ret : L"(null)");

		if (wcscmp(req, L"invalid-locale-name") == 0) {
			/* An invalid locale must be rejected. */
			if (ret == NULL) {
				printf("  PASS: invalid locale rejected\n");
			} else {
				printf("  FAIL: invalid locale accepted: %ls\n", ret);
				failed = 1;
			}
		} else {
			/* All other inputs are expected to succeed. */
			if (ret != NULL) {
				printf("  PASS: locale accepted\n");
			} else {
				printf("  FAIL: expected locale to succeed: %ls\n", req);
				failed = 1;
			}
		}
	}

	printf("xwsetlocale: %s\n\n", failed ? "FAIL" : "PASS");
	return failed;
}

static int test_setlocale(void) {
	int failed = 0;

	printf("=== setlocale test ===\n");

#ifdef _WIN32
	char *ret = setlocale(LC_ALL, ".65001");
	printf("setlocale(XLC_ALL, L\".65001\") → %s\n",ret ? ret : "(null)");
#else
	char *ret = setlocale(LC_ALL, "C.UTF-8");
	printf("setlocale(XLC_ALL, L\"C.UTF-8\") → %s\n",ret ? ret : "(null)");
#endif

	if (ret == NULL) {
		printf("  FAIL: setlocale returned NULL\n");
		failed = 1;
	}

	printf("setlocale: %s\n\n", failed ? "FAIL" : "PASS");
	return failed;
}

static int test_wsetlocale_wrapper(void) {
	int failed = 0;

	printf("=== wsetlocale wrapper test ===\n");

#ifdef _WIN32
	wchar_t *ret = wsetlocale(XLC_ALL, L".65001");
	printf("wsetlocale(XLC_ALL, L\".65001\") → %ls\n",ret ? ret : L"(null)");
#else
	wchar_t *ret = wsetlocale(XLC_ALL, L"C.UTF-8");
	printf("wsetlocale(XLC_ALL, L\"C.UTF-8\") → %ls\n",ret ? ret : L"(null)");
#endif

	if (ret == NULL) {
		printf("  FAIL: wsetlocale returned NULL\n");
		failed = 1;
	}

	printf("wsetlocale: %s\n\n", failed ? "FAIL" : "PASS");
	return failed;
}

static int test_newlocale(void) {
	int failed = 0;

	printf("=== newlocale / freelocale tests ===\n");

	xlocale_t loc = xnewlocale(
		XLC_CTYPE_MASK | XLC_NUMERIC_MASK,
		"C.UTF-8",
		NULL
	);

	if (!loc) {
		printf("  FAIL: xnewlocale returned NULL\n");
		failed = 1;
	} else {
		printf("xnewlocale succeeded\n");

		xfreelocale(loc);

		printf("xfreelocale done\n");
	}

	printf("newlocale: %s\n\n", failed ? "FAIL" : "PASS");
	return failed;
}

int main(void) {
	int failed = 0;

	printf("*************************************\n");
	printf("	  xxlocale.h Test Program\n");
	printf("*************************************\n\n");

	failed |= test_xgetlocale();
	failed |= test_xwgetlocale();
	failed |= test_xsetlocale();
	failed |= test_xwsetlocale();
	failed |= test_setlocale();
	failed |= test_wsetlocale_wrapper();
	failed |= test_newlocale();

	printf("*************************************\n");

	if (failed) {
		printf("TEST RESULT: FAIL\n");
		return 1;
	}

	printf("TEST RESULT: PASS\n");
	return 0;
}