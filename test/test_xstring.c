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

    return 0;
}
