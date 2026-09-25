# xdlfcn.h — Cross-Platform Dynamic Library Utilities

**Author:** MrR736
**License:** GNU GPLv3+

## Overview

`xdlfcn.h` provides a cross-platform interface for dynamic library loading, symbol lookup, error reporting, and library unloading.

The API provides a common interface for POSIX `dlopen()`/`dlsym()`/`dlclose()` functionality and the corresponding Windows loader APIs.

Features include:

* Dynamic library loading
* Wide-character library loading
* Dynamic symbol lookup
* Wide-character symbol lookup
* Dynamic library unloading
* Cross-platform loader flags
* Thread-local loader error messages
* POSIX `RTLD_*` compatibility
* Windows `LoadLibrary*()` and `GetProcAddress()` compatibility wrappers

All functions use the `HMODULE` and `FARPROC` types to provide a common interface across Windows and POSIX systems.

---

## Include

```c
#include "xdlfcn.h"
```

---

## Loader Flags

`xdlfcn.h` defines portable `XRTLD_*` flags corresponding to the platform's dynamic-loader flags.

### `XRTLD_LAZY`

Resolve symbols lazily when they are first referenced.

On Windows, this flag has no distinct equivalent because the Windows PE loader determines symbol resolution according to normal loader behavior.

### `XRTLD_NOW`

Resolve required symbols when the library is loaded.

On Windows, this flag has no distinct equivalent.

### `XRTLD_BINDING_MASK`

Mask for dynamic-loader binding flags.

### `XRTLD_NOLOAD`

Do not load a new library. Instead, obtain a handle to an already loaded library.

On Windows, this is implemented using `GetModuleHandleA()` or `GetModuleHandleW()`.

### `XRTLD_DEEPBIND`

Request deep symbol binding where supported.

This flag has no direct Windows equivalent.

### `XRTLD_NODELETE`

Request that a loaded library remain mapped after unloading where supported.

### `XRTLD_LOCAL`

Symbols from the loaded library are not made globally available.

### `XRTLD_GLOBAL`

Symbols from the loaded library are made globally available where supported.

### `XRTLD_FIRST`

Request first-level symbol lookup where supported.

### `XRTLD_NEXT`

Special symbol lookup handle corresponding to `RTLD_NEXT` on supported POSIX systems.

### `XRTLD_DEFAULT`

Special symbol lookup handle corresponding to `RTLD_DEFAULT` where supported.

On Apple platforms, the implementation uses the platform-specific value when the system headers do not define it.

### Apple-Specific Flags

On Apple platforms, the header also provides:

* `XRTLD_FIRST`
* `XRTLD_SELF`
* `XRTLD_MAIN_ONLY`

These correspond to the respective Darwin dynamic-loader controls where available.

---

# Functions

## `xdlopen()`

```c
HMODULE xdlopen(const char *file, int mode);
```

Loads a dynamic library using a narrow-character filename.

On Windows, `xdlopen()` uses `LoadLibraryA()`.

If `XRTLD_NOLOAD` is specified, the function uses `GetModuleHandleA()` instead of loading a new library.

On POSIX systems, the function directly calls `dlopen()`.

### Parameters

* `file` — Dynamic library filename.
* `mode` — Combination of `XRTLD_*` flags.

### Returns

Returns an `HMODULE` handle on success.

Returns `NULL` on failure.

---

## `xwdlopen()`

```c
HMODULE xwdlopen(const wchar_t *file, int mode);
```

Wide-character equivalent of `xdlopen()`.

On Windows, `xwdlopen()` uses `LoadLibraryW()`.

If `XRTLD_NOLOAD` is specified, it uses `GetModuleHandleW()`.

On POSIX systems, the wide-character filename is converted using `xwcstombs()` and passed to `dlopen()`.

### Parameters

* `file` — Wide-character dynamic library filename.
* `mode` — Combination of `XRTLD_*` flags.

### Returns

Returns an `HMODULE` handle on success.

Returns `NULL` on failure.

---

## `xdlsym()`

```c
FARPROC xdlsym(HMODULE handle, const char *name);
```

Looks up a symbol in a loaded dynamic library.

On Windows, this function uses `GetProcAddress()`.

On POSIX systems, it uses `dlsym()`.

### Parameters

* `handle` — Dynamic library handle.
* `name` — Null-terminated symbol name.

### Returns

Returns the address of the requested symbol on success.

Returns `NULL` if the symbol cannot be found.

---

## `xwdlsym()`

```c
FARPROC xwdlsym(HMODULE handle, const wchar_t *name);
```

Wide-character equivalent of `xdlsym()`.

On Windows, the symbol name is converted to a multibyte representation before calling `GetProcAddress()`.

On POSIX systems, the symbol name is converted before calling `dlsym()`.

### Parameters

