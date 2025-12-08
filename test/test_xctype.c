#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "xctype.h"

void test_xtolower() {
    char str[] = "Hello, World!";
    char* result = xtolower(str);

    if (result == NULL) {
        perror("xtolower failed");
        return;
    }

    printf("xtolower: %s\n", result);
    free(result);
}

void test_xtoupper() {
    char str[] = "Hello, World!";
    char* result = xtoupper(str);

    if (result == NULL) {
        perror("xtoupper failed");
        return;
    }

    printf("xtoupper: %s\n", result);
    free(result);
}

void test_xtoascii() {
    char *str = "Hello, World! 😀";
    char* result = xtoascii(str);

    if (result == NULL) {
        perror("xtoascii failed");
        return;
    }

    printf("xtoascii: %s\n", result);
    free(result);
}

int main() {
    test_xtolower();
    test_xtoupper();
    test_xtoascii();
    return 0;
}
