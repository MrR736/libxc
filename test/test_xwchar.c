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
    fwprintf(stderr, L"Test 1: \"%ls\" -> length: %ls\n", str1, buf);

    int num = 42;
    int hex = 0xFF;
    size_t len2 = xwcslen(L"Number: %d, Hex: 0x%X", num, hex);
    swprintf(buf, 32, L"%zu", len2);
    fwprintf(stderr, L"Test 2: formatted string -> length: %ls\n", buf);

    // Test cases for xwcscmp

    fwprintf(stderr, L"\nTest: Matching strings\n");
    // Test case 1: Matching strings
    const wchar_t *str2[] = {L"world", L"Hello, 世界!", L"foo"};
    fwprintf(stderr, L"Test 1: %ls\n", xwcscmp(str1, str2, 3) == 1 ? L"Passed" : L"Failed");

    // Test case 2: Non-matching strings
    const wchar_t *str3[] = {L"goodbye", L"hello", L"foo"};
    fwprintf(stderr, L"Test 2: %ls\n", xwcscmp(str1, str3, 3) == 0 ? L"Passed" : L"Failed");

    // Test case 3: NULL in __s2 array, non-matching strings
    const wchar_t *str4[] = {NULL, L"hello", L"foo"};
    fwprintf(stderr, L"Test 3: %ls\n", xwcscmp(str1, str4, 3) == 0 ? L"Passed" : L"Failed");

    // Test case 4: NULL __s1, matching NULL in __s2
    const wchar_t *str5 = NULL;
    const wchar_t *str6[] = {NULL, L"hello", L"foo"};
    fwprintf(stderr, L"Test 4: %ls\n", xwcscmp(str5, str6, 3) == 1 ? L"Passed" : L"Failed");

    // Test case 5: NULL __s1 and NULL in __s2, no match
    const wchar_t *str7 = NULL;
    const wchar_t *str8[] = {NULL, NULL};
    fwprintf(stderr, L"Test 5: %ls\n", xwcscmp(str7, str8, 2) == 1 ? L"Passed" : L"Failed");

    // Test case 6: NULL __s1 and NULL in __s2, all elements are NULL
    const wchar_t *str9 = NULL;
    const wchar_t *str10[] = {NULL, NULL, NULL};
    fwprintf(stderr, L"Test 6: %ls\n", xwcscmp(str9, str10, 3) == 1 ? L"Passed" : L"Failed");

    return 0;
}
