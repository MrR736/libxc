# **XLOG — Cross-Platform Log Utilities**

*Cross-platform logging, diagnostic output, and log-level abstraction for the Extern Library*
**Author:** MrR736
**License:** GPL-3.0+

---

## 1. Overview

`xlog.h` provides a lightweight, cross-platform logging interface for the Extern Library.

It defines:

* Standardized log priorities
* Platform-independent logging macros
* Automatic source file and line information
* Tagged log messages
* Variadic and `va_list` logging interfaces
* Timestamped output on desktop/POSIX platforms
* Android Logcat integration
* Windows-compatible time handling
* Debug-only logging
* Configurable output through `XLOG_STREAM`

The API is designed to provide a single logging interface across Windows, POSIX-compatible systems, and Android.

---

## 2. Header Dependencies

`xlog.h` includes:

```c
#include <stdio.h>
#include "xstdio.h"
```

The implementation additionally relies on standard facilities for:

```c
#include <stdarg.h>
#include <time.h>
```

The logging interface uses:

* `FILE` and standard streams
* `va_list` for variadic logging
* `time_t` and `struct tm` for timestamps
* `strftime()` for timestamp formatting

---

## 3. Default Configuration

The logger provides two configurable defaults:

```c
#define XLOG_DEFAULT_TAG "XLOG"
#define XLOG_STREAM      stderr
```

### Default tag

When a caller supplies a `NULL` tag, the logger uses:

```text
XLOG
```

For example:

```c
XLOGI(NULL, "Library initialized");
```

produces output similar to:

```text
2026-09-30 18:30:42 [INFO]	XLOG (main.c:42) Library initialized
```

### Output stream

Non-Android platforms write to:

```c
stderr
```

through:

```c
#define XLOG_STREAM stderr
```

Applications can redefine `XLOG_STREAM` before including `xlog.h` if a different `FILE *` is required.

---

## 4. Log Priorities

`XLogPriority` defines the available logging levels:

```c
typedef enum XLogPriority {
    XLOG_UNKNOWN = 0,
    XLOG_DEFAULT,
    XLOG_VERBOSE,
    XLOG_DEBUG,
    XLOG_INFO,
    XLOG_WARN,
    XLOG_ERROR,
    XLOG_FATAL,
    XLOG_SILENT
} XLogPriority;
```

### Priority meanings

| Priority       | Purpose                             |
| -------------- | ----------------------------------- |
| `XLOG_UNKNOWN` | Unknown or unsupported priority     |
| `XLOG_DEFAULT` | Default logging priority            |
| `XLOG_VERBOSE` | Detailed diagnostic information     |
| `XLOG_DEBUG`   | Developer/debugging information     |
| `XLOG_INFO`    | General informational messages      |
| `XLOG_WARN`    | Recoverable problems or warnings    |
| `XLOG_ERROR`   | Errors requiring attention          |
| `XLOG_FATAL`   | Fatal conditions or abort scenarios |
| `XLOG_SILENT`  | Suppresses output                   |

`XLOG_UNKNOWN`, `XLOG_DEFAULT`, and `XLOG_SILENT` are primarily intended for internal/platform-specific handling.

---

## 5. Logging API

The library exposes two core functions.

### `xlog_vcprint`

```c
XSTDAPI int XCALLAPI xlog_vcprint(
    int prio,
    int line,
    const char *file,
    const char *tag,
    const char *fmt,
    va_list ap
);
```

This is the `va_list` implementation of the logging system.

Parameters:

* `prio` — log priority
* `line` — source line number
* `file` — source filename
* `tag` — log category/tag
* `fmt` — printf-style format string
* `ap` — variable argument list

The function returns the result of the underlying formatted output operation.

---

### `xlog_cprint`

```c
XSTDAPI int XCALLAPI xlog_cprint(
    int prio,
    int line,
    const char *file,
    const char *tag,
    const char *fmt,
    ...
);
```

This is the standard variadic logging entry point.

Example:

```c
xlog_cprint(
    XLOG_INFO,
    __LINE__,
    __FILE__,
    "NETWORK",
    "Connected to %s",
    hostname
);
```

In normal code, the convenience macros should generally be preferred.

---

## 6. Automatic Source Information

The library provides two convenience macros:

```c
#define xlog_vprint(prio,tag,fmt,ap) \
    xlog_vcprint(prio,__LINE__,__FILE__,tag,fmt,ap)

#define xlog_print(prio,tag,fmt,...) \
    xlog_cprint(prio,__LINE__,__FILE__,tag,fmt,##__VA_ARGS__)
```

