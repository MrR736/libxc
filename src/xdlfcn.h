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

#include "xmemory.h"
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
# define XRTLD_NEXT RTLD_NEXT
#else
# define XRTLD_NEXT ((xhandle_t)-1L)
#endif

#ifdef RTLD_DEFAULT
# define XRTLD_DEFAULT RTLD_DEFAULT
#else
# ifdef __APPLE__
#  define XRTLD_DEFAULT ((xhandle_t)-2L)
# else
#  define XRTLD_DEFAULT ((xhandle_t)0)
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

typedef struct {
	const char *dli_fname;	/* File name of defining object.  */
	void *dli_fbase;		/* Load address of that object.  */
	const char *dli_sname;	/* Name of nearest symbol.  */
	void *dli_saddr;		/* Exact value of nearest symbol.  */
} XDl_info;

typedef struct {
	const wchar_t *wdli_fname;	/* File name of defining object.  */
	void *dli_fbase;		/* Load address of that object.  */
	const wchar_t *wdli_sname;	/* Name of nearest symbol.  */
	void *dli_saddr;		/* Exact value of nearest symbol.  */
} XWDl_info;

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI char* XCALLAPI xdllibpath(const char* dir, const char* name) {
#ifdef _WIN32
	const char* prefix = "";
	const char* suffix = ".dll";
	const char* sep = "\\";
#else
	const char* prefix = "lib";
#ifdef __APPLE__
	const char* suffix = ".dylib";
#else
	const char* suffix = ".so";
#endif
	const char* sep = "/";
#endif

	if (name == NULL || *name == '\0') {
		errno = EINVAL;
		return NULL;
	}

	const char* parts[5];
	size_t n = 0;
	if (dir != NULL && *dir != '\0') {
		size_t len = strlen(dir);
		parts[n++] = dir;
#ifdef _WIN32
		if (dir[len - 1] != '/' && dir[len - 1] != '\\') parts[n++] = sep;
#else
		if (dir[len - 1] != '/') parts[n++] = sep;
#endif
	}

	parts[n++] = prefix;
	parts[n++] = name;
	parts[n++] = suffix;

	return xstrcmbs(parts, n);
}

XSTDAPI wchar_t* XCALLAPI xwdllibpath(const wchar_t* dir,const wchar_t* name) {
#ifdef _WIN32
	const wchar_t* prefix = L"";
	const wchar_t* suffix = L".dll";
	const wchar_t* sep = L"\\";
#else
	const wchar_t* prefix = L"lib";
#ifdef __APPLE__
	const wchar_t* suffix = L".dylib";
#else
	const wchar_t* suffix = L".so";
#endif
	const wchar_t* sep = L"/";
#endif

	if (name == NULL || *name == L'\0') {
		errno = EINVAL;
		return NULL;
	}

	const wchar_t* parts[5];
	size_t n = 0;
	if (dir != NULL && *dir != L'\0') {
		size_t len = wcslen(dir);
		parts[n++] = dir;
#ifdef _WIN32
		if (dir[len - 1] != L'/' && dir[len - 1] != L'\\') parts[n++] = sep;
#else
		if (dir[len - 1] != L'/') parts[n++] = sep;
#endif
	}

	parts[n++] = prefix;
	parts[n++] = name;
	parts[n++] = suffix;

	return xwcscmbs(parts, n);
}

XSTDAPI const char* XCALLAPI xcdllibpath(const char *dir, const char *name) {
#ifdef _WIN32
	const char *prefix = "";
	const char *suffix = ".dll";
	const char psep = '\\';
#else
	const char *prefix = "lib";
#ifdef __APPLE__
	const char* suffix = ".dylib";
#else
	const char* suffix = ".so";
#endif
	const char psep = '/';
#endif

#ifdef __cplusplus
	if (name == NULL || *name == '\0') {
		errno = EINVAL;
		return NULL;
	}

	xthread_local std::string result;
	result = std::string(dir ? dir : "") +  (prefix ? prefix : "") +
			 (name ? name : "") + (suffix ? suffix : "");
	return result.c_str();
#else
	static char ret[XPATH_MAX];

	if (name == NULL) return NULL;
	if (dir == NULL) dir = "";

	size_t d = strlen(dir);
	size_t p = strlen(prefix);
	size_t n = strlen(name);
	size_t s = strlen(suffix);

	size_t sep = d > 0 && dir[d - 1] != '/' && dir[d - 1] != '\\';

	if (d >= XPATH_MAX || p >= XPATH_MAX - d ||
		sep >= XPATH_MAX - d - p || n >= XPATH_MAX - d - p - sep ||
		s >= XPATH_MAX - d - p - sep - n) return NULL;

	char *out = ret;

#define APPEND_STR(src, len) do { \
		xmemcpy(out, (src), (len)); \
		out += (len); \
	} while (0)

	APPEND_STR(dir, d);
	if (sep) *out++ = psep;

	APPEND_STR(prefix, p);
	APPEND_STR(name, n);
	APPEND_STR(suffix, s);

	*out = '\0';

