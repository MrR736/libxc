# xthread — Cross-Platform Thread Library

## Overview

`xthread` provides a cross-platform mutex API for synchronizing access to shared resources. It uses native Windows synchronization primitives on Windows and POSIX threads (`pthreads`) on other platforms.

## Header

```c
#include <xthread.h>
```

## Types

### `xmutex_t`

Platform-independent mutex type.

- **Windows:** `CRITICAL_SECTION`
- **POSIX:** `pthread_mutex_t`

The underlying type is selected automatically according to the target platform.

## Functions

### `xmutex_init`

```c
int xmutex_init(xmutex_t *m);
```

Initializes a mutex.

**Parameters**
- `m` — Pointer to the mutex to initialize.

**Returns**
- `0` on success.
- A nonzero error code on POSIX initialization failure.

On Windows, this implementation returns `0` after calling `InitializeCriticalSection()`.

### `xmutex_lock`

```c
void xmutex_lock(xmutex_t *m);
```

Acquires the mutex, blocking the calling thread until the mutex becomes available.

**Parameters**
- `m` — Pointer to an initialized mutex.

### `xmutex_unlock`

```c
void xmutex_unlock(xmutex_t *m);
```

Releases a mutex previously acquired by the calling thread.

**Parameters**
- `m` — Pointer to the locked mutex.

### `xmutex_destroy`

```c
void xmutex_destroy(xmutex_t *m);
```

Destroys an initialized mutex and releases its associated resources.

**Parameters**
- `m` — Pointer to the mutex to destroy.

The mutex must not be locked or still in use by another thread when destroyed.

## Example

```c
#include <xthread.h>

int main(void)
{
    xmutex_t mutex;

    if (xmutex_init(&mutex) != 0)
        return 1;

    xmutex_lock(&mutex);

    /* Access shared resources here. */

    xmutex_unlock(&mutex);
    xmutex_destroy(&mutex);

    return 0;
}
```

## Platform Requirements

| Platform | Synchronization backend |
|---|---|
| Windows | Windows Critical Sections |
| POSIX-compatible systems | POSIX Threads |

POSIX builds may require linking with the platform's thread library, preferably through CMake's `Threads::Threads` target.

## Notes

- Initialize each mutex before use.
- Do not destroy a mutex while it is locked or in use.
- Only the thread that owns a mutex should unlock it.
- This API does not expose recursive mutex configuration, timed locking, or reader/writer locks.

## License

GNU General Public License v3 or later (GPL-3.0-or-later).