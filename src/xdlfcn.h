/**
 * xdlfcn.h: Cross-Platform Dynamic Library Utilities
 *
 * Copyright (C) 2026 MrR736 <MrR736@users.github.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * The complete text of the GNU General Public License
 * can be found in /usr/share/common-licenses/GPL-3 file.
 */

#ifndef __XDLFCN_H__
#define __XDLFCN_H__

#include "xtypes.h"
#include "xstdlib.h"

#ifdef _WIN32
/* dlopen() flags */
#define XRTLD_LAZY			0x00001
#define XRTLD_NOW			0x00002
#define XRTLD_BINDING_MASK	0x00003
#define XRTLD_NOLOAD		0x00004
#define XRTLD_DEEPBIND		0x00008
#define XRTLD_NODELETE		0x00010
#define XRTLD_LOCAL			0x00000
#define XRTLD_GLOBAL		0x00100
#define XRTLD_FIRST			0x00000
#else
#include <dlfcn.h>

/* dlopen() flags */
#define XRTLD_LAZY			RTLD_LAZY
#define XRTLD_NOW			RTLD_NOW
#define XRTLD_BINDING_MASK	RTLD_BINDING_MASK
#define XRTLD_LOCAL			RTLD_LOCAL
#define XRTLD_GLOBAL		RTLD_GLOBAL

#ifdef RTLD_NOLOAD
# define XRTLD_NOLOAD		RTLD_NOLOAD
#else
# define XRTLD_NOLOAD		0x00004
#endif

#ifdef RTLD_DEEPBIND
# define XRTLD_DEEPBIND		RTLD_DEEPBIND
#else
# define XRTLD_DEEPBIND		0x00008
#endif

#ifdef RTLD_NODELETE
# define XRTLD_NODELETE		RTLD_NODELETE
#else
# define XRTLD_NODELETE		0x00010
#endif
#endif

/* xdlsym() lookup controls */
#ifdef RTLD_NEXT
# define XRTLD_NEXT ((HMODULE)RTLD_NEXT)
#else
# define XRTLD_NEXT ((HMODULE)-1L)
#endif

#ifdef RTLD_DEFAULT
# define XRTLD_DEFAULT ((HMODULE)RTLD_DEFAULT)
#else
# ifdef __APPLE__
#  define XRTLD_DEFAULT ((HMODULE)-2L)
# else
#  define XRTLD_DEFAULT ((HMODULE)0)
# endif
#endif

#ifdef __APPLE__
# ifdef RTLD_FIRST
#  define XRTLD_FIRST RTLD_FIRST
# else
#  define XRTLD_FIRST ((void*)0)
# endif

# ifdef RTLD_SELF
#  define XRTLD_SELF RTLD_SELF
# else
#  define XRTLD_SELF ((void*)-3L)
# endif

# ifdef RTLD_MAIN_ONLY
#  define XRTLD_MAIN_ONLY RTLD_MAIN_ONLY
# else
#  define XRTLD_MAIN_ONLY ((void*)-5L)
# endif
#endif

#define XDLFCN_ERROR_BUFFER 512
#define XDLFCN_PATH_BUFFER  XPATH_MAX
#define XDLFCN_MAX_SYM_NAME 2000

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI char *XCALLAPI xdllibpath(const char *dir, const char *name) {
	if (name == NULL || *name == '\0') {
		errno = EINVAL;
		return NULL;
	}

#ifdef _WIN32
	const char *prefix = "";
	const char *suffix = ".dll";
#elif defined(__APPLE__)
	const char *prefix = "lib";
	const char *suffix = ".dylib";
#else
	const char *prefix = "lib";
	const char *suffix = ".so";
#endif

	size_t dir_len = (dir != NULL) ? strlen(dir) : 0;
	size_t name_len = strlen(name);
	size_t prefix_len = strlen(prefix);
	size_t suffix_len = strlen(suffix);

	int separator = 0;

	if (dir_len != 0) {
		separator =
#ifdef _WIN32
		dir[dir_len - 1] != '/' && dir[dir_len - 1] != '\\';
#else
		dir[dir_len - 1] != '/';
#endif
	}

	size_t length = dir_len + (separator ? 1 : 0) + prefix_len + name_len + suffix_len + 1;
	char *path = (char*)malloc(length);
	if (path == NULL) return NULL;
	size_t pos = 0;
	if (dir_len != 0) {
		memcpy(path + pos, dir, dir_len);
		pos += dir_len;
		if (separator) {
#ifdef _WIN32
			path[pos++] = '\\';
#else
			path[pos++] = '/';
#endif
		}
	}

	memcpy(path + pos, prefix, prefix_len);
	pos += prefix_len;

	memcpy(path + pos, name, name_len);
	pos += name_len;

	memcpy(path + pos, suffix, suffix_len);
	pos += suffix_len;

	path[pos] = '\0';
	return path;
}

