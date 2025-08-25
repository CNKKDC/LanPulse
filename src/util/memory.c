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
 * @file memory.c
 * @author kkdc <1557655177@qq.com>
 */

#include "util/memory.h"


void* safe_malloc(const size_t size){
    void *ptr = malloc(size);
    if (ptr == NULL) {
        printf("Out of memory!!");
    }
    return ptr;
}

void* safe_relloc(void *ptr, const size_t size){
    void *ptr = relloc(ptr, size);
    if (ptr == NULL) {
        printf("Out of memory!!");
    }
    return ptr;
}

void* safe_calloc(const size_t count, const size_t size){
    void *ptr = calloc(count, size);
    if (ptr == NULL) {
        printf("Out of memory!!");
    }
    return ptr;
}
