#include "xlocale.h"
#include <stdio.h>
#include <wchar.h>

static void test_xsetlocale(void) {
    printf("=== xsetlocale tests ===\n");

    const char *tests[] = {
        "UTF-8", "C.UTF-8", ".UTF8", "en_US.UTF-8",
        "invalid-locale-name", "", NULL
    };

    for (int i = 0; tests[i]; i++) {
        const char *req = tests[i];
        char *ret = xsetlocale(XLC_ALL, req);
        printf("xsetlocale(XLC_ALL, \"%s\") → %s\n",
               req, ret ? ret : "(null)");
    }
    printf("\n");
}

static void test_xwsetlocale(void) {
    printf("=== xwsetlocale tests (wide) ===\n");

    const wchar_t *tests[] = {
        L"UTF-8", L"C.UTF-8", L".UTF8", L"en_US.UTF-8",
        L"invalid-locale-name", L"", NULL
    };

    for (int i = 0; tests[i]; i++) {
        const wchar_t *req = tests[i];
        wchar_t *ret = xwsetlocale(XLC_ALL, req);
        printf("xwsetlocale(XLC_ALL, \"%ls\") → %ls\n",
               req, ret ? ret : L"(null)");
    }
    printf("\n");
}

static void test_wsetlocale_wrapper(void) {
    printf("=== wsetlocale wrapper test ===\n");

    wchar_t *ret = wsetlocale(XLC_ALL, L"UTF-8");
    printf("wsetlocale(XLC_ALL, L\"UTF-8\") → %ls\n",
           ret ? ret : L"(null)");

    printf("\n");
}

static void test_newlocale(void) {
    printf("=== newlocale / freelocale tests ===\n");

    xlocale_t loc = xnewlocale(
        XLC_CTYPE_MASK | XLC_NUMERIC_MASK,
        "C.UTF-8",
        NULL
    );

    if (!loc) {
        printf("xnewlocale failed\n");
    } else {
        printf("xnewlocale succeeded\n");
        xfreelocale(loc);
        printf("xfreelocale done\n");
    }

    printf("\n");
}

int main(void) {
    printf("*************************************\n");
    printf("      xlocale.h Test Program\n");
    printf("*************************************\n\n");

    test_xsetlocale();
    test_xwsetlocale();
    test_wsetlocale_wrapper();
    test_newlocale();

    printf("Done.\n");
    return 0;
}