XSTDAPI int XCALLAPI xdladdr(const void *addr, XDl_info *info) {
	if (addr == NULL || info == NULL) {
#ifdef _WIN32
		SetLastError(ERROR_INVALID_PARAMETER);
#else
		errno = EINVAL;
#endif
		return 0;
	}

#ifdef _WIN32
	static XTHREAD_LOCAL char xdladdr_filename[XDLFCN_PATH_BUFFER];
	static XTHREAD_LOCAL char xdladdr_symbol[XDLFCN_MAX_SYM_NAME];

	HMODULE wmodule = NULL;

	// Find the module containing addr.
	if (!GetModuleHandleExA(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
			GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			(LPCSTR)addr,&wmodule)) {
		return 0;
	}

	// Get the module filename.
	DWORD length = GetModuleFileNameA(wmodule,xdladdr_filename,(DWORD)xsizeof(xdladdr_filename));
	if (length == 0) return 0;
	if (length >= xsizeof(xdladdr_filename)) {
		xdladdr_filename[xsizeof(xdladdr_filename) - 1] = '\0';
		SetLastError(ERROR_INSUFFICIENT_BUFFER);
		return 0;
	}
	xdladdr_filename[length] = '\0';

	/*
	 * Module information is always available even if the
	 * COFF symbol table cannot be read.
	 */
	info->dli_fname = xdladdr_filename;
	info->dli_fbase = (void *)wmodule;
	info->dli_sname = NULL;
	info->dli_saddr = NULL;

	/*
	 * Open the PE image.
	 */
	HANDLE file = CreateFileA(
		xdladdr_filename,GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL
	);

	if (file == INVALID_HANDLE_VALUE) return 1;

	/*
	 * Map the PE file into memory.
	 */
	HANDLE mapping = CreateFileMappingA(file,NULL,PAGE_READONLY,0,0,NULL);
	if (mapping == NULL) {
		CloseHandle(file);
		return 1;
	}

	unsigned char *image = (unsigned char *)MapViewOfFile(mapping,FILE_MAP_READ,0,0,0);
	if (image == NULL) {
		CloseHandle(mapping);
		CloseHandle(file);
		return 1;
	}

	// Validate DOS header.
	if (((IMAGE_DOS_HEADER *)image)->e_magic != IMAGE_DOS_SIGNATURE) {
		UnmapViewOfFile(image);
		CloseHandle(mapping);
		CloseHandle(file);
		return 1;
	}

	IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER *)image;
	LONG pe_offset = dos->e_lfanew;

	if (pe_offset <= 0) {
		UnmapViewOfFile(image);
		CloseHandle(mapping);
		CloseHandle(file);
		return 1;
	}

	// Validate PE signature.
	DWORD signature = *(DWORD *)(image + pe_offset);

	if (signature != IMAGE_NT_SIGNATURE) {
		UnmapViewOfFile(image);
		CloseHandle(mapping);
		CloseHandle(file);
		return 1;
	}

	IMAGE_FILE_HEADER *file_header = (IMAGE_FILE_HEADER *)(image + pe_offset + sizeof(DWORD));

	/*
	 * The COFF symbol table is optional. MinGW debug builds
	 * normally retain it unless symbols have been stripped.
	 */
	DWORD symbol_table_offset = file_header->PointerToSymbolTable;
	DWORD symbol_count = file_header->NumberOfSymbols;

	if (symbol_table_offset == 0 || symbol_count == 0) {
		UnmapViewOfFile(image);
		CloseHandle(mapping);
		CloseHandle(file);
		return 1;
	}

	// Locate the optional header.
	unsigned char *optional_header = (unsigned char *)file_header + sizeof(IMAGE_FILE_HEADER);
	WORD magic = *(WORD *)optional_header;
	uintptr_t image_base;
	if (magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
		IMAGE_OPTIONAL_HEADER32 *optional32 = (IMAGE_OPTIONAL_HEADER32 *)optional_header;
		image_base = (uintptr_t)optional32->ImageBase;
	} else if (magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
		IMAGE_OPTIONAL_HEADER64 *optional64 = (IMAGE_OPTIONAL_HEADER64 *)optional_header;
		image_base = (uintptr_t)optional64->ImageBase;
	} else {
		UnmapViewOfFile(image);
		CloseHandle(mapping);
		CloseHandle(file);
		return 1;
	}

	// The symbol table consists of IMAGE_SYMBOL entries.
	IMAGE_SYMBOL *symbols = (IMAGE_SYMBOL *)(image + symbol_table_offset);

	/*
	 * The string table follows the complete symbol table.
	 *
	 * Each IMAGE_SYMBOL is 18 bytes.
	 */
	unsigned char *string_table = (unsigned char *)symbols + ((size_t)symbol_count * sizeof(IMAGE_SYMBOL));
	DWORD string_table_size = *(DWORD *)string_table;

	/*
	 * A valid COFF string table contains at least its
	 * four-byte size field.
	 */
	if (string_table_size < sizeof(DWORD)) {
		UnmapViewOfFile(image);
		CloseHandle(mapping);
		CloseHandle(file);
		return 1;
	}

	// Convert the runtime address to an RVA.
	uintptr_t address = (uintptr_t)addr;
	uintptr_t module_base = (uintptr_t)wmodule;
	if (address < module_base) {
		UnmapViewOfFile(image);
		CloseHandle(mapping);
		CloseHandle(file);
		return 1;
	}

	uintptr_t rva = address - module_base;

	// Locate the section table.
	IMAGE_SECTION_HEADER *sections = (IMAGE_SECTION_HEADER *)(optional_header + file_header->SizeOfOptionalHeader);
	IMAGE_SYMBOL *best_symbol = NULL;
	uintptr_t best_address = 0;

	// Walk the COFF symbol table.
	DWORD i = 0;
	while (i < symbol_count) {
		IMAGE_SYMBOL *symbol = &symbols[i];

		/*
		 * Auxiliary symbols belong to the preceding symbol.
		 * They do not represent independent addresses.
		 */
		if (symbol->StorageClass != IMAGE_SYM_CLASS_FILE && symbol->SectionNumber > 0 &&
			symbol->SectionNumber <= (SHORT)file_header->NumberOfSections) {
			WORD section_number = (WORD)symbol->SectionNumber;
			IMAGE_SECTION_HEADER *section = &sections[section_number - 1];

			/*
			 * We are interested primarily in executable
			 * sections for address -> function lookup.
			 */
			if (section->Characteristics & IMAGE_SCN_MEM_EXECUTE) {
				uintptr_t symbol_rva = (uintptr_t)section->VirtualAddress + (uintptr_t)symbol->Value;
				if (symbol_rva <= rva && (best_symbol == NULL || symbol_rva > best_address)) {
					best_symbol = symbol;
					best_address = symbol_rva;
				}
			}
		}

		// Skip auxiliary symbol records.
		i += 1 + symbol->NumberOfAuxSymbols;
	}

	if (best_symbol != NULL) {
		const char *name = NULL;

		/*
		 * COFF symbol names are either:
		 *   Name.ShortName
		 *
		 * or:
		 *   Name.Zeros == 0
		 *   Name.LongName.Offset != 0
		 */
		if (best_symbol->N.Name.Short != 0) {
			name = (const char *)best_symbol->N.ShortName;
		} else {
			DWORD offset = best_symbol->N.Name.Long;
			if (offset >= sizeof(DWORD) && offset < string_table_size) {
				name = (const char *)string_table + offset;
			}
		}

		if (name != NULL && *name != '\0') {
			size_t symbol_length = strnlen(name,XDLFCN_MAX_SYM_NAME - 1);
			memcpy(xdladdr_symbol,name,symbol_length);
			xdladdr_symbol[symbol_length] = '\0';
			info->dli_sname = xdladdr_symbol;
			info->dli_saddr = (void *)(module_base + best_address);
		}
	}

	UnmapViewOfFile(image);
	CloseHandle(mapping);
	CloseHandle(file);

	return 1;