#undef APPEND_STR

	return ret;
#endif
}

XSTDAPI const wchar_t* XCALLAPI xcwdllibpath(const wchar_t *dir, const wchar_t *name) {
#ifdef _WIN32
	const wchar_t *prefix = L"";
	const wchar_t *suffix = L".dll";
	const wchar_t psep = L'\\';
#else
	const wchar_t *prefix = L"lib";
#ifdef __APPLE__
	const wchar_t* suffix = L".dylib";
#else
	const wchar_t* suffix = L".so";
	#endif
	const wchar_t psep = L'/';
#endif

#ifdef __cplusplus
	if (name == NULL || *name == L'\0') {
		errno = EINVAL;
		return NULL;
	}

	xthread_local std::wstring result;
	result = std::wstring(dir ? dir : L"") + (prefix ? prefix : L"") + (name ? name : L"") + (suffix ? suffix : L"");
	return result.c_str();
#else
	static wchar_t ret[XPATH_MAX];

	if (name == NULL) return NULL;
	if (dir == NULL) dir = L"";

	size_t d = wcslen(dir);
	size_t p = wcslen(prefix);
	size_t n = wcslen(name);
	size_t s = wcslen(suffix);

	size_t sep = d > 0 && dir[d - 1] != L'/' && dir[d - 1] != L'\\';

	if (d >= XPATH_MAX || p >= XPATH_MAX - d ||
		sep >= XPATH_MAX - d - p || n >= XPATH_MAX - d - p - sep ||
		s >= XPATH_MAX - d - p - sep - n) return NULL;

	size_t total = d + sep + p + n + s;

	if (total >= XPATH_MAX) return NULL;

	wchar_t *out = ret;

#define APPEND_WSTR(src, len) do { \
		xmemcpy(out, (src), (len) * sizeof(wchar_t)); \
		out += (len); \
	} while (0)

	APPEND_WSTR(dir, d);

	if (sep) *out++ = psep;

	APPEND_WSTR(prefix, p);
	APPEND_WSTR(name, n);
	APPEND_WSTR(suffix, s);

	*out = L'\0';
#undef APPEND_WSTR

	return ret;
#endif
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
	static xthread_local char xdladdr_filename[XDLFCN_PATH_BUFFER];
	static xthread_local char xdladdr_symbol[XDLFCN_MAX_SYM_NAME];

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
			xmemcpy(xdladdr_symbol,name,symbol_length);
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

