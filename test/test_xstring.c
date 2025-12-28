#include <stdio.h>
#include <stdarg.h>
#include "xstring.h"

int main() {
    // Test 1: simple string
    const char *str1 = "Hello, World!";
    size_t len1 = xstrlen(str1);
    printf("Test 1: \"%s\" -> length: %zu\n", str1, len1);

    // Test 2: string with format arguments
    int num = 42;
    int hex = 255;

    // Using vxstrlen directly
    va_list ap;
    // Note: vxstrlen requires a va_list. We'll wrap it in a helper.
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Number: %d, Hex: 0x%X", num, hex);

    size_t len2 = xstrlen("Number: %d, Hex: 0x%X", num, hex);

    printf("Test 2: formatted string \"%s\" -> length: %zu\n", buffer, len2);

    // Test 3: variadic xstrlen with format
    size_t len3;
    // Wrap xstrlen with formatted string
    char formatted[256];
    snprintf(formatted, sizeof(formatted), "Number: %d, Hex: 0x%X", num, hex);
    len3 = xstrlen(formatted);

    printf("Test 3: xstrlen formatted \"%s\" -> length: %zu\n", formatted, len3);

    fprintf(stderr, "\nTest: Matching strings\n");
    // Test case 1: Matching strings
    const char *str2[] = {"world", "Hello, World!", "foo"};
    fprintf(stderr, "Test 1: %s\n", xstrcmp(str1, str2, xsizeof(str2)) == 1 ? "Passed" : "Failed");

    // Test case 2: Non-matching strings
    const char *str3[] = {"goodbye", "hello", "foo"};
    fprintf(stderr, "Test 2: %s\n", xstrcmp(str1, str3, xsizeof(str3)) == 0 ? "Passed" : "Failed");

    // Test case 3: NULL in __s2 array, non-matching strings
    const char *str4[] = {NULL, "hello", "foo"};
    fprintf(stderr, "Test 3: %s\n", xstrcmp(str1, str4, xsizeof(str4)) == 0 ? "Passed" : "Failed");

    // Test case 4: NULL __s1, matching NULL in __s2
    const char *str5 = NULL;
    const char *str6[] = {NULL, "hello", "foo"};
    fprintf(stderr, "Test 4: %s\n", xstrcmp(str5, str6, xsizeof(str6)) == 1 ? "Passed" : "Failed");

    // Test case 5: NULL __s1 and NULL in __s2, no match
    const char *str7 = NULL;
    const char *str8[] = {NULL, NULL};
    fprintf(stderr, "Test 5: %s\n", xstrcmp(str7, str8, xsizeof(str8)) == 1 ? "Passed" : "Failed");

    // Test case 6: NULL __s1 and NULL in __s2, all elements are NULL
    const char *str9 = NULL;
    const char *str10[] = {NULL, NULL, NULL};
    fprintf(stderr, "Test 6: %s\n", xstrcmp(str9, str10, xsizeof(str10)) == 1 ? "Passed" : "Failed");

    return 0;
}
