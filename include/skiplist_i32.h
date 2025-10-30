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
typedef struct SkipList_i32_t SkipList_i32;
typedef struct SkipList_i32_t SkipMap_i32;

struct SM_i32_kv {
   int32_t key;
   void * value;
};


/* ────────────────────────────────────────────────
   int32_t SkipList / SkipMap
   ──────────────────────────────────────────────── */
// Set
SkipList_i32* skipList_i32_create(void);
bool  skipList_i32_insert (SkipList_i32 *list, int32_t id);
void  skipList_i32_remove (SkipList_i32 *list, int32_t id);
bool  skipList_i32_search (SkipList_i32 *list, int32_t search_id);
uint32_t skipList_i32_getSize(const SkipList_i32 *list);
bool skipList_i32_isEmpty(const SkipList_i32 * list);
bool skipList_i32_pop(SkipList_i32 *list, int32_t * removedID);
void  skipList_i32_destroy(SkipList_i32 **list);
void  skipList_i32_print  (SkipList_i32 *list);

// Map
SkipMap_i32* skipMap_i32_create(void);
bool   skipMap_i32_put     (SkipMap_i32 *sm, int32_t id, void *data);
void*  skipMap_i32_get     (SkipMap_i32 *sm, int32_t id);
void*  skipMap_i32_remove  (SkipMap_i32 *sm, int32_t id);
bool   skipMap_i32_contains(SkipMap_i32 *sm, int32_t id);\
uint32_t skipMap_i32_getSize(const SkipMap_i32 *list);
bool skipMap_i32_isEmpty(const SkipMap_i32 *list);
bool skipMap_i32_pop(SkipList_i32 * list, struct SM_i32_kv *sm);
void   skipMap_i32_destroy (SkipMap_i32 **sm);
void   skipMap_i32_print   (SkipMap_i32 *sm);
