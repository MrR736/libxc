# **XSTRING — Formatted Narrow-String Length Utilities**

*A small portability layer for computing formatted string lengths without output*
**Author:** MrR736
**License:** GPL-3.0+

---

## Overview

`xstring.h` provides lightweight helper functions for:

* Measuring the length of formatted strings without producing output:

  * **`vxstrlen()`** — uses a `va_list`
  * **`xstrlen()`** — variadic wrapper
* Comparing strings against an array of string literals:

  * **`xstrcmp()`** — boolean-style matcher

These functions are **portable** and safe across platforms:

* Windows: uses `_vscprintf()`
* POSIX: uses `vsnprintf(NULL,0,...)`

They do **not allocate memory** and **do not write output**, making them suitable for size probes.

---

## 1. Function Summary

```c
size_t vxstrlen(const char *restrict fmt, va_list ap);
size_t xstrlen(const char *restrict fmt, ...);
int xstrcmp(const char * __s1, const char ** __s2, size_t n);
```

| Function   | Purpose                                                                 |
| ---------- | ----------------------------------------------------------------------- |
| `vxstrlen` | Returns length of formatted output using `va_list`                      |
| `xstrlen`  | Convenience variadic wrapper around `vxstrlen`                          |
| `xstrcmp`  | Checks if a string matches any entry in a string array (returns 0 or 1) |

---

## 2. Detailed Function Behavior

### `vxstrlen()`

* **Windows:** `_vscprintf(fmt, ap)` returns the number of characters excluding the null terminator.
* **POSIX:** `vsnprintf(NULL, 0, fmt, ap)` returns required byte count excluding `\0`, negative on error.
* **Common behavior:**

  * Returns `-1` if format evaluation fails
  * Does not write to a buffer
  * Never allocates memory

---

### `xstrlen()`

Convenience wrapper:

```c
va_list ap;
va_start(ap, fmt);
len = vxstrlen(fmt, ap);
va_end(ap);
```

Equivalent to computing the length of `sprintf(fmt, ...)` without output. Safe and side-effect-free.

---

### `xstrcmp()`

**Description:**
Compares a single string (`__s1`) against an array of strings (`__s2`) of length `n`. Returns **1 if a match exists**, otherwise **0**. Null-safe.

#### Parameters

| Parameter | Type            | Description                                              |
| --------- | --------------- | -------------------------------------------------------- |
| `__s1`    | `const char *`  | String to compare. Can be `NULL`.                        |
| `__s2`    | `const char **` | Array of strings to compare against. Can contain `NULL`. |
| `n`       | `size_t`        | Number of elements in the array `__s2`.                  |

#### Behavior

* Both `__s1` and `__s2[i]` being `NULL` counts as a match.
* One being `NULL` and the other not → skipped.
* Otherwise, standard `strcmp()` comparison is used.

#### Example Usage

```c
#include <stdio.h>
#include <string.h>
#include "xstring.h"

const char *arr[] = {"fff", "333", NULL};

int main(void) {
    int result;

    result = xstrcmp("fff", arr, 3);   // returns 1
    result = xstrcmp("abc", arr, 3);   // returns 0
    result = xstrcmp(NULL, arr, 3);    // returns 0
    result = xstrcmp(NULL, (const char*[]){NULL, "foo"}, 2); // returns 1

    printf("Results: %d\n", result);
    return 0;
}
```

---

## 3. Error Handling

| Condition                | Result         |
| ------------------------ | -------------- |
| Format OK                | Returns length |
| Formatting error         | Returns 0      |
| Windows internal failure | Returns 0      |
| `fmt == NULL`            | Returns 0      |

No `errno` is set — mirrors behavior of `_vscprintf` and `vsnprintf`.

---

## 4. Portability Notes

* `_vscprintf()` accurately probes formatted string length on Windows.
* `vsnprintf(NULL,0,...)` works on POSIX (C99+).
* No dynamic allocation or buffer probing needed.
* Compatible with:

  * Linux, BSD, macOS
  * Windows (MSVCRT)
  * MinGW / MSYS2

---

## 5. Example Usage

### Allocate a buffer of the exact required length

```c
size_t len = xstrlen("%s: %d", "count", 42);
char *buf = malloc(len + 1);
sprintf(buf, "%s: %d", "count", 42);
```

### Variadic wrapper using `vxstrlen()`

```c
size_t format_and_measure(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    size_t len = vxstrlen(fmt, ap);
    va_end(ap);
    return len;
}
```

---

## 6. License

This library is distributed under the **GNU GPL v3 or later**.
