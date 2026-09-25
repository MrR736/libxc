# xstat.h — Cross-Platform Directory Operations

**Author:** MrR736
**License:** GNU GPLv3+

## Overview

`xstat.h` provides cross-platform directory-management utilities with support for both narrow and wide-character paths.

The library provides:

* Change-current-directory operations
* Remove-directory operations
* Formatted path arguments
* Variadic and `va_list` interfaces
* Wide-character path support
* UTF-8/multibyte conversion on POSIX systems
* Windows wide-character API support

The formatted functions use the dynamic string-building functions provided by `xstdio.h`.

---

## Include

```c
#include "xstat.h"
```

---

# Functions

## `wchdir()`

```c
int wchdir(const wchar_t *path);
```

Changes the current working directory using a wide-character path.

On Windows, `wchdir()` uses the native `_wchdir()` function.

On POSIX systems, the wide-character path is converted using `xwcstombs()` before calling `chdir()`.

### Parameters

* `path` — Wide-character path of the directory to make current.

### Returns

Returns `0` on success.

Returns `-1` on failure.

If `path` is `NULL`, the function returns `-1`.

---

## `wrmdir()`

```c
int wrmdir(const wchar_t *path);
```

Removes an empty directory using a wide-character path.

On Windows, `wrmdir()` uses `_wrmdir()`.

On POSIX systems, the path is converted using `xwcstombs()` before calling `rmdir()`.

### Parameters

* `path` — Wide-character path of the directory to remove.

### Returns

Returns `0` on success.

Returns `-1` on failure.

If `path` is `NULL`, the function returns `-1`.

The directory must satisfy the requirements imposed by the underlying platform's `rmdir()` implementation.

---

## `vxchdir()`

```c
int vxchdir(const char *xrestrict path, va_list ap);
```

Formats a narrow-character path using a `va_list` and changes the current working directory.

The `path` argument is interpreted as a formatting string and processed using `vcprintf()`.

### Parameters

* `path` — Format string containing the directory path.
* `ap` — Variable argument list corresponding to `path`.

### Returns

Returns `0` on success.

Returns `-1` on failure.

If `path` is `NULL`, `errno` is set to `EINVAL`.

### Example

```c
va_list ap;

va_start(ap, name);
int ret = vxchdir("build/%s/%s", ap);
va_end(ap);
```

---

## `xchdir()`

```c
int xchdir(const char *xrestrict path, ...);
```

Formatted narrow-character version of `chdir()`.

The path is formatted using the additional arguments before the resulting path is passed to `chdir()`.

### Parameters

* `path` — Format string containing the directory path.
* `...` — Arguments used to format the path.

### Returns

Returns `0` on success.

Returns `-1` on failure.

If `path` is `NULL`, `errno` is set to `EINVAL`.

### Example

```c
if (xchdir("build/%s/%s", "project", "debug") != 0) {
    perror("xchdir");
}
```

---

## `vxwchdir()`

```c
int vxwchdir(const wchar_t *xrestrict path, va_list ap);
```

Formats a wide-character path using a `va_list` and changes the current working directory.

The path is formatted using `vwcprintf()` and then passed to `wchdir()`.

### Parameters

* `path` — Wide-character format string.
* `ap` — Variable argument list corresponding to `path`.

### Returns

Returns `0` on success.

Returns `-1` on failure.

If `path` is `NULL`, `errno` is set to `EINVAL`.

### Example

```c
va_list ap;

va_start(ap, name);
int ret = vxwchdir(L"build/%ls/%ls", ap);
va_end(ap);
```

---

## `xwchdir()`

```c
int xwchdir(const wchar_t *xrestrict path, ...);
```

Formatted wide-character version of `wchdir()`.

The path is formatted using the supplied arguments before changing the current working directory.

### Parameters

* `path` — Wide-character format string.
* `...` — Arguments used to format the path.

### Returns

Returns `0` on success.

Returns `-1` on failure.

If `path` is `NULL`, `errno` is set to `EINVAL`.

### Example

```c
if (xwchdir(L"build/%ls/%ls", L"project", L"debug") != 0) {
    perror("xwchdir");
}
```

---

## `vxrmdir()`

```c
int vxrmdir(const char *xrestrict path, va_list ap);
```

Formats a narrow-character directory path using a `va_list` and removes
the resulting directory.

The formatted path is generated using `vcprintf()` and passed to
`rmdir()`.

### Parameters

* `path` — Format string containing the directory path.
* `ap` — Variable argument list corresponding to `path`.

