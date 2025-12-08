#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>
#include "xwctype.h"

int main() {
    // Test 1: Convert wide string to lowercase
    wchar_t input1[] = L"Hello World!";
    wchar_t* result_lower = xtowlower(input1);
    if (result_lower) {
        wprintf(L"Lowercase: %ls\n", result_lower);
        free(result_lower);
    } else {
        perror("Error in xtowlower");
    }

    // Test 2: Convert wide string to uppercase
    wchar_t input2[] = L"Hello World!";
    wchar_t* result_upper = xtowupper(input2);
    if (result_upper) {
        wprintf(L"Uppercase: %ls\n", result_upper);
        free(result_upper);
    } else {
        perror("Error in xtowupper");
    }

    // Test 3: Convert wide string to ASCII
    wchar_t input3[] = L"Hello 你好 World!";
    wchar_t* result_ascii = xtowascii(input3);
    if (result_ascii) {
        wprintf(L"ASCII: %ls\n", result_ascii);
        free(result_ascii);
    } else {
        perror("Error in xtowascii");
    }

    return 0;
}
