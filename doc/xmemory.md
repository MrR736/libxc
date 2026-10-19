# XMEMORY — Cross-Platform Memory Utilities

**`xmemory.h` — memory allocation and memory manipulation wrappers for the Extern Library**  
**Author:** MrR736  
**License:** GPL-3.0+

## 1. Overview

`xmemory.h` provides a small C API around standard C memory allocation and memory manipulation routines.

The interface exposes wrappers for:

- Dynamic memory allocation
- Zero-initialized allocation
- Reallocation
- Memory release
- Memory initialization
- Memory copying
- Memory moving
- Memory comparison
- Copy-until-character operations
- Copy operations returning the end of the destination buffer

The wrappers use the Extern Library ABI macros `XSTDAPI` and `XCALLAPI` and are available to both C and C++ applications through C linkage.

## 2. Header

The public header includes:

```c
#include "xstddef.h"
```

The implementation relies on standard memory and allocation facilities such as `malloc()`, `calloc()`, `realloc()`, `free()`, `memset()`, `memcpy()`, `memccpy()`, `memmove()`, `memcmp()`, and `mempcpy()`.

## 3. API

### `xmalloc()`

```c
XSTDAPI void* XCALLAPI xmalloc(size_t size);
```

Allocates `size` bytes of memory using `malloc()`. The returned memory is uninitialized.

### `xcalloc()`

```c
XSTDAPI void* XCALLAPI xcalloc(size_t nmemb, size_t size);
```

Allocates storage for `nmemb` objects of `size` bytes each using `calloc()`. The allocated storage is initialized according to `calloc()` semantics.

### `xrealloc()`

```c
XSTDAPI void* XCALLAPI xrealloc(void *ptr, size_t size);
```

Changes the size of an existing allocation using `realloc()`. Behavior follows the underlying implementation.

### `xfree()`

```c
XSTDAPI void XCALLAPI xfree(void *ptr);
```

Releases memory using `free()`. Passing `NULL` follows standard `free()` semantics.

### `xmemset()`

```c
XSTDAPI void* XCALLAPI xmemset(void *s, int c, size_t n);
```

Fills the first `n` bytes of the object pointed to by `s` with the byte value represented by `c`. Returns `s`.

### `xmemcpy()`

```c
XSTDAPI void* XCALLAPI xmemcpy(void *xrestrict dest, const void *xrestrict src, size_t n);
```

Copies `n` bytes from `src` to `dest` using `memcpy()`. The regions must not overlap. Returns `dest`.

### `xmemccpy()`

```c
XSTDAPI void* XCALLAPI xmemccpy(void *xrestrict dest, const void *xrestrict src, int c, size_t n);
```

Copies up to `n` bytes from `src` to `dest`, stopping after the first occurrence of the byte represented by `c`. The return value follows `memccpy()` semantics.

`memccpy()` is not part of ISO C on all supported platforms, so availability depends on the target platform and portability layer.

### `xmemmove()`

```c
XSTDAPI void* XCALLAPI xmemmove(void *dest, const void *src, size_t n);
```

Copies `n` bytes from `src` to `dest`. Unlike `xmemcpy()`, overlapping regions are supported. Returns `dest`.

### `xmemcmp()`

```c
XSTDAPI int XCALLAPI xmemcmp(const void *s1, const void *s2, size_t n);
```

Compares the first `n` bytes of two memory regions using `memcmp()`. The result has the same semantics as `memcmp()`.

### `xmempcpy()`

```c
XSTDAPI void* XCALLAPI xmempcpy(void *xrestrict dest, const void *xrestrict src, size_t n);
```

Copies `n` bytes from `src` to `dest` and returns a pointer immediately following the copied destination range. Availability follows the target platform's `mempcpy()` support.

## 4. C++ Compatibility

The API is enclosed in:

```c
#ifdef __cplusplus
extern "C" {
#endif
```

and:

```c
#ifdef __cplusplus
}
#endif
```

This gives the exported functions C linkage when included from C++.

Example:

```cpp
#include "xmemory.h"

int main()
{
    void *ptr = xmalloc(128);

    if (ptr != nullptr) {
        xmemset(ptr, 0, 128);
        xfree(ptr);
    }

    return 0;
}
```

