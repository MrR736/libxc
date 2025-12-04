# **XLOCALE — Extended Locale Utilities**

*Platform-portable locale initialization helpers (UTF-8 aware)*
**Author:** MrR736
**License:** GPL-3.0+

---

## Overview

`xlocale.h` provides a small, platform-portable abstraction layer for:

* Setting process-wide locales (`setlocale`)
* Setting wide locales (`wsetlocale`)
* Creating and freeing locale objects (`newlocale`, `_create_locale`)
* Automatically selecting UTF-8 fallback locales
* Console UTF-8 initialization on Windows
* Handling both POSIX and Windows locale models

It provides identical API names across all platforms while internally mapping to:

* POSIX: `setlocale()`, `newlocale()`, `freelocale()`
* Windows: `_wsetlocale()`, `_create_locale()`, `_free_locale()`, `_setmode()`, `SetConsoleCP()`, etc.

---

# 1. Types and Constants

### `xlocale_t`

Platform-neutral locale object:

| Platform | Type        |
| -------- | ----------- |
| POSIX    | `locale_t`  |
| Windows  | `_locale_t` |

---

### Category Aliases

The following re-map standard locale categories:

```
XLC_ALL
XLC_COLLATE
XLC_CTYPE
XLC_MONETARY
XLC_NUMERIC
XLC_TIME
```

On POSIX systems additional categories are exposed:

```
XLC_MESSAGES
XLC_PAPER
XLC_NAME
XLC_ADDRESS
XLC_TELEPHONE
XLC_MEASUREMENT
XLC_IDENTIFICATION
```

Mask macros (for `newlocale`) map directly to `LC_*_MASK`.

---

# 2. Locale Setting Helpers

## `char* xsetlocale(int category, const char *req_locale)`

Attempts to install the requested locale. If unavailable:

1. Tries UTF-8 aliases (e.g. `C.UTF-8`, `en_US.UTF-8`, `.UTF8`, `.65001`)
2. Tries system default (`""`)
3. Falls back to ANSI `"C"`

Returns pointer to locale name, or NULL only if all attempts fail.

---

## `wchar_t* wsetlocale(int category, const wchar_t *req_locale)`

A wide-character equivalent of `setlocale`.

On POSIX, it:

* Converts wide input using `xwcstombs()`
* Attempts UTF-8 fallback aliases
* Returns wide string via `xmbstowcs()`

On Windows: wraps `_wsetlocale()` directly.

---

## `wchar_t* xwsetlocale(int category, const wchar_t *req_locale)`

Pure wide-only locale setter (no multibyte conversions).
Attempts requested locale → UTF-8 aliases → system default → `"C"`.

---

# 3. Locale Object Management

## `xlocale_t xnewlocale(int category_mask, const char *req_locale, xlocale_t base)`

Creates a new locale object:

### On Windows:

Uses `_create_locale()`
Attempts: requested → UTF-8 aliases → `"C"`

### On POSIX:

Uses `newlocale()`
Attempts: requested → UTF-8 aliases → system default → `"C"`

---

## `void xfreelocale(xlocale_t loc)`

Frees the locale object:

* Windows: `_free_locale()`
* POSIX: `freelocale()`

---

# 4. Console UTF-8 Initialization (Windows only)

## `void xinit_console_utf8(void)`

Configures Windows console for UTF-8 using:

* `SetConsoleOutputCP(CP_UTF8)`
* `SetConsoleCP(CP_UTF8)`
* `_setmode(stdout, _O_U8TEXT)`

Then sets process locale to UTF-8.

---

## `void xinit_console_wutf(void)`

Wide-UTF console initializer. Sets:

* UTF-8 console codepages
* `_O_U16TEXT` mode for stderr/stdout
* Locale = UTF-8

On POSIX, only `xsetlocale(XLC_ALL,"C.UTF-8")` is used.

---

# 5. UTF-8 Alias List

Both APIs try the following fallback list:

```
"C.UTF-8"
"en_US.UTF-8"
"UTF-8"
".UTF8"
".utf8"
"utf-8"
".65001"   // Windows codepage UTF-8
```

---

# 6. Memory Ownership

Returned wide strings from `xmbstowcs()` or `xwcstombs()` must be freed by caller if those functions dynamically allocate memory (implementation-dependent).

`setlocale()` returns static strings: **do not free**.

---

# 7. Thread Safety

The `setlocale()` API is **not** thread-safe on many systems.

`xlocale` does not introduce extra locking; callers must serialize locale changes.

---

# 8. Example Usage

```c
#include "xlocale.h"

int main() {
    xsetlocale(XLC_ALL, "C.UTF-8");

    xlocale_t loc = xnewlocale(XLC_ALL_MASK, "C.UTF-8", NULL);
    if (loc) {
        // Use locale-dependent functions...
        xfreelocale(loc);
    }

    return 0;
}
```

Wide example:

```c
wchar_t *ret = wsetlocale(XLC_ALL, L"en_US.UTF-8");
if (ret)
    wprintf(L"Locale set to: %ls\n", ret);
```

Windows console example:

```c
xinit_console_utf8();
printf("UTF-8 enabled console.\n");
```

---

# 9. License

Released under the **GNU General Public License v3 or later**.