* `handle` — Dynamic library handle.
* `name` — Wide-character symbol name.

### Returns

Returns the address of the requested symbol on success.

Returns `NULL` on failure.

---

## `xdlerror()`

```c
char *xdlerror(void);
```

Returns a description of the most recent dynamic-loader error.

On Windows, the function retrieves the current error using `GetLastError()` and formats it using `FormatMessageA()`.

The returned Windows error buffer is thread-local.

On POSIX systems, the function directly returns the result of `dlerror()`.

### Returns

Returns a pointer to an error message when an error is available.

Returns `NULL` when no error is available.

On Windows, calling `xdlerror()` consumes the current `GetLastError()` value by resetting it to `ERROR_SUCCESS`.

---

## `xwdlerror()`

```c
wchar_t *xwdlerror(void);
```

Wide-character equivalent of `xdlerror()`.

On Windows, the error message is generated using `FormatMessageW()` and stored in a thread-local buffer.

On POSIX systems, the result of `dlerror()` is converted to a wide-character string using `xmbstowcs()`.

### Returns

Returns a pointer to a wide-character error message.

Returns `NULL` when no dynamic-loader error is available.

---

## `xdlclose()`

```c
int xdlclose(HMODULE handle);
```

Unloads a previously loaded dynamic library.

On Windows, this function uses `FreeLibrary()`.

On POSIX systems, it uses `dlclose()`.

### Parameters

* `handle` — Dynamic library handle returned by `xdlopen()` or `xwdlopen()`.

### Returns

Returns `0` on success.

Returns `-1` on failure.

---

# Windows-Compatible Wrappers

The header provides Windows-compatible API names on POSIX systems.

## `GetProcAddressA()`

```c
FARPROC GetProcAddressA(HMODULE hModule, LPCSTR lpProcName);
```

Looks up a symbol using a narrow-character name.

On Windows, this calls the native `GetProcAddress()` API.

On POSIX systems, it is implemented using `xdlsym()`.

---

## `GetProcAddressW()`

```c
FARPROC GetProcAddressW(HMODULE hModule, LPCWSTR lpProcName);
```

Wide-character version of `GetProcAddressA()`.

On Windows, the wide-character symbol name is converted before calling the native `GetProcAddress()` API.

On POSIX systems, it is implemented using `xwdlsym()`.

---

## POSIX Compatibility Wrappers

When compiling on non-Windows platforms, `xdlfcn.h` provides the following Windows-style APIs:

```c
HMODULE GetModuleHandleA(const char *lpModuleName);
HMODULE GetModuleHandleW(const wchar_t *lpModuleName);

HMODULE LoadLibraryA(const char *lpLibFileName);
HMODULE LoadLibraryW(const wchar_t *lpLibFileName);

FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName);
```

These functions are implemented using the corresponding `xdl*()` functions.

### `GetModuleHandleA()`

Implemented using:

```c
xdlopen(lpModuleName, XRTLD_NOLOAD);
```

### `GetModuleHandleW()`

Implemented using:

```c
xwdlopen(lpModuleName, XRTLD_NOLOAD);
```

### `LoadLibraryA()`

Implemented using:

```c
xdlopen(lpLibFileName, XRTLD_LAZY);
```

### `LoadLibraryW()`

Implemented using:

```c
xwdlopen(lpLibFileName, XRTLD_LAZY);
```

### `GetProcAddress()`

Implemented using:

```c
GetProcAddressA(hModule, lpProcName);
```

---

# Example

```c
#include <stdio.h>
#include "xdlfcn.h"

int main(void) {
    HMODULE handle = xdlopen("libexample.so", XRTLD_NOW);

    if (!handle) {
        char *error = xdlerror();
        if (error)
            fprintf(stderr, "xdlopen: %s\n", error);
        return 1;
    }

    FARPROC symbol = xdlsym(handle, "example_function");
    
    if (!symbol) {
        char *error = xdlerror();
        if (error)
            fprintf(stderr, "xdlsym: %s\n", error);
        xdlclose(handle);
        return 1;
    }

    /*
     * Convert/use symbol according to its known function type.
     */

    xdlclose(handle);
    return 0;
}
```

---

## `xdllibpath()`

```c
char *xdllibpath(const char *dir, const char *name);
```

Constructs a platform-specific dynamic-library filename from a directory and library name.

`xdllibpath()` adds the appropriate platform-specific library prefix and suffix:

| Platform    | Prefix | Suffix   |
| ----------- | ------ | -------- |
| Windows     | none   | `.dll`   |
| macOS       | `lib`  | `.dylib` |
| Other POSIX | `lib`  | `.so`    |

The function does not automatically remove an existing `lib` prefix from `name`.

### Parameters

* `dir` — Optional directory path. May be `NULL`.
* `name` — Library name. Must not be `NULL` or empty.