These automatically provide:

```c
__FILE__
__LINE__
```

to the logging implementation.

Therefore:

```c
XLOGE("FILE", "Unable to open configuration");
```

can produce:

```text
2026-09-30 18:30:42 [ERROR] FILE (config.c:127) Unable to open configuration
```

This eliminates the need for callers to manually provide source-location information.

---

## 7. Standard Logging Macros

The library provides level-specific macros:

```c
#define XLOGE(tag,fmt,...) \
    xlog_print(XLOG_ERROR,tag,fmt,##__VA_ARGS__)

#define XLOGI(tag,fmt,...) \
    xlog_print(XLOG_INFO,tag,fmt,##__VA_ARGS__)

#define XLOGV(tag,fmt,...) \
    xlog_print(XLOG_VERBOSE,tag,fmt,##__VA_ARGS__)

#define XLOGW(tag,fmt,...) \
    xlog_print(XLOG_WARN,tag,fmt,##__VA_ARGS__)

#define XLOGF(tag,fmt,...) \
    xlog_print(XLOG_FATAL,tag,fmt,##__VA_ARGS__)
```

Example:

```c
XLOGI("INIT", "Initializing library");
XLOGW("CONFIG", "Configuration file not found");
XLOGE("IO", "Failed to read file: %s", filename);
XLOGF("CORE", "Fatal initialization failure");
```

---

## 8. Debug Logging

Debug logging is conditionally compiled using `DEBUG`:

```c
#ifdef DEBUG
#  define XLOGD(tag,fmt,...) \
       xlog_print(XLOG_DEBUG,tag,fmt,##__VA_ARGS__)
#else
#  define XLOGD(tag,fmt,...) 0
#endif
```

When `DEBUG` is defined:

```c
XLOGD("PARSER", "Token count = %d", count);
```

generates a debug log.

When `DEBUG` is not defined, the macro expands to:

```c
0
```

This allows debug logging to be disabled at compile time.

---

## 9. Android Implementation

Android uses the native Android logging system rather than `stderr`.

When:

```c
__ANDROID__
```

is defined, the logger maps `XLogPriority` values to Android priorities.

The mapping is:

| XLOG           | Android               |
| -------------- | --------------------- |
| `XLOG_DEFAULT` | `ANDROID_LOG_DEFAULT` |
| `XLOG_VERBOSE` | `ANDROID_LOG_VERBOSE` |
| `XLOG_DEBUG`   | `ANDROID_LOG_DEBUG`   |
| `XLOG_INFO`    | `ANDROID_LOG_INFO`    |
| `XLOG_WARN`    | `ANDROID_LOG_WARN`    |
| `XLOG_ERROR`   | `ANDROID_LOG_ERROR`   |
| `XLOG_FATAL`   | `ANDROID_LOG_FATAL`   |
| `XLOG_SILENT`  | `ANDROID_LOG_SILENT`  |
| Unknown value  | `ANDROID_LOG_UNKNOWN` |

Logging is performed through:

```c
__android_log_vprint()
```

Android therefore receives native Logcat-compatible output.

Source file and line information are not separately emitted by the non-Android formatting path when Android logging is used.

---

## 10. Windows and POSIX Time Handling

For non-Android systems, `xlog` generates a local timestamp.

On Windows:

```c
localtime_s(&tm, &now);
```

is used.

On POSIX systems:

```c
localtime_r(&now, &tm);
```

is used.

This avoids using the non-thread-safe `localtime()` function.

The timestamp is formatted using:

```c
strftime(
    timestamp,
    sizeof(timestamp),
    "%Y-%m-%d %H:%M:%S",
    &tm
);
```

Example:

```text
2026-09-30 18:30:42
```

---

## 11. Desktop/POSIX Output Format

Non-Android output follows the general structure:

```text
TIMESTAMP [PRIORITY] TAG (FILE:LINE) MESSAGE
```

For example:

```text
2026-09-30 18:30:42 [INFO] XLOG (main.c:42) Library initialized
```

If no source location is available:

```text
2026-09-30 18:30:42 [INFO] XLOG Library initialized
```

The logger terminates every message with a newline and flushes the output stream:

```c
fputc('\n', XLOG_STREAM);
fflush(XLOG_STREAM);
```

This ensures that log messages are immediately delivered to the configured stream.

---

## 12. Silent Logging

`XLOG_SILENT` disables output:

