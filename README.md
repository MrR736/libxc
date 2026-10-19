# libxc — Extern C Utility Library

[![License: GPL-3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.en.html)

`libxc` is a lightweight C/C++ library providing extended standard utilities, including:

- String, memory, and wide-character manipulation
- File I/O helpers and safer printf variants
- Extended standard types and macros
- Locale and map utilities

It is designed to complement the C standard library with convenient, safe, and portable extensions.

---

## Modules

| Module  | Description                              | Markdown                     | Man page                          |
| ------- | ---------------------------------------- | ---------------------------- | --------------------------------- |
| xstddef | Extended standard types and macros       | [xstddef.md](doc/xstddef.md) | [xstddef.3](doc/man3/xstddef.3)   |
| xstdlib | Memory and utility helpers               | [xstdlib.md](doc/xstdlib.md) | [xstdlib.3](doc/man3/xstdlib.3)   |
| xstring | String manipulation helpers              | [xstring.md](doc/xstring.md) | [xstring.3](doc/man3/xstring.3)   |
| xwchar  | Wide-Character utilities                 | [xwchar.md](doc/xwchar.md)   | [xwchar.3](doc/man3/xwchar.3)     |
| xstdio  | File I/O helpers and safer printf        | [xstdio.md](doc/xstdio.md)   | [xstdio.3](doc/man3/xstdio.3)     |
| xlocale | Locale-related utilities                 | [xlocale.md](doc/xlocale.md) | [xlocale.3](doc/man3/xlocale.3)   |
| xmap    | Map / associative array utilities        | [xmap.md](doc/xmap.md)       | [xmap.3](doc/man3/xmap.3)         |
| xctype  | Extended Character Type Utilities        | [xctype.md](doc/xctype.md)   | [xctype.3](doc/man3/xctype.3)     |
| xwctype | Extended Wide-Character Type Utilities   | [xwctype.md](doc/xwctype.md) | [xwctype.3](doc/man3/xwctype.3)   |
| xdlfcn  | Cross-Platform Dynamic Library Utilities | [xdlfcn.md](doc/xdlfcn.md)   | [xdlfcn.3](doc/man3/xdlfcn.3)     |
| xstat   | Cross-Platform Directory Operations      | [xstat.md](doc/xstat.md)     | [xstat.3](doc/man3/xstat.3)       |
| xlog    | Cross-Platform Log Utilities             | [xlog.md](doc/xlog.md)       | [xlog.3](doc/man3/xlog.3)         |
| xmemory | Cross-Platform Memory Utilities          | [xmemory.md](doc/xmemory.md) | [xmemory.3](doc/man3/xmemory.3)   |
| xthread | Cross-Platform Thread Library            | [xthread.md](doc/xthread.md) | [xthread.3](doc/man3/xthread.3)   |
| xuchar  | Unicode Character String Utilities       | [xuchar.md](doc/xuchar.md)   | [xuchar.3](doc/man3/xuchar.3)     |

---

## Getting Started

### Include headers

```c
#include "xstdio.h"
#include "xstring.h"
#include "xstdlib.h"
```

### Quick Examples

#### example.txt
```txt
Hello World!
```

#### Using `xstdio` to read a file

```c
#include "xstdio.h"
#include <stdio.h>

int main() {
    FILE *fp = fopen("example.txt", "r");
    if (!fp) return 1;

    size_t size;
    char *data = furead(fp, &size);
    fclose(fp);

    if (data) {
        printf("File content (%zu bytes):\n%s\n", size, data);
        free(data);
    }
    return 0;
}
```

#### Using `xstring` to format a string safely

```c
#include "xstring.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Example format string
    const char *name = "World";
    int number = 42;

    // 1. Calculate the length of the formatted string
    size_t len = xstrlen("Hello %s! Number: %d", name, number);
    if (len == 0) {
        fprintf(stderr, "Failed to calculate string length.\n");
        return 1;
    }

    // 2. Allocate buffer
    char *buf = (char *)malloc(len + 1);
    if (!buf) {
        perror("malloc");
        return 1;
    }

    // 3. Format the string safely
    snprintf(buf, len + 1, "Hello %s! Number: %d", name, number);

    // 4. Print the result
    printf("%s\n", buf);

    // 5. Free memory
    free(buf);

    return 0;
}
```

---

## Documentation Policy

To prioritize library development and reduce documentation maintenance overhead, existing documentation will no longer be routinely updated.

- **Existing documentation:** Remains unchanged unless I choose to revise it.
- **New modules:** Documentation will be added when a new module is introduced.
- **Optional updates:** Improvements, corrections, and revisions to existing documentation are entirely at my discretion.
- **No mandatory maintenance:** Documentation updates are not required unless I decide they are necessary.
- **libxcxx:** No documentation will be provided for libxcxx.

The primary focus is on developing and improving the library itself rather than maintaining existing documentation. Documentation changes will be made when I consider them worthwhile, not as a routine obligation.

---

## License

This library is licensed under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl-3.0.en.html).
You are free to use, modify, and redistribute it under the terms of the GPLv3.

---

## Contributing

Contributions are welcome! To contribute:

1. Fork the repository
2. Make your changes
3. Submit a pull request with a clear description of your changes

Please follow the existing code style for consistency.

---

## Contact

Author: MrR736 — [MrR736@users.github.com](mailto:MrR736@users.github.com)
