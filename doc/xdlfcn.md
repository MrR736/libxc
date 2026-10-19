# xdlfcn.h — Cross-Platform Dynamic Library Utilities

**Author:** MrR736
**License:** GNU GPLv3+

## Overview

`xdlfcn.h` provides a cross-platform interface for dynamic library loading, symbol lookup, error reporting, library unloading, and address-to-module information.

The API provides a common interface for POSIX `dlopen()`, `dlsym()`, and `dlclose()` functionality and corresponding Windows loader APIs.

### Features

* Dynamic library loading
* Wide-character library loading
* Dynamic symbol lookup
* Wide-character symbol lookup
* Dynamic library unloading
* Cross-platform loader flags
* Thread-local loader error messages
* POSIX `RTLD_*` compatibility
* Windows `LoadLibrary*()` and `GetProcAddress()` compatibility wrappers
* Narrow- and wide-character library-path construction
* Address-to-module and best-effort symbol information through `xdladdr()`

## Include

```c
#include "xdlfcn.h"
```

## Loader Flags

| Flag                 | Description                                                             |
| -------------------- | ----------------------------------------------------------------------- |
| `XRTLD_LAZY`         | Resolve symbols lazily where supported.                                 |
| `XRTLD_NOW`          | Resolve required symbols when loading.                                  |
| `XRTLD_BINDING_MASK` | Mask for dynamic-loader binding flags.                                  |
| `XRTLD_NOLOAD`       | Obtain a handle to an already loaded library without loading a new one. |
| `XRTLD_DEEPBIND`     | Request deep symbol binding where supported.                            |
| `XRTLD_NODELETE`     | Request that a library remain mapped after unloading where supported.   |
| `XRTLD_LOCAL`        | Keep library symbols local where supported.                             |
| `XRTLD_GLOBAL`       | Make library symbols globally available where supported.                |
| `XRTLD_FIRST`        | Request first-level symbol lookup where supported.                      |
| `XRTLD_NEXT`         | Special lookup handle corresponding to `RTLD_NEXT` where supported.     |
| `XRTLD_DEFAULT`      | Special lookup handle corresponding to `RTLD_DEFAULT` where supported.  |

Apple platforms may additionally support `XRTLD_SELF` and `XRTLD_MAIN_ONLY`.

Windows does not provide direct equivalents for every POSIX loader flag.

---

# Functions

## `xdlopen()`

```c
xhandle_t xdlopen(const char *file, int mode);
```

Loads a dynamic library using a narrow-character filename.

* Windows: uses `LoadLibraryA()`.
* Windows with `XRTLD_NOLOAD`: uses `GetModuleHandleA()`.
* POSIX: uses `dlopen()`.

**Parameters**

* `file` — Dynamic library filename.
* `mode` — Combination of `XRTLD_*` flags.

**Returns:** An `xhandle_t` handle on success, or `NULL` on failure.

## `xwdlopen()`

```c
xhandle_t xwdlopen(const wchar_t *file, int mode);
```

Wide-character equivalent of `xdlopen()`.

* Windows: uses `LoadLibraryW()`.
* POSIX: converts the filename using `xwcstombs()` and calls `dlopen()`.

**Returns:** An `xhandle_t` handle on success, or `NULL` on failure.

## `xdlsym()`

```c
FARPROC xdlsym(xhandle_t handle, const char *name);
```

Looks up a symbol in a loaded library.

* Windows: uses `GetProcAddress()`.
* POSIX: uses `dlsym()`.

**Parameters**

* `handle` — Library handle.
* `name` — Null-terminated symbol name.

**Returns:** The symbol address on success, or `NULL` if lookup fails.

## `xwdlsym()`

```c
FARPROC xwdlsym(xhandle_t handle, const wchar_t *name);
```

Wide-character equivalent of `xdlsym()`. The symbol name is converted to a multibyte representation before lookup.

**Returns:** The symbol address on success, or `NULL` on failure.

## `xdlerror()`

```c
char *xdlerror(void);
```

Returns a description of the most recent dynamic-loader error.

* Windows: retrieves the error using `GetLastError()` and formats it with `FormatMessageA()`.
* POSIX: returns the result of `dlerror()`.

**Returns:** An error-message pointer, or `NULL` when no error is available.