XSTDAPI int XCALLAPI xwdladdr(const void *addr, XWDl_info *info) {
	if (addr == NULL || info == NULL) {
#ifdef _WIN32
		SetLastError(ERROR_INVALID_PARAMETER);
#else
		errno = EINVAL;
#endif
		return 0;
	}

#ifdef _WIN32
#define xwdl_range_valid(size,offset,length) \
	((offset) <= (size) && (length) <= (size) - (offset))

	static xthread_local wchar_t xdladdr_filename[XDLFCN_PATH_BUFFER];
	static xthread_local wchar_t xdladdr_symbol[XDLFCN_MAX_SYM_NAME];

	HMODULE wmodule = NULL;
	HANDLE file = INVALID_HANDLE_VALUE;
	HANDLE mapping = NULL;
	unsigned char *image = NULL;
	size_t image_size = 0;
	int result = 0;

	info->wdli_fname = NULL;
	info->dli_fbase = NULL;
	info->wdli_sname = NULL;
	info->dli_saddr = NULL;

	/*
	 * Find the module containing addr.
	 */
	if (!GetModuleHandleExW(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
			GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			(LPCWSTR)addr, &wmodule)) {
		return 0;
	}

	/*
	 * Retrieve the module filename.
	 */
	DWORD length = GetModuleFileNameW(
		wmodule,
		xdladdr_filename,
		(DWORD)xsizeof(xdladdr_filename)
	);

	if (length == 0) return 0;

	if (length >= xsizeof(xdladdr_filename)) {
		xdladdr_filename[xsizeof(xdladdr_filename) - 1] = L'\0';
		SetLastError(ERROR_INSUFFICIENT_BUFFER);
		return 0;
	}

	xdladdr_filename[length] = L'\0';

	info->wdli_fname = xdladdr_filename;
	info->dli_fbase = (void *)wmodule;

	/*
	 * The module was found successfully. Symbol information
	 * is optional and must not affect the return value.
	 */
	result = 1;

	/*
	 * Open the PE image.
	 */
	file = CreateFileW(
		xdladdr_filename,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (file == INVALID_HANDLE_VALUE) goto cleanup;

	LARGE_INTEGER file_size;

	if (!GetFileSizeEx(file, &file_size) ||
		file_size.QuadPart <= 0 ||
		(uint64_t)file_size.QuadPart > (uint64_t)SIZE_MAX) {
		goto cleanup;
	}

	image_size = (size_t)file_size.QuadPart;

	/*
	 * Map the PE file into memory.
	 */
	mapping = CreateFileMappingW(
		file, NULL, PAGE_READONLY, 0, 0, NULL
	);

	if (mapping == NULL) goto cleanup;

	image = (unsigned char *)MapViewOfFile(
		mapping, FILE_MAP_READ, 0, 0, 0
	);

	if (image == NULL) goto cleanup;

	/*
	 * Validate the DOS header.
	 */
	{
		if (!xwdl_range_valid(image_size, 0, sizeof(IMAGE_DOS_HEADER))) {
			goto cleanup;
		}

		IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER *)image;

		if (dos->e_magic != IMAGE_DOS_SIGNATURE || dos->e_lfanew <= 0) {
			goto cleanup;
		}

		size_t pe_offset = (size_t)dos->e_lfanew;

		/*
		* Validate the PE signature and COFF file header.
		*/
		if (!xwdl_range_valid(
				image_size,
				pe_offset,
				sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER))) {
			goto cleanup;
		}

		DWORD signature;
		xmemcpy(&signature, image + pe_offset, sizeof(signature));

		if (signature != IMAGE_NT_SIGNATURE) goto cleanup;

		IMAGE_FILE_HEADER *file_header =
			(IMAGE_FILE_HEADER *)(image + pe_offset + sizeof(DWORD));

		size_t optional_offset =
			pe_offset + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER);

		if (!xwdl_range_valid(
				image_size,
				optional_offset,
				file_header->SizeOfOptionalHeader) ||
			file_header->SizeOfOptionalHeader < sizeof(WORD)) {
			goto cleanup;
		}

		unsigned char *optional_header = image + optional_offset;

		WORD magic;
		xmemcpy(&magic, optional_header, sizeof(magic));

		DWORD size_of_image;

		if (magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
			if (file_header->SizeOfOptionalHeader <
					offsetof(IMAGE_OPTIONAL_HEADER32, SizeOfImage) +
					sizeof(DWORD)) {
				goto cleanup;
			}

			IMAGE_OPTIONAL_HEADER32 *optional32 =
				(IMAGE_OPTIONAL_HEADER32 *)optional_header;

			size_of_image = optional32->SizeOfImage;
		} else if (magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
			if (file_header->SizeOfOptionalHeader <
					offsetof(IMAGE_OPTIONAL_HEADER64, SizeOfImage) +
					sizeof(DWORD)) {
				goto cleanup;
			}

			IMAGE_OPTIONAL_HEADER64 *optional64 =
				(IMAGE_OPTIONAL_HEADER64 *)optional_header;

			size_of_image = optional64->SizeOfImage;
		} else {
			goto cleanup;
		}

		/*
		* Validate the section table.
		*/
		size_t section_offset =
			optional_offset + file_header->SizeOfOptionalHeader;

		size_t section_count = file_header->NumberOfSections;

		if (section_count == 0 ||
			section_count > SIZE_MAX / sizeof(IMAGE_SECTION_HEADER)) {
			goto cleanup;
		}

		size_t section_table_size =
			section_count * sizeof(IMAGE_SECTION_HEADER);

		if (!xwdl_range_valid(
				image_size, section_offset, section_table_size)) {
			goto cleanup;
		}

		IMAGE_SECTION_HEADER *sections =
			(IMAGE_SECTION_HEADER *)(image + section_offset);

		/*
		* The COFF symbol table is optional.
		*/
		DWORD symbol_table_offset = file_header->PointerToSymbolTable;
		DWORD symbol_count = file_header->NumberOfSymbols;

		if (symbol_table_offset == 0 || symbol_count == 0) {
			goto cleanup;
		}

		if ((size_t)symbol_count > SIZE_MAX / sizeof(IMAGE_SYMBOL)) {
			goto cleanup;
		}

		size_t symbol_table_size =
			(size_t)symbol_count * sizeof(IMAGE_SYMBOL);

		if (!xwdl_range_valid(
				image_size,
				(size_t)symbol_table_offset,
				symbol_table_size)) {
			goto cleanup;
		}

		size_t string_table_offset =
			(size_t)symbol_table_offset + symbol_table_size;

		if (!xwdl_range_valid(
				image_size, string_table_offset, sizeof(DWORD))) {
			goto cleanup;
		}

		DWORD string_table_size;

		xmemcpy(
			&string_table_size,
			image + string_table_offset,
			sizeof(string_table_size)
		);

		if (string_table_size < sizeof(DWORD) ||
			!xwdl_range_valid(
				image_size,
				string_table_offset,
				(size_t)string_table_size)) {
			goto cleanup;
		}

		IMAGE_SYMBOL *symbols =
			(IMAGE_SYMBOL *)(image + symbol_table_offset);

		const unsigned char *string_table =
			image + string_table_offset;

		/*
		* Convert the runtime address to an RVA.
		*/
		uintptr_t module_base = (uintptr_t)wmodule;
		uintptr_t address = (uintptr_t)addr;

		if (address < module_base) goto cleanup;

		uintptr_t rva = address - module_base;

		if (rva >= size_of_image) goto cleanup;

		IMAGE_SYMBOL *best_symbol = NULL;
		uintptr_t best_rva = 0;

		/*
		* Find the nearest preceding symbol in an executable section.
		*/
		DWORD i = 0;

		while (i < symbol_count) {
			IMAGE_SYMBOL *symbol = &symbols[i];

			if (symbol->StorageClass != IMAGE_SYM_CLASS_FILE &&
				symbol->SectionNumber > 0 &&
				symbol->SectionNumber <=
					(SHORT)file_header->NumberOfSections) {

				WORD section_number = (WORD)symbol->SectionNumber;
				IMAGE_SECTION_HEADER *section =
					&sections[section_number - 1];

				DWORD section_size = section->Misc.VirtualSize;

				if (section_size < section->SizeOfRawData) {
					section_size = section->SizeOfRawData;
				}

				if ((section->Characteristics & IMAGE_SCN_MEM_EXECUTE) &&
					symbol->Value < section_size &&
					(uintptr_t)section->VirtualAddress <=
						UINTPTR_MAX - (uintptr_t)symbol->Value) {

					uintptr_t symbol_rva =
						(uintptr_t)section->VirtualAddress +
						(uintptr_t)symbol->Value;

					if (symbol_rva <= rva &&
						symbol_rva < size_of_image &&
						(best_symbol == NULL ||
						symbol_rva > best_rva)) {
						best_symbol = symbol;
						best_rva = symbol_rva;
					}
				}
			}

			/*
			* Skip auxiliary symbol records.
			*/
			DWORD advance = 1 + symbol->NumberOfAuxSymbols;

			if (advance > symbol_count - i) break;

			i += advance;
		}

		if (best_symbol == NULL) goto cleanup;

		/*
		* Resolve the narrow COFF symbol name.
		*/
		const char *name = NULL;
		size_t name_length = 0;

		if (best_symbol->N.Name.Short != 0) {
			name = (const char *)best_symbol->N.ShortName;

			while (name_length < sizeof(best_symbol->N.ShortName) &&
				name[name_length] != '\0') {
				++name_length;
			}
		} else {
			DWORD offset = best_symbol->N.Name.Long;

			if (offset < sizeof(DWORD) || offset >= string_table_size) {
				goto cleanup;
			}

			name = (const char *)string_table + offset;

			while ((size_t)offset + name_length < string_table_size &&
				name[name_length] != '\0') {
				++name_length;
			}
		}

		if (name == NULL || name_length == 0 ||
			name_length > INT_MAX ||
			xsizeof(xdladdr_symbol) <= 1 ||
			xsizeof(xdladdr_symbol) - 1 > INT_MAX) {
			goto cleanup;
		}

		/*
		* Convert the narrow name to wide characters.
		* Do not treat the COFF bytes as wchar_t values.
		*/
		int wide_length = MultiByteToWideChar(
			CP_ACP,
			0,
			name,
			(int)name_length,
			xdladdr_symbol,
			(int)(xsizeof(xdladdr_symbol) - 1)
		);

		if (wide_length <= 0) goto cleanup;

		xdladdr_symbol[wide_length] = L'\0';

		info->wdli_sname = xdladdr_symbol;
		info->dli_saddr = (void *)(module_base + best_rva);
	}

