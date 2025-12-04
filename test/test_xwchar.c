#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include "xlocale.h"
#include "xwchar.h"


int main(void) {
    xinit_console_wutf();

    wchar_t buf[32];
    const wchar_t *str1 = L"Hello, 世界!";
    swprintf(buf, 32, L"%zu", xwcslen(str1));
    fwprintf(stderr,L"Test 1: \"%ls\" -> length: %ls\n", str1, buf);

    int num = 42;
    int hex = 0xFF;
    size_t len2 = xwcslen(L"Number: %d, Hex: 0x%X", num, hex);
    swprintf(buf, 32, L"%zu", len2);
    fwprintf(stderr,L"Test 2: formatted string -> length: %ls\n", buf);

    return 0;
}
