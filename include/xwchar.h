/**
 * xwchar.h: Extern Library
 *
 * Copyright (C) 2025 MrR736 <MrR736@users.github.com>
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

#ifndef __XWCHAR_H__
#define __XWCHAR_H__

#include <xstddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * vxstrlen:
 * Calculates the length of the formatted string for a va_list.
 *
 * @param __s  Format string
 * @param ap   va_list of arguments
 * @return     Length of the resulting formatted string, or -1 on error
 */
XSTDDEF_IMPORT_API size_t vxwcslen(const wchar_t *__restrict __s, va_list ap);

/**
 * xwcslen:
 * Calculates the length of the formatted string with variadic arguments.
 *
 * @param __s  Format string
 * @param ...  Variadic arguments
 * @return     Length of the resulting formatted string, or -1 on error
 */
XSTDDEF_IMPORT_API size_t xwcslen(const wchar_t *__restrict __s, ...);

/**
 * xwcscmp:
 * Compares a string against an array of strings and returns a boolean-style result.
 *
 * This function is null-safe:
 * - If both __s1 and an array element are NULL, it counts as a match.
 * - If only one is NULL, that element is skipped.
 *
 * @param __s1  The string to compare. Can be NULL.
 * @param __s2  Array of string pointers. Can contain NULL entries. Must not be NULL to perform comparison.
 * @param n     Number of elements in the array __s2.
 * @return      1 if a match is found, 0 otherwise
 */
XSTDDEF_IMPORT_API int xwcscmp(const wchar_t * __s1, const wchar_t ** __s2, size_t n);

#ifdef __cplusplus
}
#endif

#endif // __XWCHAR_H__