### Returns

Returns a dynamically allocated, null-terminated library path on success.

Returns `NULL` on failure.

The returned string must be released with `free()`.

### Examples

```c
char *path;

path = xdllibpath("/usr/local/lib", "xc");

if (path != NULL) {
    printf("%s\n", path);
    free(path);
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

On Windows, both `/` and `\` are recognized as directory separators. On POSIX systems, `/` is used as the directory separator.

### Memory Management

The returned path is allocated using `malloc()` and must be released by the caller:

```c
char *path = xdllibpath(NULL, "xc");

if (path != NULL) {
    /* use path */
    free(path);
}
```

If `name` is `NULL` or empty, the function sets `errno` to `EINVAL` and returns `NULL`.

---

## `xdladdr()`

```c
int xdladdr(const void *addr, XDl_info *info);
```

Obtains information about the dynamic module containing a specified address.

`xdladdr()` provides a cross-platform interface corresponding to the POSIX `dladdr()` function.

### Parameters

* `addr` — Address whose containing module and symbol information should be determined.
* `info` — Pointer to an `XDl_info` structure that receives the result.

The `XDl_info` structure is:

```c
typedef struct XDl_info {
    const char *dli_fname;
    void       *dli_fbase;
    const char *dli_sname;
    void       *dli_saddr;
} XDl_info;
```

### Returns

Returns non-zero on success.

Returns `0` on failure.

If `addr` or `info` is `NULL`, the function fails and reports `EINVAL` on POSIX systems or `ERROR_INVALID_PARAMETER` on Windows.

### `dli_fname`

`dli_fname` points to the filename of the module containing `addr`.

On Windows, the filename is obtained using `GetModuleFileNameA()`.

On POSIX systems, the value is provided by the native `dladdr()` implementation.

### `dli_fbase`

`dli_fbase` contains the base address at which the containing module is loaded.

On Windows, the module is identified using:

```c
GetModuleHandleExA(
    GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
    GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
    (LPCSTR)addr,
    &module
);
```

The returned module handle corresponds to the module base address.

The `GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT` flag means that the reference count of the module is not modified. The returned handle must therefore not be released using `FreeLibrary()`.

### `dli_sname`

`dli_sname` points to the name of the symbol containing or preceding `addr`.

On POSIX systems, this value is supplied by the native `dladdr()` implementation.

On MinGW/Windows builds, `xdladdr()` can resolve symbols from the PE/COFF symbol table contained in the executable or DLL. The symbol lookup searches executable sections and selects the nearest symbol whose address is less than or equal to `addr`.

For example, if the PE/COFF symbol table contains:

```text
00402306 T _test_xdladdr
```

and `addr` refers to an address within that function, `dli_sname` can contain:

```text
_test_xdladdr
```

The symbol name is stored in thread-local storage and remains valid until the calling thread makes another `xdladdr()` call that replaces the buffer contents.

### `dli_saddr`

`dli_saddr` contains the address of the symbol identified by `dli_sname`.

For example:

```text
Symbol  : _test_xdladdr
Address : 00402306
```

If no symbol can be resolved, both fields are set to `NULL`:

```c
info->dli_sname = NULL;
info->dli_saddr = NULL;
```

Symbol information is therefore optional and should always be checked before use.

### Windows / MinGW Symbol Resolution

On MinGW builds, `xdladdr()` does not depend on DbgHelp symbol resolution.

Instead, the implementation:

1. Uses `GetModuleHandleExA()` to identify the module containing `addr`.
2. Uses `GetModuleFileNameA()` to obtain the module filename.
3. Opens the PE image.
4. Maps the PE image into memory.
5. Validates the DOS and PE headers.
6. Locates the PE/COFF symbol table.
7. Locates the COFF string table.
8. Converts `addr` to an RVA relative to the loaded module.
9. Searches executable sections for the nearest symbol at or below that RVA.
10. Copies the symbol name into thread-local storage.
11. Stores the symbol address in `dli_saddr`.

This allows MinGW-generated PE executables containing COFF symbols to provide useful symbol information without requiring a PDB file or Windows-specific debug-symbol format.

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
           info.dli_fname
               ? info.dli_fname
               : "(null)");

    printf("Base     : %p\n",
           info.dli_fbase);

    printf("Symbol   : %s\n",
           info.dli_sname
               ? info.dli_sname
               : "(null)");

    printf("Address  : %p\n",
           info.dli_saddr);

    return 0;
}
```

### Example Result

A MinGW build may produce output similar to:

```text
Filename : Z:\path\to\test_xdlfcn.exe
Base     : 00400000
Symbol   : _test_function
Address  : 00402306
```

The exact module base, symbol address, and symbol name depend on the compiler, linker, build configuration, and executable layout.

### Symbol Availability

Symbol information is not guaranteed.