On Windows, the returned pointer refers to a thread-local buffer. Do not free it. A later call on the same thread may overwrite its contents.

## `xwdlerror()`

```c
wchar_t *xwdlerror(void);
```

Wide-character equivalent of `xdlerror()`.

* Windows: formats the error using `FormatMessageW()`.
* POSIX: converts the `dlerror()` message using `xmbstowcs()`.

The ownership and lifetime of the returned pointer depend on the platform implementation and conversion helper.

## `xdlclose()`

```c
int xdlclose(xhandle_t handle);
```

Unloads a previously loaded library.

* Windows: uses `FreeLibrary()`.
* POSIX: uses `dlclose()`.

**Returns:** `0` on success, `-1` on failure.

---

# Windows-Compatible Wrappers

On POSIX platforms, the header provides Windows-style API wrappers.

```c
HMODULE GetModuleHandleA(LPCSTR lpModuleName);
HMODULE GetModuleHandleW(LPCWSTR lpModuleName);

HMODULE LoadLibraryA(LPCSTR lpLibFileName);
HMODULE LoadLibraryW(LPCWSTR lpLibFileName);

FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName);

WINBOOL FreeLibrary(HMODULE hLibModule);
```

The described wrappers use the corresponding `xdl*()` functions.

| Function             | Wrapper implementation                 |
| -------------------- | -------------------------------------- |
| `GetModuleHandleA()` | `xdlopen(lpModuleName, XRTLD_NOLOAD)`  |
| `GetModuleHandleW()` | `xwdlopen(lpModuleName, XRTLD_NOLOAD)` |
| `LoadLibraryA()`     | `xdlopen(lpLibFileName, XRTLD_LAZY)`   |
| `LoadLibraryW()`     | `xwdlopen(lpLibFileName, XRTLD_LAZY)`  |
| `GetProcAddress()`   | `GetProcAddressA(hModule, lpProcName)` |

Check the implementation for platform-specific reference-count behavior, especially when emulating `GetModuleHandle*()`.

---

# Library-Path Utilities

These functions construct a platform-specific library filename from an optional directory and a library name.

## `xdllibpath()`

```c
char *xdllibpath(const char *dir, const char *name);
```

Constructs a narrow-character library path and returns a newly allocated string.

## `xwdllibpath()`

```c
wchar_t *xwdllibpath(const wchar_t *dir, const wchar_t *name);
```

Wide-character equivalent of `xdllibpath()`.

## `xcdllibpath()`

```c
const char *xcdllibpath(const char *dir, const char *name);
```

Returns a borrowed narrow-character path. In the provided C++ implementation, the result is stored in a thread-local `std::string`.

## `xcwdllibpath()`

```c
const wchar_t *xcwdllibpath(const wchar_t *dir, const wchar_t *name);
```

Wide-character equivalent of `xcdllibpath()`, using a thread-local `std::wstring` in the provided C++ implementation.

### Platform naming

| Platform    | Prefix | Suffix   |
| ----------- | ------ | -------- |
| Windows     | None   | `.dll`   |
| macOS       | `lib`  | `.dylib` |
| Other POSIX | `lib`  | `.so`    |

The functions do not remove an existing `lib` prefix from `name`.

For the allocating variants, a directory separator is inserted if `dir` is nonempty and does not already end in a recognized separator. Windows recognizes both `/` and `\`; POSIX uses `/`.

### Parameters

* `dir` — Optional directory path. May be `NULL` or empty.
* `name` — Library name. Must not be `NULL` or empty.

If `name` is `NULL` or empty, the functions set `errno` to `EINVAL` and return `NULL`.

### Allocating variants

`xdllibpath()` and `xwdllibpath()` return newly allocated strings. Release them using the deallocator matching `xmalloc()`—`xfree()` if that is the matching function in your project.

```c
char *path = xdllibpath("/usr/local/lib", "xc");

if (path != NULL) {
    /* Use path. */
    xfree(path);
}
```

On Linux, this produces:

```text
/usr/local/lib/libxc.so
```

On macOS:

```text
/usr/local/lib/libxc.dylib
```

On Windows:

```text
/usr/local/lib/xc.dll
```

### Thread-local variants

The provided C++ implementations of `xcdllibpath()` and `xcwdllibpath()` store their results in thread-local strings.

* Do not free the returned pointer.
* The pointer may be invalidated or its contents overwritten by a later call on the same thread.
* Copy the result if it must be retained across subsequent calls.

The provided C implementations return `NULL` for these `xx*` functions.

### Examples

```c
char *path = xdllibpath(NULL, "xc");

