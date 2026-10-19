/**
 * xwctype.h: Extern Library
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
#ifndef __XWCTYPE_H__
#define __XWCTYPE_H__

#include "xtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI wchar_t* XCALLAPI xtowlower(const wchar_t* str);

XSTDAPI wchar_t* XCALLAPI xtowlower_l(const wchar_t* str,xlocale_t l);

XSTDAPI wchar_t* XCALLAPI xtowupper(const wchar_t* str);

XSTDAPI wchar_t* XCALLAPI xtowupper_l(const wchar_t* str,xlocale_t l);

XSTDAPI wchar_t* XCALLAPI xtowascii(const wchar_t* str);

#ifdef __cplusplus
}
#endif

#endif // __XWCTYPE_H__
