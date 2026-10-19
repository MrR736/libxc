/**
 * xctype.h: Extern Library
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
#ifndef __XCTYPE_H__
#define __XCTYPE_H__

#include "xtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

XSTDAPI char* XCALLAPI xtolower(const char* str);

XSTDAPI char* XCALLAPI xtolower_l(const char* str,xlocale_t l);

XSTDAPI char* XCALLAPI xtoupper(const char* str);

XSTDAPI char* XCALLAPI xtoupper_l(const char* str,xlocale_t l);

XSTDAPI char* XCALLAPI xtoascii(const char* str);

#ifdef __cplusplus
}
#endif

#endif // __XCTYPE_H__