if (path != NULL) {
    /* For example: "libxc.so" on Linux. */
    xfree(path);
}
```

```c
wchar_t *wide_path = xwdllibpath(L"/usr/local/lib", L"xc");

if (wide_path != NULL) {
    /* Use wide_path. */
    xfree(wide_path);
}
```

```cpp
const char *borrowed_path = xcdllibpath("/usr/local/lib", "xc");

/* Use borrowed_path before another same-thread call overwrites it. */
/* Do not free borrowed_path. */
```

---

# `xdladdr()`

```c
int xdladdr(const void *addr, XDl_info *info);
```

Obtains information about the dynamic module containing a specified address. It provides a cross-platform interface corresponding to POSIX `dladdr()`.

## `XDl_info`

```c
typedef struct XDl_info {
    const char *dli_fname;
    void       *dli_fbase;
    const char *dli_sname;
    void       *dli_saddr;
} XDl_info;
```

### Parameters

* `addr` — Address whose containing module and symbol information should be determined.
* `info` — Pointer to an `XDl_info` structure that receives the result.

### Returns

Returns nonzero on success and `0` on failure.

If `addr` or `info` is `NULL`, the described implementation reports `EINVAL` on POSIX or `ERROR_INVALID_PARAMETER` on Windows.

### Structure fields

* **`dli_fname`** — Filename of the module containing `addr`.
* **`dli_fbase`** — Base address of the containing module.
* **`dli_sname`** — Name of the symbol containing or preceding `addr`, when available.
* **`dli_saddr`** — Address of the resolved symbol.

On Windows, the module filename is obtained using `GetModuleFileNameA()`. On POSIX systems, module and symbol information is supplied by native `dladdr()`.

The described MinGW/Windows implementation can resolve symbols from PE/COFF symbol tables when available. It searches executable sections for the nearest symbol at or below the requested address. Symbol information is not guaranteed; a stripped executable may provide module information without symbol information.

When a symbol cannot be resolved:

```c
info->dli_sname = NULL;
info->dli_saddr = NULL;
```

### Example

```c
#include <stdio.h>
#include <string.h>
#include "xdlfcn.h"

static void test_function(void)
{
}

int main(void)
{
    XDl_info info;
    memset(&info, 0, sizeof(info));

    if (!xdladdr((const void *)&test_function, &info)) {
        fprintf(stderr, "xdladdr() failed\n");
        return 1;
    }

    printf("Filename : %s\n",
           info.dli_fname ? info.dli_fname : "(null)");
    printf("Base     : %p\n", info.dli_fbase);
    printf("Symbol   : %s\n",
           info.dli_sname ? info.dli_sname : "(null)");
    printf("Address  : %p\n", info.dli_saddr);

    return 0;
}
```

A MinGW build may produce output similar to:

```text
Filename : Z:\path\to\test_xdlfcn.exe
Base     : 00400000
Symbol   : _test_function
Address  : 00402306
```

Exact addresses and symbol names depend on the compiler, linker, build configuration, and executable layout.

### Thread safety and lifetime

The described Windows implementation stores `dli_fname` and `dli_sname` in thread-local buffers. A subsequent `xdladdr()` call on the same thread may overwrite these strings. Copy them if they must be retained.

POSIX string lifetimes follow the underlying `dladdr()` implementation.

### Platform behavior

| Field          | Windows                               | POSIX             |
| -------------- | ------------------------------------- | ----------------- |
| `dli_fname`    | `GetModuleFileNameA()`                | Native `dladdr()` |
| `dli_fbase`    | Windows loader APIs                   | Native `dladdr()` |
| `dli_sname`    | PE/COFF symbols when available        | Native `dladdr()` |
| `dli_saddr`    | PE/COFF symbol address when available | Native `dladdr()` |
| Invalid `addr` | `ERROR_INVALID_PARAMETER`             | `EINVAL`          |
| Invalid `info` | `ERROR_INVALID_PARAMETER`             | `EINVAL`          |

Treat `xdladdr()` as a best-effort address-to-module and address-to-symbol query. Module information may be available even when symbol information is not.

---

# Example: Loading a Library

```c
#include <stdio.h>
#include "xdlfcn.h"