`dli_fname` and `dli_fbase` can be available even when `dli_sname` and `dli_saddr` are `NULL`.

For example, a stripped executable may contain enough PE information to identify its module but no usable COFF symbols.

Applications should therefore use:

```c
if (info.dli_sname != NULL) {
    printf("Symbol: %s\n", info.dli_sname);
}
```

rather than assuming that symbol information is always present.

### Thread Safety

The Windows implementation stores `dli_fname` and `dli_sname` in thread-local buffers.

Consequently, the returned strings are valid for the calling thread but may be overwritten by a subsequent `xdladdr()` call on that same thread.

Applications that need to retain the strings should copy them:

```c
char filename[PATH_MAX];

if (info.dli_fname != NULL)
    snprintf(filename, sizeof(filename), "%s", info.dli_fname);
```

The POSIX lifetime of these strings follows the underlying `dladdr()` implementation.

### Error Handling

When `addr` or `info` is `NULL`, `xdladdr()` reports an invalid-parameter error.

On Windows:

```c
GetLastError() == ERROR_INVALID_PARAMETER
```

On POSIX:

```c
errno == EINVAL
```

If the module can be identified but symbol information cannot be obtained, `xdladdr()` can still succeed with:

```c
info->dli_sname == NULL;
info->dli_saddr == NULL;
```

This distinguishes failure to identify the containing module from the absence of symbol information.

---

## Cross-Platform `xdladdr()` Behavior

| Field          | Windows                                   | POSIX             |
| -------------- | ----------------------------------------- | ----------------- |
| `dli_fname`    | `GetModuleFileNameA()`                    | `dladdr()`        |
| `dli_fbase`    | Module returned by `GetModuleHandleExA()` | `dladdr()`        |
| `dli_sname`    | PE/COFF symbol table when available       | Native `dladdr()` |
| `dli_saddr`    | PE/COFF symbol address when available     | Native `dladdr()` |
| Invalid `addr` | `ERROR_INVALID_PARAMETER`                 | `EINVAL`          |
| Invalid `info` | `ERROR_INVALID_PARAMETER`                 | `EINVAL`          |

`xdladdr()` should be treated as a best-effort address-to-module and address-to-symbol query. Module information is independent of symbol availability.

---

# Error Handling

Dynamic-loader failures are reported through platform-specific mechanisms.

### Windows

Windows loader errors are obtained using:

```c
GetLastError();
```

`xdlerror()` and `xwdlerror()` convert these error codes into human-readable messages.

### POSIX

POSIX loader errors are reported through:

```c
dlerror();
```

The `xdlerror()` wrapper returns the native `dlerror()` result.

---

# Thread Safety

On Windows, `xdlerror()` and `xwdlerror()` use thread-local error buffers.

POSIX error handling follows the thread-safety guarantees of the underlying dynamic-loader implementation.

Dynamic library handles should not be closed while another thread may still execute code or access symbols originating from that library.

---

# Cross-Platform Notes

| Feature                | Windows              | POSIX                           |
| ---------------------- | -------------------- | ------------------------------- |
| Narrow library loading | `LoadLibraryA()`     | `dlopen()`                      |
| Wide library loading   | `LoadLibraryW()`     | `xwcstombs()` + `dlopen()`      |
| Symbol lookup          | `GetProcAddress()`   | `dlsym()`                       |
| Library unloading      | `FreeLibrary()`      | `dlclose()`                     |
| Loader errors          | `GetLastError()`     | `dlerror()`                     |
| Lazy/now distinction   | No direct equivalent | `RTLD_LAZY` / `RTLD_NOW`        |
| `XRTLD_NOLOAD`         | `GetModuleHandle*()` | `RTLD_NOLOAD`                   |
| Deep binding           | No direct equivalent | `RTLD_DEEPBIND` where available |

---

# Memory Management

The following functions may return dynamically allocated memory:

* `xwdlopen()` indirectly through `xwcstombs()`
* `xwdlsym()` indirectly through `xwcstombs()`
* `xwdlerror()` on POSIX through `xmbstowcs()`

The caller must follow the ownership rules of the corresponding conversion functions.

Windows `xdlerror()` and `xwdlerror()` return pointers to thread-local static buffers and must not be passed to `free()`.

---

# See Also

* `dlopen(3)` — POSIX dynamic library loading
* `dlsym(3)` — POSIX symbol lookup
* `dlclose(3)` — POSIX dynamic library unloading
* `dlerror(3)` — POSIX dynamic loader errors
* `LoadLibraryA()` — Windows dynamic library loading
* `LoadLibraryW()` — Windows wide-character library loading
* `GetProcAddress()` — Windows symbol lookup
* `FreeLibrary()` — Windows dynamic library unloading

# License

This library is distributed under the **GNU General Public License, version 3 or later**.
