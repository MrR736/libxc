# xstdio.h — Extended STDIO Utilities (Extern Library)

**Author:** MrR736
**License:** GNU GPLv3+
**Project:** Extern Library
**Header:** `xstdio.h`

---

## Overview

`xstdio.h` provides an extended set of safe and Unicode-aware I/O utilities.
It enhances POSIX/ISO C functions by adding:

- Descriptor-based I/O helpers
- Full-file reading from `FILE*` or file descriptors
- Unicode-aware formatted output
- UTF-8 ⇄ wide-character conversions
- Unicode `fopen()` and `remove()` wrappers
- Safe dynamic string builders (`cprintf`, `wcprintf`, etc.)

All functions return dynamically allocated memory that must be freed by the caller.

---

## Functions

### Descriptor Output

#### **`int fdputs(const char *s, int fd);`**
Write the string `s` fully to file descriptor `fd`.

- Returns `0` on success, `-1` on failure.

---

### Convert File Descriptor to `FILE*`

#### **`FILE* fdno(int fd);`**
Creates a `FILE*` stream from a file descriptor using `fdopen(dup(fd))`.

#### **`FILE* fdno_unlocked(int fd);`**
Same as `fdno()`, but locks the stream (`flockfile()` or `_lock_file()`).

---

### File Stream Locking

#### **`void xflockfile(FILE *fp);`**

Locks the `FILE*` stream for the calling thread.

- On Windows: uses `_lock_file()`
- On POSIX: uses `flockfile()`

This provides a portable interface for explicitly locking a `FILE*` before performing a sequence of operations that must be protected from concurrent access.

The lock remains held until the corresponding unlock operation is called.

---

### Flag-Based File Opening

#### **`FILE* fopenm(const char *xrestrict filename, int oflag, ...);`**

Opens a UTF-8 filename using `open()`-style flags and returns a standard C `FILE*` stream.

The function translates the supported flags into an appropriate `fopen()` mode and opens the file in binary mode.

**Supported flags include:**

* `O_RDONLY`
* `O_WRONLY`
* `O_RDWR`
* `O_APPEND`
* `O_BINARY`

**Mode mapping:**

| Flags                  | `fopen()` mode |
| ---------------------- | -------------- |
| `O_RDONLY`             | `"rb"`         |
| `O_WRONLY`             | `"wb"`         |
| `O_WRONLY \| O_APPEND` | `"ab"`         |
| `O_RDWR`               | `"wb+"`        |
| `O_RDWR \| O_APPEND`   | `"ab+"`        |

Returns:

* `FILE*` on success.
* `NULL` on failure.

If an invalid access-mode combination is supplied, `errno` is set to `EINVAL`.

> `O_EXCL` cannot be safely emulated using `fopen()` and is not supported by this implementation.

---

#### **`FILE* wfopenm(const wchar_t *xrestrict filename, int oflag, ...);`**

Wide-character equivalent of `fopenm()`.

On Windows, `wfopenm()` uses `_wfopen()` with a wide-character filename. On POSIX systems, the filename is converted to the platform's multibyte representation before opening the file.

The same `oflag` values and mode mapping used by `fopenm()` apply.

**Supported flags include:**

* `O_RDONLY`
* `O_WRONLY`
* `O_RDWR`
* `O_APPEND`
* `O_BINARY`

**Example:**

```c
FILE *fp = fopenm("data.bin", O_RDWR | O_APPEND);

if (!fp) {
    perror("fopenm");
    return 1;
}

fputs("data\n", fp);
fclose(fp);
```

Wide-character example:

```c
FILE *fp = wfopenm(L"data.bin", O_RDWR | O_APPEND);

if (!fp) {
    perror("wfopenm");
    return 1;
}

fputs("data\n", fp);
fclose(fp);
```

Both functions provide a convenient flag-based interface while returning a standard `FILE*` stream suitable for use with the normal C stdio functions.

---

### Get File Size

#### **`size_t fpsize(FILE *fp);`**
Reads entire `FILE*` into memory, preserving original file position.

Returns:
- The size of the file in bytes.

#### **`size_t fdsize(int fd);`**
Retrieves the size of a file associated with a file descriptor (fd), typically used in lower-level file operations.
It calculates the file size by moving the file pointer to the end, then returning it to the original position.

Returns:
- The size of the file in bytes.

### Full File Read

#### **`void *furead(FILE *fp, size_t *out_size);`**
Reads entire `FILE*` into memory, preserving original file position.

Returns:
- pointer to buffer (NUL-terminated)
- sets `*out_size` to bytes read

#### **`void *fduread(int fd, size_t *out_size);`**
Reads all data from a file descriptor until EOF.

---

## Dynamic String Builders (Formatted Allocation)

### UTF-8 versions

#### **`char* vcprintf(const char *fmt, va_list ap);`**
#### **`char* cprintf(const char *fmt, ...);`**

Equivalent to `sprintf()` but returns a newly allocated memory buffer.

---

### Wide-char versions

#### **`wchar_t* vwcprintf(const wchar_t *fmt, va_list ap);`**
#### **`wchar_t* wcprintf(const wchar_t *fmt, ...);`**

---

### Cross-type conversions

#### **`char* vwccprintf(const wchar_t *fmt, va_list ap);`**
Wide format → UTF-8 result.

#### **`char* wccprintf(const wchar_t *fmt, ...);`**

---

## Unicode File Operations

### **`FILE* wfopen(const wchar_t *filename, const wchar_t *modes);`**
- On Windows: uses `_wfopen()`
- On POSIX: converts using UTF-8 and calls regular `fopen()`

---

### Remove file (UTF-8)

#### **`int xremove(const char *fmt, ...);`**

Formats a filename and removes it.

### Remove file (wide-character)

#### **`int wremove(const wchar_t *s);`**
#### **`int xwremove(const wchar_t *fmt, ...);`**

### Get Directory of Current Executable

#### **`char* getcurrentdirectory_size(size_t);`**
#### **`char* getcurrentdirectory(void);`**

#### **`wchar_t* wgetcurrentdirectory_size(size_t);`**
#### **`wchar_t* wgetcurrentdirectory(void);`**

---

## Error Handling

- Most functions set `errno` on failure.
- Functions returning allocated memory return `NULL` on error.

---

## Memory Management

⚠ All returned buffers from these functions must be freed:

- `cprintf()`, `wcprintf()`
- `furead()`, `fduread()`
- `vwccprintf()`, `wccprintf()`

---

## Thread Safety

- `fdno_unlocked()` explicitly locks the resulting `FILE*`
- All other functions follow the OS stdio safety rules (same as `<stdio.h>`)

---

## Cross-Platform Notes

- Windows differences handled with `_wfopen`, `_lock_file`, wide-API path handling
- POSIX systems use UTF-8 conversions via `xwcstombs` and standard `fopen()`

---

## See Also

- `xstdlib.h` — Unicode system() wrappers, perror extensions, conversions
- `xstring.h` — length calculators used by this module
- `man 3 fopen`, `man 3 read`, `man 3 printf`

---

## License

This library is distributed under the **GNU GPL v3 or later**.

// add
XSTDAPI FILE* XCALLAPI fopenm(const char *xrestrict filename, int oflag, ...)
XSTDAPI FILE* XCALLAPI wfopenm(const wchar_t *xrestrict filename, int oflag, ...) 