## 5. ABI and Export Macros

The exported functions use:

```c
XSTDAPI
XCALLAPI
```

These macros provide the Extern Library's platform-specific symbol visibility, DLL import/export, calling convention, and ABI abstraction.

## 6. Design

`xmemory.h` intentionally keeps its implementation thin. Each public function directly delegates to its corresponding memory or allocation primitive while providing consistent Extern Library naming and ABI handling.

## 7. Safety and Semantics

The wrappers do not add bounds checking, ownership tracking, lifetime management, or synchronization. Callers remain responsible for satisfying the requirements of the underlying C functions.

In particular:

- `xmalloc()` returns uninitialized storage.
- `xcalloc()` follows `calloc()` initialization semantics.
- `xrealloc()` follows `realloc()` semantics.
- `xfree()` must be used with a compatible allocated pointer or `NULL`.
- `xmemcpy()` requires non-overlapping source and destination regions.
- `xmemmove()` supports overlapping regions.
- Memory operations require valid accessible memory ranges.
- `xmemcmp()` requires both compared ranges to be readable for the requested length.
- `xmemccpy()` and `xmempcpy()` depend on platform support for their underlying functions.

## 8. Header Guard

The header uses:

```c
#ifndef __XMEMORY_H__
#define __XMEMORY_H__
```

The supplied implementation ends with `__XSTRING_H__` in its comment. Since the actual guard is `__XMEMORY_H__`, the closing comment should normally be:

```c
#endif // __XMEMORY_H__
```

## 9. Examples

```c
#include "xmemory.h"

int main(void)
{
    const char source[] = "Extern";
    char buffer[7];

    xmemcpy(buffer, source, sizeof(source));

    return 0;
}
```

Dynamic allocation:

```c
#include "xmemory.h"

int main(void)
{
    int *values = xcalloc(16, sizeof(*values));

    if (values == NULL)
        return 1;

    values[0] = 42;

    int *tmp = xrealloc(values, 32 * sizeof(*values));

    if (tmp == NULL) {
        xfree(values);
        return 1;
    }

    values = tmp;
    xfree(values);
    return 0;
}
```

## 10. Return Values

| Function | Return value |
|---|---|
| `xmalloc()` | Pointer returned by `malloc()` |
| `xcalloc()` | Pointer returned by `calloc()` |
| `xrealloc()` | Pointer returned by `realloc()` |
| `xfree()` | No return value |
| `xmemset()` | Destination pointer |
| `xmemcpy()` | Destination pointer |
| `xmemccpy()` | Return value of `memccpy()` |
| `xmemmove()` | Destination pointer |
| `xmemcmp()` | Comparison result from `memcmp()` |
| `xmempcpy()` | Return value of `mempcpy()` |

## 11. Portability

The standard memory-operation wrappers inherit the portability characteristics of their underlying C facilities.

`memccpy()` and `mempcpy()` are not universally provided by ISO C implementations. A platform portability implementation may therefore be required where those functions are unavailable.

## 12. Errors

The XMEMORY interface does not define a separate error-reporting mechanism.

Allocation failures are reported through the return values of `xmalloc()`, `xcalloc()`, and `xrealloc()`.

Invalid pointers, invalid ranges, prohibited overlap, and other violations of the underlying function contracts are not converted into XMEMORY-specific error codes.

## 13. Thread Safety

The wrappers do not maintain library-level mutable state. Thread-safety characteristics therefore follow those of the underlying allocation and memory functions supplied by the target C runtime and platform.

Applications remain responsible for synchronizing access to shared memory objects when required.

## 14. See Also

- `xstddef(3)`
- `malloc(3)`
- `calloc(3)`
- `realloc(3)`
- `free(3)`
- `memset(3)`
- `memcpy(3)`
- `memccpy(3)`
- `memmove(3)`
- `memcmp(3)`
- `mempcpy(3)`

## 15. Author

Written by **MrR736**.

## 16. License

This library is distributed under the **GNU General Public License v3 or later**.

See the project license for the complete terms and conditions.
