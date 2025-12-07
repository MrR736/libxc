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

### ### Descriptor Output

#### **`int fdputs(const char *s, int fd);`**
Write the string `s` fully to file descriptor `fd`.

- Returns `0` on success, `-1` on failure.

---

### ### Convert File Descriptor to `FILE*`

#### **`FILE* fdno(int fd);`**
Creates a `FILE*` stream from a file descriptor using `fdopen(dup(fd))`.

#### **`FILE* fdno_unlocked(int fd);`**
Same as `fdno()`, but locks the stream (`flockfile()` or `_lock_file()`).

---

### ### Full File Read

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