#else
	Dl_info native_info;

	if (dladdr(addr, &native_info) == 0) return 0;

	info->dli_fname = native_info.dli_fname;
	info->dli_fbase = native_info.dli_fbase;
	info->dli_sname = native_info.dli_sname;
	info->dli_saddr = native_info.dli_saddr;

	return 1;
#endif
}

XSTDAPI HMODULE XCALLAPI xdlopen(const char * file, int mode) {
#ifdef _WIN32
	//if (!file) {
	//	SetLastError(ERROR_INVALID_PARAMETER);
	//	return NULL;
	//}

	if (mode & XRTLD_NOLOAD) {
		HMODULE wmodule = GetModuleHandleA(file);
		if (!wmodule) {
			SetLastError(ERROR_MOD_NOT_FOUND);
			return NULL;
		}
		return wmodule;
	}

	/*
	 * XRTLD_LAZY and XRTLD_NOW have no distinct Windows
	 * equivalent. The PE loader resolves imported symbols
	 * according to the normal Windows loader rules.
	 *
	 * XRTLD_DEEPBIND likewise has no Windows equivalent.
	 */
	return LoadLibraryA(file);
#else
	return dlopen(file, mode);
#endif
}

XSTDAPI FARPROC XCALLAPI xdlsym(HMODULE handle, const char *name) {
	if (name == NULL || *name == '\0') {
#ifdef _WIN32
		SetLastError(ERROR_INVALID_PARAMETER);
#else
		errno = EINVAL;
#endif
		return NULL;
	}

#ifdef _WIN32
	if (handle == NULL) {
		SetLastError(ERROR_INVALID_HANDLE);
		return NULL;
	}
	return GetProcAddress(handle, name);
#else
	return dlsym(handle, name);
#endif
}

