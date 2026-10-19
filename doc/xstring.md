# **XSTRING — Formatted Narrow-String & Path Utilities**

*A small portability layer for formatted string operations, string matching, concatenation, and path utilities*
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

* Combining strings safely:

  * **`xstrcmb()`** — concatenates two strings into a newly allocated buffer

* Extracting the filename component from a path:

  * **`xbasename()`** — returns a pointer to the final path component

These functions are designed to be portable across Windows and POSIX-style platforms.

The formatted-length functions do **not allocate memory** and do **not write output**, making them suitable for size probes.

`xbasename()` supports both Unix/POSIX (`/`) and Windows (`\`) path separators.

---

## 1. Function Summary

```c
size_t vxstrlen(const char *restrict fmt, va_list ap);
size_t xstrlen(const char *restrict fmt, ...);
int xstrcmp(const char *__s1, const char **__s2, size_t n);
char *xstrcmb(const char *s1, const char *s2);
const char *xbasename(const char *path);
```

| Function    | Purpose                                                    |
| ----------- | ---------------------------------------------------------- |
| `vxstrlen`  | Returns the length of formatted output using `va_list`     |
| `xstrlen`   | Convenience variadic wrapper around `vxstrlen`             |
| `xstrcmp`   | Checks whether a string matches an entry in a string array |
| `xstrcmb`   | Concatenates two C strings into a newly allocated string   |
| `xbasename` | Returns the final component of a filesystem path           |

---

## 2. Detailed Function Behavior

### `vxstrlen()`

`vxstrlen()` determines the number of characters that formatted output would require without actually producing the output.

```c
size_t vxstrlen(const char *restrict fmt, va_list ap);
```

* **Windows:** uses `_vscprintf()`
* **POSIX:** uses `vsnprintf(NULL, 0, ...)`
* Does not allocate memory
* Does not write formatted output
* Returns the required character count excluding the terminating `'\0'`

---

### `xstrlen()`

`xstrlen()` is the convenient variadic interface to `vxstrlen()`.

```c
size_t xstrlen(const char *restrict fmt, ...);
```

Conceptually:

```c
va_list ap;

va_start(ap, fmt);
len = vxstrlen(fmt, ap);
va_end(ap);
```

Example:

```c
size_t len = xstrlen("Value: %d", 42);
```

The result represents the number of characters required for:

```text
Value: 42
```

excluding the terminating null character.

---

### `xstrcmp()`

`xstrcmp()` checks whether a string matches any element in an array of strings.

```c
int xstrcmp(
    const char *__s1,
    const char **__s2,
    size_t n
);
```

It returns:

* `1` — a matching element was found
* `0` — no matching element was found

#### Parameters

| Parameter | Type            | Description                  |
| --------- | --------------- | ---------------------------- |
| `__s1`    | `const char *`  | String to search for         |
| `__s2`    | `const char **` | Array of strings             |
| `n`       | `size_t`        | Number of elements in `__s2` |

`NULL` values are handled explicitly.

If both values being compared are `NULL`, they are considered equal.

Example:

```c
const char *arr[] = {
    "fff",
    "333",
    NULL
};

xstrcmp("fff", arr, 3);   // 1
xstrcmp("abc", arr, 3);   // 0
xstrcmp(NULL, arr, 3);    // 0
```

A `NULL` element can therefore be used as a valid searchable value:

```c
const char *arr[] = {
    NULL,
    "foo"
};

xstrcmp(NULL, arr, 2);    // 1
```

---

### `xstrcmb()`

`xstrcmb()` concatenates two strings and returns a newly allocated string.

```c
char *xstrcmb(const char *s1, const char *s2);
```

The returned buffer contains:

```text
s1 + s2 + '\0'
```

Example:

```c
char *result = xstrcmb("hello", "world");

if (result) {
    printf("%s\n", result);
    free(result);
}
```

Output:

```text
helloworld
```

The caller owns the returned allocation and must release it with `free()`.

`NULL` string arguments can be treated as empty/fallback strings according to the implementation.

---

### `xbasename()`

`xbasename()` extracts the final component of a filesystem path.

```c
const char *xbasename(const char *path);
```

Implementation:

```c
XSTDAPI const char *XCALLAPI xbasename(const char *path)
{
    const char *p1 = strrchr(path, '/');
    const char *p2 = strrchr(path, '\\');
    const char *p = p1 > p2 ? p1 : p2;

    return p ? p + 1 : path;
}
```

The function recognizes both common path separators:

```text
/
\
```

This makes it suitable for paths originating from either POSIX or Windows environments.

#### Examples

```c
xbasename("/home/user/file.txt");
```

returns:

```text
file.txt
```

For a Windows path:

```c
xbasename("C:\\Users\\MrR\\file.txt");
```

returns:

```text
file.txt
```

For a mixed-separator path:

```c
xbasename("C:\\Users/MrR\\project/main.c");
```

returns:

```text
main.c
```

If the path contains no separator:

```c
xbasename("main.c");
```

returns:

```text
main.c
```

#### Important ownership behavior

`xbasename()` does **not** allocate or copy the filename.

It returns a pointer into the original `path` string.

For example:

```c
const char *path = "/tmp/test.txt";
const char *name = xbasename(path);
```

`name` points directly into `path`.

Therefore, the returned pointer:

* Must not be passed to `free()`
* Is only valid while the original string remains valid
* Should be treated as read-only

#### Example

```c
#include <stdio.h>
#include "xstring.h"

int main(void)
{
    const char *path = "C:\\project\\src\\main.c";

    printf("File: %s\n", xbasename(path));

    return 0;
}
```

Output:

```text
File: main.c
```

---

## 3. Error Handling

| Function / Condition                             | Result                              |
| ------------------------------------------------ | ----------------------------------- |
| `vxstrlen()` formatting succeeds                 | Required character count            |
| `vxstrlen()` formatting fails                    | Implementation-defined error result |
| `xstrlen()` formatting succeeds                  | Required character count            |
| `xstrcmp()` finds a match                        | `1`                                 |
| `xstrcmp()` finds no match                       | `0`                                 |
| `xstrcmb()` allocation succeeds                  | Newly allocated string              |
| `xstrcmb()` allocation fails                     | `NULL`                              |
| `xbasename()` receives a path with separators    | Pointer to final component          |
| `xbasename()` receives a path without separators | Original `path` pointer             |

`xbasename()` itself performs no allocation and therefore has no allocation failure condition.

The caller should nevertheless provide a valid non-`NULL` `path` pointer unless the implementation explicitly documents `NULL` handling.

---

## 4. Portability Notes

### Formatted string length

Windows uses:

```c
_vscprintf()
```

POSIX-compatible implementations use:

```c
vsnprintf(NULL, 0, ...)
```

This avoids temporary buffers when determining formatted output size.

### Path handling

`xbasename()` intentionally does not depend on the host operating system's path separator.

It recognizes both:

```text
/
\
```

Therefore:

```c
xbasename("/usr/local/bin/program");
xbasename("C:\\Program Files\\program.exe");
```

can be processed through the same API.

This is particularly useful when the library processes paths generated on another operating system.

---

## 5. Example Usage

### Formatted length

```c
size_t len = xstrlen(
    "%s: %d",
    "count",
    42
);

char *buf = malloc(len + 1);

if (buf) {
    sprintf(buf, "%s: %d", "count", 42);
    printf("%s\n", buf);
    free(buf);
}
```

---

### String matching

```c
const char *formats[] = {
    "png",
    "jpg",
    "gif"
};

if (xstrcmp("png", formats, 3)) {
    printf("Supported format\n");
}
```

---

### String concatenation

```c
char *s = xstrcmb("hello", "world");

if (s) {
    printf("%s\n", s);
    free(s);
}
```

---

### Extracting a filename

```c
const char *path =
    "C:\\Users\\MrR\\Projects\\libxc\\src\\xstring.c";

printf(
    "Filename: %s\n",
    xbasename(path)
);
```

Output:

```text
Filename: xstring.c
```

---

### Using `xbasename()` with the logging system

`xbasename()` can also be used to keep log source locations concise:

```c
fprintf(
    stderr,
    "%s:%d\n",
    xbasename(__FILE__),
    __LINE__
);
```

For example:

```text
xstring.c:128
```

This is useful when full source paths would make diagnostic output unnecessarily long.

---

## 6. Memory Ownership

The functions have different ownership semantics:

| Function      | Allocates? | Caller must `free()`? |
| ------------- | ---------: | --------------------: |
| `vxstrlen()`  |         No |                    No |
| `xstrlen()`   |         No |                    No |
| `xstrcmp()`   |         No |                    No |
| `xstrcmb()`   |        Yes |                   Yes |
| `xbasename()` |         No |                    No |

In particular, `xbasename()` only returns a pointer into the supplied string.

---

## 7. Relationship to Standard C Functions

`xstring.h` provides higher-level or portable helpers around standard C functionality.

| XSTRING       | Related standard/platform functionality   |
| ------------- | ----------------------------------------- |
| `vxstrlen()`  | `_vscprintf()` / `vsnprintf()`            |
| `xstrlen()`   | `printf()`-style formatting               |
| `xstrcmp()`   | `strcmp()`                                |
| `xstrcmb()`   | `malloc()` + `memcpy()`/string operations |
| `xbasename()` | `strrchr()`                               |

`xbasename()` differs from platform-specific `basename()` implementations by explicitly recognizing **both** Windows and POSIX path separators.

---

## 8. License

This library is distributed under the **GNU General Public License v3 or later**.