### Returns

Returns `0` on success.

Returns `-1` on failure.

If `path` is `NULL`, `errno` is set to `EINVAL`.

---

## `xrmdir()`

```c
int xrmdir(const char *xrestrict path, ...);
```

Formatted narrow-character version of `rmdir()`.

### Parameters

* `path` — Format string containing the directory path.
* `...` — Arguments used to format the path.

### Returns

Returns `0` on success.

Returns `-1` on failure.

If `path` is `NULL`, `errno` is set to `EINVAL`.

### Example

```c
if (xrmdir("build/%s/%s", "project", "debug") != 0) {
    perror("xrmdir");
}
```

---

## `vxwrmdir()`

```c
int vxwrmdir(const wchar_t *xrestrict path, va_list ap);
```

Formats a wide-character directory path using a `va_list` and removes
the resulting directory.

The formatted path is generated using `vwcprintf()` and passed to
`wrmdir()`.

### Parameters

* `path` — Wide-character format string.
* `ap` — Variable argument list corresponding to `path`.

### Returns

Returns `0` on success.

Returns `-1` on failure.

If `path` is `NULL`, `errno` is set to `EINVAL`.

---

## `xwrmdir()`

```c
int xwrmdir(const wchar_t *xrestrict path, ...);
```

Formatted wide-character version of `wrmdir()`.

### Parameters

* `path` — Wide-character format string.
* `...` — Arguments used to format the path.

### Returns

Returns `0` on success.

Returns `-1` on failure.

If `path` is `NULL`, `errno` is set to `EINVAL`.

### Example

```c
if (xwrmdir(L"build/%ls/%ls", L"project", L"debug") != 0) {
    perror("xwrmdir");
}
```

---

# Platform Behavior

| Function    | Windows     | POSIX                     |
| ----------- | ----------- | ------------------------- |
| `wchdir()`  | `_wchdir()` | `xwcstombs()` + `chdir()` |
| `wrmdir()`  | `_wrmdir()` | `xwcstombs()` + `rmdir()` |
| `xchdir()`  | `chdir()`   | `chdir()`                 |
| `xwchdir()` | `wchdir()`  | `wchdir()`                |
| `xrmdir()`  | `rmdir()`   | `rmdir()`                 |
| `xwrmdir()` | `wrmdir()`  | `wrmdir()`                |

The formatted functions ultimately use the corresponding narrow or wide
directory operation.

---

# Error Handling

The functions return `-1` when an operation fails.

For invalid `NULL` format/path arguments, the formatted functions set:

```c
errno = EINVAL;
```

Other errors originate from the underlying system functions, including
`chdir()` and `rmdir()`.

Typical errors include:

* `EINVAL` — Invalid argument.
* `ENOENT` — Directory does not exist.
* `ENOTDIR` — A component of the path is not a directory.
* `EACCES` — Permission denied.
* `ENOTEMPTY` — Directory is not empty.
* `ENOMEM` — Memory allocation failure during path formatting or conversion.

---

# Memory Management

The formatted functions allocate temporary path strings using the
dynamic formatting functions from `xstdio.h`.

The implementation automatically frees these temporary buffers before
returning.

The caller does not need to free the path generated internally by:

* `vxchdir()`
* `xchdir()`
* `vxwchdir()`
* `xwchdir()`
* `vxrmdir()`
* `xrmdir()`
* `vxwrmdir()`
* `xwrmdir()`

---

# Thread Safety

The functions themselves do not maintain global library state.

The current working directory is process-wide, so changing it affects
the process as a whole and may affect other threads executing in the
same process.

Applications using multiple threads should synchronize changes to the
current working directory when necessary.

---

# Cross-Platform Notes

On Windows, native wide-character APIs are used for:

* `wchdir()`
* `wrmdir()`

On POSIX systems, wide-character paths are converted using
`xwcstombs()` before being passed to the corresponding narrow-character
system calls.

The formatted functions use `vcprintf()` and `vwcprintf()` from
`xstdio.h` to construct the final path.

---

# See Also

* `chdir(2)` — Change the current working directory
* `rmdir(2)` — Remove an empty directory
* `_wchdir()` — Windows wide-character directory change
* `_wrmdir()` — Windows wide-character directory removal
* `xstdio.h` — Extended stdio and dynamic string utilities
* `xstdlib.h` — Extended system and conversion utilities

# License

This library is distributed under the **GNU General Public License,
version 3 or later**.
