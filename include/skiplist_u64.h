/*
 * SkipList Library
 * Copyright (C) 2025  Andrew Pegg
 *
 * The SkipList Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3 of the License only.
 *
 * The SkipList Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <https://www.gnu.org/licenses/lgpl-3.0.html>.
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifndef SL_MAX_HEIGHT
#define SL_MAX_HEIGHT 32
#endif

/* ────────────────────────────────────────────────
   Type Declarations
──────────────────────────────────────────────── */

typedef struct SkipList_u64_t SkipList_u64;
typedef struct SkipList_u64_t SkipMap_u64;


// todo add a getSize func
// todo add a isEmpty func
// todo add a pop func
// todo think about adding a pop right func


/* ────────────────────────────────────────────────
   uint64_t SkipList / SkipMap
   ──────────────────────────────────────────────── */
// Set
SkipList_u64* skipList_u64_create(void);
bool  skipList_u64_insert (SkipList_u64 *list, uint64_t id);
void  skipList_u64_remove (SkipList_u64 *list, uint64_t id);
bool  skipList_u64_search (SkipList_u64 *list, uint64_t search_id);
uint32_t skipList_u64_getSize(const SkipList_u64 *list);
bool skipList_u64_isEmpty(const SkipList_u64 *list);
void  skipList_u64_destroy(SkipList_u64 **list);
void  skipList_u64_print  (SkipList_u64 *list);

// Map
SkipMap_u64* skipMap_u64_create(void);
bool   skipMap_u64_put     (SkipMap_u64 *sm, uint64_t id, void *data);
void*  skipMap_u64_get     (SkipMap_u64 *sm, uint64_t id);
void*  skipMap_u64_remove  (SkipMap_u64 *sm, uint64_t id);
bool   skipMap_u64_contains(SkipMap_u64 *sm, uint64_t id);
uint32_t skipMap_u64_size(SkipMap_u64 *sm);
bool   skipMap_u64_isEmpty(SkipMap_u64 *sm);
void   skipMap_u64_destroy (SkipMap_u64 **sm);
void   skipMap_u64_print   (SkipMap_u64 *sm);