```c
if (prio == XLOG_SILENT) {
    ...
    return 0;
}
```

It can therefore be used to explicitly suppress a logging operation.

Android maps this priority to:

```c
ANDROID_LOG_SILENT
```

---

## 13. Variadic Argument Handling

The logger uses `va_list` internally so that both public logging functions can share the same implementation.

The implementation creates a copy:

```c
va_list apc;
va_copy(apc, ap);
```

The copied argument list is then passed to the underlying formatted output function.

This allows the original `va_list` ownership to remain with the caller while `xlog_vcprint()` consumes its own copy.

The copied list is released with:

```c
va_end(apc);
```

---

## 14. Example Usage

A typical application can include the library through:

```c
#include "libxc.h"
```

and then use:

```c
int main(void)
{
    XLOGI("MAIN", "Application started");

    int value = 42;

    XLOGD("MAIN", "value = %d", value);
    XLOGW("MAIN", "This is a warning");
    XLOGE("MAIN", "This is an error");

    return 0;
}
```

Example output:

```text
2026-09-30 18:30:42 [INFO] MAIN (main.c:3) Application started
2026-09-30 18:30:42 [DEBUG] MAIN (main.c:7) value = 42
2026-09-30 18:30:42 [Warn] MAIN (main.c:8) This is a warning
2026-09-30 18:30:42 [ERROR] MAIN (main.c:9) This is an error
```

`XLOGD` appears only when the library is compiled with:

```text
DEBUG
```

defined.

---

## 15. Platform Behavior

| Platform | Output mechanism                |          Timestamp |        Source location |
| -------- | ------------------------------- | -----------------: | ---------------------: |
| Windows  | `XLOG_STREAM` / `stderr`        |                Yes |                    Yes |
| POSIX    | `XLOG_STREAM` / `stderr`        |                Yes |                    Yes |
| Android  | Android Logcat                  |    Android-managed | Not separately printed |
| C++      | Same C API through `extern "C"` | Platform-dependent |     Platform-dependent |

The public logging interface remains the same regardless of platform.

---

## 16. C++ Compatibility

The exported functions are wrapped in:

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

This prevents C++ name mangling and allows the library to be called directly from C++ applications.

For example:

```cpp
#include "xlog.h"

int main()
{
    XLOGI("CPP", "Hello from C++");
}
```

The underlying functions retain C linkage.

---

## 17. ABI and Export Macros

The logging functions use the Extern Library's API abstraction:

```c
XSTDAPI
XCALLAPI
```

For example:

```c
XSTDAPI int XCALLAPI xlog_cprint(...);
```

These macros allow the library to control:

* Symbol visibility
* DLL exports/imports
* Calling conventions
* Platform-specific ABI requirements

The exact behavior of these macros is provided by the library's portability headers.

---

## 18. Design Goals

`xlog.h` is designed around several principles:

* **Small API** — only two core logging functions
* **Cross-platform behavior** — Windows, POSIX, and Android support
* **Source diagnostics** — automatic file and line information
* **Log levels** — standardized priority classification
* **Tags** — logical grouping of messages
* **Debug elimination** — debug messages can be compiled out
* **C ABI** — usable from both C and C++
* **Existing library integration** — uses `XSTDAPI`, `XCALLAPI`, and `xbasename()`
* **Immediate output** — streams are flushed after each message

---

## 19. Header Integration

`xlog.h` is included by the umbrella library header:

```c
#include "xlog.h"
```

For example:

```c
#ifndef LIBXC_H
#define LIBXC_H

#include "xlocale.h"
#include "xlog.h"
#include "xstdio.h"
#include "xstdlib.h"
#include "xstring.h"
#include "xwchar.h"
#include "xmap.h"
#include "xctype.h"
#include "xwctype.h"
#include "xstat.h"
#include "xdlfcn.h"

#endif
```

This makes the logging API available to applications through the main `libxc.h` interface.

---

## 20. License

This library is distributed under the **GNU General Public License v3 or later**.

See the project license for the complete terms and conditions.

---

## 21. Summary

`xlog.h` provides the Extern Library with a unified logging layer across desktop and mobile platforms.

Its primary interface consists of:

```c
xlog_vcprint()
xlog_cprint()
```

and the convenience macros:

```c
XLOGV()
XLOGD()
XLOGI()
XLOGW()
XLOGE()
XLOGF()
```

Applications can therefore use one logging API while the library handles platform-specific output, timestamp generation, priority mapping, source information, and debug configuration.