cleanup:
	if (image != NULL) {
		UnmapViewOfFile(image);
	}

	if (mapping != NULL) {
		CloseHandle(mapping);
	}

	if (file != INVALID_HANDLE_VALUE) {
		CloseHandle(file);
	}

#undef xwdl_range_valid
	return result;

#else
	Dl_info native_info;

	if (dladdr(addr, &native_info) == 0) return 0;

	info->dli_fbase = native_info.dli_fbase;
	info->dli_saddr = native_info.dli_saddr;
	info->wdli_fname = NULL;
	info->wdli_sname = NULL;

	wchar_t *wdli_fname = xmbstowcs(native_info.dli_fname);
	if (wdli_fname == NULL) return 0;

	wchar_t *wdli_sname = xmbstowcs(native_info.dli_sname);
	if (wdli_sname == NULL) {
		xfree(wdli_fname);
		return 0;
	}

	info->wdli_fname = wdli_fname;
	info->wdli_sname = wdli_sname;

	return 1;
#endif
}

XSTDAPI xhandle_t XCALLAPI xdlopen(const char * file, int mode) {
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

XSTDAPI xproc_t XCALLAPI xdlsym(xhandle_t handle, const char *name) {
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
	return GetProcAddress(handle,name);
#else
	return dlsym(handle,name);
#endif
}

XSTDAPI char* XCALLAPI xdlerror(void) {
#ifdef _WIN32
	static xthread_local char error_buffer[XDLFCN_ERROR_BUFFER];
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

XSTDAPI xhandle_t XCALLAPI xwdlopen(const wchar_t* file, int mode) {
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
	xfree(s);
	return ret;
#endif
}

XSTDAPI xproc_t XCALLAPI xwdlsym(xhandle_t handle, const wchar_t *name) {
	xproc_t ret;
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
#endif

	char *s = xwcstombs(name);
	if (!s) return NULL;
#ifdef _WIN32
	ret = GetProcAddress(handle,s);
#else
	ret = dlsym(handle,s);
#endif
	xfree(s);
	return ret;
}

XSTDAPI wchar_t* XCALLAPI xwdlerror(void) {
#ifdef _WIN32
	static xthread_local wchar_t error_buffer[XDLFCN_ERROR_BUFFER];
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

XSTDAPI int XCALLAPI xdlclose(xhandle_t handle) {
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

XSTDAPI FARPROC XCALLAPI GetProcAddressA(HMODULE hModule, LPCSTR lpProcName) {
#ifdef _WIN32
	return GetProcAddress(hModule,lpProcName);
#else
	return xdlsym(hModule,lpProcName);
#endif
}

XSTDAPI FARPROC XCALLAPI GetProcAddressW(HMODULE hModule, LPCWSTR lpProcName) {
#ifdef _WIN32
	if (!hModule || !lpProcName) return NULL;
	char *s = xwcstombs(lpProcName);
	if (!s) return NULL;
	FARPROC ret = GetProcAddress(hModule,s);
	xfree(s);
	return ret;
#else
	return xwdlsym(hModule,lpProcName);
#endif
}

#ifndef _WIN32
XSTDAPI HMODULE XCALLAPI GetModuleHandleA(LPCSTR lpModuleName) {
	return xdlopen(lpModuleName,XRTLD_NOLOAD);
}

XSTDAPI HMODULE XCALLAPI GetModuleHandleW(LPCWSTR lpModuleName) {
	return xwdlopen(lpModuleName,XRTLD_NOLOAD);
}

XSTDAPI HMODULE XCALLAPI LoadLibraryA(LPCSTR lpLibFileName) {
	return xdlopen(lpLibFileName,XRTLD_LAZY);
}

XSTDAPI HMODULE XCALLAPI LoadLibraryW(LPCWSTR lpLibFileName) {
	return xwdlopen(lpLibFileName,XRTLD_LAZY);
}

XSTDAPI FARPROC XCALLAPI GetProcAddress(HMODULE hModule, LPCSTR lpProcName) {
	return xdlsym(hModule,lpProcName);
}

XSTDAPI WINBOOL XCALLAPI FreeLibrary(HMODULE hLibModule) {
	return dlclose(hLibModule) == 0 ? TRUE : FALSE;
}

#define GetModuleHandle __LIBXC_NAME_AW(GetModuleHandle)
#define LoadLibrary __LIBXC_NAME_AW(LoadLibrary)
#endif

#ifdef __cplusplus
}
#endif

#endif