int main(void)
{
    xhandle_t handle = xdlopen("libexample.so", XRTLD_NOW);

    if (handle == NULL) {
        char *error = xdlerror();
        if (error != NULL)
            fprintf(stderr, "xdlopen: %s\n", error);
        return 1;
    }

    FARPROC symbol = xdlsym(handle, "example_function");

    if (symbol == NULL) {
        char *error = xdlerror();
        if (error != NULL)
            fprintf(stderr, "xdlsym: %s\n", error);

        xdlclose(handle);
        return 1;
    }

    /*
     * Convert/use the symbol according to its known function type.
     * The conversion and invocation are platform/compiler dependent.
     */

    xdlclose(handle);
    return 0;
}
```

Use a library filename and loader flags appropriate for the target platform. On Windows, the library name would normally end in `.dll`.

---

# Error Handling

## Windows

Windows loader errors are obtained using:

```c
GetLastError();
```

`xdlerror()` and `xwdlerror()` convert error codes into human-readable messages. Check each function's ownership and lifetime rules before retaining or releasing its returned pointer.

## POSIX

POSIX loader errors are reported through:

```c
dlerror();
```

`xdlerror()` returns the native `dlerror()` result. Check the returned pointer before using it.

---

# Thread Safety

Thread-local buffers prevent different threads from sharing the same buffer, but they do not make returned pointers permanent.

* `xdlerror()` and `xwdlerror()` use thread-local buffers in the described Windows implementation.
* `xcdllibpath()` and `xcwdllibpath()` use thread-local strings in the provided C++ implementation.
* `xdladdr()` uses thread-local buffers for the relevant strings in the described Windows implementation.

A subsequent call on the same thread may overwrite or invalidate a previously returned pointer.

Do not unload a dynamic library while another thread may still execute code from it or use pointers to its symbols.

---

# Cross-Platform Notes

| Feature                | Windows              | POSIX                                       |
| ---------------------- | -------------------- | ------------------------------------------- |
| Narrow library loading | `LoadLibraryA()`     | `dlopen()`                                  |
| Wide library loading   | `LoadLibraryW()`     | Convert with `xwcstombs()`, then `dlopen()` |
| Symbol lookup          | `GetProcAddress()`   | `dlsym()`                                   |
| Library unloading      | `FreeLibrary()`      | `dlclose()`                                 |
| Loader errors          | `GetLastError()`     | `dlerror()`                                 |
| Lazy/now distinction   | No direct equivalent | `RTLD_LAZY` / `RTLD_NOW`                    |
| `XRTLD_NOLOAD`         | `GetModuleHandle*()` | `RTLD_NOLOAD` where available               |
| Deep binding           | No direct equivalent | `RTLD_DEEPBIND` where available             |

---

# Memory Management

Follow the ownership contract of each API:

* **`xdllibpath()` and `xwdllibpath()`** return allocated strings. Release them with the matching deallocator for `xmalloc()`—`xfree()` if that is the matching function.
* **`xcdllibpath()` and `xcwdllibpath()` in the provided C++ implementation** return borrowed pointers to thread-local strings. Do not free them; copy them if they must outlive subsequent calls on the same thread.
* **`xdlerror()` and `xwdlerror()`** may return thread-local buffers on Windows. Do not free those buffers.
* **Wide-character conversion helpers** may allocate memory. Follow the specific conversion function's documented ownership contract.
* **Library handles** returned by successful loading calls should be released using the matching unloading API, subject to the API's documented semantics.

Never call `free()` or `xfree()` on a pointer unless its ownership contract says that the caller owns the allocation and that deallocator is the correct match.

---

# See Also

* `dlopen(3)` — POSIX dynamic library loading
* `dlsym(3)` — POSIX symbol lookup
* `dlclose(3)` — POSIX dynamic library unloading
* `dlerror(3)` — POSIX dynamic-loader errors
* `LoadLibraryA()` — Windows dynamic library loading
* `LoadLibraryW()` — Windows wide-character library loading
* `GetProcAddress()` — Windows symbol lookup
* `FreeLibrary()` — Windows dynamic library unloading

---

# License

This library is distributed under the **GNU General Public License, version 3 or later**.
