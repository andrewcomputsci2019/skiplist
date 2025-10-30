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
typedef struct SkipList_i64_t SkipList_i64;
typedef struct SkipList_i64_t SkipMap_i64;

struct SM_i64_kv {
   int64_t key;
   void * value;
};

/* ────────────────────────────────────────────────
   int64_t SkipList / SkipMap
   ──────────────────────────────────────────────── */
// Set
SkipList_i64* skipList_i64_create(void);
bool  skipList_i64_insert (SkipList_i64 *list, int64_t id);
void  skipList_i64_remove (SkipList_i64 *list, int64_t id);
bool  skipList_i64_search (SkipList_i64 *list, int64_t search_id);
uint32_t skipList_i64_getSize(const SkipList_i64 *list);
bool skipList_i64_isEmpty(const SkipList_i64 * list);
bool skipList_i64_pop(SkipList_i64 * list, int64_t * removedID);
void  skipList_i64_destroy(SkipList_i64 **list);
void  skipList_i64_print  (SkipList_i64 *list);

// Map
SkipMap_i64* skipMap_i64_create(void);
bool   skipMap_i64_put     (SkipMap_i64 *sm, int64_t id, void *data);
void*  skipMap_i64_get     (SkipMap_i64 *sm, int64_t id);
void*  skipMap_i64_remove  (SkipMap_i64 *sm, int64_t id);
bool   skipMap_i64_contains(SkipMap_i64 *sm, int64_t id);
uint32_t skipMap_i64_getSize(const SkipMap_i64 *list);
bool skipMap_i64_isEmpty(const SkipMap_i64 *list);
bool skipMap_i64_pop(SkipMap_i64 *list, struct SM_i64_kv * kv);
void   skipMap_i64_destroy (SkipMap_i64 **sm);
void   skipMap_i64_print   (SkipMap_i64 *sm);