XSTDAPI char* XCALLAPI xdlerror(void) {
#ifdef _WIN32
	static XTHREAD_LOCAL char error_buffer[XDLFCN_ERROR_BUFFER];
	DWORD error_code = GetLastError();
	if (error_code == ERROR_SUCCESS) return NULL;
	DWORD length = FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,error_code,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		error_buffer,(DWORD)sizeof(error_buffer),NULL
	);
	if (length == 0) {
		_snprintf(error_buffer,sizeof(error_buffer),"Windows error %lu",(unsigned long)error_code);
		error_buffer[sizeof(error_buffer) - 1] = '\0';
	} else {
		while (length > 0 && (error_buffer[length - 1] == '\r' || error_buffer[length - 1] == '\n')) {
			error_buffer[--length] = '\0';
		}
	}
	// xdlerror() consumes the error.
	SetLastError(ERROR_SUCCESS);
	return error_buffer;
#else
	return dlerror();
#endif
}


XSTDAPI HMODULE XCALLAPI xwdlopen(const wchar_t* file, int mode) {
#ifdef _WIN32
	//if (!file) {
	//	SetLastError(ERROR_INVALID_PARAMETER);
	//	return NULL;
	//}

	if (mode & XRTLD_NOLOAD) {
		HMODULE wmodule = GetModuleHandleW(file);
		if (!wmodule) {
			SetLastError(ERROR_MOD_NOT_FOUND);
			return NULL;
		}
		return wmodule;
	}

	/*
	 * XRTLD_LAZY and XRTLD_NOW have no distinct Windows
	 * equivalent. The PE loader resolves imported symbols
	 * according to the normal Windows loader rules.
	 *
	 * XRTLD_DEEPBIND likewise has no Windows equivalent.
	 */
	return LoadLibraryW(file);
#else
	if (!file) return NULL;
	char *s = xwcstombs(file);
	if (!s) return NULL;
	HMODULE ret = dlopen(s, mode);
	free(s);
	return ret;
#endif
}

