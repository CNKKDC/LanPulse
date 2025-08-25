/*
 *  Copyright (C) 2025 kkdc <1557655177@qq.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  SPDX-License-Identifier: GPL-3.0-or-later
 */

/**
 * @file memory.h
 * @author kkdc <1557655177@qq.com>
 */

#ifndef __MEMORY_H__
#define __MEMORY_H__

void* safe_malloc(const size_t size);

void* safe_relloc(void *ptr, const size_t size);

void* safe_calloc(const size_t count, const size_t size);

#define MALLOC_S(size) safe_malloc((size))

#define RELLOC_S(ptr, size) safe_relloc((ptr), (size))

#define CALLOC_S(count, size) safe_calloc((count), (size))

#define FREE_S(ptr) do{ \
    free((ptr)); \
    ptr = NULL; \
}while(0)


#endif /* __MEMORY_H__ */