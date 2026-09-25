# **XSTDDEF — Core Platform & Visibility Definitions**

*Base portability and compiler-feature header for the Extern Library*
**Author:** MrR736
**License:** GPL-3.0+

---

## 1. Overview

`xstddef.h` provides essential compiler, attribute, visibility, and compatibility definitions used throughout the Extern Library.

It includes:

* Standard system headers
* Platform-specific includes for Windows and POSIX
* Compiler feature wrappers
* Symbol visibility macros
* Portable inline definition
* Restrict keyword compatibility

This header contains **no functions**. It serves as the foundation for all other headers (`xmap`, `xstring`, `xwchar`, `xlocale`, etc.).

---

## 2. Included Standard Headers

The file provides immediate availability of common ANSI/ISO C and POSIX headers:

```c
#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
```

Platform-specific additions:

### On Windows:

```c
#include <process.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>
```

### On POSIX:

```c
#include <unistd.h>
#include <sys/wait.h>
```

These provide:

* process spawning
* low-level I/O
* Unicode/console handling (Windows)
* filesystem & stat() structures
* wide-character support

---

## 3. Inline Keyword Handling

MSVC versions prior to 2015 (`_MSC_VER < 1900`) require `__inline` instead of `inline`:

```c
#if defined(_MSC_VER) && _MSC_VER < 1900 && !defined(inline)
#define inline __inline
#endif
```

Ensures the entire library uses `inline` portably across compilers.

---

## 4. Attribute and Visibility Abstractions

### GCC/Clang: generic attribute support

```c
#define __xattribute__(V) __attribute__(V)
```

### Symbol visibility

When supported:

```c
#define __xvisibility__(V) __attribute__((visibility(#V)))
```

Otherwise visibility expands to nothing.

On ARM toolchains with externally-visible quirk:

```c
__attribute__((externally_visible, visibility(#V)))
```

---

## 5. restrict Keyword Normalization

```c
#ifdef __GNUC__
#define __restrict __restrict
#else
#define __restrict /* nothing */
#endif
```

Ensures uniform optimization clarity on all compilers.

---

## 6. Export/Import API Macros

Three main visibility/export macros define the library's ABI footprint:

### Export symbols (shared library build)

```c
#define XSTDDEF_EXPORT_API __xvisibility__(default)
```

### Import symbols (external use)

```c
#define XSTDDEF_IMPORT_API extern __xvisibility__(default)
```

### Inline API macros

```c
#define XSTDDEF_INLINE_API static inline
```

Used throughout all other headers to mark inline ABI-safe functions.

---

## 7. Purpose in the Extern Library

`xstddef.h` is the required baseline header that:

* Unifies Windows/POSIX compat
* Provides attribute wrappers
* Normalizes visibility & inline handling
* Ensures the rest of the library is warning-clean across GCC, Clang, and MSVC
* Prevents duplication of system includes in each module

All other headers include it *first*.

---

## 8. License

This library is distributed under the **GNU GPL v3 or later**.
