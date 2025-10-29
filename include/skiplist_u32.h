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

typedef struct SkipList_u32_t SkipList_u32;
typedef struct SkipList_u32_t SkipMap_u32;


/* ────────────────────────────────────────────────
   uint32_t SkipList / SkipMap
──────────────────────────────────────────────── */


// Set (membership only)
SkipList_u32* skipList_u32_create(void);
bool  skipList_u32_insert (SkipList_u32 *list, uint32_t id);
void  skipList_u32_remove (SkipList_u32 *list, uint32_t id);
bool  skipList_u32_search (SkipList_u32 *list, uint32_t search_id);
uint32_t skipList_u32_getSize(const SkipList_u32 *list);
bool skipList_u32_isEmpty(const SkipList_u32 *list);
void  skipList_u32_destroy(SkipList_u32 **list);
void  skipList_u32_print  (SkipList_u32 *list);


// Map (key → value)
SkipMap_u32* skipMap_u32_create(void);
bool   skipMap_u32_put     (SkipMap_u32 *sm, uint32_t id, void *data);
void*  skipMap_u32_get     (SkipMap_u32 *sm, uint32_t id);
void*  skipMap_u32_remove  (SkipMap_u32 *sm, uint32_t id);
bool   skipMap_u32_contains(SkipMap_u32 *sm, uint32_t id);
void   skipMap_u32_destroy (SkipMap_u32 **sm);
void   skipMap_u32_print   (SkipMap_u32 *sm);