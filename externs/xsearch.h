/**
 * xsearch.h: Extern Library
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

#ifndef __XSEARCH_H__
#define __XSEARCH_H__

#include <stddef.h>
#include <string.h>
#include <dirent.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <pcre.h>
#else
#include <regex.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

static inline int xsearch(const char *name, const char *dir, char **out_files, size_t *out_files_count, int use_regex) {
	if (!name || !dir || !out_files || !out_files_count)
		return -1;

	size_t count = 0;
	regex_t regex;
	if (use_regex) {
		if (regcomp(&regex, name, REG_EXTENDED | REG_NOSUB) != 0)
			return -4;
	}

#if defined(_WIN32) || defined(_WIN64)
	char *search_path = cprintf("%s\\%s", dir, name);

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(search_path, &findFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		return -2;

	do {
		// Compare using regex or exact match
		if (use_regex) {
			if (regexec(&regex, findFileData.cFileName, 0, NULL, 0) == 0) {
				out_files[count] = strdup(findFileData.cFileName);
				if (out_files[count] == NULL) {
					FindClose(hFind);
					regfree(&regex);
					return -3; // Memory allocation failed
				}
				count++;
			}
		} else if (strcmp(findFileData.cFileName, name) == 0) {
			out_files[count] = strdup(findFileData.cFileName);
			if (out_files[count] == NULL) {
				FindClose(hFind);
				return -3;
			}
			count++;
		}
	} while (FindNextFile(hFind, &findFileData) != 0);

	FindClose(hFind);
	regfree(&regex);
	free(search_path);
#else
	DIR *d = opendir(dir);
	if (!d)
		return -2; // Could not open directory

	struct dirent *entry;
	while ((entry = readdir(d)) != NULL) {
		if (use_regex) {
			if (regexec(&regex, entry->d_name, 0, NULL, 0) == 0) {
				out_files[count] = strdup(entry->d_name);
				if (out_files[count] == NULL) {
					closedir(d);
					regfree(&regex);
					return -3; // Memory allocation failed
				}
				count++;
			}
		} else if (strcmp(entry->d_name, name) == 0) {
			out_files[count] = strdup(entry->d_name);
			if (out_files[count] == NULL) {
				closedir(d);
				return -3;
			}
			count++;
		}
	}

	closedir(d);
	regfree(&regex);
#endif

	*out_files_count = count;
	return 0;
}

#ifdef __cplusplus
}
#endif

#endif // __XSEARCH_H__
