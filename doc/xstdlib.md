# **xstdlib — Extended stdlib utilities of the Extern Library**

Provides Unicode conversion helpers, formatted system execution, perror wrappers, and random byte generation.

**Author:** MrR736
**License:** GPL-3.0+

---

## 1. Overview

`xstdlib.h` extends the standard C library with:

* UTF-8 ↔ wide–string conversion
* Portable UTF-8 aware `system()` wrappers
* Formatted `system()` and `perror()` utilities
* Wide-character equivalents of the above
* Child-process launcher (`csystem`) using fork/exec or _spawnlp on POSIX/Windows
* Random byte generator (`xrand`)

These functions are inlined (`XSTDDEF_INLINE_API`) for performance.

All routines depend on `xstring.h`, `xwchar.h`, and `xstddef.h`.

---

## 2. Functions

---

## 2.1 `xrand()`

```c
void *xrand(size_t size, unsigned int flags);
```

Allocates a block of memory of **size bytes** and fills it with pseudorandom values `rand() % flags`.

### Parameters

* `size` — number of bytes to allocate
* `flags` — modulus for each random byte (e.g., 256 → full byte range)

### Returns

* pointer to allocated buffer
* `NULL` with `errno = EINVAL` or `ENOMEM`

### Notes

* Not cryptographically secure
* Seeds `rand()` with current time every call

---

## 2.2 Multi-byte ↔ Wide character conversions

### `xwcstombs()`

```c
char *xwcstombs(const wchar_t *wcs);
```

Converts a wide string to UTF-8.
Returns a newly allocated UTF-8 char buffer.

### `xmbstowcs()`

```c
wchar_t *xmbstowcs(const char *mbs);
```

Converts UTF-8 to wide.

Both return dynamically allocated memory that must be freed by the caller.

---

## 2.3 System command helpers

### 2.3.1 Wide-character system

```c
int wsystem(const wchar_t *command);
```

* Windows: calls `_wsystem()`.
* POSIX: converts UTF-8 then calls `system()`.

Returned value is normalized to shell exit code.

---

## 2.3.2 Formatted `system()` wrappers:

#### For char strings:

```c
int vnxsystem(size_t len, const char *fmt, va_list ap);
int nxsystem(size_t len, const char *fmt, ...);
int vxsystem(const char *fmt, va_list ap);
int xsystem(const char *fmt, ...);
```

#### For wide strings:

```c
int vnxwsystem(size_t len, const wchar_t *fmt, va_list ap);
int nxwsystem(size_t len, const wchar_t *fmt, ...);
int vxwsystem(const wchar_t *fmt, va_list ap);
int xwsystem(const wchar_t *fmt, ...);
```

These format a command string, allocate memory, then pass the command to the system executor.

---

## 2.4 perror wrappers

### char versions:

```c
void vnxperror(size_t len, const char *fmt, va_list ap);
void nxperror(size_t len, const char *fmt, ...);
void vxperror(const char *fmt, va_list ap);
void xperror(const char *fmt, ...);
```

### wide versions:

```c
void vnxwperror(size_t len, const wchar_t *fmt, va_list ap);
void nxwperror(size_t len, const wchar_t *fmt, ...);
void vxwperror(const wchar_t *fmt, va_list ap);
void xwperror(const wchar_t *fmt, ...);
```

These functions generate formatted error messages then forward to `perror()` or wide equivalent.

---

## 2.5 Child process system call (POSIX fork/exec)

```c
int csystem(const char *cmd);
```

### Description

Executes `cmd` using:

* `_spawnlp()` on Windows
* `fork() → execl("/bin/sh", "sh", "-c", cmd)` on POSIX

Returns:

* exit code
* or a normalized signal result
* −1 on failure (with perror)

---

## 2.6 Wide-version child system

```c
int wcsystem(const wchar_t *cmd);
```

POSIX implementation converts to UTF-8 and forwards to `csystem()`.

---

## 2.7 Formatted variants of csystem:

### char:

```c
int vnxcsystem(size_t len, const char *fmt, va_list ap);
int nxcsystem(size_t len, const char *fmt, ...);
int vxcsystem(const char *fmt, va_list ap);
int xcsystem(const char *fmt, ...);
```

### wide:

```c
int vnxwcsystem(size_t len, const wchar_t *fmt, va_list ap);
int nxwcsystem(size_t len, const wchar_t *fmt, ...);
int vxwcsystem(const wchar_t *fmt, va_list ap);
int xwcsystem(const wchar_t *fmt, ...);
```

---

## 3. Memory Ownership

All conversion and formatted-system wrappers allocate buffers.
**You must free the returned memory where appropriate.**

---

## 4. Error Handling

* Functions set `errno` appropriately (`EINVAL`, `ENOMEM`).
* System wrappers adjust the return value to match shell conventions:

  * normal exit: `WEXITSTATUS`
  * signal: `128 + signal_number`

---

## 5. License

This library is distributed under the **GNU GPL v3 or later**.