XSTDAPI FARPROC XCALLAPI xwdlsym(HMODULE handle, const wchar_t *name) {
	if (name == NULL || *name == L'\0') {
#ifdef _WIN32
		SetLastError(ERROR_INVALID_PARAMETER);
#else
		errno = EINVAL;
#endif
		return NULL;
	}
#ifdef _WIN32
	if (handle == NULL) {
		SetLastError(ERROR_INVALID_HANDLE);
		return NULL;
	}
	char *s = xwcstombs(name);
	if (s == NULL) return NULL;
	FARPROC ret = GetProcAddress(handle, s);
#else
	char *s = xwcstombs(name);
	if (s == NULL) return NULL;
	FARPROC ret = dlsym(handle, s);
#endif
	free(s);
	return ret;
}

XSTDAPI wchar_t* XCALLAPI xwdlerror(void) {
#ifdef _WIN32
	static XTHREAD_LOCAL wchar_t error_buffer[XDLFCN_ERROR_BUFFER];
	DWORD error_code = GetLastError();
	if (error_code == ERROR_SUCCESS) return NULL;
	DWORD length = FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,error_code,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		error_buffer,(DWORD)xsizeof(error_buffer),NULL
	);
	if (length == 0) {
		_snwprintf(error_buffer,xsizeof(error_buffer),L"Windows error %lu",(unsigned long)error_code);
		error_buffer[xsizeof(error_buffer) - 1] = L'\0';
	} else {
		while (length > 0 && (error_buffer[length - 1] == L'\r' || error_buffer[length - 1] == L'\n')) {
			error_buffer[--length] = L'\0';
		}
	}
	/* xwdlerror() consumes the error. */
	SetLastError(ERROR_SUCCESS);
	return error_buffer;
#else
	const char *s = dlerror();
	if (!s) return NULL;
	return xmbstowcs(s);
#endif
}

XSTDAPI int XCALLAPI xdlclose(HMODULE handle) {
#ifdef _WIN32
	if (handle == NULL) {
		SetLastError(ERROR_INVALID_HANDLE);
		return -1;
	}
	return FreeLibrary(handle) ? 0 : -1;
#else
	return dlclose(handle);
#endif
}


XSTDAPI FARPROC XCALLAPI GetProcAddressA (HMODULE hModule, LPCSTR lpProcName) {
#ifdef _WIN32
	return GetProcAddress(hModule,lpProcName);
#else
	return xdlsym(hModule,lpProcName);
#endif
}

XSTDAPI FARPROC XCALLAPI GetProcAddressW (HMODULE hModule, LPCWSTR lpProcName) {
#ifdef _WIN32
	if (!hModule || !lpProcName) return NULL;
	char *s = xwcstombs(lpProcName);
	if (!s) return NULL;
	FARPROC ret = GetProcAddress(hModule, s);
	free(s);
	return ret;
#else
	return xwdlsym(hModule,lpProcName);
#endif
}

#ifndef _WIN32
XSTDAPI HMODULE XCALLAPI GetModuleHandleA(const char* lpModuleName) {
	return xdlopen(lpModuleName,XRTLD_NOLOAD);
}

XSTDAPI HMODULE XCALLAPI GetModuleHandleW(const wchar_t* lpModuleName) {
	return xwdlopen(lpModuleName,XRTLD_NOLOAD);
}

XSTDAPI HMODULE XCALLAPI LoadLibraryA(const char* lpLibFileName) {
	return xdlopen(lpLibFileName,XRTLD_LAZY);
}

XSTDAPI HMODULE XCALLAPI LoadLibraryW(const wchar_t* lpLibFileName) {
	return xwdlopen(lpLibFileName,XRTLD_LAZY);
}

XSTDAPI FARPROC XCALLAPI GetProcAddress (HMODULE hModule, LPCSTR lpProcName) {
	return GetProcAddressA(hModule,lpProcName);
}
#endif

#ifdef __cplusplus
}
#endif

#endif